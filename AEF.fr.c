#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


//CODE SOURCE PROJET: PROGRAMMATION D'UN AUTOMATE D'ETATS FINIS EN C

//Pour utiliser un tableau de chaines
typedef struct
{
    int numEtat;
    bool blockage;
    bool estFinal;
    struct tTrans *transit;
}tEtat;

typedef struct tTrans
{
    char* lettre;
    tEtat *suiv;
}tTransition;

typedef struct
{
    char lettre[100];
}tAlphabets;

typedef struct
{
    int numEtat;
    bool estFinal;
    char* lettre;
}tTransEtats;

void GetEtatsIn(int *EtatsAcc, tEtat *mEtat, int nbEtats, int nbAlphab, int *cmp, int cmp2);
bool Check_Alphabet(tEtat **all_etats, int nbEtats, tAlphabets *alphab, int nbAlphab, tTransEtats *TransTab, int *nbTrans, char *mot);
void Set_Transit(int numEtat, tAlphabets *alph, int nbAlph, tEtat **etats, int nbEtats);
tEtat *Set_Etats(int numEtat);
bool VeriferLettre(tAlphabets *alphab, int nbAlph, char* alph);
void Set_Alphabet(tAlphabets *alphab, int nbAlph);
void Presentation();

int main()
{
    Presentation();

///Introduction de l'alphabet
    tAlphabets *alphab;
    int nbAlph = 0;
    printf("\nALPHABETS:\n");

    do
    {
        printf("Donnez le nombre de lettres dans votre Alphabet (doit etre > 0): ");
        scanf("%d",&nbAlph);
    }while(nbAlph <= 0);

    //Allocation dynamique du tableau d'alphabet
    alphab = malloc(sizeof(tAlphabets) * nbAlph);

    //Ajouts des alphabets
    Set_Alphabet(alphab, nbAlph);
///-----------------------------------

///Introduction des états
    printf("\n\nETATS:\n");
    int nbEtats = 0, nbTrans;
    tEtat **all_etats;
    tTransEtats *TransitTab;
    do
    {
        printf("Donnez le nombre d'etats possibles (doit etre > 0): ");
        scanf("%d",&nbEtats);
    }while(nbEtats <= 0);

    all_etats = malloc(sizeof(tEtat) * nbEtats);

    //Ajouts des etats
    int indEtat = 0;
    for(indEtat = 0; indEtat < nbEtats; indEtat++)
    {
        all_etats[indEtat] = Set_Etats(indEtat);
    }

    //Ajouts des transitions
    for(indEtat = 0; indEtat < nbEtats; indEtat++)
    {
        Set_Transit(indEtat,alphab,nbAlph,all_etats,nbEtats);
    }
///-----------------------------------

///Etats inaccessibles
    int *EtatsAcc, nbEtatsAcc = 0;
    EtatsAcc = malloc(sizeof(int) * nbEtats);

    int i = 0;

    for(i = 0; i < nbEtats; i++)
    {
        EtatsAcc[i] = -2;
    }

    GetEtatsIn(EtatsAcc, all_etats[0], nbEtats, nbAlph, &nbEtatsAcc, 0);
    printf("\n\n");

    int j = 0;
    bool inacc = true;

    if(nbEtatsAcc < nbEtats)
    {
        printf("Les etats inaccessibles: ");
        for(i = 0; i < nbEtats; i++)
        {
            for(j = 0; j < nbEtatsAcc; j++)
            {
                if(EtatsAcc[j] == i)
                {
                    inacc = false;
                    break;
                }
                else
                {
                    inacc = true;
                }
            }

            if(inacc)
            {
                printf("q%d\t",i);
            }
        }
    }
    else
    {
        printf("Tout les etats sont accessibles.");
    }
    printf("\n");
///-----------------------------------

///Detection du mot
    int choix = 0;
    char mot[10];
    do
    {

        printf("\n1- Tester un mot. ");
        printf("\n2- Quitter. ");
        printf("\nVotre choix: ");
        scanf("%d",&choix);
        switch(choix)
        {
            case 1:
            {
                printf("\n\nDonnez un mot constitue a partire de l'alphabet {%s",alphab[0].lettre);
                for(i = 1; i < nbAlph; i++)
                {
                    printf(", %s",alphab[i].lettre);
                }
                printf("}: ");
                scanf("%s",mot);
                TransitTab = malloc(sizeof(tTransEtats) * strlen(mot));

                if(Check_Alphabet(all_etats,nbEtats,alphab,nbAlph, TransitTab, &nbTrans, mot))
                {
                    printf("\n\nLe mot est accepte !\n\nDiagramme de Transition:");
                    int t = 0;
                    for(t = 0; t < nbTrans; t++)
                    {
                        if(TransitTab[t].estFinal && !TransitTab[t+1].estFinal)
                        {
                            printf("\n((%d))--- %s ---> (%d)",TransitTab[t].numEtat, TransitTab[t].lettre, TransitTab[t+1].numEtat);
                            continue;
                        }

                        if(!TransitTab[t].estFinal && TransitTab[t+1].estFinal)
                        {
                            printf("\n (%d) --- %s --->((%d))",TransitTab[t].numEtat, TransitTab[t].lettre, TransitTab[t+1].numEtat);
                            continue;
                        }

                        if(!TransitTab[t].estFinal && !TransitTab[t+1].estFinal)
                        {
                            printf("\n (%d) --- %s ---> (%d)",TransitTab[t].numEtat, TransitTab[t].lettre, TransitTab[t+1].numEtat);
                            continue;
                        }

                        if(TransitTab[t].estFinal && TransitTab[t+1].estFinal)
                        {
                            printf("\n((%d))--- %s --->((%d))",TransitTab[t].numEtat, TransitTab[t].lettre, TransitTab[t].numEtat);
                            continue;
                        }
                    }
                }
                else
                {
                    printf("\n\nLe mot n'est pas accepte !\n");
                }

                break;
            }

            case 2: printf("\n\nBye :)"); break;

            default: printf("\nDonnez un choix entre 1 et 2"); break;
        }
        printf("\n");
    }while(choix != 2);

    printf("\n");
///-----------------------------------

///De-allocation
    free(EtatsAcc);
    free(alphab);
    free(all_etats);
    free(TransitTab);
    getchar();
///-----------------------------------
    return 0;
}

