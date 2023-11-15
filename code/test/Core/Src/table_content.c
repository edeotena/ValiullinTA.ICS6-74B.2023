#include "table_content.h"

TableContent main_table = {0, {}};
StateEnum state = CHOOSING_SIZE;
TimeResult results[] = {
		{0, 0, INT_MAX},  // 3x3
		{0, 0, INT_MAX},  // 4x4
		{0, 0, INT_MAX},  // 5x5
		{0, 0, INT_MAX},  // 6x6
		{0, 0, INT_MAX},  // 7x7
};

void fill_table(TableContent *table, int entropy, int sr) {
	int elem = 1;
	for (int i = 0; i < table->size; ++i) {
		for (int j = 0; j < table->size; ++j) {
			table->data[i][j] = elem++;
		}
	}


	srand(sr);
	for (int i = 0; i < entropy; ++i) {
		int row1 = rand() % table->size;
		int col1 = rand() % table->size;

		int temp = table->data[row1][col1];

		int row2 = rand() % table->size;
		int col2 = rand() % table->size;

		table->data[row1][col1] = table->data[row2][col2];
		table->data[row2][col2] = temp;
	}
}
