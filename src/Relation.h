#ifndef RELATION_H
#define RELATION_H

#include <stdint.h>

#include "Tuple.h"

class Relation
{
public:

  Relation (uint64_t relationSize);
  ~Relation ();

  uint64_t getSize ();
  Tuple *getData ();

  uint64_t selfSimilar (uint64_t N, double h);

  void fillUniqueValues (uint64_t startKeyValue, uint64_t startRidValue);
  void fillUniform (uint64_t N);
  void fillNonUniform (uint64_t N);
  void fillGamma (uint64_t N);

protected:

  uint64_t relationSize;
  Tuple *data;

};

#endif
