#include "RadixJoin.h"

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "Histogram.h"
#include "Config.h"
#include "Pool.h"
#include "BuildProbe.h"
#include "Performance.h"

#define TUPLES_PER_CACHELINE (CACHELINE_SIZE_BYTES / sizeof(Tuple))

#define RADIX_BITS(KEY, RBITS, BITS) (((KEY) & (RBITS)) >> (BITS))

uint64_t RadixJoin::MATCH_COUNTER = 0;
uint64_t RadixJoin::thread_num = 0;
uint64_t RadixJoin::part_num = 0;
uint64_t RadixJoin::parts_per_thread = 0;

RadixJoin::RadixJoin(Relation *innerRelation, Relation *outerRelation)
{
	if (innerRelation->getSize() < outerRelation->getSize())
	{
		this->innerRelation = innerRelation;
		this->outerRelation = outerRelation;
	}
	else
	{
		this->innerRelation = outerRelation;
		this->outerRelation = innerRelation;
	}
}

void RadixJoin::join()
{

	RadixJoin::partitioning();

}

void RadixJoin::partitioning()
{

	Performance::beginPhase(JOIN_TOTAL);
	Performance::beginPhase(PARTITIONING);

	// Compute histograms
	Histogram *innerHistogram = new Histogram(this->innerRelation);
	Histogram *outerHistogram = new Histogram(this->outerRelation);

	// innerHistogram->debugPrint();
	// outerHistogram->debugPrint();

	// Compute offsets of partitions
	uint64_t *innerOffsets = innerHistogram->computePrefixSum();
	uint64_t *outerOffsets = outerHistogram->computePrefixSum();

	// Scatter inner relation tuples to new locations
	Tuple * innerPartitions = NULL;
	Tuple * const idata = this->innerRelation->getData();

	uint64_t *inPartitionCounters =
		(uint64_t *) calloc(RadixJoin::part_num, sizeof(uint64_t));

	innerPartitions = (Tuple *)
		Pool::getMemory(this->innerRelation->getSize() * sizeof(Tuple));

	for (uint64_t i = 0; i < this->innerRelation->getSize(); ++i) {
		uint64_t clusterIndex =
			RADIX_BITS(idata[i].key, RadixJoin::part_num-1, 0);
		innerPartitions[innerOffsets[clusterIndex]
			+ inPartitionCounters[clusterIndex]] = idata[i];
		++inPartitionCounters[clusterIndex];
	}

	free(inPartitionCounters);

	// Scatter outer relation tuples to new locations
	Tuple *outerPartitions = NULL;
	Tuple * const odata = this->outerRelation->getData();

	inPartitionCounters =
		(uint64_t *) calloc(RadixJoin::part_num, sizeof(uint64_t));

	outerPartitions = (Tuple *)
		Pool::getMemory(this->outerRelation->getSize() * sizeof(Tuple));

	for (uint64_t i = 0; i < this->outerRelation->getSize(); ++i)
	{
		uint64_t clusterIndex =
			RADIX_BITS(odata[i].key, RadixJoin::part_num-1, 0);
		outerPartitions[outerOffsets[clusterIndex] +
			inPartitionCounters[clusterIndex]] = odata[i];
		++inPartitionCounters[clusterIndex];
	}

	free(inPartitionCounters);

	Performance::finishPhase(PARTITIONING);

	// Build and probe

	Performance::beginPhase(BUILD_PROBE);

	pthread_t thread_id[RadixJoin::thread_num];

	uint64_t* innerSizes;
	uint64_t* outerSizes;
	Tuple** innerJobs;
	Tuple** outerJobs;
	uint64_t coursor;

	for (uint64_t i = 0; i < RadixJoin::thread_num; i++){

		innerSizes = (uint64_t *) calloc(RadixJoin::parts_per_thread,
				sizeof(uint64_t));
		outerSizes = (uint64_t *) calloc(RadixJoin::parts_per_thread,
				sizeof(uint64_t));
		innerJobs = (Tuple **) calloc(RadixJoin::parts_per_thread,
				sizeof(Tuple *));
		outerJobs = (Tuple **) calloc(RadixJoin::parts_per_thread,
				sizeof(Tuple *));

		for (uint64_t p = 0; p < RadixJoin::parts_per_thread; p++)
		{
			coursor = i + p * RadixJoin::thread_num;
			innerSizes[p] = innerHistogram->getHistogram()[coursor];
			outerSizes[p] = outerHistogram->getHistogram()[coursor];
			innerJobs[p] = innerPartitions + innerOffsets[coursor];
			outerJobs[p] = outerPartitions + outerOffsets[coursor];
		}

		// Structure to pass attributes to a thread.  
		BuildProbe* buildprobe_job =
			new BuildProbe(innerSizes,
					outerSizes,
					innerJobs,
					outerJobs,
					i);

		int result = pthread_create(&thread_id[i],
						NULL,
						BuildProbe::execute,
						buildprobe_job);
	}

	for (uint64_t i = 0; i < RadixJoin::thread_num; i++){
		pthread_join(thread_id[i], NULL);
		//Performance::finishThread(i);
	}

	Performance::finishPhase(BUILD_PROBE);
	Performance::finishPhase(JOIN_TOTAL);

	delete(innerHistogram);
	delete(outerHistogram);

	free(innerOffsets);
	free(outerOffsets);

}
