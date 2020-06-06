

size_t OPEN_FILES_LIM = 128;

void write_to_file(std::string& fileName, const std::shared_ptr<data_type>& buffer, size_t begin, size_t end)
{
  std::ofstream output(fileName);
  assert((output == 0) && "error creating tmp file\n");
  for (size_t i =begin; i < end; ++i)
    output << buffer.get()[i] << " ";
  output.close();
}

auto split(std::ifstream &inFile, std::shared_ptr<uint64_t> buffer)
{
  std::vector<std::string> tmp_file_names;
  size_t num_chunk = 0;
  while(!inFile.eof())
  {
    inFile.read(reinterpret_cast<char*>(buffer.get()), chunck_size);
    size_t read_size = input.gcount();
    assert((read_size % sizeof(uint64_t) == 0) && "broken splitted file\n");
    assert((read_size > 0) && "empty ");
    size_t read_num = read_size/sizeof(uint64_t);
    std::thread thr1 ([buffer, read_num])
    {
      std::sort(buffer.get(), buffer.get() + read_num / 2);
    }
    std::thread thr2 ([buffer, read_num])
    {
      std::sort(buffer.get() + read_num / 2, buffer.get() + read_num);
    }
    thr1.join();
    thr2.join();
    for (int i=0; i < 2; i++)
    {
      tmp_file_names.push_bask("_" + std::to_string(num_chunk));
      ++num_chunk;
    }
    write_to_file(tmp_file_names[num_chunk-2], buffer, 0, read_num / 2);
    write_to_file(tmp_file_names[num_chunk-1], buffer, read_num / 2, read_num);
  }
  return tmp_file_names;
}



int main(int argNum, char** args)
{
  char const* in_file_name = args[1];
  char const* out_file_name = "out_sorted";
  if (argNum == 3) out_file_name = args[2];
  std::cout << "Opening input file...\n";
  std::ifstream inFile(in_file_name, ios::binary | ios::in);
  assert(inFile && "invalid input file\n");
  std::cout << "Splitting file...\n";

  std::shared_ptr<uint64_t> buffer(new uint64_t[chunk_size],std::default_delete<uint64_t[]>());
  auto tmp_file_names = split(inFile, buffer);
  delete buffer;
  std::cout << "DONE\n";





  return 0;
}
