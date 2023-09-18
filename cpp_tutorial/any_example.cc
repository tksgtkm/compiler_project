#include <iostream>
#include <any>

/*
anyクラスはあらゆる型の値を保持できる記憶域型である。
保持すると値と型は動的に切り替えることができる。
*/

int main() {
  // int型の値を代入して取り出す
  std::any x = 3;
  int n = std::any_cast<int>(x);

  std::cout << n << std::endl;

  // 文字列を再代入して取り出す
  x = "Hello";
  const char* s = std::any_cast<const char*>(x);

  std::cout << s << std::endl;

  // 間違った型で取り出そうとすると例外が送出される
  try {
    std::any_cast<double>(x);
  } catch (std::bad_any_cast& e) {
    std::cout << e.what() << std::endl;
  }
}