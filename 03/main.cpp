#include "matrix.h"

#define equal(a, b) do { if (a != b) std::cout << "error in checking for equality\n"; } while(0)

int main()
{

    size_t nrows = 5;
    size_t ncolumns = 6;
    int n = 0;

    Matrix m1(nrows, ncolumns);
    for (int row = 0; row < nrows; ++row)
    {
        for (int col = 0; col < ncolumns; ++col)
        {
            m1[row][col] = ++n;
        }
    }

    equal(m1[0][0], 1);
    equal(m1[0][1], 2);
    equal(m1[0][2], 3);
    equal(m1[1][0], 7);
    equal(m1[1][1], 8);
    equal(m1[nrows-1][ncolumns-1], nrows * ncolumns);

    equal(m1.getRows(), nrows);
    equal(m1.getColumns(), ncolumns);

    m1 *= 2;

    const Matrix& m2 = m1;

    equal(m2[0][0], 1 * 2);
    equal(m2[0][1], 2 * 2);
    equal(m2[0][2], 3 * 2);
    equal(m2[1][0], 7 * 2);
    equal(m2[1][1], 8 * 2);
    equal(m1[nrows-1][ncolumns-1], nrows * ncolumns * 2);

    const Matrix& m4 = m1;

    equal(m1, m1);
    equal(m1, m4);

    std::cout << "Everything works as expected!\n";

    return 0;
}
