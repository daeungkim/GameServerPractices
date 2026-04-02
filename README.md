# GameServerPractices

C++ 언어 동작 방식에 대한 의문을 테스트 코드로 검증하는 저장소입니다.

## 진행 방식

1. C++ 동작 방식에 대한 의문을 이슈/문서로 남긴다.
2. GoogleTest로 재현 가능한 테스트를 작성한다.
3. CI에서 테스트를 실행해 결과를 확인한다.
4. 테스트가 통과할 때까지 구현/검증 코드를 수정한다.
5. 통과 후 브랜치를 머지한다.

## 테스트 스타일 가이드 (BDD)

- 테스트 이름은 `Given_When_Then` 구조를 따릅니다.
- 테스트 설명 문구는 한글로 작성합니다.
- 의문 하나당 테스트 시나리오 하나를 기본 원칙으로 합니다.

예시:

- `Given_값객체_When_const참조로_전달하면_Then_복사생성자는_호출되지_않는다`

## 로컬 실행

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```
