#include <gtest/gtest.h>

#include <cstdint>
#include <string>
#include <type_traits>

namespace {

struct CopyCounter {
  static inline int copy_count = 0;
  std::string value;

  explicit CopyCounter(std::string v) : value(std::move(v)) {}

  CopyCounter(const CopyCounter& other) : value(other.value) { ++copy_count; }
};

}  // namespace

TEST(Cpp_언어_동작_검증, 값_객체를_const_참조로_전달하면_복사_생성자가_호출되지_않는다) {
  // 주어진 상황: 복사 횟수를 측정할 수 있는 값 객체
  CopyCounter::copy_count = 0;
  CopyCounter input{"hello"};

  // 행동: 객체를 const 참조로 함수에 전달
  auto length_of = [](const CopyCounter& c) { return c.value.size(); };
  const auto length = length_of(input);

  // 기대 결과: 복사 생성 없이 값에 접근 가능
  EXPECT_EQ(length, 5U);
  EXPECT_EQ(CopyCounter::copy_count, 0)
      << "const 참조 전달에서는 복사 생성자가 호출되지 않아야 합니다.";
}

namespace {

enum LegacyColor { Red = 1, Green = 2, Blue = 3 };
enum class SafeColor : std::uint8_t { Red = 1, Green = 2, Blue = 3 };

}  // namespace

TEST(Cpp_언어_동작_검증, enum은_정수로_암시적_변환되지만_enum_class는_명시적_변환이_필요하다) {
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

TEST(Cpp_언어_동작_검증, enum_class는_열거자_이름이_스코프를_오염시키지_않는다) {
  // enum: 열거자 이름이 바깥 스코프로 노출됨
  const auto legacy_green = Green;
  EXPECT_EQ(static_cast<int>(legacy_green), 2);

  // enum class: 열거자 접근 시 타입 이름을 반드시 명시
  const auto safe_green = SafeColor::Green;
  EXPECT_EQ(static_cast<int>(safe_green), 2);
}

TEST(Cpp_언어_동작_검증, enum_class는_기반_타입을_명시해_메모리_크기를_제어할_수_있다) {
  EXPECT_EQ(sizeof(LegacyColor), sizeof(int));
  EXPECT_EQ(sizeof(SafeColor), sizeof(std::uint8_t));
}

namespace {

struct ImplicitFromInt {
  ImplicitFromInt(int) {}
};

struct ExplicitFromInt {
  explicit ExplicitFromInt(int) {}
};

}  // namespace

TEST(Cpp_타입_트레잇_검증, is_convertible_v는_암시적_변환_가능_여부를_컴파일타임에_판단한다) {
  // std::is_convertible_v<From, To>는
  // "From 타입 값이 To 타입으로 '암시적' 변환 가능한가?"를 bool 상수로 제공한다.
  constexpr bool int_to_implicit = std::is_convertible_v<int, ImplicitFromInt>;
  constexpr bool int_to_explicit = std::is_convertible_v<int, ExplicitFromInt>;
  constexpr bool int_to_double = std::is_convertible_v<int, double>;
  constexpr bool cstr_to_string = std::is_convertible_v<const char*, std::string>;

  EXPECT_TRUE(int_to_implicit);
  EXPECT_FALSE(int_to_explicit);
  EXPECT_TRUE(int_to_double);
  EXPECT_TRUE(cstr_to_string);
}
