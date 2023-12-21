#include "ExamTest.h"

ExamTest::ExamTest(const std::optional<std::chrono::seconds>& testDuration) : testDuration{testDuration} {}

ExamTest& ExamTest::addQuestion(const ExamQuestion& newQuestion) {
    examQuestions.emplace_back(newQuestion);
    return *this;
}
