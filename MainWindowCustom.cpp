#include "MainWindowCustom.h"
#include "ExamTest.h"

static constexpr int windowWidth = 400;
static constexpr int windowHeight = 400;

static constexpr int buttonStartWidth = 200;
static constexpr int buttonStartHeight = 50;
const QString MainWindowCustom::labelTimeText = "Часу лишилось:";
const QString MainWindowCustom::labelBackText= "Назад";
const QString MainWindowCustom::labelNextText= "Вперед";
const QString MainWindowCustom::labelFinishText= "Завершити";
const QString MainWindowCustom::labelStartText= "Розпочати";

MainWindowCustom::MainWindowCustom(const ExamTest* test, QWidget *parent)
    : QWidget{parent}, testInstance{test} {

    resize(windowWidth, windowHeight);

    initializeStartButton();
    createQuestionWidgets();

    resultLabel = new QLabel(this);
    resultLabel->hide();

    givenAnswers.resize(test->examQuestions.size());
    for(auto i = 0; i < test->examQuestions.size(); ++i) {
        givenAnswers[i].assign(test->examQuestions[i].answerOptions.size(), false);
    }
    questionNumberLabel->hide();
    questionTextLabel->hide();
    answerContainer->hide();
    scrollArea->hide();
    nextButton->hide();
    backButton->hide();
    finishButton->hide();
    timerLabel->hide();
    remainingTimeLabel->hide();

}

void MainWindowCustom::initializeStartButton() {
    startButton = new QPushButton(this);
    startButton->setFixedWidth(buttonStartWidth);
    startButton->setFixedHeight(buttonStartHeight);
    startButton->setText(labelStartText);
    startButton->move((windowWidth - buttonStartWidth) / 2, (windowHeight - buttonStartHeight) / 2);
    QObject::connect(startButton, &QPushButton::clicked, this, &MainWindowCustom::onTestStart);
}

void MainWindowCustom::createQuestionWidgets() {
    QVBoxLayout* layoutMain;
    if (this->layout() == nullptr) {
        layoutMain = new QVBoxLayout(this);
        this->setLayout(layoutMain);
    } else {
        layoutMain = qobject_cast<QVBoxLayout*>(this->layout());
    }
    QHBoxLayout* layoutInfo = new QHBoxLayout();
    questionNumberLabel = new QLabel(this);
    timerLabel = new QLabel(MainWindowCustom::labelTimeText, this);
    remainingTimeLabel = new QLabel("0", this);
    layoutInfo->addWidget(questionNumberLabel);
    layoutInfo->addWidget(timerLabel);
    layoutInfo->addWidget(remainingTimeLabel);
    layoutInfo->addStretch();
    layoutMain->addLayout(layoutInfo);
    questionTextLabel = new QLabel(this);
    layoutMain->addWidget(questionTextLabel);
    answerContainer = new QWidget();
    answerLayout = new QVBoxLayout(answerContainer);
    scrollArea = new QScrollArea(this);
    scrollArea->setWidget(answerContainer);
    scrollArea->setWidgetResizable(true);
    layoutMain->addWidget(scrollArea);
    answerLayout->setSpacing(2);
    answerContainer->setMaximumHeight(testInstance->examQuestions.size() * 50);
    QHBoxLayout* layoutBottom = new QHBoxLayout();
    backButton = new QPushButton(labelBackText, this);
    QObject::connect(backButton, &QPushButton::clicked, this, &MainWindowCustom::onQuestionBack);
    layoutBottom->addWidget(backButton);
    nextButton = new QPushButton(labelNextText, this);
    QObject::connect(nextButton, &QPushButton::clicked, this, &MainWindowCustom::onQuestionNext);
    layoutBottom->addWidget(nextButton);
    finishButton = new QPushButton(labelFinishText, this);
    QObject::connect(finishButton, &QPushButton::clicked, this, &MainWindowCustom::onTestFinish);
    layoutBottom->addWidget(finishButton);
    layoutMain->addLayout(layoutBottom);
    refreshQuestionDisplay();
}

