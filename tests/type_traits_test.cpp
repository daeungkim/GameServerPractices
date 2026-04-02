#include <gtest/gtest.h>

#include <string>
#include <type_traits>

namespace {

struct ImplicitFromInt {
  ImplicitFromInt(int) {}
};

struct ExplicitFromInt {
  explicit ExplicitFromInt(int) {}
};

}  // namespace

TEST(TypeTraits_검증, is_convertible_v는_암시적_변환_가능_여부를_컴파일타임에_판단한다) {
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
