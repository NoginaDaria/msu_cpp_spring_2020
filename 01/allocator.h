class Allocator {
  std::size_t maxSize;
  char* begin_ptr;
  char* current_ptr;
public:
  Allocator(std::size_t maxSize);
  char* alloc(std::size_t size);
  void reset();
  ~Allocator();
  }
};
