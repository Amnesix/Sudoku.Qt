#include "sudokuwidget.h"
#include <QTime>

SudokuWidget::SudokuWidget(Sudoku *sudoku, QWidget *parent) : QWidget(parent)
{
    this->sudoku = sudoku;

    QHBoxLayout *hLayout = new QHBoxLayout;
    QVBoxLayout *vLayout = new QVBoxLayout;
    QGridLayout *gLayout = new QGridLayout;
    QVBoxLayout *vLayoutBtn = new QVBoxLayout;

    solution = new QPushButton(tr("Solution"));
    verifier = new QPushButton(tr("Vérifier"));
    fixer = new QPushButton(tr("Fixer"));
    raz = new QPushButton(tr("Annuler"));
    nouveau = new QPushButton(tr("Nouveau"));
    info = new QLabel();
    info->setObjectName(tr("info"));

    hLayout->addItem(vLayout);
    hLayout->addItem(gLayout);

    for (int l=0; l<3; l++)
        for (int c=0; c<3; c++)
        {
            this->jeu[l][c] = new DragWidget(sudoku, QString::number(l).append(QString::number(c)));
            connect(this, SIGNAL(majGrille()), jeu[l][c],SLOT(majValeurs()));
            connect(this, SIGNAL(grilleFixee()), jeu[l][c], SLOT(fixerGrille()));
            connect(this, SIGNAL(grilleDefixee()), jeu[l][c], SLOT(defixerGrille()));
            gLayout->addWidget(jeu[l][c], l, c);
        }

    vLayout->addWidget(new DragWidget(sudoku, "SRC", 0, true));
    vLayout->addLayout(vLayoutBtn);
    vLayoutBtn->addWidget(info);
    vLayoutBtn->addWidget(verifier);
    vLayoutBtn->addWidget(solution);
    vLayoutBtn->addWidget(nouveau);
    vLayoutBtn->addWidget(raz);
    vLayoutBtn->addWidget(fixer);

    this->setLayout(hLayout);
    this->setWindowTitle(QObject::tr("Sudoku"));

    this->connect(solution, SIGNAL(clicked()), this, SLOT(trouverSolution()));
    this->connect(verifier, SIGNAL(clicked()), this, SLOT(verifierGrille()));
    this->connect(raz, SIGNAL(clicked()), this, SLOT(razGrille()));
    this->connect(fixer, SIGNAL(clicked()), this, SLOT(fixerGrille()));
    this->connect(nouveau, SIGNAL(clicked()), this, SLOT(nouvelleGrille()));
    this->connect(sudoku, SIGNAL(resultReady(bool)), this, SLOT(handleResult(bool)));
//    this->connect(sudoku, SIGNAL(finished()), sudoku, SLOT(deleteLater()));

    setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);

    timer = new QTimer();
    timer->setInterval(1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(majTimer()));

    QFile *style = new QFile(".\\style.qss");
    if (!style->exists())
        style = new QFile("..\\style.qss");
    if (style->open(QIODevice::ReadOnly))
    {
        QString styleSheet = style->readAll();
        this->setStyleSheet(styleSheet);
    }
#ifdef DEBUG
    else
        qDebug() << "raté";
#endif
}

SudokuWidget::~SudokuWidget()
{
}

void SudokuWidget::verifierGrille()
{
    if (sudoku->verifieGrille())
        info->setText("Grille OK");
    else
        info->setText("Grille NOK");
}

void SudokuWidget::trouverSolution()
{
    if (sudoku->isRunning())
    {
        timer->stop();
        sudoku->arretDemande = true;
    }
    else
    {
        compteur = 0;
        timer->start();
        sudoku->start();
    }
}

void SudokuWidget::handleResult(bool solution)
{
    if (solution)
    {
        timer->stop();
        emit majGrille();
#ifdef DEBUG
        sudoku->afficheGrille();
#endif
        info->setText(QTime(0, 0, compteur).toString("mm:ss"));
    }
    else
    {
        timer->stop();
        if (sudoku->arretDemande)
            info->setText("Stop");
        else
            info->setText("Impossible");
    }
}

void SudokuWidget::razGrille()
{
    if (base.isEmpty())
        sudoku->razGrille();
    else if (sudoku->identique(base.last()))
    {
        base.removeLast();
        emit grilleFixee();
        if (base.isEmpty())
            emit grilleDefixee();
        else
            sudoku->razGrille(base.last()->grille);
    }
    else
    {
        sudoku->razGrille(base.last()->grille);
    }
    int n=base.size();
    if (n == 0)
        fixer->setText(tr("Fixer"));
    else
        fixer->setText(QString(tr("Fixer")).append(" (").append(QString::number(n)).append(")"));
    emit majGrille();
#ifdef DEBUG
    sudoku->afficheGrille();
#endif
}

void SudokuWidget::fixerGrille()
{
    base.append(sudoku->clone());
    fixer->setText(QString(QString(tr("Fixer")).append(" (").append(QString::number(base.size())).append(")")));
    emit grilleFixee();
}

void SudokuWidget::majTimer()
{
    compteur++;
    info->setText(QTime(0, compteur/60, compteur%60).toString("mm:ss"));
}

void SudokuWidget::nouvelleGrille()
{
    if (sudoku->isRunning())
        return;
    // Supprimer toutes les étapes intermédiaires.
    base.clear();
    fixer->setText(tr("Fixer"));
    // Effacer complètement la grille
    sudoku->razGrille();
    emit majGrille();
    // Créer une nouvelle grille aléatoire
    sudoku->aleatoire(0);
    // Et mettre à jour l'affichage
    emit majGrille();
#ifdef DEBUG
    sudoku->afficheGrille();
    qDebug() << "----";
#endif
}
