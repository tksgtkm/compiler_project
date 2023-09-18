#include <iostream>
#include <memory>

/*
shared_ptr<void>に、あらゆる型のポインタを格納する。
voidをテンプレート引数とするshared_ptrに対してどんな型のポインタを代入したとしても、代入した型のデストラクタは、正しく実行される。
通常void*に型変換して代入されたポインタは、delete演算子を呼んだとしても元の型のデストラクタは呼び出されない。
しかしshared_ptrの場合は代入されたポインタの型が持つデストラクタが新しく実行されることが保証される。
*/

struct X {
  ~X() {
    std::cout << "X dtor" << std::endl;
  }
};

struct Y {
  ~Y() {
    std::cout << "Y dtor" << std::endl;
  }
};

int main() {
  std::shared_ptr<void> p(new X());

  std::cout << 0 << std::endl;

  p.reset(new Y()); // Xが破棄される

  std::cout << 1 << std::endl;
  // Yが破棄される。
}