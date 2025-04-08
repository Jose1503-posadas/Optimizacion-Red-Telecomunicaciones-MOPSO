#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define POP_SIZE 10   //tamaño de la población en DE
#define GEN_MAX 50    //numero máximo de generaciones en DE
#define F 0.5         //factor de mutación en DE
#define CR 0.5        //factor de recombinación en DE

//rango de parámetros del Algoritmo MOPSO
#define MIN_ENJAMBRE 10
#define MAX_ENJAMBRE 100
#define MIN_ITER 10
#define MAX_ITER 200
#define MIN_C 0.1
#define MAX_C 2.0

typedef struct{
    int tamEnjambre;
    int iteraciones;
    double c1;
    double c2;
}Individuo;

//generamos un número aleatorio en un rango dado
double random_range(double min, double max){
    return min + ((double)rand() / RAND_MAX) * (max - min);
}

//evaluamos ejecutando el código del algoritmo MOPSO
//el MOPSO debe devolver el fitness (calidad de servicio) promedio o el mejor valor encontrado
double evaluar(Individuo ind){
    char comando[500];
    snprintf(comando, sizeof(comando), "./mopso %d %d %lf %lf > resultado.txt", 
             ind.tamEnjambre, ind.iteraciones, ind.c1, ind.c2);
  
    system(comando);
    
    FILE *fp = fopen("resultado.txt", "r");
    if(!fp) return -1e9; //si falla, devolver un valor chiquito (mala solución)
    double fitness;
    fscanf(fp, "%lf", &fitness);
    fclose(fp);
    return fitness;
}

void evolucion_diferencial(){

   FILE *log = fopen("historial_configuraciones.csv", "w");
   if(log)
       fprintf(log, "Generacion,TamEnjambre,Iteraciones,c1,c2,Fitness\n");

    Individuo poblacion[POP_SIZE];
    Individuo mejor;
    double mejor_fitness = 1e9;
   
    srand(time(NULL));
    //inicializamos la población con valores aleatorios dentro de los rangos
    for(int i = 0; i < POP_SIZE; i++){
        poblacion[i].tamEnjambre = (int)random_range(MIN_ENJAMBRE, MAX_ENJAMBRE);
        poblacion[i].iteraciones = (int)random_range(MIN_ITER, MAX_ITER);
        poblacion[i].c1 = random_range(MIN_C, MAX_C);
        poblacion[i].c2 = random_range(MIN_C, MAX_C);
    }

    for(int gen = 0; gen < GEN_MAX; gen++){
        for(int i = 0; i < POP_SIZE; i++){
            int a, b, c;
            do{ a = rand() % POP_SIZE; }while (a == i);
            do{ b = rand() % POP_SIZE; }while (b == i || b == a);
            do{ c = rand() % POP_SIZE; }while (c == i || c == a || c == b);
            
            Individuo trial = poblacion[i];
            if((rand() / (double)RAND_MAX) < CR) trial.tamEnjambre = (int)(poblacion[a].tamEnjambre + F * (poblacion[b].tamEnjambre - poblacion[c].tamEnjambre));
            if((rand() / (double)RAND_MAX) < CR) trial.iteraciones = (int)(poblacion[a].iteraciones + F * (poblacion[b].iteraciones - poblacion[c].iteraciones));
            if((rand() / (double)RAND_MAX) < CR) trial.c1 = poblacion[a].c1 + F * (poblacion[b].c1 - poblacion[c].c1);
            if((rand() / (double)RAND_MAX) < CR) trial.c2 = poblacion[a].c2 + F * (poblacion[b].c2 - poblacion[c].c2);
            
            //nos aseguramos que los valores estén en los rangos válidos
            if(trial.tamEnjambre < MIN_ENJAMBRE) trial.tamEnjambre = MIN_ENJAMBRE;
            if(trial.tamEnjambre > MAX_ENJAMBRE) trial.tamEnjambre = MAX_ENJAMBRE;
            if(trial.iteraciones < MIN_ITER) trial.iteraciones = MIN_ITER;
            if(trial.iteraciones > MAX_ITER) trial.iteraciones = MAX_ITER;
            if(trial.c1 < MIN_C) trial.c1 = MIN_C;
            if(trial.c1 > MAX_C) trial.c1 = MAX_C;
            if(trial.c2 < MIN_C) trial.c2 = MIN_C;
            if(trial.c2 > MAX_C) trial.c2 = MAX_C;
          

            double fitness_trial = evaluar(trial);
            double fitness_actual = evaluar(poblacion[i]);
            if(log)
  		  fprintf(log, "%d,%d,%d,%lf,%lf,%lf\n", gen, trial.tamEnjambre, trial.iteraciones, trial.c1, trial.c2, fitness_trial);
            if(fitness_trial < fitness_actual){
                poblacion[i] = trial;
                if(fitness_trial < mejor_fitness){
                    mejor_fitness = fitness_trial;
                    mejor = trial;
                }
            }
        }
    }
    
    //mostramos la mejor configuración encontrada
    printf("Mejores parámetros para el Algoritmo MOPSO:\n");
    printf("Tamaño del enjambre: %d\n", mejor.tamEnjambre);
    printf("Número de iteraciones: %d\n", mejor.iteraciones);
    printf("Parámetro c1: %lf\n", mejor.c1);
    printf("Parámetro c2: %lf\n", mejor.c2);
}

int main(){
    printf("Iniciando optimización con Evolución Diferencial para MOPSO...\n");
    evolucion_diferencial();
    return 0;
}

