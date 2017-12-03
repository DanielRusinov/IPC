#ifndef SHM_STRUCT_
#define SHM_STRUCT_
#include <stdint.h>

struct circular_buffer{

	volatile uint64_t position;
	volatile int array[4096];

};

#endif
