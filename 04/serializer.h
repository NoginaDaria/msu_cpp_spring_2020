#pragma once

enum class Error
{
    NoError,
    CorruptedArchive
};

class Serializer
{
    static constexpr char Separator = ' ';
    std::ostream& out_;
public:
    explicit Serializer(std::ostream& out)
        : out_(out)
    {
    }

    template <class T>
    Error save(T& object)
    {
        return object.serialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT&&... args)
    {
        return process(args...);
    }

private:
    template <class T, class... ArgsT>
    Error process(T&& k, ArgsT&&... args)
    {
      process(k);
      process(std::forward<ArgsT>(args)...);
      return Error::NoError;
    }

    Error process(bool k)
    {
      out_ << (k ? "true" : "false") << Separator;
      return Error::NoError;
    }

    Error process(uint64_t k)
    {
      out_ << k << Separator;
      return Error::NoError;
    }
};

class Deserializer
{
    static constexpr char Separator = ' ';
    std::istream& _in;
public:
    explicit Deserializer(std::istream& in)
        : _in(in)
    {
    }

    template <class T>
    Error load(T& object)
    {
        return object.serialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT&&... args)
    {
        return process(args...);
    }

private:
    template <class T, class... ArgsT>
    Error process(T&& k, ArgsT&&... args)
    {
      process(k);
      process(std::forward<ArgsT>(args)...);
      return Error::NoError;
    }

    Error process(bool& k)
    {
      std::string tmp;
      _in >> tmp;
      if ( (tmp == "true") || (tmp == "false") ){
        k = (tmp == "true" ? true : false);
        return Error::NoError;
      } else {
        return Error::CorruptedArchive;
      }
    }

    Error process(uint64_t& k)
    {
      std::string tmp;
      _in >> tmp;
      try {
        k = std::stoull(tmp);
        return Error::NoError;
      } catch (std::exception& e) {
        return Error::CorruptedArchive;
      }
    }
};
