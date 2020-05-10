template <class T>
struct Allocator
{
  T* alloc(size_t n) { return static_cast<T*>(operator new(n * sizeof(T))); }
  void dealloc(T* ptr) { delete ptr; }
  void kill(T* ptr) { ptr->~T(); }
  template <typename... Args>
  void make(T* ptr, Args&&... args)
  {
    new(ptr) T(std::forward <Args> (args)...);
  }
};

template<typename T>
class Iterator : public std::iterator<std::random_access_iterator_tag, T>
{
  T* ptr_;
public:
  explicit Iterator(T* ptr) : ptr_(ptr) {}
  bool operator==(const Iterator<T>& other) const
  {
    return ptr_ == other.ptr_;
  }
  bool operator!=(const Iterator<T>& other) const
  {
    return !(*this == other);
  }

  T& operator*() const { return *ptr_; }
  T& operator[](size_t n) { return *(ptr_ + n); }

  Iterator& operator++()
  {
    ++ptr_;
    return *this;
  }
  Iterator& operator--()
  {
    --ptr_;
    return *this;
  }
  Iterator& operator+=(size_t n)
  {
    ptr_ += n;
    return *this;
  }
  Iterator& operator-=(size_t n)
  {
    ptr_ -= n;
    return *this;
  }
};

template <class T, class Alloc = Allocator<T>>
class Vector
{
  using reverse_iterator = std::reverse_iterator<Iterator<T>>;
  Alloc allocator;
  size_t N=0;
  size_t cap=0;
  T* data_=nullptr;
public:
  Vector(){}
  explicit Vector(size_t n): data_(allocator.alloc(n*2)), N(n), cap(n*2)
  {
    for (size_t i=0; i<n; i++) allocator.make(data_+i);
  }

  bool empty()  { return N == 0; }
  size_t size() { return N; }
  size_t capacity() { return cap; }

  T& operator[](size_t n) {
    if ((n > N-1) | (n < 0)) throw std::out_of_range("out of range");
    return *(data_+n);
  }

  void reserve(size_t n)
  {
    if (n <= cap) return;
    T* new_place = allocator.alloc(n);
    for (size_t i=0; i <n; i++)
    {
      allocator.make(new_place + i, std::move(data_[i]));
      allocator.kill(data_+i);
    }
    allocator.dealloc(data_);
    cap = n;
    data_ = new_place;
  }
  void resize(size_t n)
  {
    if (n == N) return;
    if (n > N)
    {
      reserve(n*2);
      for (size_t i=N; i<n; i++) allocator.make(data_+i);
    } else {
      for (size_t i=n; i<N; i++) allocator.kill(data_+i);
    }
    N = n;
  }

  void push_back(const T& n)
  {
    if (N  > cap)
    {
      size_t to_reserve = cap > 0 ? cap * 2 : 4;
      reserve(to_reserve);
    }
    allocator.make(data_+N, n);
    ++N;
  }


  void push_back(T&& n)
  {
    if (N +1 > cap)
    {
      size_t to_reserve = cap > 0 ? cap * 2 : 4;
      reserve(to_reserve);
    }
    allocator.make(data_+N, std::move(n));
    ++N;
  }

  void pop_back()
  {
    if (N <= 0) throw std::out_of_range("pop from empty vector");
    allocator.kill(data_+N);
    --N;
  }

  void clear()
  {
    for (size_t i=0; i<N; ++i) (data_ + i) -> ~T();
    N = 0;
    cap = 0;
  }

  Iterator<T> begin() { return Iterator<T>(data_); }
  Iterator<T> end() { return Iterator<T>(data_+ N); }

  reverse_iterator rbegin()
  {
    return reverse_iterator(end());
  }
  reverse_iterator rend()
  {
    return reverse_iterator(begin());
  }

  ~Vector()
  {
    clear();
    allocator.dealloc(data_);
  }
};