void GetEtatsIn(int *EtatsAcc, tEtat *mEtat, int nbEtats, int nbAlphab, int *cmp, int cmp2)
{
    int i = 0, j = 0;
    if(cmp2 < nbEtats && EtatsAcc != NULL) 
    {
        for(i = 0; i < nbAlphab; i++)
        {
            if(EtatsAcc[*cmp] != mEtat->numEtat)
            {
                bool exists = false;
                for(j = 0; j < *cmp; j++)
                {
                    if(EtatsAcc[j] == mEtat->numEtat)
                    {
                        exists = true;
                        break;
                    }
                }

                if(!exists)
                {
                    EtatsAcc[(*cmp)++] = mEtat->numEtat;
                }
                mEtat = mEtat->transit[i].suiv;
                GetEtatsIn(EtatsAcc, mEtat, nbEtats, nbAlphab, cmp, (cmp2+1));
            }
        }
    }
}

bool Check_Alphabet(tEtat **all_etats, int nbEtats, tAlphabets *alphab, int nbAlphab, tTransEtats *TransTab, int *nbTrans, char *mot)
{
    tEtat *mEtat = all_etats[0];
    bool acceptee = false;
    int i = 0, j = 0, l = 0;

    for(i = 0; i < nbAlphab; i++)
    {
        if(strlen(mot) < strlen(alphab[i].lettre))
        {
            free(TransTab);
            return false;
        }
    }

    for(i = 0; i < strlen(mot); i++)
    {
        for(j = 0; j < nbAlphab; j++)
        {
            if(mot[i] == mEtat->transit[j].lettre[0])
            {
                if(mEtat->transit[j].suiv == NULL)
                {
                    printf("\n\nEtat de blockage !\n");
                    return false;
                }

                acceptee = true;
                TransTab[l].numEtat = mEtat->numEtat;
                TransTab[l].estFinal = mEtat->estFinal;
                TransTab[l].lettre = mEtat->transit[j].lettre;
                l++;
                mEtat = mEtat->transit[j].suiv;
                break;
            }
            else
            {
                acceptee = false;
            }

        }
    }

    if(mEtat->estFinal && acceptee)
    {
        TransTab[l].estFinal = true;
        TransTab[l].numEtat = mEtat->numEtat;
        *nbTrans = l;
        return true;
    }
    else
    {
        return false;
    }

    return true;
}

void Set_Transit(int numEtat, tAlphabets *alph, int nbAlph, tEtat **etats, int nbEtats)
{
    printf("\n\n----------------------------------- Transitions de q%d -----------------------------------",numEtat);
    int i = 0;
    etats[numEtat]->transit = malloc(sizeof(tTransition) * nbAlph);
    int etatTrans;
    for(i = 0; i < nbAlph; i++)
    {
        do
        {
            printf("\nDonnez l'etat resultant de 'q%d' avec '%s'(utilisez un entier entre -1 ~ %d)\nNB: -1 signifie qu'il ne transite vers rien: ",numEtat,alph[i].lettre,nbEtats-1);
            scanf("%d",&etatTrans);
        }while(etatTrans < -1 || etatTrans >= nbEtats);

        etats[numEtat]->transit[i].lettre = alph[i].lettre;
        if(etatTrans != -1)
        {
            etats[numEtat]->transit[i].suiv = etats[etatTrans];
        }
        else
        {
            etats[numEtat]->transit[i].suiv = NULL;
        }

    }

    printf("\n--------------------------------------------------------------------------------------------");
}

tEtat *Set_Etats(int numEtat)
{
    tEtat *etat;
    etat = malloc(sizeof(*etat));
    etat->numEtat = numEtat;
    etat->estFinal = false;
    char f;
    printf("\nEst-ce que 'q%d' est un etat final ? (O/N): ",numEtat);
    do
    {
        scanf("%c",&f);
    }while(((f != 'O') && (f != 'o')) && ((f != 'N') && (f != 'n')));

    if(f == 'O' || f == 'o')
    {
        etat->estFinal = true;
    }

    return etat;
}

void Set_Alphabet(tAlphabets *alphab, int nbAlph)
{
    int i;
    printf("\n");
    for(i = 0; i < nbAlph; i++)
    {
        do
        {
            printf("Donnez la lettre numero %d de votre alphabet (NB: Aucune occurence n'est acceptee): ", i+1);
            scanf("%s",alphab[i].lettre);
        }while(!VeriferLettre(alphab,i,alphab[i].lettre));
    }
}

bool VeriferLettre(tAlphabets *alphab, int nbAlph, char* alph)
{
    int i;
    if(strlen(alph) <= 1)
    {
        for(i = 0; i < nbAlph; i++)
        {
            if(strcmp(alphab[i].lettre,alph) == 0)
            {
                return false;
            }
        }
    }
    else
    {
        return false;
    }

    return true;
}

void Presentation()
{
    printf("Projet: Construction d'AEF en langage C.\n");
    printf("By ROUANE Anis.\n");
    printf("\nAppuiez sur une touche pour commencer...");

    getchar();

    printf("\n\n----------------------------------------------------------------------\n\n");
}
