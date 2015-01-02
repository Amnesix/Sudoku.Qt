#ifndef SUDOKU_H
#define SUDOKU_H

#include <QObject>
#include <QThread>

#define DEBUG

class Sudoku : public QThread
{
    Q_OBJECT
public:
    int grille[9][9];

    Sudoku();
    Sudoku(int grille[9][9]);
    ~Sudoku();
    // Initialisation
    void razGrille();
    void razGrille(int grille[9][9]);
    void aleatoire(int niveau);
    // Vérification
    bool verifieLigne(int l);
    bool verifieColonne(int c);
    bool verifieCarre(int l, int c);
    bool verifieGrille();
    bool ajouteValeur(int l, int c, int val);
    int  supprimeValeur(int l, int c);
    // Résolution
    bool verifiePossible(int l, int c, int val);
    bool resousGrille(int l, int c);
    bool resousGrille();
    int solutionUnique(int l, int c);
    int solutionUnique();
    // Affichage
    void afficheGrille();
    // Opération de copie
    Sudoku* clone();
    bool identique(Sudoku *that);
    bool arretDemande;

    void run() Q_DECL_OVERRIDE
    {
        arretDemande = false;
        emit resultReady(resousGrille());
    }

signals:
    void resultReady(bool result);
};

#endif // SUDOKU_H
