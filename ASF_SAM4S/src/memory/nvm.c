/*
 * nvm.c
 *
 * Created: 2/20/2020 11:18:16 AM
 *  Author: Angel Popov
 */ 
#include "nvm.h"
#include <asf.h>
#include <string.h>

#define TMP 0
#define HF 2

#define COMMANDS 50
#define DATA 150
#define block_size 100

static uint8_t mem_size, page_size, page_num;

int target_temp = 90;

void nvmem_init(){
	/* Initialize internal NVM */
	nvm_init(INT_FLASH);
	/* NVM Configuration */
	nvm_get_size(INT_FLASH, &mem_size);
	nvm_get_page_size(INT_FLASH, &page_size);
	nvm_get_pagenumber(INT_FLASH, DATA, &page_num);
	printf("Memory Size: %d\nPage Size: %d\n Page number:%d\n", mem_size, page_size, page_num);
	/* Read the data from nvm memory */
	uint8_t data_read[8];
	if(nvm_read(INT_FLASH, DATA, (void *)data_read, sizeof(data_read)) == STATUS_OK) {
		printf("Successfully read from nvm\n");
		//Check read content
		if(data_read[0] == 0xAA){
			printf("Read content checked!\n");
		}else{
			printf("Read content incorrect!\n");
		}
	}
	/* Write test data to nvm */
	uint8_t buffer[] = {0xAA, 0xBB, 0xCC, 0xDD, 0xEE};
	if(nvm_write(INT_FLASH, DATA, (void *)buffer, sizeof(buffer)) == STATUS_OK){
		printf("Successfully write to nvm\n");
	}
}

void from_memory_map(char *buffer, int addr){
	printf("addr off set %d\n", addr);
	printf("memory addr %d\n", DATA + (addr * block_size));
	if(nvm_read(INT_FLASH, DATA + (addr * block_size), (void *)buffer, block_size) == STATUS_OK){
		*(buffer + 101) = 0x00;
		printf("size of the data %d\n", strlen(buffer));
	}
}

void update_memory_block(char *data, int addr){
	if(nvm_write(INT_FLASH, DATA + (addr * block_size), (void *)data, block_size) == STATUS_OK){
		printf("Successfully write to nvm\n");
	}
}