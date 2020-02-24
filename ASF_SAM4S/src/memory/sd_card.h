/*
 * sd_card.h
 *
 * Created: 2/20/2020 12:38:29 PM
 *  Author: angelpopov
 */ 


#ifndef SD_CARD_H_
#define SD_CARD_H_

void sd_card_init(void);
static void write(const char *file_name, const char *data);
static void read(char *buff, int file);

#endif /* SD_CARD_H_ */