#ifndef POOL_H
#define POOL_H

#include <stdint.h>

class Pool
{

public:

  static void allocate (uint64_t size);
  static void *getMemory (uint64_t size);
  static void free (void *memory);
  static void freeAll ();
  static void reset ();

protected:

  static uint64_t dataSize;
  static void *data;

  static uint64_t remainingSize;
  static void *nextFree;

  static uint64_t lowerAddressBound;
  static uint64_t upperAddressBound;

};

#endif
