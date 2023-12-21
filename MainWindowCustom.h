#ifndef MAINWINDOWCUSTOM_H
#define MAINWINDOWCUSTOM_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QScrollArea>
#include <QLayout>
#include <QCheckBox>
#include <QTimer>

#include <vector>

class ExamTest;

class MainWindowCustom : public QWidget {
    Q_OBJECT
public:
    explicit MainWindowCustom(const ExamTest* test, QWidget *parent = nullptr);

signals:

private:
    void createQuestionWidgets();
    void initializeStartButton();

private:
    void refreshQuestionDisplay();
    void storeAnswer();
    void retrieveAnswer();
    void completeTest();
    void updateTimerDisplay();

private slots:
    void onTestStart();
    void onQuestionNext();
    void onQuestionBack();
    void onTestFinish();

private:
    QWidget* answerContainer = nullptr;
    QScrollArea* scrollArea = nullptr;
    QPushButton* nextButton = nullptr;
    QPushButton* backButton = nullptr;
    QPushButton* finishButton = nullptr;
    QPushButton* startButton = nullptr;
    QLabel* questionNumberLabel = nullptr;
    QLabel* questionTextLabel = nullptr;
    QVBoxLayout* answerLayout = nullptr;
    QLabel* timerLabel = nullptr;
    QLabel* remainingTimeLabel = nullptr;
    QLabel* resultLabel = nullptr;

private:
    QList<QCheckBox*> answerCheckBoxes;

private:
    QTimer* testTimer = nullptr;

private:
    const ExamTest* testInstance;

private:
    std::vector<std::vector<bool>> givenAnswers;
    unsigned currentQuestionIndex = 0;
    std::time_t testStartTime;

private:
    static const QString labelTimeText;
    static const QString labelBackText;
    static const QString labelNextText;
    static const QString labelFinishText;
    static const QString labelStartText;
};

#endif // MAINWINDOWCUSTOM_H

