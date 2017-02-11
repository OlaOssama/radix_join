#ifndef PERFORMANCE_H
#define PERFORMANCE_H

#include <stdint.h>
#include <stdio.h>
#include <ctime>
#include <string>
#include <pthread.h>

#include "Config.h"
#include "RadixJoin.h"

enum phase_type_t {
	JOIN_TOTAL = 0,
	PARTITIONING = 1,
	BUILD_PROBE = 2
};

class Performance 
{
	public:

		static void beginPhase(phase_type_t phase);
		static void finishPhase(phase_type_t phase);

		static void beginThread(uint64_t thread_id);
		static void finishThread(uint64_t thread_id);

		static void printRuntimes();
		static void printThreadRuntimes();

		static double phaseTime[sizeof(phase_type_t)];
		static double threadTime[Config::N_CPUS];

	protected:

		static clock_t threadBegin[Config::N_CPUS];
		static clock_t threadFinish[Config::N_CPUS];

		static clock_t phaseBegin[sizeof(phase_type_t)];
		static clock_t phaseFinish[sizeof(phase_type_t)];

		int timeval_subtract (struct timeval *result,
				struct timeval *x,
				struct timeval *y);
};

#endif
