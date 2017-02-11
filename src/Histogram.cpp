#include "Histogram.h"

#include <stdlib.h>
#include <stdio.h>

#include "Config.h"
#include "RadixJoin.h"

#define TUPLES_PER_CACHELINE (Config::CACHELINE_SIZE_BYTES / sizeof(Tuple))
#define RADIX_BITS(KEY, RBITS, BITS) ((KEY) & (RBITS) >> (BITS))

Histogram::Histogram (Relation * relation)
{
  this->relation = relation;
  this->values = (uint64_t *) calloc (RadixJoin::part_num, sizeof (uint64_t));

  uint64_t const numberOfElements = relation->getSize ();
  Tuple *const data = relation->getData ();

  for (uint64_t i = 0; i < numberOfElements; ++i)
    ++values[(RADIX_BITS (data[i].key, RadixJoin::part_num - 1, 0))];
}

Histogram::~Histogram ()
{
  free (values);
}

uint64_t *
Histogram::computePrefixSum ()
{
  uint64_t *prefix =
    (uint64_t *) calloc (RadixJoin::part_num, sizeof (uint64_t));

  uint64_t sum = 0;
  for (uint32_t p = 0; p < RadixJoin::part_num; ++p)
    {
      prefix[p] = sum;
      sum += this->getHistogram ()[p];

      /* 
         if (this->getHistogram()[p] >
         Config::MEMORY_BUFFER_SIZE_BYTES /
         sizeof(Tuple))
         {
         //printf("HIST %lu: %lu\n", p, this->getHistogram()[p]);
         }
       */

      /* Add padding to ensure that every
       * partition is cache-line aligned */
      if (sum % TUPLES_PER_CACHELINE != 0)
	{
	  sum += TUPLES_PER_CACHELINE - (sum % TUPLES_PER_CACHELINE);
	}
    }

  return prefix;
}

uint64_t *
Histogram::getHistogram ()
{
  return this->values;
}

void
Histogram::debugPrint ()
{
  for (uint64_t i = 0; i < RadixJoin::part_num; i++)
    {
      printf ("%lu: %lu\n", i, this->values[i]);
    }
}
