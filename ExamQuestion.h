#ifndef LAB_3_EXAMQUESTION_H
#define LAB_3_EXAMQUESTION_H

#include <string>
#include <vector>

struct AnswerOption {
    std::string text;
    bool isCorrect = false;
};

class ExamQuestion {
public:
    explicit ExamQuestion(const std::string& questionText, unsigned pointValue);
    ExamQuestion& addAnswer(const AnswerOption& newAnswer);

    unsigned pointValue = 0;
    std::string questionText;
    std::vector<AnswerOption> answerOptions;
};

#endif //LAB_3_EXAMQUESTION_H
