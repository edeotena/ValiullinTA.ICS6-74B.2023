#include <limits.h>
#include <string.h>
#include <stdio.h>

#include "control.h"
#include "table.h"
#include "main.h"
#include "ST7735.h"

StateEnum state = CHOSED_SIZE;
TimeResult results[] = {
		{0, 0, INT_MAX},  // 3x3
		{0, 0, INT_MAX},  // 4x4
		{0, 0, INT_MAX},  // 5x5
		{0, 0, INT_MAX},  // 6x6
		{0, 0, INT_MAX},  // 7x7
};

/* Helpers prototypes */

static void PlaySound(int time);
static void IsBestTime(int time);
static void SetSize(int size);

/* Main functions */

void ShowTable()
{
	state = TABLE_SHOWING;
	ST7735_FillScreen();

	for (int i = 0; i < schulte_table.size; ++i) {
		char format_string[20];
		for (int j = 0; j < schulte_table.size; ++j) {
			format_string[j * 3] = schulte_table.data[i][j] / 10 + '0';
			format_string[j * 3 + 1] = schulte_table.data[i][j] % 10 + '0';
			format_string[j * 3 + 2] = ' ';
		}
		format_string[schulte_table.size * 3 - 1] = '\0';
		ST7735_DrawString(3, 3 + i * 15, format_string, Font_7x10);
	}

	for(int i = 1; i < schulte_table.size; ++i) {
		ST7735_DrawLine(i * 18, 0, i * 18, schulte_table.size * 14 + 5);
		ST7735_DrawLine(0, i * 15 - 1, 18 * schulte_table.size, i * 15 - 1);
	}

	char format_string[] = "size: _x_";
	format_string[6] = format_string[8] =  schulte_table.size + '0';
	ST7735_DrawString(35, 3 + (schulte_table.size + 1) * 15, format_string, Font_7x10);
}

void ResetScreen() {
	ST7735_FillScreen();
	char *text[] = { "Choose size from", "[3;7] by pressing", "corresponding button", "" };
	for (int i = 0; i < sizeof(text) / sizeof(text[i]); ++i) {
		ST7735_DrawString(3, 3 + i * 15, text[i], Font_7x10);
	}

	SetSize(schulte_table.size);
}

void OnStopPressed(int time) {
	time = HAL_GetTick() - time;
	results[schulte_table.size - 3].time_sum += time;
	++results[schulte_table.size - 3].count;

	IsBestTime(time);

	char new_record[100], mean_record[100];
	sprintf(new_record, "New result for %dx%d table:  %02d:%02d:%03d\r\n", schulte_table.size, schulte_table.size, time / 60000, (time / 1000) % 60, time % 1000);
	int sum_time = results[schulte_table.size - 3].time_sum / results[schulte_table.size - 3].count;
	sprintf(mean_record, "Mean result for %dx%d table: %02d:%02d:%03d\r\n", schulte_table.size, schulte_table.size, sum_time / 60000, (sum_time / 1000) % 60, sum_time % 1000);

	HAL_UART_Transmit(&huart1, (uint8_t *)new_record, strlen(new_record), 100);
	HAL_UART_Transmit(&huart1, (uint8_t *)mean_record, strlen(mean_record), 100);
	HAL_UART_Transmit(&huart1, (uint8_t *)"\r\n", strlen("\r\n"), 100);
}

void WriteError(char *msg) {
	ST7735_DrawString(3, 148, "                    ", Font_7x10);
	ST7735_DrawString(3, 148, msg, Font_7x10);
}

void DecreaseSize() {
	if (schulte_table.size <= 3) {
		WriteError("Min size - 3");
		return;
	}
	SetSize(schulte_table.size - 1);
}

void IncreaseSize() {
	if (schulte_table.size >= 7) {
		WriteError("Max size - 7");
		return;
	}
	SetSize(schulte_table.size + 1);
}

/* Helpers */

void SetSize(int size) {
	schulte_table.size = size;
	char format_str[15] = "Chosen size:  \0";
	format_str[13] = schulte_table.size + '0';
	ST7735_DrawString(3, 63, format_str, Font_7x10);
	FillTable(5 * size * size, HAL_GetTick());
	state = CHOSED_SIZE;
}

void PlaySound(int time) {
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
	HAL_Delay(time);
	HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_2);
}

void IsBestTime(int time) {
	if (time <= results[schulte_table.size - 3].best_time) {
		char message[40];
		sprintf(message, "New best time for %dx%d table!\r\n", schulte_table.size, schulte_table.size);
		HAL_UART_Transmit(&huart1, (uint8_t *)message, strlen(message), 100);
		results[schulte_table.size - 3].best_time = time;
		PlaySound(1000);
	}
}
