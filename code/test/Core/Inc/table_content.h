#ifndef TABLE_CONTENT_H_
#define TABLE_CONTENT_H_

#include <stdlib.h>
#include <limits.h>

typedef struct {
    int size;
    int data[7][7];
} TableContent;

typedef enum {
	CHOOSING_SIZE,
	CHOSED_SIZE,
	TABLE_SHOWING
} StateEnum;

typedef struct {
	int time_sum;
	int count;
	int best_time;
} TimeResult;

void fill_table(TableContent *table, int entropy, int sr);

extern TableContent main_table;
extern StateEnum state;
extern TimeResult results[];

#endif /* TABLE_CONTENT_H_ */
