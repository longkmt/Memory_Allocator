/* 
    File: my_allocator.c

    Author: Long Tran
            Department of Computer Science
            Texas A&M University
    Date  : <date>

    Modified: 

    This file contains the implementation of the module "MY_ALLOCATOR".

*/

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include<stdlib.h>
#include<math.h>
#include "my_allocator.h"

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/
void** Free_List;
void* memory;

void** index_ptr; //this will be the index of the Free_List

unsigned int basic_block_size;

int number_Of_Initial_Pointer;

int isEmpty;



struct Header{
	
	int blockSize;
	struct Header* next;
	struct Header* prev;
	char buddy;
	char inherit;
	

};


/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FORWARDS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FUNCTIONS FOR MODULE MY_ALLOCATOR */
/*--------------------------------------------------------------------------*/

extern struct Header* split(void** target){ //with target = current + sizeof(void)
	
	void** temp = target;
	
	
	if (*temp == NULL ) {
			 printf("\nCHECK POINT 2! \n");
			 printf("\n Splitting.............................." );
			 printf("\nWe are looking at index: %i\n",temp);
			
				*temp = split(temp + sizeof(void));
						
	
	}	
	
	if ( ((struct Header*)(*temp))->prev == NULL) { //we are not splitting the right child
		
		struct Header* l_child1;
		struct Header* r_child1;
		
		printf("\n...........CHECK POINT 3........LEFT CHILD IS SPLITTED! \n");
		printf("\nSplitting FROM %i INTO %i! \n",((struct Header*)(*temp))->blockSize, ((struct Header*)(*temp))->blockSize /2);
		printf("\nWe are at index: %i\n",temp);
		printf("\nThe l_child is at: %i\n",*temp);
		
		l_child1 = (struct Header*)(*temp);
		l_child1->blockSize = ((struct Header*)(*temp))->blockSize /2;
		l_child1->buddy = 'L';
		l_child1->inherit = ((struct Header*)(*temp))->buddy;
		
		printf("\nThe children size is: %i\n",l_child1->blockSize);
		
		r_child1 = (int)l_child1 + (l_child1->blockSize);
		printf("\nWe are looking at r_child at: %i\n",r_child1);
		r_child1->blockSize = l_child1->blockSize;
		r_child1->buddy = 'R';
		r_child1->inherit = ((struct Header*)(*temp))->inherit;
		
		
		l_child1->next = r_child1;
		l_child1->prev = NULL;
		r_child1->next = NULL;
		r_child1->prev = l_child1;
		*temp = NULL; //mark the free_list as used 
		
		//void** previous_index = temp - sizeof(void);
		//(struct Header*)(*previous_index) = l_child1;
		//printf("\nThe l_child now should located at index %i at %i\n",previous_index, l_child1);
		
		
		return r_child1;
		
	}
	
	else {
	
	    printf("\n..............CHECK POINT 4........RIGHT CHILD IS SPLITTED! \n");
		printf("\nSplitting FROM %i INTO %i! \n",((struct Header*)(*temp))->blockSize, ((struct Header*)(*temp))->blockSize /2);
		struct Header* l_child;
		struct Header* r_child;
		/*void** temp1;
		temp1 = temp; 
	
		*temp1 = *temp;
		printf("\nThe r_child now should be %i\n",*temp1);*/
		
		//(struct Header*) (*temp) = ((struct Header*) (*temp))->prev;
			
		l_child = *temp;
		printf("\nWe are looking at l_child at: %i\n",l_child);
		l_child->blockSize = ((struct Header*)(*temp))->blockSize / 2;
		printf("\nThe children size is: %i\n",l_child->blockSize);
		l_child->buddy = 'L';
		l_child->inherit = ((struct Header*)(*temp))->buddy;
		
		
		r_child = (int)l_child + (l_child->blockSize);
		printf("\nWe are looking at r_child at: %i\n",r_child);
		r_child->blockSize = l_child->blockSize;
		r_child->buddy = 'R';
		r_child->inherit = ((struct Header*)(*temp))->inherit;
		
		(struct Header*) (*temp) = ((struct Header*) (*temp))->prev;
		
		
		l_child->next = r_child;
		//l_child->prev = NULL;
		r_child->next = NULL;
		r_child->prev = l_child;
		
		
		//void** previous_index1 = temp1 - sizeof(void);
	//	printf("\nWe are looking at previous index at: %i\n",previous_index1);
		//(struct Header*)(*previous_index1) = l_child;
		//printf("\nThe l_child should located in the previous index %i at: %i\n",previous_index1,l_child);
		//printf("\nThe r_child now should be %i\n",r_child);
		
		return r_child;
				

	}
}


