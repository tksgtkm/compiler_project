#include <cassert>
#include <memory>

/*
shared_from_this: thisポインタをshared_ptrに変換する。

C++14まで
thisポインタを、enable_shared_from_thisの派生クラス型Tのshared_ptrオブジェクトとして構築して返す。
要件を満たしていない場合は未定義動作となる。

C++17から
保持するthisを指すポインタをweak_this（weak_ptr<T>）とすると、以下と等価。

return shared_ptr<T>(weak_this);
*/

struct X: public std::enable_shared_from_this<X> {
  std::shared_ptr<X> f() {
    return shared_from_this();
  }
};

int main() {
  std::shared_ptr<X> p(new X());
  std::shared_ptr<X> q = p->f();

  assert(p == q);
}