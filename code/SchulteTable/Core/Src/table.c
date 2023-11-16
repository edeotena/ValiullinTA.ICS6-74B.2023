#include <stdlib.h>
#include <limits.h>

#include "table.h"

TableContent schulte_table = {0, {}};

void FillTable(int entropy, int sr) {
	srand(sr);

	int elem = 1;
	for (int i = 0; i < schulte_table.size; ++i) {
		for (int j = 0; j < schulte_table.size; ++j) {
			schulte_table.data[i][j] = elem++;
		}
	}

	for (int i = 0; i < entropy; ++i) {
		int row1 = rand() % schulte_table.size;
		int col1 = rand() % schulte_table.size;
		int row2 = rand() % schulte_table.size;
		int col2 = rand() % schulte_table.size;

		int temp = schulte_table.data[row1][col1];
		schulte_table.data[row1][col1] = schulte_table.data[row2][col2];
		schulte_table.data[row2][col2] = temp;
	}
}
