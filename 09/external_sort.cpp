#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <thread>
#include <vector>
#include <iostream>
#define TEMPORARY_FILE_MASK "temporaryXXXXXX"
#define OPEN_FILES_LIMIT 128

typedef uint64_t DTYPE;

size_t S = 1024 * 8 * sizeof(DTYPE);
size_t B = 1024 * sizeof(DTYPE);

using std::vector;
using std::cout;

class TempFile
{
private:
  char path_name[sizeof(TEMPORARY_FILE_MASK)];
  int fd;
  char* buf;
  size_t buf_size;
  size_t buf_pos;
  size_t capacity;
  bool at_end;
  int flags;
public:
  TempFile(char* _buf=0, size_t _capacity=0);
  ~TempFile();
  char* getPath() {return path_name;}
  void writeNum(DTYPE num);
  void flush();
  ssize_t readNum(DTYPE* num);
  void writeRange(void* range, size_t range_size);
  bool get_at_end() {return at_end;}
  void set_buffer(char* _buf, size_t _capacity)
  {
    buf = _buf;
    capacity = _capacity;
    buf_pos = 0;
  }
  void close()
  {
    int succ = ::close(fd);
    assert((succ != -1) && "Error occured while closing file\n");
  }
  void open(int flags);
};

void TempFile::open(int flags)
{
    fd = ::open(path_name, flags);
    flags = flags;
    assert((fd != -1) && "Error occured while opening file\n");
}

TempFile::TempFile(char* _buf, size_t _capacity) : \
  buf(_buf), capacity(_capacity), buf_pos(0), buf_size(0),\
  fd(-1), flags(O_RDWR), at_end(false)
{
  assert(((capacity % sizeof(DTYPE)) != 1) && "Error: capacity doesn't match with DTYPE size\n");
  strcpy(path_name, TEMPORARY_FILE_MASK);
  fd = mkstemp(path_name);
  assert((fd != -1) && "Error occured while handling a file\n");
}
// +
void TempFile::flush()
{
  if ((flags & (O_WRONLY | O_RDWR)) && buf_pos != 0)
  {
    ssize_t w_num = write(fd, buf, buf_pos);
    assert((w_num != -1) && "Error occured while flushing a file\n");
    assert((w_num == buf_pos) && "Error: wrong write size\n");
    buf_pos = 0;
  }
}
// +
TempFile::~TempFile()
{
  flush();
  ::close(fd);
  unlink(path_name);
}
// +
ssize_t TempFile::readNum(DTYPE* num)
{
  if (at_end) return 0;

  if (! buf)
  {
    ssize_t r_num = read(fd, (void*)num, sizeof(DTYPE));
    assert((r_num != -1) && "Error while reading from a file\n");
    if (r_num == 0)
    {
      at_end = true;
      return 0;
    }
  }
  else
  {
    if (buf_pos == buf_size)
    {
      buf_pos = 0;
      buf_size = read(fd, buf, capacity);
      assert((buf_size != -1) && "Error while reading from a file\n");
      assert(((buf_size % sizeof(DTYPE)) != 1) && "Error: bufsize doesn't match with DTYPE size\n");
    }
    if (buf_size == 0)
    {
      at_end = true;
      return 0;
    }
    *num = *(DTYPE*)(buf + buf_pos);
    buf_pos += sizeof(DTYPE);
  }
  return 1;
}
// +
void TempFile::writeNum(DTYPE num)
{
  if (! buf)
  {
    ssize_t w_num = write(fd, &num, sizeof(DTYPE));
    assert((w_num != -1) && "Error while writing num to a file\n");
  }

  if (buf_pos == capacity) flush();

  *(DTYPE*)(buf + buf_pos) = num;
  buf_pos += sizeof(DTYPE);
}
// +
void TempFile::writeRange(void* range, size_t range_size)
{
  flush();
  ssize_t w_num = write(fd, range, range_size);
  assert((w_num != -1) && "Error while writing range to a file\n");
}
// +
int cmpfunc (const void* _a, const void* _b)
{
    DTYPE a = *(DTYPE*)_a;
    DTYPE b = *(DTYPE*)_b;

    if (a > b) { return 1; }
    else if (a < b) { return -1; }
    else { return 0; }
}

