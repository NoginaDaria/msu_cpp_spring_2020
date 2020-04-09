#include <iostream>
#include <algorithm>
#pragma once

class BigInt
{
  bool is_negative;
  size_t size;
  size_t data_size = 20;
  char* data;
  BigInt(char *a, size_t s, bool neg);
  BigInt plus(const BigInt &n1, const BigInt &n2) const;
  BigInt minus(const BigInt &n1, const BigInt &n2) const;
public:
  BigInt();
  BigInt(long long n);
  BigInt(const BigInt &n);
  BigInt(const BigInt &&n);

  friend std::ostream& operator<<(std::ostream& out, const BigInt& n);

  BigInt& operator=(const BigInt &n);
  BigInt& operator=(const BigInt &&n);

  ~BigInt();

  BigInt& operator-() const;
  BigInt& operator+(const BigInt &n) const;
  BigInt& operator-(const BigInt &n) const;

  bool operator==(const BigInt &n) const;
  bool operator!=(const BigInt &n) const;
  bool operator<(const BigInt &n) const;
  bool operator<=(const BigInt &n) const;
  bool operator>(const BigInt &n) const;
  bool operator>=(const BigInt &n) const;
};
