#include "sudoku.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <QDebug>


Sudoku::Sudoku()
{
    razGrille();
}

Sudoku::Sudoku(int grille[9][9])
{
    razGrille(grille);
}

Sudoku::~Sudoku()
{

}

void Sudoku::razGrille()
{
    for (int l=0; l<9; l++)
        for (int c=0; c<9; c++)
            this->grille[l][c]=0;
}

void Sudoku::razGrille(int grille[9][9])
{
    for (int l=0; l<9; l++)
        for (int c=0; c<9; c++)
            this->grille[l][c]=grille[l][c];
}

///
/// \brief Sudoku::aleatoire
/// Création d'une grille aléatoire
void Sudoku::aleatoire(int niveau)
{
    int compteur = 0;
    srand(time(NULL));

#define RAND9() ((rand()*9)/(RAND_MAX+1))
debut:
    while (true)
    {
        int l = RAND9();
        int c = RAND9();
        int val = RAND9()+1;
        if (verifiePossible(l, c, val))
        {
            grille[l][c] = val;
            if (compteur++ > 15) break;
        }
    }
    arretDemande = false;
    if (!resousGrille())
    {
        razGrille();
        goto debut;
    }
    compteur = 81;
    while (compteur > 40)
    {
        int l = RAND9();
        int c = RAND9();
        if (grille[l][c] == 0)
            continue;
        grille[l][c] = 0;
        compteur --;
    }
}

///
/// \brief Sudoku::verifieLigne
/// \param l ligne à vérifier (0-8)
/// \return true si ligne correcte
///
bool Sudoku::verifieLigne(int l)
{
    int result[10];
    for (int c=1; c<=9; c++)
    {
        result[c] = 0;
    }
    for (int c=0; c<9; c++)
    {
        int val = grille[l][c];
        if (val != 0 && ++result[val] > 1)
        {
//            qDebug() << "Invalide ligne" << l << val << result[val];
            return false;
        }
    }
    return true;
}

///
/// \brief Sudoku::verifieColonne
/// \param c colonne à vérifier (0-8)
/// \return true si colonne correcte
///
bool Sudoku::verifieColonne(int c)
{
    int result[10];
    for (int l=1; l<=9; l++)
    {
        result[l] = 0;
    }
    for (int l=0; l<9; l++)
    {
        int val = grille[l][c];
        if (val != 0 && ++result[val] > 1)
        {
//            qDebug() << "Invalide colonne" << c << val << result[val];
            return false;
        }
    }
    return true;
}

///
/// \brief Sudoku::verifieCarre
/// \param l l'une des lignes du carré (0-8)
/// \param c l'une des colonnes du carré (0-8)
/// \return true si carré correct
///
bool Sudoku::verifieCarre(int l, int c)
{
    int result[10];
    for (int i=1; i<=9; i++)
    {
        result[i] = 0;
    }
    int dl = (l/3)*3;
    int dc = (c/3)*3;
    for (int i=dl; i<dl+3; i++)
    {
        for (int j=dc; j<dc+3; j++)
        {
            int val = grille[i][j];
            if (val != 0 && ++result[val] > 1)
            {
//                qDebug() << "Invalide carré" << l << c << val << result[val];
                return false;
            }
        }
    }
    return true;
}

///
/// \brief Sudoku::ajouteValeur ajoute une valeur à la grille
/// \param l ligne
/// \param c colonne
/// \param val valeur à ajouter
/// \return true si grille correcte après ajout
///
bool Sudoku::ajouteValeur(int l, int c, int val)
{
    if (grille[l][c] != 0)
    {
        // Non vide, ajout impossible
        return false;
    }
    bool ok = verifiePossible(l, c, val);
    grille[l][c] = val;
    return ok;
}

int Sudoku::supprimeValeur(int l, int c)
{
    int valeur = grille[l][c];
    grille[l][c] = 0;
    return valeur;
}

///
/// \brief Sudoku::verifieGrille vérifie la validité actuelle
/// de la grille. Il peut y avoir des cases vides.
/// \return true si la grille est valide
///
bool Sudoku::verifieGrille()
{
    for (int i=0; i<9; i++)
    {
        if (!verifieLigne(i))
            return false;
        if (!verifieColonne(i))
            return false;
        if (!verifieCarre((i/3)*3, (i%3)*3))
            return false;
    }
    return true;
}

