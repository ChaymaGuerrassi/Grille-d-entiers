#include <stdio.h>
#include <stdlib.h>
#include <MLV/MLV_all.h>
#include <time.h>
#include <string.h>
#define TAILLE_MAX 1000

//Fonctions nécessaires au bon déroulement du jeu.

//Fonctions principales


void cree_grille(int taille_grille, int valeur_max, int grille[][taille_grille]){
    //    Fonction qui stocke des entiers compris entre [-MAX,MAX] dans une matrice prise en paramètre de taille prise en paramètre.
    
    int i, j;
    
    for(i = 0; i < taille_grille; i++){
        
        for(j = 0; j < taille_grille; j++){
            
            grille[i][j] = (rand()%(valeur_max*2)) - valeur_max;
        }
    }
}


void lirefichier(FILE *fichier, int taille, int grille[][taille]){
    /*Fonction qui lit les caractère d'un fichier et les stocke dans une matrice n*n.*/
    
    int i_cha = 0;
     int ligne = 0;
     int i = 0;
     int j = 0;
     int caractere = 0;
     char cha[100] = "";

     
     while(caractere != EOF){
     
         caractere = fgetc(fichier);
         
     if (ligne >= 2) {
             if (caractere == ' ' || caractere == '\n'){

                 
                 grille[i][j] = atoi(cha);
                 j++;
                 
                 if (j == taille) {
                     j = 0;
                     i++;
                 }
             
                 memset(cha, 0, sizeof(cha));
             
                 i_cha = -1;
             }
         
             else{
                 cha[i_cha] = caractere;
             }
         
             i_cha++;
         }
      
     if (caractere == '\n') {
         ligne++;
     }
     }
}
    

void sauvegardeFichier(FILE *fichier, int taille, int grille[][taille], int somme, int coups, int valeur_max ){
    
    //Fonction qui sauvegarde la matrice actuelle dans un fichier fourni en respectant le format specifié dans le sujet.
    
    int i,j;
    
    //Première ligne pour la taille
    fprintf(fichier, "%d\n" ,taille);
    
    //Deuxième ligne pour la somme atteinte, le nombre de coup et la valeur max.
    fprintf(fichier, "%d %d %d\n", somme, coups, valeur_max);
    
    //Les entiers de la matrice séparés par des espaces.
    for (i=0; i<taille; i++){
        
        for (j=0; j<taille; j++){
            
            fprintf(fichier, "%d " ,grille[i][j]);
            
            if (j==taille-1)
                
                fprintf(fichier,"\n");
        }
    }
}



//Fonctions d'affichage

void affichage_grille_term(int taille_grille, int grille[][taille_grille]){
	//Fonction qui affiche une matrice de taille n*n prises en paramètre, sous forme d'une grille de jeu. Elle alligne les nombres selon des conditions.
    
    int X = taille_grille;
	int i, j;

	for(i = 0; i < X; i++){
        
		printf("       %d", i);
	}
	
    printf("\n      *");
	
    for(i = 0; i < X-1; i++){
        
		printf("*********");
	}
    
	for(i = 0; i < X; i++){
        
		printf("\n%d  *   ", i);
        
		for(j = 0; j < X; j++){

			if(grille[i][j] >= 10  || (grille[i][j] > -10 && grille[i][j] < 0)){
				printf("%d      ", grille[i][j]); 
			}

			if(grille[i][j] < 10 && grille[i][j] >= 0){
				printf("%d       ", grille[i][j]); 
			}

			if(grille[i][j] < -9){
				printf("%d     ", grille[i][j]); 
			}
		}
	}
    
	printf("\n");
}


