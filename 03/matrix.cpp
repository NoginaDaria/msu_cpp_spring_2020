#include "matrix.h"

Matrix::Matrix(size_t rows, size_t cols){
  this->rows = rows;
  this->cols = cols;
  this->matrix = new int[rows * cols];
}

size_t Matrix::getRows(){
  return this->rows;
}

size_t Matrix::getColumns(){
  return this->cols;
}

Matrix::Proxy::Proxy(const Matrix * m, size_t j){
  this->ncols = m->cols;
  this->torow = m->matrix + j * this->ncols;
}

const int& Matrix::Proxy::operator[](size_t j) const{
  assert(this->ncols > j && "Index Error: index out of range");
  return this->torow[j];
}

int& Matrix::Proxy::operator[](size_t j){
  assert(this->ncols > j && "Index Error: index out of range");
  return this->torow[j];
}

const Matrix::Proxy Matrix::operator[](size_t j) const{
  assert(this->cols > j && "Index Error: index out of range");
  return Matrix::Proxy(this, j);
}

Matrix::Proxy Matrix::operator[](size_t j){
  assert(this->cols > j && "Index Error: index out of range");
  return Matrix::Proxy(this, j);
}

Matrix::Proxy::~Proxy() {}

bool Matrix::operator==(const Matrix& i) const{
  if (this == &i) {
    return true;
  }
  if ((this->cols == i.cols) & (this->rows == i.rows)){
    for (size_t k=0; k < i.rows * i.cols;k++){
      if (i.matrix[k] == this->matrix[k]) {}
    }
    return true;
  }
  return false;
}

bool Matrix::operator!=(const Matrix& i) const{
  return !(*this == i);
}

Matrix& Matrix::operator*=(int p){
  for (size_t k=0; k < this->rows * this->cols;k++){
    this->matrix[k] *= p;
  }
  return *this;
}

Matrix::~Matrix(){
  delete [] matrix;
}
