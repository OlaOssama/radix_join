#ifndef BUILDPROBE_H
#define BUILDPROBE_H

#include "Tuple.h"

#include <stdint.h>

#include "Config.h"

class BuildProbe
{
public:

  BuildProbe (uint64_t * innerPartitionSizes,
	      uint64_t * outerPartitionSizes,
	      Tuple ** innerPartitions,
	      Tuple ** outerPartitions, uint64_t thread_id);

  static void *execute (void *context);

protected:

    uint64_t * innerPartitionSizes;
  uint64_t *outerPartitionSizes;
  Tuple **innerPartitions;
  Tuple **outerPartitions;

  uint64_t thread_id;

};

#endif
