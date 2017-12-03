#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <unistd.h>
#include "buffer.h"

int main(){
	int memFd = shm_open( "virtual_memory", O_RDONLY, 0 );
	if(memFd == -1){
		perror("Can not open file");
		return 1;
	}

	struct circular_buffer* mem = mmap( NULL, sizeof(struct circular_buffer), PROT_READ, MAP_SHARED, memFd, 0 );
	if( mem == NULL ){
		perror("Can not mmap");
		return -1;
	}	

	uint64_t position = mem->position  % 4096;
	while(1){
		if( mem->position == position ){
			sleep(1);
			continue;
		}
		printf("%d\n", mem->array[position]);
		position++;
		position %= 4096;
	}

	munmap(mem, sizeof(struct circular_buffer));
	shm_unlink("virtual_memory");

	return 0;
}

