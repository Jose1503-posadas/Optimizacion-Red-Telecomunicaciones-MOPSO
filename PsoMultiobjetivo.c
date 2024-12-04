#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

//funcion de particula
typedef struct{
    float *posicion; 
    float *velocidad;
}Particula;

//generamos números aleatorios en un rango dado
void generaaleatorio(float *elemento, float inferior, float superior){
    float val = (float)rand()/RAND_MAX;
    *elemento = inferior+((superior - inferior)*val);
}

//inicializamos posiciones y velocidades en la iteración 0
void iteracion_cero(int tam_enjambre, int num_vecindarios, int num_variables, Particula *enjambre, float **Lista_metricas, float margen_diferencia){
    for(int i=0; i<tam_enjambre; i++){
        int vecindario = i%num_vecindarios; //vecindario apartir del índice de la partícula
        for(int j=0; j<num_variables; j++){
            float lim_inferior = Lista_metricas[vecindario][j]*(1.0-margen_diferencia);
            float lim_superior = Lista_metricas[vecindario][j]*(1.0+margen_diferencia);
            generaaleatorio(&enjambre[i].posicion[j], lim_inferior, lim_superior);
            enjambre[i].velocidad[j] = 0.0;
        }
    }
}


int main(int argc, char *argv[]){
    srand(time(NULL));
    int iteraciones, tam_enjambre, num_variables;
    float c1, c2;
    char *archivo;

    iteraciones = atoi(argv[1]);
    tam_enjambre = atoi(argv[2]);
    num_variables = atoi(argv[3]);
    c1 = atof(argv[4]);
    c2 = atof(argv[5]);
    archivo = argv[6]; // tiene las métricas para inicializar la posición

    FILE *metricas = fopen(archivo, "r");
    if(!metricas){
        perror("Error al abrir el archivo");
        return -1;
    }

    // Margen de diferencia 30%
    float margen_diferencia;
    fscanf(metricas, "%f", &margen_diferencia);

    printf("Margen de diferencia: %.2f \n", margen_diferencia);

    // Leer las métricas para los vecindarios
    int num_vecindarios = 0;
    float **Lista_metricas = NULL;
    while(!feof(metricas)){
        Lista_metricas = (float **)realloc(Lista_metricas,(num_vecindarios+1)*sizeof(float *));
        Lista_metricas[num_vecindarios] = (float *)malloc(num_variables*sizeof(float));
        if(fscanf(metricas, "%f %f %f",&Lista_metricas[num_vecindarios][0],&Lista_metricas[num_vecindarios][1],&Lista_metricas[num_vecindarios][2])==num_variables){
            printf("Metricas[%d]: %.2f, %.2f, %.2f\n", num_vecindarios+1,Lista_metricas[num_vecindarios][0],Lista_metricas[num_vecindarios][1],Lista_metricas[num_vecindarios][2]);
            num_vecindarios++;
        }
    }
    fclose(metricas);

    printf("Numero de vecindarios: %d\n", num_vecindarios);

    Particula *enjambre = (Particula *)malloc(tam_enjambre * sizeof(Particula));
    for(int i=0; i<tam_enjambre; i++){
        enjambre[i].posicion = (float *)malloc(num_variables * sizeof(float));
        enjambre[i].velocidad = (float *)malloc(num_variables * sizeof(float));
    }


    iteracion_cero(tam_enjambre, num_vecindarios, num_variables, enjambre, Lista_metricas, margen_diferencia);

    //imprimir el enjambre con vecindarios y posicion
    for(int i=0; i<tam_enjambre; i++){
        int vecindario = i % num_vecindarios;
        printf("Particula %d (vecindario %d): ", i+1, vecindario+1);
        for(int j=0; j<num_variables; j++)
            printf("%.2f ", enjambre[i].posicion[j]);
        printf("\n");
    }

}

