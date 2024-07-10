#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>
#include <stdlib.h>
#include <float.h>
#include <limits.h>
//#include "graphUtils.h"

struct graphe_s {int nbSommets; int** listAdj;}; // L'ensemble des sommets est [| 0; nbSommets - 1|], et listAdj contient des tables d'ajacence dont la première case 
typedef struct graphe_s graphe;                         // indique la taille

graphe initGraphe(int nbSomm, int nMax) {
    assert(nbSomm > 0);
    assert(nbSomm <= nMax);
    int** adj = malloc(sizeof(int*) * nbSomm);
    for(int i = 0; i< nbSomm; i++) {
        adj[i] = malloc (sizeof(int) * nbSomm);
        adj[i][0] = 0;
    }
    graphe ans = {.nbSommets = nbSomm, .listAdj = adj};
    return ans;
}

void destroyGraphe(graphe myGraphe) {
    for(int i = 0; i < myGraphe.nbSommets; i++) {
        free(myGraphe.listAdj[i]);
    }
    free(myGraphe.listAdj);
}

void ajoutArete(graphe myGraphe, int sommet1, int sommet2) { // Il faut au préalable vérifier qu'on n'ajoute pas la même arete deux fois
    myGraphe.listAdj[sommet1][0]++;
    myGraphe.listAdj[sommet1][myGraphe.listAdj[sommet1][0]] = sommet2;
    myGraphe.listAdj[sommet2][0]++;
    myGraphe.listAdj[sommet2][myGraphe.listAdj[sommet2][0]] = sommet1;
}

void supprimeArete(graphe myGraphe, int sommet1, int sommet2){
    int j = 1;
    int tmp;
    while(myGraphe.listAdj[sommet1][j] != sommet2){
        j++;
    }
    myGraphe.listAdj[sommet1][j] = 0;
    while(j != myGraphe.nbSommets){
        int next = myGraphe.listAdj[sommet1][j+1];
        tmp = next;
        next = 0;
        myGraphe.listAdj[sommet1][j] = tmp;
        j++;
    }
    myGraphe.listAdj[sommet1][0]--;
    j = 1;
    while(myGraphe.listAdj[sommet2][j] != sommet1){
        j++;
    }
    myGraphe.listAdj[sommet2][j] = 0;
    while(j != myGraphe.nbSommets){
        int next = myGraphe.listAdj[sommet2][j+1];
        tmp = next;
        next = 0;
        myGraphe.listAdj[sommet2][j] = tmp;
        j++;
    }
    myGraphe.listAdj[sommet2][0]--;
}

void supprimeSommet(graphe myG, int sommet1){
    while(myG.listAdj[sommet1][0] != 0){
            int ans = myG.listAdj[sommet1][1];
            supprimeArete(myG, sommet1, ans);
            }
}

int nbAretes(graphe myGraphe){
    int total = 0;
    for(int i = 0; i< myGraphe.nbSommets; i++){
        total = total + myGraphe.listAdj[i][0];
    }
    return (total/2);
}

void printGraph(graphe myG){
    for(int i = 0; i<(myG.nbSommets); i++){
        printf("%d : ", i);
        for(int j = 0; j<(myG.listAdj[i][0]); j++){
            printf("%d ", myG.listAdj[i][j + 1]);
        }
        printf("\n");
    }
}

