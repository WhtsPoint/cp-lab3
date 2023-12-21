#ifndef LAB_3_EXAMTEST_H
#define LAB_3_EXAMTEST_H

#include "ExamQuestion.h"

#include <chrono>
#include <optional>

class ExamTest {
public:
    explicit ExamTest(const std::optional<std::chrono::seconds>& testDuration);
    ExamTest& addQuestion(const ExamQuestion& newQuestion);

    std::optional<std::chrono::seconds> testDuration;
    std::vector<ExamQuestion> examQuestions;
};

#endif //LAB_3_EXAMTEST_H

