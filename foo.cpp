#include <iostream> // std::cout
#include <array>
#include <fstream> // std::ofstream
#include <cassert> // assert
#include <cstring> // memcpy
#include <memory> // std::unique_ptr

class StreamBuffer: public std::streambuf
{
 public:
  using Base = std::streambuf;
  using char_type = char;

  static_assert(
      std::is_same<Base::char_type, char_type>::value,
      "Unexpected char_type"
  );

  StreamBuffer()
  {
    setp(buffer_.begin(), buffer_.end());
    stream_ = std::ofstream("out.txt");
  }

 private:
  int_type overflow(int_type c)
  {
    const int_type not_eof_value = 'x';
    assert(not_eof_value == traits_type::not_eof(not_eof_value));

    if (sync() == -1)
    {
      return traits_type::eof();
    }

    if (traits_type::eq_int_type(c, traits_type::eof()))
    {
      return not_eof_value;
    }

    assert(pptr() == pbase());

    sputc(traits_type::to_char_type(c));

    return not_eof_value;
  }

  int sync()
  {
    const int write_size = pptr() - pbase();

    if (write_size == 0)
    {
      return 0;
    }

    assert(write_size > 0);
    assert(write_size <= SIZE);

    std::array<char_type, SIZE + 1> writebuf;

    memcpy(writebuf.data(), pbase(), write_size);
    writebuf[write_size] = '\0';

    stream_ << writebuf.data();

    setp(buffer_.begin(), buffer_.end());

    return 0;
  }

  enum
  {
    SIZE = 128
  };

  std::array<char_type, SIZE> buffer_;
  std::ofstream stream_;
};

class BufferManager
{
 public:
  BufferManager()
  {
    old_buf_ = std::cout.rdbuf();
    buf_.reset(new StreamBuffer());
    std::cout.rdbuf(buf_.get());
  }

  ~BufferManager()
  {
    std::cout.rdbuf(old_buf_);
  }

 private:
  std::streambuf *old_buf_;
  std::unique_ptr<StreamBuffer> buf_;
};

void external_print()
{
  std::cout << "Hello (std::cout)" << std::endl;
  printf("Hello (printf)\n");
}

int main()
{
  if (true)
  {
    BufferManager buffer_manager;
    external_print();
  }

  std::cout << "Done" << std::endl;
}
