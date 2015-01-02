#ifndef SUDOKUWIDGET_H
#define SUDOKUWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QFile>
#include <QList>
#include <QDebug>
#include <QTimer>
#include "sudoku.h"
#include "dragwidget.h"

class SudokuWidget : public QWidget
{
    Q_OBJECT

public:
    SudokuWidget(Sudoku *sudoku, QWidget *parent = 0);
    ~SudokuWidget();

    Sudoku *sudoku;
    QList<Sudoku*> base;

public slots:
    void verifierGrille();
    void trouverSolution();
    void handleResult(bool solution);
    void razGrille();
    void fixerGrille();
    void majTimer();
    void nouvelleGrille();

signals:
    void majGrille();
    void grilleFixee();
    void grilleDefixee();

private:
    QPushButton *solution;
    QPushButton *verifier;
    QPushButton *fixer;
    QPushButton *raz;
    QPushButton *nouveau;
    DragWidget *jeu[3][3];
    QLabel *info;
    QTimer *timer;
    int compteur;
};

#endif // SUDOKUWIDGET_H
