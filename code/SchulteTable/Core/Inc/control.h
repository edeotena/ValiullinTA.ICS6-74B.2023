#ifndef CONTROL_H_
#define CONTROL_H_

typedef enum {
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

void ShowTable();
void ResetScreen();
void OnStopPressed(int time);
void DecreaseSize();
void IncreaseSize();
void WriteError(char *msg);

#endif  // CONTROL_H_
