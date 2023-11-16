#ifndef CONTROL_H_
#define CONTROL_H_

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

extern StateEnum state;
extern TimeResult results[];

void ShowTable(int bcolor, int fcolor);
void ResetScreen(int bcolor, int fcolor);
void SizeChosed(int size, int entropy, int bcolor, int fcolor);
void OnStopPressed(int time);

#endif  // CONTROL_H_
