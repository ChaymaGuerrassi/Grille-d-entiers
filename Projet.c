#include <stdio.h>
#include <stdlib.h>
#include "fonctions.h"
#include <MLV/MLV_all.h>
#include <time.h>
#include <string.h>
//Définition de la macro.
#define N 20
#define TAILLE_MAX 1000


int main(int argc, char *argv[]){

    //Initialisation des variables
	char affichage, methode;
	char *nom_fichier[25];
	int i, taille_grille = 20, valeur_max;
	int somme_totale = 0, coups = 0, somme;
    int grille[taille_grille][taille_grille];
    FILE *fichier;
    
	int x, y;
	char choix_action;
	int X1, Y1, X2, Y2;//Variables pour stocker les coordonnées des cases.

	srand(time(NULL));
    

    //Lecture des paramètres d'affichage (ASCII ou graphique), de méthode (Création ou lecture d'une grille).
	for(i = 0; i < argc; i++){
        
		if(argv[i][0] == '-'){
            
            if(argv[i][1] == 'a' || argv[i][1] == 'g') {
                
                affichage = argv[i][1];
                methode = argv[i][2];
            }
            
            else{
                
                affichage = argv[i][2];
                methode = argv[i][1];
            }
            
		}
		else
			*nom_fichier = argv[i];
    }

	
    
    
	if(methode == 'c'){//Le joueur choisi de créer une matrice
        
        
        printf("Choisissez la taille de la grille : ");// Détermination de la taille de la grille
        scanf("%d", &taille_grille);

        while(taille_grille > N){// Recommence tant que la taille est supérieure à 20
            printf("Valeur trop grande (la taille ne doit pas être supérieure à 20)\nChoisissez la taille de la grille : ");
            scanf("%d", &taille_grille);
        }
        printf("Choisissez la valeur maximale dans la grille : ");// Détermination de la valeur max des nombres
        scanf("%d", &valeur_max);

        cree_grille(taille_grille, valeur_max, grille);
	}
    
    
   
	else if (methode == 'l'){//Le joueur choisi d'utiliser une matrice existante dans un fichier texte
        
        
        FILE *fichier;
        fichier = fopen(*nom_fichier, "r");
        
        if (fichier == NULL){
            
            printf("Fichier non valide. \n");
        }
        
        else{
            
            taille_grille = compteLignes(fichier) - 2;
            fclose(fichier);
        }
        
        fichier = fopen(*nom_fichier, "r"); //Ici, le fichier est ouvert et fermé deux fois car les valeurs ne sont pas bonnes si c'est pas fait.
        
        lirefichier(fichier, taille_grille, grille);
        

        printf("Choisissez la valeur maximale dans la grille : ");// Détermination de la valeur max des nombres
        scanf("%d", &valeur_max);
    }  
        
    
    
    if(affichage == 'a'){

        while( somme_totale != 100 && choix_action != 'q'){
        //Boucle Pricipale
            
            printf("Somme : %d, Coups : %d\n", somme_totale, coups);
            affichage_grille_term(taille_grille, grille);

            printf("Que souhaitez vous faire ? 'c' : continuer, 's', sauvegarder, 'q' : quitter : ");
            scanf(" %c", &choix_action);

            if(choix_action == 'c'){
                
                printf("Choisissez la ligne de la première case : ");
                scanf(" %d", &X1);
                printf("Choisissez la colonne de la première case : ");
                scanf(" %d", &Y1);
                printf("Choisissez la ligne de la deuxième case : ");
                scanf(" %d", &X2);
                printf("Choisissez la colonne de la deuxième case : ");
                scanf(" %d", &Y2);

                while(coupPossible(X1, Y1, X2, Y2, taille_grille) == 0){
                    
                    printf("Alignement invalide !\nVeuillez saisir de nouvelles cases !\n");
                    printf("Choisissez la ligne de la première case : ");
                    scanf(" %d", &X1);
                    printf("Choisissez la colonne de la première case : ");
                    scanf(" %d", &Y1);
                    printf("Choisissez la ligne de la deuxième case : ");
                    scanf(" %d", &X2);
                    printf("Choisissez la colonne de la deuxième case : ");
                    scanf(" %d", &Y2);
                }
                
              
                somme = somme_case(X1, Y1, X2, Y2, taille_grille, grille);
                somme_totale += somme;
                coups += 1;

                reapprovisionnement_total(X1, Y1, X2, Y2, valeur_max, taille_grille, grille);
                
            }
            
            else if (choix_action == 's'){//Sauvegarde de la matrice Actuelle.
                    
                FILE* fichier = NULL;
                 
                fichier = fopen(*nom_fichier, "w+");
                    
                if (fichier != NULL){
                    
                    sauvegardeFichier(fichier, taille_grille, grille, somme, coups, valeur_max);
                    
                    printf("Sauvergarde faite !");
                    
                    fclose(fichier);
                }
                
                else{
                    
                    printf("Fichier non valide ou non renseigné. \n");
                        
                }
            }
        }

        if(somme_totale == 100){
            printf("Vous avez atteint la somme de 100 en %d coups !\n", coups);
        }

    }

    if(affichage == 'g'){

        MLV_create_window("Projet", "Projet", 1500, 1000);

        while(somme_totale != 100 && choix_action != 'q'){

            affichage_grille_graph(taille_grille, grille, somme_totale, coups);

            MLV_actualise_window();

            MLV_wait_mouse(&x, &y);

            if(x > 1050 && x < 1450 && y > 200 && y < 325)

                fichier = fopen(*nom_fichier, "w+");
                sauvegardeFichier(fichier, taille_grille, grille, somme, coups, valeur_max);
                fclose(fichier);

            if(x > 1050 && x < 1450 && y > 450 && y < 575)
                choix_action = 'q';

            if(x >= 0 && x < 1000){

                X1 = determine_case(x, taille_grille);
                Y1 = determine_case(y, taille_grille);

                MLV_actualise_window();

                MLV_wait_mouse(&x, &y);

                X2 = determine_case(x, taille_grille);
                Y2 = determine_case(y, taille_grille);

                MLV_actualise_window();

                printf("Case 1 : %d %d, Case 2 : %d %d\n", X1, Y1, X2, Y2);

                while(coupPossible(X1, Y1, X2, Y2, taille_grille) == 0){
                    printf("Alignement invalide !\n");

                    MLV_wait_mouse(&x, &y);

                    X1 = determine_case(x, taille_grille);
                    Y1 = determine_case(y, taille_grille);

                    MLV_actualise_window();

                    MLV_wait_mouse(&x, &y);

                    X2 = determine_case(x, taille_grille);
                    Y2 = determine_case(y, taille_grille);

                    MLV_actualise_window();
                }

                somme = somme_case(X1, Y1, X2, Y2, taille_grille, grille);
                somme_totale += somme;
                coups += 1;
                printf("somme = %d\n", somme_totale);

                reapprovisionnement_total(X1, Y1, X2, Y2, valeur_max, taille_grille, grille);
            }


        }


        MLV_free_window();
    }

	return 0;
}
