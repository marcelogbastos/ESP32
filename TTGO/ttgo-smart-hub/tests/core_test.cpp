#include "core/Button.h"
#include "modules/Advertising.h"
#define CHECK(condition) do { if (!(condition)) return __LINE__; } while (false)
extern "C" int main() {
  char name[33] = {};
  const uint8_t complete[] = {4, 0x09, 'A', 'B', 'C'};
  advertisingName(name, complete, sizeof(complete));
  CHECK(name[0] == 'A' && name[2] == 'C' && name[3] == 0);
  const uint8_t shortened[] = {2, 0x08, 'X'};
  advertisingName(name, shortened, sizeof(shortened));
  CHECK(name[0] == 'A'); // Short name cannot replace the full name.
  name[0] = 0;
  advertisingName(name, shortened, sizeof(shortened)); CHECK(name[0] == 'X');
  const uint8_t truncated[] = {8, 0x09, 'Y'};
  advertisingName(name, truncated, sizeof(truncated)); CHECK(name[0] == 'X');
  const uint8_t empty[] = {0};
  advertisingName(name, empty, sizeof(empty)); CHECK(name[0] == 'X');
  const uint8_t tooLong[] = "abcdefghijklmnopqrstuvwxyz0123456789";
  copyRadioName(name, tooLong, sizeof(tooLong)); CHECK(name[31] == '5' && name[32] == 0);
  const uint8_t terminated[] = {'T', 0, 'Z'};
  copyRadioName(name, terminated, sizeof(terminated)); CHECK(name[0] == 'T' && name[1] == 0);
  Button button;
  button.begin(false, 0);
  CHECK(button.update(true, 5) == ButtonEvent::None);
  CHECK(button.update(false, 15) == ButtonEvent::None);
  CHECK(button.update(true, 20) == ButtonEvent::None);
  CHECK(button.update(true, 49) == ButtonEvent::None);
  CHECK(button.update(true, 50) == ButtonEvent::None);
  CHECK(button.update(false, 90) == ButtonEvent::None);
  CHECK(button.update(false, 120) == ButtonEvent::Click);
  CHECK(button.update(false, 200) == ButtonEvent::None);
  CHECK(button.update(true, 300) == ButtonEvent::None);
  CHECK(button.update(true, 330) == ButtonEvent::None);
  CHECK(button.update(true, 1029) == ButtonEvent::None);
  CHECK(button.update(true, 1030) == ButtonEvent::Hold);
  CHECK(button.update(true, 2000) == ButtonEvent::None);
  CHECK(button.update(false, 2010) == ButtonEvent::None);
  CHECK(button.update(false, 2040) == ButtonEvent::None);
  button.begin(true, 0);
  CHECK(button.update(true, 1000) == ButtonEvent::None);
  button.begin(false, 0xfffffff0u);
  CHECK(button.update(true, 0xfffffff5u) == ButtonEvent::None);
  CHECK(button.update(true, 20) == ButtonEvent::None);
  CHECK(button.update(false, 30) == ButtonEvent::None);
  CHECK(button.update(false, 60) == ButtonEvent::Click);

  return 0;
}
