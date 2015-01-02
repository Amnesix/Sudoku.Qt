#ifndef DRAGWIDGET_H
#define DRAGWIDGET_H

#include <QWidget>
#include <QFrame>
#include "draglabel.h"
#include "sudoku.h"

QT_BEGIN_NAMESPACE
class QDragEnterEvent;
class QDropEvent;
QT_END_NAMESPACE

class DragWidget : public QWidget
{
    Q_OBJECT

public:
    DragWidget(Sudoku *sudoku, QString name, QWidget *parent = 0, bool init = false);

public slots:
    void majValeurs();
    void fixerGrille();
    void defixerGrille();

signals:
    void clicked();

protected:
    void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;
    void dropEvent(QDropEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:
    Sudoku *sudoku;
    DragLabel* lbl[9];
    QFrame *frm[9];
    QPoint depart;

    void ajouteValeur(QString dst, int valeur);
    void supprimeValeur(QString src);
};

#endif // DRAGWIDGET_H
