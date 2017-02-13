#include "Relation.h"

#include <stdlib.h>
#include <cstdio>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <random>

#include "Pool.h"

Relation::Relation (uint64_t relationSize)
{
  this->relationSize = relationSize;
  this->data = (Tuple *) Pool::getMemory (relationSize * sizeof (Tuple));
  memset (this->data, 0, relationSize * sizeof (Tuple));
}

Relation::~Relation ()
{
  free (this->data);
}

uint64_t
Relation::getSize ()
{
  return this->relationSize;
}

Tuple *
Relation::getData ()
{
  return this->data;
}

void
Relation::fillUniqueValues (uint64_t startKeyValue, uint64_t startRidValue)
{
  for (uint64_t i = 0; i < this->relationSize; i++)
    this->data[i].key = startKeyValue + i;

  Tuple temp;
  for (uint64_t i = this->relationSize - 1; i > 0; --i)
    {
      uint64_t j = (uint64_t) ((double) rand () /
			       ((double) RAND_MAX + 1) * i);
      temp.key = data[i].key;
      data[i].key = data[j].key;
      data[j].key = temp.key;
    }

  for (uint64_t i = 0; i < this->relationSize; i++)
    {
      this->data[i].rid = startRidValue + i;
    }
}

// Function to generate random integers with self-similar distrubtion
uint64_t
Relation::selfSimilar (uint64_t N, double h)
{
  return (1 + (uint64_t)
	  (N * pow (rand () / (float) RAND_MAX, log(h) / log(1.0 - h))));
}

// Function to generate uniformly distributed datatset
void
Relation::fillUniform (uint64_t N)
{
  for (uint64_t i = 0; i < this->relationSize; i++)
    {
      data[i].key = (uint64_t) rand () % N + 1;
      this->data[i].rid = i;
    }
}

// Function to generate non-uniformly distributed datatset
void
Relation::fillNonUniform (uint64_t N)
{
  for (uint64_t i = 0; i < this->relationSize; i++)
    {
      this->data[i].key = Relation::selfSimilar(N, 0.6);
      this->data[i].rid = i;
    }
}

// Function to generate a dataset with gamma distribution
void
Relation::fillGamma (uint64_t N)
{
  std::default_random_engine generator;
  std::gamma_distribution<double> distribution(9.0, 9.0);

  for (uint64_t i = 0; i < this->relationSize; i++)
    {
      this->data[i].key = distribution(generator);
      this->data[i].rid = i;
    }
}
