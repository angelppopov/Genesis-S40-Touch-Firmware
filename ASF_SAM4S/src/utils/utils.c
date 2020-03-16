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
static uint8_t get_node_by_type(nodes *data,  int type_to_search_for);
static uint8_t get_node_to_search_for(const char *data);


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
        /* Write the command to the next address in memory */
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

static uint8_t get_node_by_type(nodes *data,  uint8_t type_to_search_for){
	int ret = 0;
	nodes *cNode;
	for(int i = 0; i < data->size; i++){
		cNode = (nodes *)(data + i);
		if(cNode->node.type == type_to_search_for) ret++;
	}
	return ret;
}

static uint8_t get_node_to_search_for( char *data){
	char delim[] = "-";
	char *ptr = strtok(data, delim);
	ptr = strtok(NULL, delim);
	ptr = strtok(NULL, delim);
	
	return get_node_type(ptr);
}

char *get_tp_node(const nodes *nodes_data, const char* cmd){
	/**/
	char *res = (char *)malloc(sizeof(char) * get_node_by_type(nodes_data, get_node_to_search_for(cmd)));
	
	
	
	return res;
}


char get_tp_number(const char *data){
	char delim[] = "-";
	char *ptr = strtok(data, delim);
	ptr = strtok(NULL, delim);
	
	return atof(ptr);
}