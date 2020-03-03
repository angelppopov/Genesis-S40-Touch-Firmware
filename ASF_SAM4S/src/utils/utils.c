/*
 * utils.c
 *
 * Created: 2/20/2020 11:12:03 AM
 *  Author: Angel Popov
 */ 

#include "utils.h"
#include "serial/serial_mcu.h"
#include <stdlib.h>
#include <string.h>

static uint8_t get_node_type(const char *data);

void reset(void){
	
}

nodes* get_command_notes(char *data){
	/*
		Allocate memory in heap for the first node.
		Memory should be allocated in this function due to the use of realloc.
		The function returns the address of the first node
	*/
	nodes *m_nodes = (nodes *)malloc(sizeof(nodes));
    char delim[] = " ";
    char *ptr = strtok(data, delim);
    int i_node = 0;
    while (ptr != NULL){
        /* Increment node size */
        i_node++;
        if(i_node > 1){
            /* Reallocate command nodes */
            m_nodes = (nodes *)realloc(m_nodes, sizeof(nodes) * i_node);
			/* Realloc could change the initial address that has been allocated for m_nodes. 
			   Thus the function returns nodes pointer
			*/
	    }
        /* Assign the data to last memory address */
        nodes *next = (nodes *) (m_nodes + i_node - 1);
        /* Write the command to the next address in memory*/
		next->node.data = ptr;
		next->node.size = strlen(ptr);
		next->node.type = get_node_type(ptr);
		/* Go to the next node */
        ptr = strtok(NULL, delim);
    }
	/* Write the number of nodes to the first node */
	m_nodes->size = i_node;
    return m_nodes;
    /*
        m_nodes should be freed in caller function
    */
}

static uint8_t get_node_type(const char *data){
	 if(strstr(data, "C1")) return C1;
	 else if(strstr(data, "C2")) return C2;
	 else if(strstr(data, "W1")) return W1;
	 else if(strstr(data, "W2")) return W2;
	 else if(strstr(data, "M1")) return M1;
	 else if(strstr(data, "M2")) return M2;
	 else if(strstr(data, "E:")) return E;
	 else return 0;
}

int get_current_temp(const char* data){
	if(strstr(data, "Boiler") != NULL){
		char delim[] = "-";
		char *ptr = strtok(data, delim);
		ptr = strtok(NULL, delim);
		return atoi(ptr);
	}
	return 0;
}