/* Don't forget to implement "init_allocator" and "release_allocator"! */
unsigned int init_allocator(unsigned int _basic_block_size, unsigned int _length)
{

	// _basic_block_size is an abitrary number
	
	basic_block_size = _basic_block_size;
	printf("\nBasic block size is defined to be: %i ",_basic_block_size);
	
	printf("\n SIZE OF A HEADER IS: %i\n",sizeof(struct Header));
	
	int number_Of_Basic_Block = _length / _basic_block_size; //find the total number of basic block size
	printf("\nNumber of basic blocks: %i\n",number_Of_Basic_Block);
	
	//determine the number of pointers in the free list, this is also the size of the Free_List
	number_Of_Initial_Pointer = ceil(log10(number_Of_Basic_Block) / log10(2)); 
	printf("\nNumber of pointers in the Free_List: %i\n",number_Of_Initial_Pointer);
	
	//allocate the free list
	Free_List = (void**) malloc(number_Of_Initial_Pointer * sizeof(void*)); //size of one pointer is 4 byte
    

	//allocate the total required memory
	memory = (struct Header*) malloc(_length);
	
	printf("\nSize of a void* VS size of a header* is: %i  VS  %i\n", sizeof(void*),sizeof(struct Header));
	
	//initialize the free list, set everything to NULL
	printf("\n The Free_List is: \n");
	
	 int i;
	for (i=0; i < number_Of_Initial_Pointer; i++)
		{
			void** tmp_index_ptr;
			tmp_index_ptr = Free_List + i; //index_ptr = Free_List + i*4 
			*tmp_index_ptr = NULL; //create an empty Free_List
			printf("Index %i at %i \n",i,tmp_index_ptr);
			
			
		}

	

	
	while (number_Of_Basic_Block > 0){
		
		printf("\n Memory is at: %i", memory);
		
		//calculate the log base 2 of basic block
		int log_of_basic_block = floor(log(number_Of_Basic_Block) / log(2));
		printf("\nlog_of_basic_block: %i", log_of_basic_block);
		
		//calculating the power of 2 of basic block size
		double tmpBlockSize = pow (2,log_of_basic_block);
		
		//calculate the index of the pointers in the free list based on logarithm
		//void** index_ptr;
		
		index_ptr = Free_List + log_of_basic_block;

		//create header for each free block
		(struct Header*) (*index_ptr) = memory; 
		
		
		
		//set up information for the newly created memory block
		((struct Header*) memory)->blockSize = tmpBlockSize * _basic_block_size;
		printf("\nBlock size: %i\n", ((struct Header*) memory)->blockSize);
		
		//move to the starting address of the next block
		memory = memory + (int) (tmpBlockSize * _basic_block_size);
		number_Of_Basic_Block = number_Of_Basic_Block - (int)(tmpBlockSize);
		
	 
	}
	
	
	
	
	 return 0;                
	

	

}

	


extern Addr my_malloc(unsigned int _length) {
  /* This preliminary implementation simply hands the call over the 
     the C standard library! 
     Of course this needs to be replaced by your implementation.
  */
  
  int request_Size = ceil(_length / basic_block_size);
  printf("\n Requested size is around: %d%c = %d %s \n", request_Size,'B', request_Size * basic_block_size, "bytes");
    
  //calculate the index of a suitable memory block in the Free_List that meets user's need 
  int index = ceil(log(request_Size)/log(2));

  if (index > number_Of_Initial_Pointer){
		printf("\nInvalid request of memory!\n");
		return 0;
	}
	
	
  if(pow(2,index) == request_Size)
	{
		index++; //get the next higher index in the Free_List
		
		if (index > number_Of_Initial_Pointer){
			printf("\n Invalid memory request. Try a different size!\n");
			return NULL;
		}
			
	}
  
  printf("\n Index of suitable block in the Free_List is %d\n", index);
  
  struct Header* desire_header = (struct Header*) *(Free_List + index);
  
  if (desire_header != NULL){
	printf("\nLuckily, the our desire header is already in the Free_List!\n");
	struct Header* return_this;
	*(Free_List + index) = NULL; //mark this memory block as used
	return ( (int)desire_header + sizeof(struct Header)  )  ;
  }
	
  else{
    printf("\nCHECK POINT 1! \n");
	printf("\nOur desire header is not in the Free_List. Proceed to split!\n");
	void** next_index_ptr = Free_List + (index + 1);
	return  ( (int)split(next_index_ptr) + sizeof(struct Header) );

	}
	
	
	
  
  return NULL;
  
  

}

extern int my_free(Addr _a) {
  if  ( ( (struct Header*) (_a) == NULL )  ){
		printf("\nOut of memory size allocated! \n");
		return 1;
	}
	
	struct Header* free_this;
	free_this = _a - sizeof(struct Header); 
	printf("Size of free this is: %i ", (free_this)->blockSize);
	
	//Looking for position of free_this in the memory
	int number_Basic_Block = (free_this)->blockSize / basic_block_size;
	int position_In_Free_List = floor(log(number_Basic_Block) / log(2)); 
	
	
	//now we get the index of the memory in the Free_List
	void** index_In_Free_List = Free_List + position_In_Free_List;
	
	
	
	if ( ((struct Header*) *index_In_Free_List) == NULL ){//easy case just push it in the Free_List
		*index_In_Free_List = free_this;
		return 0;
	
	}
	
	
	//Look to see if this memory block has buddy
	while ( ((struct Header*) *index_In_Free_List) != NULL ){
		//there is a buddy in the Free_List, merge them up
		struct Header* new_Memory_Block;
		new_Memory_Block = free_this->prev;
		new_Memory_Block->blockSize = (free_this->blockSize) * 2;
		new_Memory_Block->buddy = (free_this->prev)->inherit;
		new_Memory_Block->inherit = (free_this)->inherit;
		
		index_In_Free_List++;

	}
		
  
	
  
  //free(_a);
  return 0;
}

