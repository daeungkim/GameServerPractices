#include <gtest/gtest.h>

#include <string>

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


TEST(Cpp_언어_동작_검증, CI_동작_확인을_위한_의도적_실패_테스트) {
  // 주어진 상황: 정상적으로 비교 가능한 상수 값
  const int value = 1;

  // 행동: 잘못된 기대값과 비교
  // 기대 결과: 이 테스트는 의도적으로 실패해야 한다.
  EXPECT_EQ(value, 2) << "CI 파이프라인 동작 확인을 위한 의도적 실패 테스트입니다.";
}
