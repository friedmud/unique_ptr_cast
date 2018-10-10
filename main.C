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

template <typename T_SRC, typename T_DEST, typename T_DELETER>
bool
dynamic_pointer_move(std::unique_ptr<T_DEST, T_DELETER> & dest,
                     std::unique_ptr<T_SRC, T_DELETER> & src)
{
  if (!src)
  {
    dest.reset();
    return true;
  }

  T_DEST * dest_ptr = dynamic_cast<T_DEST *>(src.get());
  if (!dest_ptr)
    return false;

  std::unique_ptr<T_DEST, T_DELETER> dest_temp(dest_ptr, std::move(src.get_deleter()));

  src.release();
  dest.swap(dest_temp);
  return true;
}

template <typename T_SRC, typename T_DEST>
bool
dynamic_pointer_move(std::unique_ptr<T_DEST> & dest, std::unique_ptr<T_SRC> & src)
{
  if (!src)
  {
    dest.reset();
    return true;
  }

  T_DEST * dest_ptr = dynamic_cast<T_DEST *>(src.get());
  if (!dest_ptr)
    return false;

  src.release();
  dest.reset(dest_ptr);
  return true;
}


int main()
{
  std::unique_ptr<A> b(new B);

  std::unique_ptr<A> c(new C);

  std::unique_ptr<C> c_ptr;

  {
    auto worked = dynamic_pointer_move(c_ptr, c);
    std::cout << "Worked: " << worked << std::endl;
  }


  {
    auto worked = dynamic_pointer_move(c_ptr, b);
    std::cout << "Worked: " << worked << std::endl;
  }
}
