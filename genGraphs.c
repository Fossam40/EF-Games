#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "graphUtils.h"

int* generateCouples(int nbSomm) { //Génération des couples d'arêtes
    int* ans = malloc(sizeof(int) * ((nbSomm*(nbSomm + 1))) );
    int i = 1;
    int j = 0;
    int current = 0;
    while( i < nbSomm ) {
        ans[current] = i;
        ans[current + 1] = j;
        current = current + 2;
        if((j+1) == i){
            j = 0;
            i++;
        }
        else {
            j++;
        }
    }
    return ans;
}

graphe dePrufer(int* pruferList, int nbSomm, int* arrArete, int maxSomm){
    graphe ans = initGraphe(nbSomm, maxSomm);
    // Decodage de Prüfer
    int* degrees = calloc(nbSomm, sizeof(int));
    int min = 0;
    int current = 0;
    for(int i = 0; i< (nbSomm - 2); i ++) {
        degrees[pruferList[i]]++;
    }
    // "degrees" contient maintenant le degrés des sommets dans l'arbre d'origine moins un 
    while(degrees[min] != 0) {
        min++;
    }
    int supprArete;
    for(int i = 0; i < (nbSomm - 2); i++) {
        ajoutArete(ans, pruferList[i], min);
	supprArete = (pruferList[i]>min)?(pruferList[i]):min; //calcul du max 
	supprArete = ((supprArete*(supprArete - 1))/2) + (min + pruferList[i] - supprArete);
	arrArete[2*supprArete] = 0;
	arrArete[2*supprArete + 1] = 0;
        degrees[min]--;
        degrees[pruferList[i]]--;
        if((pruferList[i]<min) && (degrees[pruferList[i]] == 0)) {
            min = pruferList[i];
        }
        while(degrees[min] != 0) {
            min++;
        }        
    }
    int tmp = min;
    min++;
    while(degrees[min] != 0) {
            min++;
    }
    ajoutArete(ans, min, tmp);
    supprArete = (tmp>min)?(tmp):min; //calcul du max 
    supprArete = ((supprArete*(supprArete - 1))/2) + (min + tmp - supprArete);
    arrArete[2*supprArete] = 0;
    arrArete[2*supprArete + 1] = 0;
    free(degrees);
    return ans;
}

graphe generateTree(int nbSomm, int* arrArete, int maxSomm) {
    int* pruferList = malloc(sizeof(int)* (nbSomm - 2));
    for(int i = 0; i < (nbSomm - 2); i++) {
        pruferList[i] = rand() % (nbSomm);
    }
    return dePrufer(pruferList, nbSomm, arrArete, maxSomm);
}

void shuffleNth(int* arrete, int nbTermes, int length){ // Knuth Shuffle
    int newIndex;
    int tmpi;
    int tmpj;
    for(int i = 0; i< (nbTermes); i++){
        newIndex = (rand() % (length - i)) + i;
        tmpi = arrete[2*i];
        tmpj = arrete[2*i + 1];
        arrete[2*i] = arrete[2 * newIndex];
        arrete[2*i + 1] = arrete[2 * newIndex + 1];
        arrete[2 * newIndex] = tmpi;
        arrete[2 * newIndex + 1] = tmpj;
    }
}

graphe generateGraphe(int nbSomm,int nbArete, int maxSomm) { //nbSomm doit être supérieur ou égal à 4 
    assert((nbSomm - 1) <= nbArete);
    int* arrArete = generateCouples(nbSomm);
    graphe myG = generateTree(nbSomm, arrArete, maxSomm);
    shuffleNth(arrArete, nbArete, (nbSomm*(nbSomm - 1))/2);
    int j = 0;
    for(int i = 0; i<(nbArete-nbSomm + 1); i++){
        while(arrArete[2*j] == 0){
            j++;
        }
        ajoutArete(myG, arrArete[2*j], arrArete[2*j + 1]);
        j++;
    }
    free(arrArete);
    return myG;
}

void printGraphToFile(graphe myG, FILE* myFile) {
	fprintf(myFile, "%d\n", myG.nbSommets);
	for(int i =0; i<myG.nbSommets;i++){
		for(int j = 1; j<myG.listAdj[i][0]; j++){
			fprintf(myFile, "%d ", myG.listAdj[i][j]);
		}
		fprintf(myFile, "%d", myG.listAdj[i][myG.listAdj[i][0]]);
		fprintf(myFile, "\n");
	}
}

void storeGraphs(int nbGraphs, int nMax, int nbFichier){
	graphe toBeAppended;
	char name[20];
	int nbSomm;
	int nbAretes;
	FILE* myFile;
	for(int i = 0; i<nbGraphs; i++){
		snprintf(name, 20,"./graphs%d/graph%d",nbFichier, i);
		myFile = fopen(name, "a");
		nbSomm = (rand() % (nMax-3)) + 4;
		nbAretes = (rand() % ( ((nbSomm*(nbSomm-1))/2) - (nbSomm - 2) ) ) + (nbSomm - 1);
		printf("Graph number %d: %d vertices, %d edges\n",i, nbSomm,nbAretes);
		toBeAppended = generateGraphe(nbSomm, nbAretes, nMax);
		printGraphToFile(toBeAppended, myFile);
		destroyGraphe(toBeAppended);
		fclose(myFile);
	}
}

int main(int argc, char* argv[])
{
    if(argc != 4) {
        printf("Le programme a besoin de trois arguments, le nombre de graphes à générer, le nombre de sommets maximal, et le nombre du fichier à créer.\n");
    }
    else {
	srand(time(NULL));
        int nbGraphes = atoi(argv[1]);
        int maxSommets = atoi(argv[2]);
	int nbFichier = atoi(argv[3]);
	storeGraphs(nbGraphes, maxSommets, nbFichier);
    }
    return 0;
}
