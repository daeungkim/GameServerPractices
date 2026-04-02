#include <gtest/gtest.h>

#include <cstdint>
#include <type_traits>

namespace {

enum LegacyColor { Red = 1, Green = 2, Blue = 3 };
enum class SafeColor : std::uint8_t { Red = 1, Green = 2, Blue = 3 };

}  // namespace

TEST(Enum_동작_검증, enum은_정수로_암시적_변환되지만_enum_class는_명시적_변환이_필요하다) {
  constexpr LegacyColor legacy = Green;
  constexpr SafeColor safe = SafeColor::Green;

  // enum은 정수로 암시적 변환 가능
  constexpr int legacy_to_int = legacy;
  EXPECT_EQ(legacy_to_int, 2);

  // enum class는 정수로 암시적 변환되지 않음(컴파일 타임 속성으로 검증)
  static_assert(std::is_convertible_v<LegacyColor, int>);
  static_assert(!std::is_convertible_v<SafeColor, int>);

  // enum class는 명시적 변환이 필요함
  EXPECT_EQ(static_cast<int>(safe), 2);
}

TEST(Enum_동작_검증, enum_class는_열거자_이름이_스코프를_오염시키지_않는다) {
  // enum: 열거자 이름이 바깥 스코프로 노출됨
  const auto legacy_green = Green;
  EXPECT_EQ(static_cast<int>(legacy_green), 2);

  // enum class: 열거자 접근 시 타입 이름을 반드시 명시
  const auto safe_green = SafeColor::Green;
  EXPECT_EQ(static_cast<int>(safe_green), 2);
}

TEST(Enum_동작_검증, enum_class는_기반_타입을_명시해_메모리_크기를_제어할_수_있다) {
  EXPECT_EQ(sizeof(LegacyColor), sizeof(int));
  EXPECT_EQ(sizeof(SafeColor), sizeof(std::uint8_t));
}
