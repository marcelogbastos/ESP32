#define CHECK(condition) do { if (!(condition)) return __LINE__; } while (false)
#include "input/Button.h"
extern "C" int main() {
  Button b;
  b.begin(false, 0);
  CHECK(!b.update(true, 10));
  CHECK(!b.update(false, 20));
  CHECK(!b.update(true, 25));
  CHECK(!b.update(true, 54));
  CHECK(b.update(true, 55));
  CHECK(!b.update(true, 1000));
  CHECK(!b.update(false, 1010));
  CHECK(!b.update(false, 1040));
  CHECK(!b.update(true, 1050));
  CHECK(b.update(true, 1080));
  b.begin(false, 0xfffffff0u);
  CHECK(!b.update(true, 0xfffffff5u));
  CHECK(b.update(true, 20)); // millis rollover
  b.begin(true, 0);
  CHECK(!b.update(true, 100)); // no phantom press at startup
  return 0;
}
