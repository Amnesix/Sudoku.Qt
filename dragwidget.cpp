#include <QtWidgets>
#include <QDebug>

#include "dragwidget.h"
#include "draglabel.h"

DragWidget::DragWidget(Sudoku *sudoku, QString name, QWidget *parent, bool init)
    : QWidget(parent)
{
    int x;
    int y;

    this->sudoku = sudoku;
    this->setObjectName(name);

    for (int i=0; i<9; i++)
    {
        x = (i%3)*44+4;
        y = (i/3)*44+4;
        if (init)
        {
            lbl[i] = new DragLabel(QString::number(i+1), this);
            lbl[i]->move(x, y);
            lbl[i]->show();
            lbl[i]->setAttribute(Qt::WA_DeleteOnClose);
            lbl[i]->setObjectName("lbl"+name+QString::number(i+1));
        }
        else
        {
            frm[i] = new QFrame(this);
            frm[i]->move(x, y);
            frm[i]->show();
            frm[i]->setObjectName("frm"+name+QString::number(i+1));
        }
    }

    setAcceptDrops(true);
    setMinimumSize(138, 138);
}

///
/// \brief DragWidget::majValeurs
/// Mise à jour de l'affichage pour le bloc courant par rapport au sudoku
void DragWidget::majValeurs()
{
    for (int i=0; i<9; i++)
    {
        int l = frm[i]->objectName().mid(3,1).toInt()*3;
        int c = frm[i]->objectName().mid(4,1).toInt()*3;
        int n = frm[i]->objectName().mid(5,1).toInt()-1;
        l += n/3;
        c += n%3;
        QFrame *obj = static_cast<QFrame*>(childAt(frm[i]->geometry().x()+10, frm[i]->geometry().y()+10));
        if (sudoku->grille[l][c] != 0)
        {
            // A ne faire que si la case est actuellement vide
            if (obj->objectName().startsWith("frm"))
            {
                // Création d'un DragLabel avec la valeur de la case
                DragLabel *lbl = new DragLabel(QString::number(sudoku->grille[l][c]), this);
                lbl->move(frm[i]->geometry().x()+1, frm[i]->geometry().y()+1);
                lbl->setObjectName(QString("lbl").append(obj->objectName().right(3)));
                lbl->show();
            }
        }
        else
        {
            // Si la case doit est vide
            if (!obj->objectName().startsWith("frm"))
            {
                // Supprimer le chiffre
                delete obj;
            }
        }
    }
}

void DragWidget::fixerGrille()
{
    for (int i=0; i<9; i++)
    {
        int l = frm[i]->objectName().mid(3,1).toInt()*3;
        int c = frm[i]->objectName().mid(4,1).toInt()*3;
        int n = frm[i]->objectName().mid(5,1).toInt()-1;
        l += n/3;
        c += n%3;
        DragLabel *obj = static_cast<DragLabel*>(childAt(frm[i]->geometry().x()+10, frm[i]->geometry().y()+10));
        if (!obj->objectName().startsWith("frm"))
        {
            obj->setEnabled(false);
        }
    }
}

void DragWidget::defixerGrille()
{
    for (int i=0; i<9; i++)
    {
        int l = frm[i]->objectName().mid(3,1).toInt()*3;
        int c = frm[i]->objectName().mid(4,1).toInt()*3;
        int n = frm[i]->objectName().mid(5,1).toInt()-1;
        l += n/3;
        c += n%3;
        DragLabel *obj = static_cast<DragLabel*>(childAt(frm[i]->geometry().x()+10, frm[i]->geometry().y()+10));
        if (!obj->objectName().startsWith("frm"))
        {
            obj->setEnabled(true);
        }
    }
}

void DragWidget::ajouteValeur(QString dst, int valeur)
{
    int l = dst.mid(3,1).toInt()*3;
    int c = dst.mid(4,1).toInt()*3;
    int n = dst.mid(5,1).toInt()-1;
    l += n/3;
    c += n%3;
//    qDebug() << "Ajoute valeur" << valeur << "en" << l << c << n << dst;
    sudoku->ajouteValeur(l, c, valeur);
}

void DragWidget::supprimeValeur(QString src)
{
    int l = src.mid(3,1).toInt()*3;
    int c = src.mid(4,1).toInt()*3;
    int n = src.mid(5,1).toInt()-1;
    l += n/3;
    c += n%3;
//    qDebug() << "Ajoute valeur" << valeur << "en" << l << c << n << dst;
    sudoku->supprimeValeur(l, c);
}

void DragWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasText())
    {
        if (event->source() == this)
        {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        }
        else
        {
            event->acceptProposedAction();
        }
    }
    else
    {
        event->ignore();
    }
}

void DragWidget::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasText())
    {
        const QMimeData *mime = event->mimeData();
        QStringList pieces = mime->text().split(QRegExp("\\s+"),
                             QString::SkipEmptyParts);
        QPoint position = event->pos();
        QPoint hotSpot;

        QList<QByteArray> hotSpotPos = mime->data("application/x-hotspot").split(' ');
        if (hotSpotPos.size() == 2)
        {
            hotSpot.setX(hotSpotPos.first().toInt());
            hotSpot.setY(hotSpotPos.last().toInt());
        }

        // Recherche sur quelle position on lache le nombre
        QFrame *child = static_cast<QFrame*>(childAt(position));

        if (child == NULL)
        {
            child = static_cast<QFrame*>(childAt(hotSpot));
            if (child == NULL)
            {
                qDebug() << "Abandon child NULL" << (child != NULL ? child->objectName():"");
                return;
            }
        }

        DragLabel *newLabel;
        QString src;
        foreach (QString piece, pieces)
        {
            newLabel = new DragLabel(piece, this);
            newLabel->move(position - hotSpot);
            newLabel->show();
            newLabel->setAttribute(Qt::WA_DeleteOnClose);

            if (child && child->objectName().startsWith("frm"))
                newLabel->setObjectName("lbl"+child->objectName().right(3));

            position += QPoint(newLabel->width(), 0);
        }
#ifdef DEBUG
        qDebug() << event->source()->objectName() << "-->" << this->objectName()
                 << "(" << (child != NULL ? child->objectName() : "NULL")<< "," << position << ")";
#endif
        // Action en fonction du type de déplacement
        if (event->source()->objectName().compare("SRC") == 0)
        {
            // Déplacement à partir de la source
            if (event->source() != this)
            {
                event->setDropAction(Qt::CopyAction);
                event->accept();
                newLabel->move(child->geometry().x()+1,child->geometry().y()+1);
                ajouteValeur(child->objectName(), newLabel->text().toInt());
            }
            else
                delete newLabel;
        }
        else
        {
            // Déplacement à partir du jeu
            event->acceptProposedAction();
            if (this->objectName().compare("SRC") == 0)
            {
#ifdef DEBUG
                qDebug()<<"K1"<<child->objectName()<<event->mimeData()->objectName();
#endif
                // Suppression du label (on retire le chiffre du jeu)
                supprimeValeur(event->mimeData()->objectName());
                delete newLabel;
            }
            else
            {
                if (child->objectName().startsWith("frm"))
                {
#ifdef DEBUG
                    qDebug()<<"K2"<<child->objectName()<<event->mimeData()->objectName();
#endif
                    // Positionnement du chiffre sur la case
                    newLabel->move(child->geometry().x()+1,child->geometry().y()+1);
                    ajouteValeur(child->objectName(), newLabel->text().toInt());
                    supprimeValeur(event->mimeData()->objectName());
                }
                else if (this == event->source())
                {
#ifdef DEBUG
                    qDebug()<<"K3"<<child->objectName()<<event->mimeData()->objectName();
#endif
                    // On reste dans la même case
                    event->setDropAction(Qt::CopyAction);
                    event->accept();
                    delete newLabel;
                }
#ifdef DEBUG
                else
                    qDebug()<<"K4"<<child->objectName()<<event->source()->objectName();
#endif
            }
        }
    }
    else
    {
        event->ignore();
    }
#ifdef DEBUG
    sudoku->afficheGrille();
#endif
}

void DragWidget::mousePressEvent(QMouseEvent *event)
{
    QLabel *child = static_cast<QLabel*>(childAt(event->pos()));
    if (!child || child->objectName().startsWith("frm"))
        return;

    if (!child->isEnabled())
        return;

    depart = event->pos() - child->pos();

    QMimeData *mimeData = new QMimeData;
    mimeData->setText(child->text());
    mimeData->setObjectName(child->objectName());
    mimeData->setData("application/x-hotspot",
                      QByteArray::number(depart.x()) + " " + QByteArray::number(depart.y()));

    QPixmap pixmap(child->size());
    child->render(&pixmap);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setPixmap(pixmap);
    drag->setHotSpot(depart);

    // Uniquement du déplacement
    Qt::DropAction dropAction = drag->exec(Qt::MoveAction | Qt::CopyAction, Qt::MoveAction);

    if (dropAction == Qt::MoveAction)
        child->close();
}