void printArray(int* arr, int size){
    for(int i = 0; i<size; i++){
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int min(int a, int b){
    return((a>b)?b:a);
}

int max(int a, int b){
    return((a>b)?a:b);
}

graphe readGraphe(FILE* myFile){
	int tmp;
	int separator;
	fscanf(myFile, "%d\n", &tmp);
	graphe ans = initGraphe(tmp, 50); //NMAX
	for(int i = 0; i<ans.nbSommets; i++){
		separator = 20;
		while(separator != 10){
			fscanf(myFile, "%d", &tmp);
			if(i>tmp){
				ajoutArete(ans, i, tmp);
			}
			separator = fgetc(myFile);
		}

	}
	return ans;
}

int compareInt(const void * a, const void * b){
    int* nb = (int*)b;
    int* na = (int*)a;
    return (*na)-(*nb);
}

void sortIntArray(int* arr, int size){ //Ordonne en ordre croissant, O( n * log(n) ) 
    qsort((void*)arr, size, sizeof(int), compareInt);
}

bool sameElements(int* arr1, int* arr2, int size){
    sortIntArray(arr1, size);
    sortIntArray(arr2, size);
    for(int i = 0; i<size; i++){
        if(arr1[i] != arr2[i]){
            return false;
        }
    }
    return true;
}

double averageTime(double* myArr){
    double mind = DBL_MAX;
    double maxd = 0;
    double total = 0;
    for(int i = 0; i<5; i++){
        mind = (myArr[i]<mind)?(myArr[i]):mind;
        maxd = (myArr[i]>maxd)?(myArr[i]):maxd;
        total = total + myArr[i];
    }
    total = total - maxd - mind;
    return (total/3);
}

bool checkIso(int* isomorphismD, int* isomorphismS, int current, int* playedG1, int* playedG2, graphe g1, graphe g2){  //O(current * log(current)), peut être amélioré
    if(current>1){
        if(isomorphismD[current-1]>0){
            int* definedNext  = calloc(current-1,sizeof(int));
            int j = 0;
            for(int i = 1; i <= g1.listAdj[isomorphismD[current - 1]-1][0]; i++){
                if(playedG1[g1.listAdj[isomorphismD[current - 1] - 1][i] ] != 0){
                    definedNext[j] = playedG1[g1.listAdj[isomorphismD[current - 1] - 1][i]];
                    j++;
                }
            }
            int* definedNextS  = calloc(current-1,sizeof(int));
            j = 0;
            for(int i = 1; i <= g2.listAdj[-isomorphismS[current - 1]-1][0]; i++){
                if(playedG2[g2.listAdj[-isomorphismS[current - 1] - 1][i] ] != 0){
                    definedNextS[j] = playedG2[g2.listAdj[-isomorphismS[current - 1] - 1][i]];
                    j++;
                }
            }
            bool ans = sameElements(definedNext, definedNextS, current-1);
            free(definedNext);
            free(definedNextS);
            return ans;
        }
        else{
            int* definedNext  = calloc(current,sizeof(int));
            int j = 0;
            for(int i = 1; i <= g1.listAdj[isomorphismS[current - 1]-1][0]; i++){
                if(playedG1[g1.listAdj[isomorphismS[current - 1] - 1][i] ] != 0){
                    definedNext[j] = playedG1[g1.listAdj[isomorphismS[current - 1] - 1][i]];
                    j++;
                }
            }
            int* definedNextS  = calloc(current,sizeof(int));
            j = 0;
            for(int i = 1; i <= g2.listAdj[-isomorphismD[current - 1]-1][0]; i++){
                if(playedG2[g2.listAdj[-isomorphismD[current - 1] - 1][i] ] != 0){
                    definedNextS[j] = playedG2[g2.listAdj[-isomorphismD[current - 1] - 1][i]];
                    j++;
                }
            }
            bool ans = sameElements(definedNext, definedNextS, current);
            free(definedNext);
            free(definedNextS);
            return ans;
        }
    }
    return true;
}

int minMax(graphe myG1, graphe myG2, int current, int player, int* isomorphismD, int* isomorphismS, int* playedG1, int* playedG2){ //returns minimum number of rounds to distinguish the 2 graphs
    if((player==0) && (!(checkIso(isomorphismD, isomorphismS, current, playedG1, playedG2, myG1, myG2)) || (current==myG1.nbSommets))){
        return current;
    }

    else if(player == 0){ //Si c'est le tour du Spoiler (min)
        int minGain = myG1.nbSommets; //On impose que myG1 soit le graphe avec le moins de sommets (et donc une borne sup du nombre de tours du jeu)
        for(int i = 0; i< myG1.nbSommets; i++){
            if(playedG1[i] == 0){
                isomorphismS[current]= i+1;
                playedG1[i] = current + 1;
                minGain = min(minGain, minMax(myG1, myG2, current, 1, isomorphismD, isomorphismS, playedG1, playedG2));
                playedG1[i] = 0;
            }
        } 
        for(int i = 0; i< myG2.nbSommets; i++){
            if(playedG2[i] == 0){
                isomorphismS[current]= -i-1;
                playedG2[i] = current + 1;
                minGain = min(minGain, minMax(myG1, myG2, current, 1, isomorphismD, isomorphismS, playedG1, playedG2));
                playedG2[i] = 0;
            }
        }
        return minGain;        
    }
    else if(player == 1){ //Joueur Max (duplicateur)
        int maxGain = 0;
        if(isomorphismS[current]>0){
            for(int i = 0; i< myG2.nbSommets; i++){
                if(!playedG2[i]){
                    isomorphismD[current]= -i-1;
                    playedG2[i] = current + 1;
                    maxGain = max(maxGain, minMax(myG1, myG2, current + 1, 0, isomorphismD, isomorphismS, playedG1, playedG2));
                    playedG2[i] = 0;
                } 
            }
            return maxGain;
        }
        else{
            for(int i = 0; i< myG1.nbSommets; i++){
                if(!playedG1[i]){
                    isomorphismD[current]= i+1;
                    playedG1[i] = current + 1;
                    maxGain = max(maxGain, minMax(myG1, myG2, current + 1, 0, isomorphismD, isomorphismS, playedG1, playedG2));
                    playedG1[i] = 0;
                } 
            }
            return maxGain;  
        }
    }
}

int minMaxWrapper(graphe g1, graphe g2){
    int* isomorphismD = calloc(g1.nbSommets,sizeof(int));
    int* isomorphismS = calloc(g1.nbSommets,sizeof(int));
    int* playedG1 = calloc(g1.nbSommets,sizeof(int));
    int* playedG2 = calloc(g2.nbSommets,sizeof(int));
    int ans = minMax(g1, g2, 0, 0, isomorphismD, isomorphismS, playedG1, playedG2 );
    free(isomorphismD);
    free(isomorphismS);
    free(playedG1);
    free(playedG2);
    return ans;
}


int alphaBeta(graphe myG1, graphe myG2, int current, int player, int* isomorphismD, int* isomorphismS, int* playedG1, int* playedG2,int alpha, int beta, int borne) { 
    int ans;
    if((player==0) && (!(checkIso(isomorphismD, isomorphismS, current, playedG1, playedG2, myG1, myG2)) || (current==min(myG1.nbSommets, borne)))){
        return current;
    }
    else if(player == 0){ //Si c'est le tour du Spoiler
        int minGain = myG1.nbSommets; //On impose que myG1 soit le graphe avec le moins de sommets (et donc une borne sup du nombre de tours du jeu)
        for(int i = 0; i< myG1.nbSommets; i++){
            if(playedG1[i] == 0){
                isomorphismS[current]= i+1;
                playedG1[i] = current + 1;
		        ans = alphaBeta(myG1, myG2, current, 1, isomorphismD, isomorphismS, playedG1, playedG2, alpha, beta, borne);
                minGain = min(minGain, ans);
                playedG1[i] = 0;
		        if(minGain <= alpha){
			        return minGain;
		        }
		        beta = min(beta, ans);
            }
        }
        for(int i = 0; i< myG2.nbSommets; i++){
            if(playedG2[i] == 0){
                isomorphismS[current]= -i-1;
                playedG2[i] = current + 1;
		        ans = alphaBeta(myG1, myG2, current, 1, isomorphismD, isomorphismS, playedG1, playedG2, alpha, beta, borne);
                minGain = min(minGain, ans);
                playedG2[i] = 0;
        		if(minGain <= alpha){
		        	return minGain;
	        	    beta = min(beta, ans);
		        }
            }
        }
        return minGain;
    }

    else if(player == 1){
        int maxGain = 0;
        if(isomorphismS[current]>0){
            for(int i = 0; i< myG2.nbSommets; i++){
                if(playedG2[i] == 0){
                    isomorphismD[current]= -i-1;
                    playedG2[i] = current + 1;
        		    ans = alphaBeta(myG1, myG2, current + 1, 0, isomorphismD, isomorphismS, playedG1, playedG2, alpha, beta, borne);
                    maxGain = max(maxGain, ans);
                    playedG2[i] = 0;
		            if(maxGain >= beta){
		    	        return maxGain;
		            }
		            alpha = max(alpha, ans);
                }
            }
            return maxGain;
        }
        else{
            for(int i = 0; i< myG1.nbSommets; i++){
                if(playedG1[i] == 0){
                    isomorphismD[current]= i+1;
                    playedG1[i] = current + 1;
		            ans = alphaBeta(myG1, myG2, current + 1, 0, isomorphismD, isomorphismS, playedG1, playedG2, alpha, beta, borne);
                    maxGain = max(maxGain, ans);
                    playedG1[i] = 0;
        		    if(maxGain >= beta){
		        	    return maxGain;
		            }
		            alpha = max(alpha, ans);
                }
            }
            return maxGain;
        }
    }
}

int alphaBetaWrapper(graphe g1, graphe g2, int borne){
    int* isomorphismD = calloc(g1.nbSommets,sizeof(int));
    int* isomorphismS = calloc(g1.nbSommets,sizeof(int));
    int* playedG1 = calloc(g1.nbSommets,sizeof(int));
    int* playedG2 = calloc(g2.nbSommets,sizeof(int));
    int ans = alphaBeta(g1, g2, 0, 0, isomorphismD, isomorphismS, playedG1, playedG2, 0, g1.nbSommets, borne);
    free(isomorphismD);
    free(isomorphismS);
    free(playedG1);
    free(playedG2);
    return ans;
}

int couvertureMinimale(graphe myG){
    int len = myG.nbSommets;
    graphe copy = myG;
    int maxSom = 0;
    int* array = malloc(sizeof(int)*len);
    int maxdeg = myG.listAdj[0][0];
    int j = 0;
    while(copy.nbSommets != 0){
        maxdeg = 0;
        for(int i = 0; i<len; i++){
            if(copy.listAdj[i][0]>maxdeg){
                maxdeg = copy.listAdj[i][0];
                maxSom = i;
            }
        }
        while(copy.listAdj[maxSom][0] != 0){
            int ans = copy.listAdj[maxSom][1];
            supprimeSommet(copy, ans);
            copy.nbSommets--;
        }
        array[j] = maxSom + 1;
        j++;
        copy.nbSommets--;
            } 
    free(array); 
    return j; 
}

int domSetEst(graphe graphe1, graphe graphe2){
    int ans1 = couvertureMinimale(graphe1);
    int ans2 = couvertureMinimale(graphe2);
    return (min(ans1, ans2) + 1);
}

int** floydWarshall(graphe graph1){
    int** ans = malloc(sizeof(int)*graph1.nbSommets*graph1.nbSommets);
    for(int i = 0; i<graph1.nbSommets; i++){
        ans[i] = malloc(sizeof(int)*graph1.nbSommets);
        for(int j = 0; j<graph1.nbSommets; j++){
            ans[i][j] = INT_MAX;
            if(i==j){
                ans[i][i] = 0;
            }
        }
    }

    for(int i = 0; i<graph1.nbSommets; i++){
        for(int j = 1; j<=graph1.listAdj[i][0]; j++){
            ans[i][graph1.listAdj[i][j]] = 1;
        }
    }

    int currentDist;
    for(int k = 0; k<graph1.nbSommets;k++){
        for(int i = 0; i<graph1.nbSommets; i++){
            for(int j = 0; j<graph1.nbSommets; j++){
                if( (ans[i][k]== INT_MAX) || (ans[k][j]== INT_MAX)){
                    currentDist = INT_MAX;
                }
                else{
                    currentDist = ans[i][k] + ans[k][j];
                }
                if(currentDist < ans[i][j]){
                    ans[i][j] = currentDist;
                    ans[j][i] = currentDist;
                }
            }
        }
    }
    return ans;
}


void destroyTab(int** tab, int len){
    for(int i = 0; i<len; i++){
        free(tab[i]);
    }
    free(tab);
}


int* calculateNthColor(graphe myG,int** warshall,int sommet,int maxColor){ //maxColor <= 8. O(maxColor ^ 2).
    int* ans = calloc(maxColor, sizeof(int));
    int* kthNeighbour = calloc(maxColor, sizeof(int));
    int* multiply = calloc(maxColor, sizeof(int));
    for(int i = 0; i<maxColor; i++){
        multiply[i]=1;
    }
    for(int i = 0; i<4; i++){
        if(warshall[sommet][i]<= maxColor && (warshall[sommet][i] >0)){
            kthNeighbour[(warshall[sommet][i]) - 1]++;
        }
    }
    for(int i = 0; i<maxColor; i++){
        for(int j = i; j<maxColor; j++){
            ans[j] = ans[j] + (multiply[j]*(min(j+1,kthNeighbour[i])));
        }
        for(int j = i+1;j<maxColor; j++){
            multiply[j] = multiply[j] * (j+2);
        }
    }
    free(kthNeighbour);
    free(multiply);
    return ans;
}

int** calculateAllColors(graphe myG, int maxColor){ //maxColor <= 8, O(myG.nbSommets * (maxColor ^ 2))
    int** floyd = floydWarshall(myG);
    int** ans = malloc(sizeof(int*)*myG.nbSommets);
    for(int i = 0; i<myG.nbSommets; i++){
        ans[i] = calculateNthColor(myG, floyd, i, maxColor);
    }
    free(floyd);
    return ans;
}

void printArrayArray(int** arr, int y, int x){
    for(int i = 0; i<y; i++){
        for(int j = 0; j<x; j++){
            printf("%d ", arr[i][j]);
        }
        printf("\n");
    }
}

int colorApprox(graphe graphe1, graphe graphe2){
    int k = 0;
    bool found;
    int minSomm = min(graphe1.nbSommets, graphe2.nbSommets);
    int** g1Colors = calculateAllColors(graphe1, min(minSomm, 8));
    int** g2Colors = calculateAllColors(graphe2, min(minSomm, 8));
    while(k < min(minSomm,8)){
        for(int i = 0; i<graphe2.nbSommets; i++){
            found = false;
            for(int j = 0; j < graphe1.nbSommets; j++){
                if(g2Colors[i][k] == g1Colors[j][k]){
                    found = true;
                }
            }
            if(!found){
                return min(minSomm,k+2);
            }
        }
        for(int i = 0; i<graphe1.nbSommets; i++){
            found = false;
            for(int j = 0; j < graphe2.nbSommets; j++){
                if(g1Colors[i][k] == g2Colors[j][k]){
                    found = true;
                }
            }
            if(!found){
                return min(minSomm,k+2); 
            }
        }
        k++;
    }
    return minSomm;
}

void testAgainst(int n1, int n2, char name[20], FILE* myFile, double* chrono, int nbFichier){
    snprintf(name, 20,"./graphs%d/graph%d", nbFichier, n1);
	myFile = fopen(name, "r");
    graphe graphe1 = readGraphe(myFile);
    fclose(myFile);

    snprintf(name, 20,"./graphs%d/graph%d", nbFichier, n2);
	myFile = fopen(name, "r");
    graphe graphe2 = readGraphe(myFile);
    fclose(myFile);

    printf("Graph %d vs Graph %d: \n", n1, n2);
    printf(" - Le graphe %d a %d sommets et %d aretes \n", n1, graphe1.nbSommets, nbAretes(graphe1));
    printf(" - Le graphe %d a %d sommets et %d aretes \n", n2, graphe2.nbSommets, nbAretes(graphe2));
    if(graphe1.nbSommets > graphe2.nbSommets){
        graphe tmp = graphe1;
        graphe1 = graphe2;
        graphe2 = tmp;
    }
    int borne;
    int ans;
    for(int i = 0; i<5; i++){
        snprintf(name, 20,"./graphs%d/graph%d", nbFichier, n1);
	    myFile = fopen(name, "r");
        graphe1 = readGraphe(myFile);
        fclose(myFile);
        snprintf(name, 20,"./graphs%d/graph%d", nbFichier, n2);
	    myFile = fopen(name, "r");
        graphe2 = readGraphe(myFile);
        fclose(myFile);
        clock_t start = clock();
        borne = min(colorApprox(graphe1, graphe2) + 1, graphe1.nbSommets);
        ans = alphaBetaWrapper(graphe1, graphe2, borne);
        clock_t stop = clock();
        chrono[i]= (double)(stop-start) / CLOCKS_PER_SEC;
    }
    printf("Average time: %f for %d rounds.\n", averageTime(chrono), ans);
    destroyGraphe(graphe1);
    destroyGraphe(graphe2);
}



void testNgraphs(int n, int nbFichier, int minGraph){
    FILE* myFile;
    double* chrono = malloc(sizeof(double)*5);
    char name[20];
    for(int i = minGraph; i<n; i++){
        for(int j= i+1; j<=n; j++){
            testAgainst(i, j, name, myFile, chrono, nbFichier);
        }
    }
    free(chrono);
}





int main(int argc, char* argv[]) {
   if((argc != 4) && (argc != 3)) {
        printf("Le programme a besoin de deux arguments, le nombre de graphes à comparer, le nombre du fichier où ils sont, et le premier graphe à analyser (default 0).\n");
    }
    else if(argc == 3){
	    srand(time(NULL));
        int nbGraphes = atoi(argv[1]);
        int nbFichier = atoi(argv[2]);
	    testNgraphs(nbGraphes - 1, nbFichier, 0);
    }
    else {
        srand(time(NULL));
        int nbGraphes = atoi(argv[1]);
        int nbFichier = atoi(argv[2]);
	    int minGraph = atoi(argv[3]);
        testNgraphs(nbGraphes - 1, nbFichier, minGraph);
    }
    return 0;	
}
