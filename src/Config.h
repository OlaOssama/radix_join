#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

class Config
{
public:

  static const uint64_t N_CPUS = 16;

  static const uint64_t CACHELINE_SIZE_BYTES = 64;
  static const uint64_t CACHELINES_PER_MEMORY_BUFFER = 1024;

  static const uint64_t MEMORY_BUFFER_SIZE_BYTES =
    CACHELINES_PER_MEMORY_BUFFER * CACHELINE_SIZE_BYTES;

  static const uint64_t ALLOCATION_FACTOR = 3;

};

#endif
