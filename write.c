#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdbool.h>
#include "buffer.h"

int main(){
	int memFd = shm_open( "virtual_memory", O_CREAT | O_RDWR, S_IRWXU );
	if(memFd == -1){
		perror("Can not open file");
		return 1;
	}

	int res = ftruncate( memFd, sizeof(struct circular_buffer) );
	if(res == -1){
		perror("Can not truncate file");
		return res;
	}
	
	struct circular_buffer* mem = mmap( NULL, sizeof(struct circular_buffer), PROT_READ | PROT_WRITE, MAP_SHARED, memFd, 0 );
	if( mem == NULL ){
		perror("Can not mmap");
		return -1;
	}

	for(int i = 0; i < 4096; i++){
		mem->array[i] = 0;
	}

	mem->position %= 4096;
	int current = mem->array[mem->position];
	while(1){
		mem->position++;
		mem->position %= 4096;
		mem->array[mem->position] = current;
		current++;
		if(mem->position % 1 == 0){ sleep(1); }
	}	

	munmap(mem, sizeof(struct circular_buffer));
	shm_unlink("virtual_memory");

	return 0;
}