void affichage_grille_graph(int taille_grille, int grille[][taille_grille], int somme_totale, int coups){
    //Fonction d'affichage graphique de la grille.
    
    int i, j;
    char text[10];

    MLV_draw_filled_rectangle(0, 0, 1500, 1000, MLV_COLOR_GREY);

    for(i=0; i<= taille_grille; i++){
        MLV_draw_line(0, 0 + (i * 1000/taille_grille), 1000, 0 + (i * 1000/taille_grille), MLV_COLOR_BLACK);//Ligne horizontale
        MLV_draw_line(0 + (i * 1000/taille_grille), 0, 0 + (i * 1000/taille_grille), 1000, MLV_COLOR_BLACK);//Ligne verticale
    }

    MLV_draw_filled_rectangle(1000, 0, 1500, 1000, MLV_COLOR_DARK_GREY);
    MLV_draw_rectangle(1000, 0, 1500, 1000, MLV_COLOR_BLACK);

    for(i =0; i<taille_grille; i++){
        for(j = 0; j<taille_grille; j++){

            sprintf(text, "%d", grille[i][j]);

            MLV_draw_text((1000/taille_grille)/2 + (j * 1000/taille_grille), (1000/taille_grille)/2 + (i * 1000/taille_grille), text, MLV_COLOR_BLACK);
        }
    }

    MLV_draw_text(1200, 75, "Grille d'entiers", MLV_COLOR_BLACK);
    MLV_draw_rectangle(1025, 25, 450, 125, MLV_COLOR_BLACK);

    sprintf(text, "Somme : %d", somme_totale);
    MLV_draw_text(1030, 725, text, MLV_COLOR_BLACK);

    sprintf(text, "Coups : %d", coups);
    MLV_draw_text(1030, 850, text, MLV_COLOR_BLACK);

    MLV_draw_rectangle(1025, 700, 450, 275, MLV_COLOR_BLACK);

    MLV_draw_rectangle(1050, 200, 400, 125, MLV_COLOR_RED);
    MLV_draw_filled_rectangle(1051, 201, 398, 123, MLV_COLOR_DARK_GREEN);
    MLV_draw_text(1215, 250, "Sauvegarde", MLV_COLOR_BLACK);

    MLV_draw_rectangle(1050, 4500, 400, 125, MLV_COLOR_WHITE);
    MLV_draw_filled_rectangle(1051, 451, 398, 123, MLV_COLOR_RED);
    MLV_draw_text(1215, 500, "Quitter", MLV_COLOR_BLACK);

}


int determine_case(int x, int taille_grille){
    //Convertir les coordonnées du clic en coordonnées de case.
    
    //Dans cette fonction, y correspond à la ligne et x à la colonne
    int i = 0;
    int testx = x;
    while(testx > 1000/taille_grille){
        testx = x;
        testx = testx - i * (1000/taille_grille);
        if(testx > 1000/taille_grille)
            i += 1;
    }
    return i;
}



//Fonctions de calcul

int compteLignes(FILE *fichier){
    //Fonction qui compte les lignes d'un fichier (et ainsi determiner la taille de la grille lors de la lecture du fichier)
    
    int nb_lignes = 0;
    int caractere = 0;
    
    while (caractere != EOF){
        
        caractere = fgetc(fichier);
        
        if (caractere == '\n')//Si saut à la ligne.
            
            nb_lignes++;
    }
    
    return nb_lignes;
}


int max(int x, int y){
    //Renvoi la valeur maximale entre x et y

    if(x > y)//x est supérieur à y
        return x;
    else//x est inférieur à y
        return y;
}


int min(int x, int y){
    //Renvoi la valeur minimale entre x et y

    if(x < y)//x est inférieur à y
        return x;
    else//x est supérieur à y
        return y;
}


int coupPossible(int x1, int y1, int x2, int y2,int taille_grille){
    
    //Fonction qui vérifie si les deux cases forment un segment valide.
    
    int possible = 0, verticale = 0, horizontale = 0;
    int diagonale = 0;
    int x,y; //variable utilisée pour reperer la diagonale
    
    //Verticale
    if( y1 == y2){
        verticale = 1;
        possible = 1; 
    }
    //Horizontale
    if (x1 == x2){
        horizontale = 1;
        possible = 1;
    }
        
    //Diagonale
    for (x= 0 ; x< taille_grille; x++){
        
        y = x + y1 - x1;
        
        //Parcours la diagonale dans un sens.
        if (y>=0 && y< taille_grille){
            if (x==x2 && y==y2 ) //Si l'une des cases de la diagonale correspond à la deuxième case passée en argument.
            
                diagonale = 1;
            }
        y = -x + y1 + x1;
        //Parcours la diagonale dans l'autre sens.
        if (y>=0 && y < taille_grille){
            if (x==x2 && y==y2 )
                diagonale = 1;
        }
    }
    
    if (diagonale==1){
        
        possible = 1;
    }
    
    if(verticale != 0 && horizontale != 0 && diagonale != 0)
        possible = 0;
    
    return possible;
    
}


