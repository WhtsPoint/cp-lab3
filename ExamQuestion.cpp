#include "ExamQuestion.h"

ExamQuestion::ExamQuestion(const std::string& questionText, unsigned pointValue)
    : questionText{questionText}, pointValue{pointValue} {}

ExamQuestion& ExamQuestion::addAnswer(const AnswerOption& newAnswer) {
    answerOptions.emplace_back(newAnswer);
    return *this;
}