vector<std::shared_ptr<TempFile>> split(int in_file)
{
  char buffer[S];
  vector<std::shared_ptr<TempFile>> splitFiles;

  ssize_t in_rnum = read(in_file, buffer, S);
  assert((in_rnum != -1) && "Error while reading from the input file\n");
  while (in_rnum != 0)
  {
    std::shared_ptr<TempFile> tf = std::make_shared<TempFile>(nullptr, 0);
    qsort(buffer, in_rnum / sizeof(DTYPE), sizeof(DTYPE), cmpfunc);
    tf->writeRange(buffer, in_rnum);
    tf->close();
    splitFiles.push_back(tf);
    in_rnum = read(in_file, buffer, S);
    assert((in_rnum != -1) && "Error while reading from the input file\n");
  }
  return splitFiles;
}

void close_func(vector<std::shared_ptr<TempFile>> in_files,\
                int start, int stop, size_t cur_files_pos)
{
  for (int i = start; i < stop; ++i) in_files[i + cur_files_pos]->close();
}

vector<std::shared_ptr<TempFile>> merge(vector<std::shared_ptr<TempFile>>& in_files)
{
  char buffer[S];
  vector<std::shared_ptr<TempFile>> out_files;
  size_t cur_files_pos = 0;
  size_t merge_files_num;
  size_t file_limit = S / B;

  if (file_limit > OPEN_FILES_LIMIT) file_limit = OPEN_FILES_LIMIT;
  --file_limit; // оставим чуть места для итогового файла

  while (cur_files_pos != in_files.size())
  {
    merge_files_num = in_files.size() - cur_files_pos;
    if (merge_files_num > file_limit) merge_files_num = file_limit;

    char* cur_buffer_pos = buffer;
    std::shared_ptr<TempFile> out_file = std::make_shared<TempFile>(cur_buffer_pos, B);
    cur_buffer_pos += B;
    for(int i = 0; i < merge_files_num; ++i)
    {
      in_files[i + cur_files_pos]->open(O_RDONLY);
      in_files[i + cur_files_pos]->set_buffer(cur_buffer_pos, B);
      cur_buffer_pos += B;
    }
    vector<bool> is_ended(merge_files_num);
    std::fill(std::begin(is_ended), end(is_ended), false);
    std::vector<DTYPE> cur_candidates(merge_files_num);

    //init candidates
    for(int i = 0; i < merge_files_num; ++i)
    {
      DTYPE is_read = in_files[i + cur_files_pos]->readNum(&cur_candidates[i]);
      if (! is_read) is_ended[i] = true;
    }

    size_t cur_pos = 0;
    while (1)
    {
      while (cur_pos < merge_files_num && is_ended[cur_pos]) ++cur_pos;
      // all files at end
      if (cur_pos == merge_files_num) break;

      int min_pos = cur_pos;
      int min_value = cur_candidates[cur_pos];

      for(int i = cur_pos + 1; i < merge_files_num; ++i)
      {
        if (! is_ended[i] && cur_candidates[i] < min_value)
        {
          min_value = cur_candidates[i];
          min_pos = i;
        }
      }
      out_file->writeNum(min_value);

      DTYPE is_read = in_files[min_pos + cur_files_pos]->readNum(&cur_candidates[min_pos]);

      if (!is_read) is_ended[min_pos] = true;
    }
    out_file->flush();
    out_file->close();
    out_files.push_back(out_file);
    int a_half = merge_files_num/2;
    std::thread t1(close_func, in_files, 0, a_half, cur_files_pos);
    std::thread t2(close_func, in_files, a_half, merge_files_num, cur_files_pos);
    t1.join();
    t2.join();
    cur_files_pos += merge_files_num;
  }
  return out_files;
}

int main(int argNum, char** args)
{
  char const* in_file_name = args[1];
  char const* out_file_name = "out.bin";
  if (argNum == 3) out_file_name = args[2];

  vector<std::shared_ptr<TempFile>> sort_files;
  vector<std::shared_ptr<TempFile>> new_sort_files;

  int in_file = open(in_file_name, O_RDONLY);

  cout << "Start splitting file...";
  sort_files = split(in_file);
  cout << " done\n";

  close(in_file);

  cout << "Merge is started\n";

  size_t phase_id = 0;
  while (sort_files.size() != 1)
  {
    ++phase_id;
    cout << "     Phase " << phase_id << "is started ...";
    new_sort_files = merge(sort_files);
    sort_files = new_sort_files;
    cout << "   done\n";
  }
  cout << "Merge is finished\n";

  unlink(out_file_name);
  int succ_flag = link(sort_files[0]->getPath(), out_file_name);
  return 0;
}
