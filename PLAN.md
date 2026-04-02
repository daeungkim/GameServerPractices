# C++ 언어 동작 검증 레포지토리 구축 계획서

## 1. 목표
- **목표**: C++ 언어 동작에 대한 의문을 빠르게 실험하고, 테스트/CI를 통해 재현 가능하게 검증하는 레포지토리를 구축한다.
- **핵심 원칙**
  1. 질문은 자연어(한국어)로 남긴다.
  2. 질문마다 BDD 스타일의 테스트를 작성한다.
  3. CI에서 자동으로 테스트를 실행한다.
  4. 테스트 통과 후에만 머지한다.

---

## 2. 요구사항 정리

### 2.1 기능 요구사항
1. C++ 표준 버전별(예: C++17, C++20, C++23) 테스트 실행 가능
2. 테스트 이름/시나리오를 **한국어**로 작성 가능
3. 새로운 의문(가설)을 빠르게 테스트 케이스로 추가 가능
4. CI에서 PR마다 자동 실행 및 결과 확인 가능
5. 실패 시 원인 파악이 쉬운 로그 제공

### 2.2 비기능 요구사항
1. 로컬/CI 환경에서 동일하게 동작(재현성)
2. 테스트 구조가 단순하고 확장 가능
3. 문서화(작성 규칙, 디렉토리 규칙, 리뷰 규칙)가 명확

---

## 3. 권장 기술 스택

## 3.1 테스트 프레임워크
- **Catch2** 또는 **GoogleTest** 권장
  - BDD 문법을 강조하려면 Catch2의 `SCENARIO / GIVEN / WHEN / THEN`이 직관적
  - GoogleTest를 사용할 경우 테스트 이름 규칙으로 BDD 형태를 강제

> 권장안: **Catch2**

### 3.2 빌드 시스템
- **CMake** 사용
  - 로컬/CI 표준화에 적합
  - `ctest` 연동 용이

### 3.3 CI 플랫폼
- **GitHub Actions**
  - PR 트리거, 매트릭스 빌드(C++ 표준/컴파일러 조합) 구성 용이

---

## 4. 레포지토리 구조 제안

```text
.
├─ CMakeLists.txt
├─ cmake/
│  └─ (필요시 공통 설정)
├─ tests/
│  ├─ bdd/
│  │  ├─ value_category/
│  │  ├─ object_lifetime/
│  │  ├─ template/
│  │  └─ concurrency/
│  ├─ support/
│  │  └─ test_helpers.hpp
│  └─ main.cpp
├─ docs/
│  ├─ testing-guidelines.md
│  ├─ question-template.md
│  └─ decision-log.md
└─ .github/
   └─ workflows/
      └─ ci.yml
```

---

## 5. 테스트 작성 규칙 (BDD + 한국어)

### 5.1 시나리오 구조
질문 1개당 최소 1개 `SCENARIO`를 만든다.

- **SCENARIO**: 검증하려는 핵심 의문
- **GIVEN**: 전제 조건
- **WHEN**: 수행 동작
- **THEN**: 기대 결과

### 5.2 문구 규칙 (한국어)
- 시나리오 제목은 “무엇을 검증하는지”가 즉시 드러나게 작성
- 테스트 문구에 표준 근거/조건을 함께 표기(가능하면 주석으로 출처)

예시:

```cpp
SCENARIO("임시 객체에 바인딩된 const 참조는 수명을 연장한다", "[lifetime][reference]") {
    GIVEN("함수에서 생성된 임시 std::string 값이 있고") {
        WHEN("const std::string& 에 바인딩하면") {
            THEN("참조가 살아있는 범위 동안 임시 객체도 유효하다") {
                // 검증 코드
            }
        }
    }
}
```

### 5.3 태그 규칙
- `[주제]` + `[세부]` 조합
- 예: `[lifetime][temporary]`, `[move][value-category]`

### 5.4 파일 네이밍
- `tests/bdd/<topic>/question_<번호>_<slug>.cpp`
- 예: `question_001_temp_lifetime.cpp`

