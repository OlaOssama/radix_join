#include "Performance.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "Config.h"
#include "RadixJoin.h"

clock_t
Performance::phaseBegin[sizeof (phase_type_t)];
clock_t
Performance::phaseFinish[sizeof (phase_type_t)];
double
Performance::phaseTime[sizeof (phase_type_t)];;

clock_t
  Performance::threadBegin[Config::N_CPUS];
clock_t
  Performance::threadFinish[Config::N_CPUS];
double
  Performance::threadTime[Config::N_CPUS];

void
Performance::beginPhase (phase_type_t phase)
{
  phaseBegin[phase] = clock ();
}

void
Performance::finishPhase (phase_type_t phase)
{
  phaseFinish[phase] = clock ();
  phaseTime[phase] =
    double (phaseFinish[phase] - phaseBegin[phase]) / CLOCKS_PER_SEC;
}

void
Performance::beginThread (uint64_t thread_id)
{
  threadBegin[thread_id] = clock ();
}

void
Performance::finishThread (uint64_t thread_id)
{

  threadFinish[thread_id] = clock ();
  threadTime[thread_id] =
    double (threadFinish[thread_id] - threadBegin[thread_id])
    / CLOCKS_PER_SEC;

}

void
Performance::printRuntimes ()
{
  printf ("%f,%f,%f,%lu\n",
	  Performance::phaseTime[JOIN_TOTAL],
	  Performance::phaseTime[PARTITIONING],
	  Performance::phaseTime[BUILD_PROBE],
	  RadixJoin::MATCH_COUNTER);
}

void
Performance::printThreadRuntimes ()
{
  double checksum = 0;
  for (uint64_t i = 0; i < RadixJoin::thread_num; i++)
    {
      printf ("%f\n", Performance::threadTime[i]);
//printf("%2lu : %f\n", i, Performance::threadTime[i]);
      checksum += Performance::threadTime[i];
    }
  printf ("total,%f,%f\n", checksum, Performance::phaseTime[BUILD_PROBE]);
}


/* Subtract the ‘struct timeval’ values X and Y,
 *    storing the result in RESULT.
 *       Return 1 if the difference is negative, otherwise 0. */

int
Performance::timeval_subtract (struct timeval *result,
			       struct timeval *x, struct timeval *y)
{

  /* Perform the carry for the later subtraction by updating y. */
  if (x->tv_usec < y->tv_usec)
    {
      int nsec = (y->tv_usec - x->tv_usec) / 1000000 + 1;
      y->tv_usec -= 1000000 * nsec;
      y->tv_sec += nsec;
    }

  if (x->tv_usec - y->tv_usec > 1000000)
    {
      int nsec = (x->tv_usec - y->tv_usec) / 1000000;
      y->tv_usec += 1000000 * nsec;
      y->tv_sec -= nsec;
    }

  /* Compute the time remaining to wait.
   *      tv_usec is certainly positive. */
  result->tv_sec = x->tv_sec - y->tv_sec;
  result->tv_usec = x->tv_usec - y->tv_usec;

  /* Return 1 if result is negative. */
  return x->tv_sec < y->tv_sec;

}
