#include "bigint.h"

BigInt::BigInt() : size(1), is_negative(false)
{
  data = new char[20];
  data[0] = '0';
}

BigInt::BigInt(char *a, size_t s, bool neg) : size(s), is_negative(neg)
{
  data = new char[20];
  std::copy(a, a + s, data);
}

BigInt::BigInt(long long n)
{
  bool is_negative = n < 0;
  n = is_negative ? -n: n;
  data = new char[20];
  size = 0;
  while (n > 0){
    data[size] = n % 10 + '0';
    n /= 10;
    size++;
  }
}

BigInt::BigInt(const BigInt &n) : size(n.size), is_negative(n.is_negative)
{
  data = new char[n.size];
  std::copy(n.data, n.data + size, data);
}

BigInt::BigInt(BigInt &&n) : size(n.size), is_negative(n.is_negative), data(n.data)
{
  n.data = nullptr;
  n.size = 0;
}

std::ostream& operator<<(std::ostream& out, const BigInt& n)
{
    if (n.is_negative) out << '-';
    for (int i = 0; i < n.size; i++) {
      out << n.data[i];
    }
    return out;
}

BigInt& BigInt::operator=(const BigInt &n)
{
  if (this == &n)
    return *this;
  char* ptr = new char[n.size];
  delete [] data;
  data = ptr;
  size = n.size;
  is_negative = n.is_negative;
  std::copy(n.data, n.data + size, data);
  return *this;
}

BigInt& BigInt::operator=(BigInt &&n)
{
  if (this == &n)
    return *this;
  delete [] data;
  data = n.data;
  size = n.size;
  is_negative = n.is_negative;
  n.data = nullptr;
  n.size = 0;
  return *this;
}

BigInt::~BigInt()
{
    delete [] data;
}

BigInt BigInt::operator-() const
{
  BigInt n = *this;
  n.is_negative = ! is_negative;
  return n;
}
// n2 >= n1 and both are positive
BigInt BigInt::plus(const BigInt &n1, const BigInt &n2) const
{
  char* res = new char[n2.size + 1];
  int left = 0;
  size_t current = n2.size-1;
  for (int i=0, j=0; i < n1.size & j < n2.size; i++, j++)
  {
    res[current] =  (n1.data[i] + n2.data[j] - 2 *'0' + left) % 10 + '0';
    left = (n1.data[i] + n2.data[j] - 2 *'0' + left) / 10;
    current--;
  }
  for (int i=n1.size; i < n2.size; i++)
  {
    res[current] = (n2.data[i] - '0' + left) % 10 + '0';
    left = (n2.data[i] - '0'+ left) / 10;
    current--;
  }
  if (left > 0) res[current] = left + '0';
  size_t res_size = left > 0 ? n2.size + 1 : n2.size;
  return BigInt(res, n2.size - current, false);
}
// n2 >= n1 and both are positive
BigInt BigInt::minus(const BigInt &n1, const BigInt &n2) const
{
  char* res = new char[n2.size];
  int left = 0;
  size_t current = n2.size - 1;
  int d = 0;
  for (int i=0, j=0; i < n1.size && j < n2.size; i++, j++)
  {
      d = n2.data[j] - n1.data[i] - left;
      std::cout << d << " d\n";
      std::cout << n2.data[j] - n1.data[i] - left << " d\n";
      left = d < 0 ? 1 : 0;
      d = d < 0 ? d + 10 : d;
      res[current] = d == '0' ? '0' : d + '0';
      current--;
  }
  for (int i = n1.size; i < n2.size; i++)
  {
    d = n2.data[i] - left;
    left = d < 0 ? 1 : 0;
    d = d < 0 ? d + 10 : d;
    std::cout << d << " d\n";
    res[current] = d;
    current--;
  }
  size_t begin_pos = 0;
  while (res[begin_pos] == '0')
  {
    ++begin_pos;
    if (begin_pos == n2.size) break;
  }
  if (begin_pos != 0){
    for (int i=0; i < n2.size; ++i)
    {
      res[i] = res[i+begin_pos];
    }
  }
  return BigInt(res, n2.size-begin_pos, false);
}

BigInt BigInt::operator+(const BigInt &n) const
{
  std::cout << is_negative << " " << n.is_negative << "\n";
  BigInt res;
  if (!is_negative && !n.is_negative){
    res = *this >= n ? BigInt::plus(n, *this) : BigInt::plus(*this,n);
  } else if (is_negative && n.is_negative){
    std::cout << "hi\n";
    res = *this <= n ? -BigInt::plus(*this,n) : -BigInt::plus(n,*this);
  } else {
    if (is_negative) res = -*this >= n ? -BigInt::minus(*this, n) : BigInt::minus(n, *this);
    if (!is_negative) res = *this >= -n ? BigInt::minus(*this, n) : -BigInt::minus(n, *this);
  }
  return res;
}

BigInt BigInt::operator-(const BigInt &n) const
{
  if (*this == n) return BigInt(0);
  BigInt res;
  if (!is_negative && !n.is_negative){
    res = *this >= n ? BigInt::minus(*this, n) : BigInt::minus(n, *this);
  } else if (is_negative && n.is_negative){
    res = *this <= n ? -BigInt::minus(*this,n) : BigInt::minus(n,*this);
  } else {
    if (is_negative) res = -*this >= n ? -BigInt::minus(*this, n) : -BigInt::plus(n, *this);
    if (!is_negative) *this >= -n ? BigInt::plus(*this,n) : BigInt::plus(n,*this);
  }
  return res;
}

bool BigInt::operator==(const BigInt &n) const
{
    if (this == &n) return true;
    if (is_negative != n.is_negative || size != n.size) return false;
    for (int i = 0; i < size; ++i) {
        if (data[i] != n.data[i]) return false;
    }
    return true;
}

bool BigInt::operator!=(const BigInt &n) const
{
  return !(*this == n);
}

bool BigInt::operator<(const BigInt &n) const
{
    std::cout << is_negative << " " << n.is_negative;
    if (is_negative != n.is_negative) return is_negative < n.is_negative;
    if (size != n.size) return size < n.size;
    for (int i = 0; i < size; ++i) {
        if (data[i] > n.data[i]) {
            return false;
        } else if (data[i] < n.data[i]) {
            return true;
        }
    }
    return false;
}

bool BigInt::operator<=(const BigInt &n) const
{
    return (*this < n) || (*this == n);
}

bool BigInt::operator>=(const BigInt &n) const
{
    return !(*this < n);
}

bool BigInt::operator>(const BigInt &n) const
{
    return !(*this < n) && (*this != n);
}
