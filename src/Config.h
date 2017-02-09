#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

class Config
{
public:

	static const uint32_t N_CPUS = 64;

	static const uint32_t CACHELINE_SIZE_BYTES = 64;
	static const uint32_t CACHELINES_PER_MEMORY_BUFFER = 1024;
	static const uint32_t MEMORY_BUFFERS_PER_PARTITION = 2;

	static const uint64_t MEMORY_BUFFER_SIZE_BYTES = CACHELINES_PER_MEMORY_BUFFER * CACHELINE_SIZE_BYTES;
	static const uint64_t MEMORY_PARTITION_SIZE_BYTES = MEMORY_BUFFERS_PER_PARTITION * MEMORY_BUFFER_SIZE_BYTES;

	static const uint64_t PART_BITS = 6;
	static const uint64_t PART_COUNT = (1 << PART_BITS);

	static const uint64_t ALLOCATION_FACTOR = 2;

};

#endif
