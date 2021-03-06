#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctime>
#include <algorithm>

#include "Relation.h"
#include "Config.h"
#include "Histogram.h"
#include "Pool.h"
#include "RadixJoin.h"
#include "Performance.h"

int
main (int argc, char *argv[])
{
  srand (time (NULL));

  // Number of partitions
  uint64_t radix_bits = atoi (argv[1]);

  // Number of threads
  RadixJoin::part_num = 1 << radix_bits;
  RadixJoin::thread_num = std::min(RadixJoin::part_num, (uint64_t) 16);

  // Sizes of both relations
  uint64_t l_size = atoi (argv[2]);
  uint64_t r_size = atoi (argv[3]);

  // Random range maximum
  uint64_t max_rand = atoi (argv[4]);

  uint64_t innerRelationSize = l_size;
  uint64_t outerRelationSize = r_size;

  // Create relations
  Pool::allocate (Config::ALLOCATION_FACTOR *
		  (innerRelationSize + outerRelationSize) * sizeof (Tuple));
  Relation *innerRelation = new Relation (innerRelationSize);
  Relation *outerRelation = new Relation (outerRelationSize);

  // Generate values
  innerRelation->fillUniform (max_rand);
  srand (time (NULL));
  outerRelation->fillUniform (max_rand);

  // Join
  RadixJoin *r_join =
    new RadixJoin (innerRelation, outerRelation, radix_bits);

  r_join->join ();
  delete r_join;

  // Output configuration
  printf ("%lu,%lu,%lu,%lu,%lu,",
	  RadixJoin::thread_num,
	  RadixJoin::part_num, l_size, r_size, max_rand);

  // Output results
  Performance::printRuntimes ();
  // Performance::printThreadRuntimes();

  Pool::freeAll ();

  return 0;

}
