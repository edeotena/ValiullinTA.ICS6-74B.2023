#ifndef TABLE_H_
#define TABLE_H_

#define DEFAULT_SIZE 7

typedef struct {
    int size;
    int data[7][7];
} TableContent;

void FillTable(int entropy, int sr);

extern TableContent schulte_table;

#endif  // TABLE_H_
