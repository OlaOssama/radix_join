#ifndef RELATION_H
#define RELATION_H

#include <stdint.h>

#include "Tuple.h"

class Relation
{
	public:

		Relation(uint64_t relationSize);
		~Relation();

		uint64_t getSize();
		Tuple* getData();

		void fillUniqueValues(uint64_t startKeyValue,
				uint64_t startRidValue);
		void fillRandomValues(uint64_t startKeyValue,
				uint64_t startRidValue);

	protected:

		uint64_t relationSize;
		Tuple *data;

};

#endif
