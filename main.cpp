#include "ExamTest.h"
#include "MainWindowCustom.h"

#include <QApplication>

ExamTest createExamTest();

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    auto test = createExamTest();
    MainWindowCustom mainWindow(&test);
    mainWindow.show();
    return app.exec();
}

ExamTest createExamTest() {
    using namespace std::chrono_literals;

    return ExamTest(200s)
    .addQuestion(
        ExamQuestion("Яка найдовша ріка у світі?", 1)
            .addAnswer({"Ніл", true})
            .addAnswer({"Амазонка", false})
            .addAnswer({"Янцзи", false})
            .addAnswer({"Міссісіпі", false})
    )
    .addQuestion(
        ExamQuestion("Хто був першим президентом Сполучених Штатів?", 2)
            .addAnswer({"Томас Джефферсон", false})
            .addAnswer({"Джордж Вашингтон", true})
            .addAnswer({"Джон Адамс", false})
            .addAnswer({"Авраам Лінкольн", false})
    )
    .addQuestion(
        ExamQuestion("Якого кольору не існує у веселці?", 1)
            .addAnswer({"Рожевий", true})
            .addAnswer({"Червоний", false})
            .addAnswer({"Зелений", false})
            .addAnswer({"Синій", false})
    )
    .addQuestion(
        ExamQuestion("У якому році була заснована Організація Об'єднаних Націй?", 2)
            .addAnswer({"1945", true})
            .addAnswer({"1950", false})
            .addAnswer({"1939", false})
            .addAnswer({"1960", false})
    );
}
