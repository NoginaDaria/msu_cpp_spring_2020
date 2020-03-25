#include <iostream>
#include <cassert>
#pragma once

class Matrix {
  size_t rows;
  size_t cols;
  int * matrix;

public:

  class Proxy {
    int * torow;
    size_t ncols;
  public:
    Proxy(Matrix * m, size_t j);
    const int& operator[](size_t j) const;
    int& operator[](size_t j);
    ~Proxy();
  };

  Matrix(size_t rows, size_t cols);
  size_t getRows();
  size_t getColumns();
  const Proxy operator[](size_t j) const;
  Proxy operator[](size_t j);
  bool operator==(const Matrix& i) const;
  bool operator!=(const Matrix& i) const;
  Matrix& operator*=(int p);
  ~Matrix();
};
