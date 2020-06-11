#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <assert.h>
#include <iostream>
#include <fstream>
#include <sstream>

class Sum
{
  int result=0;
  bool curr_reading=true;
  std::queue<std::string> stringsToSum;
  std::vector<std::thread> ths;

  std::mutex upd_res;
  std::mutex adding_lines;
  std::condition_variable cv;

public:
  Sum(std::string filename);
  ~Sum() {};
  bool calcSum();
  void add_line(std::string& line);
  void updateResult(int line);
};

void Sum::add_line(std::string& line)
{
  if (line.empty()) return;
  std::unique_lock<std::mutex> lock(adding_lines);
  stringsToSum.push(line);
  cv.notify_one();
}

bool Sum::calcSum()
{
  std::unique_lock<std::mutex> lock(adding_lines);
  while (stringsToSum.empty())
  {
    if (curr_reading) cv.wait(lock);
    else return false;
  }
  std::string currentString = stringsToSum.front();
  stringsToSum.pop();
  lock.unlock();

  std::istringstream iss(currentString);
  int i = 0;
  while (iss >> i) updateResult(i);
  return true;
}

void Sum::updateResult(int i)
{
  std::unique_lock<std::mutex> lock(upd_res);
  result += i;
}

Sum::Sum(std::string filename)
{
  std::ifstream myfile;
  myfile.open(filename);
  assert((myfile.is_open()) && "Error opening the input file\n");
  
  for (int i = 0; i < 2; ++i)
  {
    ths.emplace_back([this]
      {
        while (this->calcSum()) {};
      });
  }
  std::string line;
  while(getline(myfile, line)) add_line(line);
  cv.notify_all();
  myfile.close();
  curr_reading=false;

  for (auto& thread: ths)
    thread.join();
  std::cout << "Result = " << result << std::endl;
}

int main()
{
  Sum("test.txt");
  return 0;
}
