#include <iostream>
#include <string>
#include <vector>

using namespace std;

namespace DEBUG {

template <typename T>
ostream& operator<<(ostream& os, const vector<T>& V) {
  os << "[ ";
  for (const auto& vv : V) os << vv << ", ";
  os << "]";
  return os;
}

template <typename T>
inline void _debug(const char* format, T t) {
  cerr << format << '=' << t << endl;
}

template <class First, class... Rest>
inline void _debug(const char* format, First first, Rest... rest) {
  while (*format != ',') cerr << *format++;
  cerr << '=' << first << ",";
  _debug(format + 1, rest...);
}

#define debug(...) _debug(#__VA_ARGS__, __VA_ARGS__)

}  // namespace DEBUG

using namespace DEBUG;

int main(int argc, char* argv[]) {
  int a = 666;
  vector<int> b({1, 2, 3});
  string c = "hello world";

  // before
  cout << "a=" << a << ", b=" << b << ", c=" << c << endl;  // a=666, b=[ 1, 2, 3, ], c=hello world
  // 如果用printf的话，在只有基本数据类型的时候是比较方便的，然是如果要输出vector等的内容的话，就会比较麻烦

  // after
  // #__VA_ARGS__将format参数设置为"a, b, c"
  debug(a, b, c);  // a=666, b=[ 1, 2, 3, ], c=hello world

  return 0;
}
