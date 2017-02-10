#include "BuildProbe.h"

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "Histogram.h"
#include "Config.h"
#include "Pool.h"
#include "RadixJoin.h"
#include "Performance.h"

#define NEXT_POW_2(V)                           	\
	do {                                        	\
		V--;                                    \
		V |= V >> 1;                            \
		V |= V >> 2;                            \
		V |= V >> 4;                            \
		V |= V >> 8;                            \
		V |= V >> 16;                           \
			V++;                                    \
	} while(0)

//#define HASH_BITS(KEY, N) ((KEY) % N)
#define HASH_BITS(KEY, RBITS) (((KEY) & (RBITS)) >> (0))

BuildProbe::BuildProbe(uint64_t *innerPartitionSizes,
		uint64_t *outerPartitionSizes,
		Tuple **innerPartitions,
		Tuple **outerPartitions,
		uint64_t thread_id)
{

	this->innerPartitionSizes = innerPartitionSizes;
	this->innerPartitions = innerPartitions;
	this->outerPartitionSizes = outerPartitionSizes;
	this->outerPartitions = outerPartitions;
	this->thread_id = thread_id;

}

void* BuildProbe::execute(void* context)
{
	//	printf("%ld\n", pthread_self());

	BuildProbe *task = (BuildProbe *) context;

	uint64_t N;
	Tuple* innerPart = NULL;
	Tuple* outerPart = NULL;
	uint64_t matches = 0;

	Performance::beginThread(task->thread_id);

	for (int i = 0; i < RadixJoin::parts_per_thread; i++)
	{
		// Retrieve partitions
		innerPart = (Tuple *) task->innerPartitions[i];
		outerPart = (Tuple *) task->outerPartitions[i];

		// Allocate hash table
		uint64_t *hashTableNext =
			(uint64_t*) calloc(task->innerPartitionSizes[i],
				sizeof(uint64_t));
		N = task->innerPartitionSizes[i];
		NEXT_POW_2(N);
		uint64_t *hashTableBucket =
				(uint64_t*) calloc(N, sizeof(uint64_t));

		// Build hash table
		for (uint64_t t = 0; t < task->innerPartitionSizes[i];)
		{
			uint64_t idx = HASH_BITS(innerPart[t].key, N-1);

			hashTableNext[t] = hashTableBucket[idx];
			hashTableBucket[idx] = ++t;
		}

		// Probe hash table
		for (uint64_t t = 0; t < task->outerPartitionSizes[i]; t++)
		{
			uint64_t idx = HASH_BITS(outerPart[t].key, N-1);
			for (uint64_t hit = hashTableBucket[idx];
				hit > 0;
				hit = hashTableNext[hit-1])
			{
				if (outerPart[t].key == innerPart[hit-1].key)
				{
					++matches;
				}
			}
		}

		free(hashTableNext);
		free(hashTableBucket);
	}

	Performance::finishThread(task->thread_id);
	//	printf("thread: %lu, matches: %lu, performance:%f\n",
	//			task->thread_id,
	//			matches,
	//			Performance::threadTime[task->thread_id]);

	RadixJoin::MATCH_COUNTER += matches;

}
