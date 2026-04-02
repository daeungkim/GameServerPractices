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

struct Meter {
  int value;

  Meter(int v) : value(v) {}
};

struct Kilometer {
  int value;

  explicit Kilometer(int v) : value(v) {}
  operator Meter() const { return Meter{value * 1000}; }
};

struct A {
  operator int() const { return 42; }
};

struct B {
  B(A) {}
};

struct C {
  C(B) {}
};

int read_meter_value(Meter meter) { return meter.value; }

Meter make_meter_from_number(int number) { return number; }

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

TEST(TypeTraits_검증, 클래스의_암시적_형변환은_비_explicit_생성자와_변환_연산자에서_발생한다) {
  // 주어진 상황: 비 explicit 단일 인자 생성자(Meter)와 사용자 정의 변환 연산자(Kilometer)
  static_assert(std::is_convertible_v<int, Meter>);
  static_assert(!std::is_convertible_v<int, Kilometer>);
  static_assert(std::is_convertible_v<Kilometer, Meter>);

  // 행동 1: 함수 인자 전달 시 int -> Meter 암시적 변환
  const int from_parameter = read_meter_value(7);

  // 행동 2: 반환문에서 int -> Meter 암시적 변환
  const Meter from_return = make_meter_from_number(3);

  // 행동 3: Kilometer -> Meter 사용자 정의 암시적 변환
  const Meter from_operator = Kilometer{2};

  // 기대 결과: 각 지점에서 의도한 암시적 형변환이 발생한다.
  EXPECT_EQ(from_parameter, 7);
  EXPECT_EQ(from_return.value, 3);
  EXPECT_EQ(from_operator.value, 2000);
}

TEST(TypeTraits_검증, 조합_변환에서는_사용자_정의_변환을_한_번만_포함할_수_있다) {
  // 주어진 상황: A -> int (사용자 정의), A -> B (사용자 정의 생성자), B -> C (사용자 정의 생성자)
  static_assert(std::is_convertible_v<A, int>);
  static_assert(std::is_convertible_v<A, B>);
  static_assert(std::is_convertible_v<B, C>);

  // 행동/기대 결과 1: 사용자 정의 1회 + 표준 변환은 허용됨 (A -> int -> double)
  static_assert(std::is_convertible_v<A, double>);

  // 행동/기대 결과 2: 사용자 정의 변환 2회를 연속으로 요구하면 암시적 변환 불가 (A -> B -> C)
  static_assert(!std::is_convertible_v<A, C>);

  const double widened = A{};
  EXPECT_DOUBLE_EQ(widened, 42.0);
}
