#include <QApplication>
#include "dragwidget.h"
#include "sudoku.h"
#include "sudokuwidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Sudoku *sudoku = new Sudoku();
    SudokuWidget *mainWidget = new SudokuWidget(sudoku);

    mainWidget->show();
    return app.exec();
}