void MainWindowCustom::refreshQuestionDisplay() {
    if(testInstance->examQuestions.empty()) return;
    const auto& currentQuestion = testInstance->examQuestions[currentQuestionIndex];
    questionNumberLabel->setText(
        "Номер питання: " + QString::number(currentQuestionIndex + 1) + ". Бали за питання: " + QString::number(currentQuestion.pointValue) + ".");
    questionTextLabel->setText(QString::fromStdString(currentQuestion.questionText));
    qDeleteAll(answerCheckBoxes);
    answerCheckBoxes.clear();
    for(const auto& answer : currentQuestion.answerOptions) {
        auto checkBox = new QCheckBox(QString::fromStdString(answer.text));
        QObject::connect(checkBox, &QCheckBox::stateChanged, this, &MainWindowCustom::storeAnswer);
        answerCheckBoxes.push_back(checkBox);
        answerLayout->addWidget(checkBox);
    }
    answerContainer->setLayout(answerLayout);
}

void MainWindowCustom::storeAnswer() {
    for(auto i = 0; i < answerCheckBoxes.size(); ++i) {
        givenAnswers[currentQuestionIndex][i] = answerCheckBoxes[i]->isChecked();
    }
}

void MainWindowCustom::retrieveAnswer() {
    for(auto i = 0; i < answerCheckBoxes.size(); ++i) {
        answerCheckBoxes[i]->blockSignals(true);
        answerCheckBoxes[i]->setChecked(givenAnswers[currentQuestionIndex][i]);
        answerCheckBoxes[i]->blockSignals(false);
    }
}

void MainWindowCustom::completeTest() {
    questionNumberLabel->hide();
    questionTextLabel->hide();
    answerContainer->hide();
    scrollArea->hide();
    nextButton->hide();
    backButton->hide();
    finishButton->hide();
    timerLabel->hide();
    remainingTimeLabel->hide();

    unsigned totalScore = 0u;
    for(auto i = 0; i < givenAnswers.size(); ++i) {
        bool answerCorrect = true;
        for(auto j = 0; j < givenAnswers[i].size(); ++j) {
            if(givenAnswers[i][j] != testInstance->examQuestions[i].answerOptions[j].isCorrect) {
                answerCorrect = false;
                break;
            }
        }
        if(answerCorrect) {
            totalScore += testInstance->examQuestions[i].pointValue;
        }
    }
    resultLabel->setText("Набрані бали: " + QString::number(totalScore));
    resultLabel->show();
}

void MainWindowCustom::updateTimerDisplay() {
    auto currentTime = std::chrono::system_clock::now();
    auto elapsed = std::chrono::system_clock::to_time_t(currentTime) - testStartTime;
    auto elapsedDuration = std::chrono::system_clock::from_time_t(elapsed);
    auto elapsedSeconds = std::chrono::time_point_cast<std::chrono::seconds>(elapsedDuration);
    auto elapsedCount = elapsedSeconds.time_since_epoch();

    auto timeLeft = testInstance->testDuration.value() - elapsedCount;
    remainingTimeLabel->setText(QString::number(timeLeft.count()));
}

void MainWindowCustom::onTestStart() {
    startButton->hide();

    if(testInstance->testDuration) {
        auto currentTime = std::chrono::system_clock::now();
        testStartTime = std::chrono::system_clock::to_time_t(currentTime);
        auto durationMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(testInstance->testDuration.value());
        QTimer::singleShot(durationMilliseconds.count(), this, &MainWindowCustom::onTestFinish);
        testTimer = new QTimer(this);
        QObject::connect(testTimer, &QTimer::timeout, this, &MainWindowCustom::updateTimerDisplay);
        testTimer->start(1000);
        updateTimerDisplay();
    }

    if(testInstance->examQuestions.empty()) {
        completeTest();
        return;
    }

    questionNumberLabel->show();
    questionTextLabel->show();
    answerContainer->show();
    scrollArea->show();
    nextButton->show();
    backButton->show();
    finishButton->show();
    timerLabel->show();
    remainingTimeLabel->show();
    finishButton->setDisabled(true);
    backButton->setDisabled(true);
}

void MainWindowCustom::onQuestionNext() {
    storeAnswer();

    currentQuestionIndex++;

    refreshQuestionDisplay();
    retrieveAnswer();

    backButton->setDisabled(false);
    if(currentQuestionIndex == testInstance->examQuestions.size() - 1) {
        nextButton->setDisabled(true);
        finishButton->setDisabled(false);
    }
}

void MainWindowCustom::onQuestionBack() {
    storeAnswer();

    currentQuestionIndex--;

    refreshQuestionDisplay();
    retrieveAnswer();

    if(currentQuestionIndex == 0) {
        backButton->setDisabled(true);
    }
    nextButton->setDisabled(false);
    finishButton->setDisabled(true);
}

void MainWindowCustom::onTestFinish() {
    completeTest();
}

