#pragma once
#include <vector>
#include <algorithm>
#include <sstream>

void process(std::vector<std::string>& strings) {}


template <class T, class... Args>
void process(std::vector<std::string>& given, const T& val, const Args&... args)
{
    std::stringstream stream;
    stream << val;
    given.push_back(stream.str());
    process(given, args...);
}

template <class... ArgsT>
std::string format(const std::string& s, const ArgsT&... args)
{
  std::vector<std::string> given;
  process(given, args...);

  size_t n1 = std::count(s.begin(), s.end(), '{');
  size_t n2 = std::count(s.begin(), s.end(), '}');
  if (n1 != n2) throw std::runtime_error("brackets mismatch\n");

  int ind_number = 0;
  int begin_replace = 0;
  std::string res = s;
  int i = 0;
  while (i < res.length())
  {
    if (res[i] == '}') throw std::runtime_error("closing bracket before opening\n");
    if (res[i] != '{') {
      i++;
      continue;
    }
    begin_replace = i;
    ind_number = 0;
    i++;
    while(res[i] != '}')
    {
      ind_number = ind_number * 10 + res[i] - '0';
      i++;
    }
    if (ind_number > given.size()-1) throw std::runtime_error("incorrect number of arguments\n");
    res.replace(begin_replace, i - begin_replace + 1, given[ind_number]);
    i = begin_replace + given[ind_number].length();
  }
  return res;
}