---

## 6. CI 파이프라인 설계

### 6.1 실행 흐름
1. 코드 체크아웃
2. CMake configure
3. Build
4. Test(`ctest --output-on-failure`)
5. (선택) sanitizer/job 추가

### 6.2 매트릭스 예시
- OS: ubuntu-latest
- Compiler: gcc, clang
- Standard: C++20 (초기), 이후 C++17/23 확장

### 6.3 머지 정책
- PR 필수 체크:
  - `build-and-test (gcc, c++20)`
  - `build-and-test (clang, c++20)`
- 모든 필수 체크 통과 시에만 머지 허용

---

## 7. 운영 프로세스 (질문 → 테스트 → 머지)

1. **질문 등록**
   - 이슈/PR 본문에 의문을 한국어로 기록
2. **가설 작성**
   - “~일 것이다” 형태로 기대 동작 명시
3. **BDD 테스트 구현**
   - GIVEN/WHEN/THEN을 한국어 문구로 작성
4. **로컬 실행**
   - `cmake -S . -B build`
   - `cmake --build build`
   - `ctest --test-dir build --output-on-failure`
5. **CI 확인**
   - PR의 모든 테스트 통과 확인
6. **리뷰/머지**
   - 문구 명확성, 재현성, 표준 근거 확인 후 머지

---

## 8. 초기 구축 작업 항목 (체크리스트)

### 8.1 기본 뼈대
- [ ] CMake 프로젝트 초기화
- [ ] 테스트 프레임워크(Catch2) 연결
- [ ] `tests/main.cpp` 및 샘플 BDD 테스트 1개 추가

### 8.2 문서화
- [ ] `docs/testing-guidelines.md` 작성 (문구 규칙, 태그 규칙)
- [ ] `docs/question-template.md` 작성 (질문/가설/결과 템플릿)
- [ ] `README.md`에 빠른 시작(로컬 실행법) 추가

### 8.3 CI
- [ ] `.github/workflows/ci.yml` 작성
- [ ] PR 필수 체크 설정
- [ ] 실패 로그 가독성 옵션(`--output-on-failure`) 적용

### 8.4 품질 강화(선택)
- [ ] clang-tidy/clang-format 도입
- [ ] ASan/UBSan 잡 추가
- [ ] 컴파일 경고를 엄격 모드로 설정

---

## 9. 템플릿 제안

### 9.1 질문 템플릿
```md
## 의문
(예: rvalue 참조 변수 자체는 lvalue인가?)

## 가설
(예: 이름이 있는 rvalue 참조 변수는 표현식에서 lvalue로 취급된다.)

## 검증 방법
(어떤 코드 경로/오버로드를 통해 확인할지)

## 기대 결과
(컴파일/실행 결과를 어떻게 판단할지)
```

### 9.2 PR 템플릿 핵심 항목
- 검증하려는 C++ 동작
- 추가/수정한 BDD 시나리오 목록(한국어 제목)
- 로컬 테스트 결과
- CI 결과

---

## 10. 단계별 도입 로드맵

### Phase 1 (빠른 시작)
- Catch2 + CMake + GitHub Actions 최소 구성
- 샘플 시나리오 3개(수명, 오버로드 해석, 이동/복사)

### Phase 2 (확장)
- 표준 버전 매트릭스(C++17/20/23)
- 주제별 디렉토리 정리 및 문서 템플릿 고도화

### Phase 3 (신뢰도 강화)
- Sanitizer/정적 분석 도입
- flaky 테스트 방지 규칙 추가

---

## 11. 완료 기준(Definition of Done)
아래 조건을 모두 만족하면 “시스템 구축 완료”로 본다.

1. 새 질문을 템플릿으로 기록할 수 있다.
2. 질문을 한국어 BDD 테스트로 작성할 수 있다.
3. 로컬에서 동일 명령으로 테스트를 재현할 수 있다.
4. PR마다 CI에서 자동 검증된다.
5. 테스트 통과 전에는 머지되지 않는다.
