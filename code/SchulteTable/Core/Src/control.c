#include <limits.h>
#include <string.h>
#include <stdio.h>

#include "control.h"
#include "table.h"
#include "main.h"
#include "ST7735.h"

StateEnum state = CHOOSING_SIZE;
TimeResult results[] = {
		{0, 0, INT_MAX},  // 3x3
		{0, 0, INT_MAX},  // 4x4
		{0, 0, INT_MAX},  // 5x5
		{0, 0, INT_MAX},  // 6x6
		{0, 0, INT_MAX},  // 7x7
};

/* Helpers prototypes */

static void WriteSize(int bcolor, int fcolor);
static void PlaySound(int duration1, int duration2, int time);
static void IsBestTime(int time);

/* Main functions */

void ShowTable(int bcolor, int fcolor)
{
	state = TABLE_SHOWING;
	ST7735_FillScreen(bcolor);

	for (int i = 0; i < schulte_table.size; ++i) {
		char format_string[20];
		for (int j = 0; j < schulte_table.size; ++j) {
			format_string[j * 3] = schulte_table.data[i][j] / 10 + '0';
			format_string[j * 3 + 1] = schulte_table.data[i][j] % 10 + '0';
			format_string[j * 3 + 2] = ' ';
		}
		format_string[schulte_table.size * 3 - 1] = '\0';
		ST7735_DrawString(3, 3 + i * 15, format_string, Font_7x10, fcolor, bcolor);
	}

	for(int i = 1; i < schulte_table.size; ++i) {
		ST7735_DrawLine(i * 18, 0, i * 18, schulte_table.size * 14 + 4, fcolor);
		ST7735_DrawLine(0, i * 15, 18 * schulte_table.size + 2, i * 15, fcolor);
	}

	char format_string[] = "Size: _x_";
	format_string[6] = format_string[8] =  schulte_table.size + '0';
	ST7735_DrawString(3, 3 + (schulte_table.size + 1) * 15, format_string, Font_7x10, fcolor, bcolor);
}

void ResetScreen(int bcolor, int fcolor) {
	  ST7735_FillScreen(bcolor);
	  ST7735_DrawString(3, 3, "Choose size from", Font_7x10, fcolor, bcolor);
	  ST7735_DrawString(3, 18, "[3;7] by pressing", Font_7x10, fcolor, bcolor);
	  ST7735_DrawString(3, 33, "corresponding button", Font_7x10, fcolor, bcolor);
	  ST7735_DrawString(3, 63, "Chosen size:", Font_7x10, fcolor, bcolor);
	  WriteSize(bcolor, fcolor);
}

void SizeChosed(int size, int entropy, int bcolor, int fcolor) {
	schulte_table.size = size;
	WriteSize(bcolor, fcolor);
	FillTable(entropy, HAL_GetTick());
	state = CHOSED_SIZE;
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

	schulte_table.size = 0;
}

/* Helpers */

void WriteSize(int bcolor, int fcolor) {
	if (schulte_table.size == 0) {
		ST7735_DrawString(3, 78, "None", Font_7x10, fcolor, bcolor);
		state = CHOOSING_SIZE;
	} else {
		char format_str[5] = "    \0";
		format_str[0] = schulte_table.size + '0';
		ST7735_DrawString(3, 78, format_str, Font_7x10, fcolor, bcolor);
		state = CHOSED_SIZE;
	}
}

void PlaySound(int duration1, int duration2, int time) {
	for (int i = 0; i < time / (duration1 + duration2); ++i) {
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, 0);
		HAL_Delay(duration1);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, 1);
		HAL_Delay(duration2);
	}
}

void IsBestTime(int time) {
	if (time <= results[schulte_table.size - 3].best_time) {
		char message[40];
		sprintf(message, "New best time for %dx%d table!\r\n", schulte_table.size, schulte_table.size);
		HAL_UART_Transmit(&huart1, (uint8_t *)message, strlen(message), 100);
		results[schulte_table.size - 3].best_time = time;
		PlaySound(15, 5, 1000);
	}
}