///
/// \brief Sudoku::verifiePossible vérifie si l'ajout
/// de la valeur à l'emplacement désigné est possible
/// \param l ligne
/// \param c colonne
/// \param val valeur
/// \return true si la grille est correcte après ajout
///
bool Sudoku::verifiePossible(int l, int c, int val)
{
    // Si non vide, impossible
    if (grille[l][c] != 0)
    {
        return false;
    }
    // Enregistrer la possibilite
    grille[l][c] = val;
    bool result = verifieLigne(l) & verifieColonne(c) & verifieCarre(l, c);
    grille[l][c] = 0;
    return result;
}

///
/// \brief Sudoku::resousGrille fonction récursive de recherche de solution
/// \param l
/// \param c
/// \return true si solution trouvée
///
bool Sudoku::resousGrille(int l, int c)
{
    for (int val = 1; val <= 9; val++)
    {
        if (arretDemande)
            return false;
        if (verifiePossible(l, c, val))
        {
            int nc = c, nl = l;
            grille[l][c] = val;
            // Rechercher la prochaine case vide
            do
            {
                if (arretDemande)
                    return false;
                if (nc < 8)
                {
                    // Colonne suivante
                    nc++;
                }
                else
                {
                    nc = 0;
                    if (nl < 8)
                    {
                        // Ligne suivante
                        nl++;
                    }
                    else
                    {
                        // On a fait toutes les cases, on a donc la solution.
                        return true;
                    }
                }
            } while (grille[nl][nc] != 0);
            // Appel récursif pour la case suivante.
            bool resultat = resousGrille(nl, nc);
            if (!resultat)
            {
                grille[l][c] = 0;
            }
            else
            {
                // Solution trouvÃ©e
                return true;
            }
        }
    }
    // On a fait toutes les valeurs sans trouver de solution.
    return false;
}

///
/// \brief Sudoku::resousGrille Recherche une solution pour la grille courante
/// \return true si solution trouvée.
///
bool Sudoku::resousGrille()
{
    // Rechercher la première case vide
    for (int l=0; l<9; l++)
    {
        for (int c=0; c<9; c++)
        {
            if (arretDemande)
                return false;
            if (grille[l][c] == 0)
            {
                return resousGrille(l, c);
            }
        }
    }
    // Grille déjà  remplie, la vérifier
    return true;
}

///
/// \brief Sudoku::solutionUnique fonction récursive de vérification
/// \param l
/// \param c
/// \return true si la grille n'accepte qu'une seule solution
///
int Sudoku::solutionUnique(int l, int c)
{
    int nombreSolutions=0;
    for (int val = 1; val <= 9; val++)
    {
        if (verifiePossible(l, c, val))
        {
            int nc = c, nl = l;
            grille[l][c] = val;
            // Rechercher la prochaine case vide
            do {
                if (nc < 8)
                {
                    // Colonne suivante
                    nc++;
                }
                else
                {
                    nc = 0;
                    if (nl < 8)
                    {
                        // Ligne suivante
                        nl++;
                    }
                    else
                    {
                        // On a fait toutes les cases, on a donc une solution.
                        return 1;
                    }
                }
            } while (grille[nl][nc] != 0);
            // Appel récursif pour la case suivante.
            int resultat = solutionUnique(nl, nc);
            grille[l][c] = 0;
            // Ajouter le nombre de solution au nombre global
            nombreSolutions += resultat;
        }
    }
    // On a fait toutes les valeurs sans trouver de solution.
    return nombreSolutions;
}

///
/// \brief Sudoku::solutionUnique Vérifie si la grille accepte une solution unique
/// \return le nombre de solutions
///
int Sudoku::solutionUnique()
{
    // Rechercher la première case vide
    for (int l=0; l<9; l++)
    {
        for (int c=0; c<9; c++)
        {
            if (grille[l][c] == 0)
            {
                int nombre = solutionUnique(l, c);
                if (nombre > 2)
                {
                    nombre = 2;
                }
                return nombre;
            }
        }
    }
    // Grille déjà  remplie, la vérifier
    return 0;
}

void Sudoku::afficheGrille()
{
    for (int i = 0; i < 9; i++)
    {
        QString l = "";
        for (int j = 0; j < 9; j++)
        {
            l+= QString::number(this->grille[i][j])+" ";
        }
        qDebug() << l;
    }
}

Sudoku* Sudoku::clone()
{
    return new Sudoku(this->grille);
}

bool Sudoku::identique(Sudoku *that)
{
    for (int l=0; l<9; l++)
        for (int c=0; c<9; c++)
            if (this->grille[l][c] != that->grille[l][c])
                return false;
    return true;
}
