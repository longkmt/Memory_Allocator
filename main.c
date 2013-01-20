#include "ackerman.h"
#include "my_allocator.h"
#include <unistd.h>


int main(int argc, char ** argv) {
 // ackerman_main();
 
 
 unsigned int input_basic_block_size = 128;
 unsigned int input_memory_size =51200;
 int c; 
 
 char *bopt = 0, *sopt = 0;

 if(argc == 1)
	printf("\nUSING DEFAULT VALUES");

/*NOTE: THE SYSTAX FOR MEMTEST IS GOING TO BE: ./MEMTEST -bblocksize -smemorysize      NO SPACE BETWEEN -b and blocksize */
	
	while ( (c = getopt(argc, argv, "b::s::")) != -1) {
		
		switch (c) {
			
			case 'b': //BLOCK SIZE
				bopt = optarg;
				input_basic_block_size = atoi(bopt);
				break;
		
			case 's': //MEMORY SEGMENT SIZE
				sopt = optarg;
				input_memory_size = atoi(sopt);
				break;
		
			default:break;
		}
 
	}
 


 init_allocator(input_basic_block_size,input_memory_size);
 
 
//ackerman_main();


 
 
 printf("\n");
 
}