void reapprovisionnement(int x, int y, int valeur_max,  int taille_grille, int grille[][taille_grille]){
    
    //Fonction qui réapprovisionne une case de la matrice avec les cases supérieures.
    
    //Si la case est dans la première ligne de la matrice.
    if (x == 0){
        
        // On réapprovisionne que la case
        grille[x][y] = (rand()% 2* valeur_max)- valeur_max;
    }
    
    else{
        //Remplacer les entiers par les cases supérieures.
        for (int i = x-1; i>0; i-- ){
            
            grille[i+1][y] = grille[i][y];
            //Réapprovisionner la case du haut avec un entier compris entre [-MAX,MAX].
        }
        
        grille[0][y] = (rand()% 2* valeur_max)- valeur_max;
        
    }
}   



void reapprovisionnement_total(int x, int y, int x1, int y1, int valeur_max, int taille_grille, int grille[][taille_grille]){

//    Fonction utilisant la fonction reapprovisionnement, qui reapprovisionne la totalité des cases du segment.
    
    int i = min(x, x1), j = min(y, y1);
    int cas;

    if(x == x1)
        cas = 1;

    if(y == y1)
        cas = 2;    

    if( x != x1 && y != y1){
        if(max(x, x1) == x)
            cas = 4;
    }

    if( x != x1 && y != y1){
        if(max(x, x1) == x1)
            cas = 5;
    }

    if((x > x1 && y > y1) || (x1 > x && y1 > y))
        cas = 3;
        

    switch(cas){

        case 1 :
            for(j =min(y, y1); j <= max(y, y1); j++){
                reapprovisionnement(i, j, valeur_max, taille_grille, grille);
            }
            break;

        case 2 :
            for(i =min(x, x1); i <= max(x, x1); i++){
                reapprovisionnement(i, j, valeur_max, taille_grille, grille);
            }
            break;

        case 3 :
            while(i<= max(x, x1)){
                while(j<= max(y, y1)){
                    reapprovisionnement(i, j, valeur_max, taille_grille, grille);
                    i +=1;
                    j+=1;
                }
            }
            break;

        case 4 :
            i = max(x, x1);
            j = min(y, y1);
            while(i >= min(x, x1)){
                while(j<= max(y, y1)){
                    reapprovisionnement(i, j, valeur_max, taille_grille, grille);
                    i -=1;
                    j+=1; 
                }
            }
            break;

        case 5 :
            i = min(x, x1);
            j = max(y, y1);

            while(i<= max(x, x1)){
                while(j>= min(y, y1)){
                    reapprovisionnement(i, j, valeur_max, taille_grille, grille);
                    i +=1;
                    j-=1;
                    
                }
            }
            break;
        default : break;

    }
}


int somme_case(int x, int y, int x1, int y1, int taille_grille, int grille[][taille_grille]){
    
    //Fonction qui renvoie la somme des cases du segment.
    
    int somme = 0;
    int i = min(x, x1), j = min(y, y1);

    if(y == y1){
        for(i =min(x, x1); i <= max(x, x1); i++){
            somme += grille[i][j];
            printf("%d, %d, %d\n", grille[i][j], i, j);         
        }
        return somme;

    }

    if(x == x1){
        for(j =min(y, y1); j <= max(y, y1); j++){
            somme += grille[i][j];
            printf("%d, %d, %d\n", grille[i][j], i, j);
            
        }
        return somme;

    }

    if((x > x1 && y > y1) || (x1 > x && y1 > y)){
        while(i<= max(x, x1)){
            while(j<= max(y, y1)){
                somme += grille[i][j];
                printf("%d, %d, %d\n", grille[i][j], i, j);
                i +=1;
                j+=1;
                
            }
        }
        return somme;
    }

    if(max(x, x1) == x){
        i = max(x, x1);
        j = min(y, y1);
        while(i >= min(x, x1)){
            while(j<= max(y, y1)){
                somme += grille[i][j];
                printf("%d, %d, %d\n", grille[i][j], i, j);
                i -=1;
                j+=1;
                
            }
        }
        return somme;
    }

    if(max(x, x1) == x1){
        i = min(x, x1);
        j = max(y, y1);

        while(i<= max(x, x1)){
            while(j>= min(y, y1)){
                somme += grille[i][j];
                printf("%d, %d, %d\n", grille[i][j], i, j);
                i +=1;
                j-=1;
                
            }
        }
        return somme;
    }
    return 0;

}

