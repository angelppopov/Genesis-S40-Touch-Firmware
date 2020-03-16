/*
 * utils.h
 *
 * Created: 2/20/2020 11:12:15 AM
 *  Author: Angel Popov
 */ 


#ifndef UTILS_H_
#define UTILS_H_

#include <asf.h>

#define C1 11
#define C2 12
#define W1 13
#define W2 14
#define M1 15
#define M2 16
#define E  17

typedef struct {
	uint8_t type;
	char *data;
	int size;
}node;

typedef struct {
	node node;
	uint8_t size;
}nodes;

void reset(void);
nodes* get_command_notes(char *data);
int get_current_temp(const char *data);
char get_tp_number(const char *data);
char *get_tp_node(const nodes *data, const char* data);
#endif /* UTILS_H_ */