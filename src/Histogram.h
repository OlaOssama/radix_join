#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include "Relation.h"

class Histogram
{
public:
  Histogram (Relation * relation);
  ~Histogram ();

  void debugPrint ();
  uint64_t *computePrefixSum ();
  uint64_t *getHistogram ();

protected:

    Relation * relation;
  uint64_t *values;

};

#endif
