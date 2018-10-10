#include <memory>
#include <iostream>

class A
{
  virtual void f()
    {
    }
};

class B : public A
{
};

class C : public A
{
};

// These are from: https://stackoverflow.com/a/11003103

template <typename T_DEST, typename T_SRC, typename T_DELETER>
std::unique_ptr<T_DEST, T_DELETER>
dynamic_pointer_move(std::unique_ptr<T_SRC, T_DELETER> & src)
{
  if (!src)
    return std::unique_ptr<T_DEST, T_DELETER>(nullptr);

  T_DEST * dest_ptr = dynamic_cast<T_DEST *>(src.get());
  if (!dest_ptr)
    return std::unique_ptr<T_DEST, T_DELETER>(nullptr);

  std::unique_ptr<T_DEST, T_DELETER> dest_temp(dest_ptr, std::move(src.get_deleter()));

  src.release();

  return dest_temp;
}

template <typename T_SRC, typename T_DEST>
std::unique_ptr<T_DEST>
dynamic_pointer_move(std::unique_ptr<T_SRC> & src)
{
  if (!src)
    return std::unique_ptr<T_DEST>(nullptr);

  T_DEST * dest_ptr = dynamic_cast<T_DEST *>(src.get());
  if (!dest_ptr)
    return std::unique_ptr<T_DEST>(nullptr);

  std::unique_ptr<T_DEST> dest_temp(dest_ptr);

  src.release();

  return dest_temp;
}


int main()
{
  std::unique_ptr<A> b(new B);

  std::unique_ptr<A> c(new C);

  std::unique_ptr<C> c_ptr;

  {
    auto worked = dynamic_pointer_move<C>(c);
    std::cout << "Worked: " << (bool)worked << std::endl;
  }


  {
    auto worked = dynamic_pointer_move<C>(b);
    std::cout << "Worked: " << (bool)worked << std::endl;
  }
}
