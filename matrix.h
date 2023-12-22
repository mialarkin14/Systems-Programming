#ifndef     __MATRIX__H__
#define     __MATRIX__H__

#define     TElement    double

typedef struct Matrix {
    unsigned int nrows;
    unsigned int ncols;
    TElement **data;
} TMatrix;

TMatrix * newMatrix(unsigned int nrows, unsigned int ncols);
void freeMatrix(TMatrix * m);

int compareMatrix(TMatrix * m, TMatrix *n);

TMatrix * fillMatrix(TMatrix * m);
TMatrix * transposeMatrix(TMatrix * m);

void printMatrix(TMatrix * m);

TMatrix * addMatrix(TMatrix * m, TMatrix * n);
TMatrix * mulMatrix(TMatrix * m, TMatrix * n);

TMatrix * addMatrix_thread(TMatrix * m, TMatrix * n);
TMatrix * mulMatrix_thread(TMatrix * m, TMatrix * n);

#endif
