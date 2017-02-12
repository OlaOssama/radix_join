#include "Relation.h"

#include <stdlib.h>
#include <cstdio>
#include <string.h>
#include <unistd.h>


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

void
Relation:: (uint64_t min_value, uint64_t max_value)
{
  for (uint64_t i = 0; i < this->relationSize; i++)
    {
      data[i].key = (uint64_t) (rand () % max_value + min_value) % max_value;
      this->data[i].rid = i;
    }
}

void
Relation::fillRandomValues (uint64_t min_value, uint64_t max_value)
{
  for (uint64_t i = 0; i < this->relationSize; i++)
    {
      data[i].key = (uint64_t) (rand () % max_value + min_value) % max_value;
      this->data[i].rid = i;
    }
}
