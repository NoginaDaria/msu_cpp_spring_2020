#include <iostream>
#include <fstream>
#include <array>

int main() {
  // generate two files: sorted and shuffled
  const size_t num = 1024;
  uint64_t arr_shuffled[num];
  uint64_t arr_sorted[num];
  for (uint64_t i = 0; i < num; i++)
  {
    arr_sorted[i] = i;
    arr_shuffled[i] = num - i;
  }
  std::ofstream f_shuffled("./shuffled_file", std::ios::binary);
  std::ofstream f_sorted("./sorted_file", std::ios::binary);

  for (size_t i = 0; i < num; i++)
  {
    f_shuffled.write(reinterpret_cast<const char*>(&arr_shuffled[i]), sizeof(uint64_t));
    f_sorted.write(reinterpret_cast<const char*>(&arr_sorted[i]), sizeof(uint64_t));
  }
  f_shuffled.close();
  f_sorted.close();
  std::cout << "files were sucsessfully generated\n";
  return 0;
}
