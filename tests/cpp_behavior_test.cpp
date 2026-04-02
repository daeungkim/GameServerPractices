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

TEST(Cpp_언어_동작_검증, Given_값객체_When_const참조로_전달하면_Then_복사생성자는_호출되지_않는다) {
  // Given: 복사 횟수를 측정할 수 있는 값 객체
  CopyCounter::copy_count = 0;
  CopyCounter input{"hello"};

  // When: const 참조로 함수에 전달
  auto length_of = [](const CopyCounter& c) { return c.value.size(); };
  const auto length = length_of(input);

  // Then: 복사 생성 없이 값에 접근 가능
  EXPECT_EQ(length, 5U);
  EXPECT_EQ(CopyCounter::copy_count, 0)
      << "const 참조 전달에서는 복사 생성자가 호출되지 않아야 합니다.";
}
