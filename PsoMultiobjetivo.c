#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

//float pesos[] = {2.0, 1.0, 0.2, 0.5}; 

//funcion de particula
typedef struct{
    float *posicion; 
    float *velocidad;
    float *mejorPosicion;
    float mejorFO;
}Particula;

//evaluamos la función objetivo
void evaluaFO(float *individuo, int num_variables){
    individuo[num_variables] = 100.0 * pow((pow(individuo[0], 2) - individuo[1]), 2) + pow((1 - individuo[0]), 2);
}

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

void actualiza_lbest(int particula, int tam_enjambre, int num_variables, Particula *enjambre, float *lbest){
    int rango = 2; //definimos el número de vecinos a cada lado
    int inicio = particula - rango;
    int fin = particula + rango;

    //ajustamos los índices para vecindad circular 
    if(inicio < 0) inicio += tam_enjambre;
    if(fin >= tam_enjambre) fin -= tam_enjambre;

    float mejorFO = -1e9;
    for(int i = inicio; i != (fin + 1) % tam_enjambre; i = (i + 1) % tam_enjambre){
        if(enjambre[i].mejorFO > mejorFO){
            mejorFO = enjambre[i].mejorFO;
            for(int j = 0; j < num_variables; j++){
                lbest[j] = enjambre[i].mejorPosicion[j];
            }
        }
    }
}

//actualizamos la velocidad de cada partícula
void actualiza_velocidad(Particula *enjambre, float **lbestVecindario, int tam_enjambre, int num_variables, float c1, float c2){
    float r1, r2;
    for(int i = 0; i < tam_enjambre; i++){
        for(int j = 0; j < num_variables; j++){
            r1 = (float)rand() / RAND_MAX;
            r2 = (float)rand() / RAND_MAX;
            enjambre[i].velocidad[j] += c1 * r1 * (enjambre[i].mejorPosicion[j] - enjambre[i].posicion[j]) +
                                       c2 * r2 * (lbestVecindario[i][j] - enjambre[i].posicion[j]);
        }
    }
}

// Función para actualizar la posición de cada partícula
void actualiza_posicion(Particula *enjambre, int tam_enjambre, int num_variables, float lim_inf, float lim_sup){
    for(int i = 0; i < tam_enjambre; i++){
        for(int j = 0; j < num_variables; j++){
            enjambre[i].posicion[j] += enjambre[i].velocidad[j];
            if(enjambre[i].posicion[j] < lim_inf)
                enjambre[i].posicion[j] = lim_inf;
            if(enjambre[i].posicion[j] > lim_sup)
                enjambre[i].posicion[j] = lim_sup;
        }
    }
}

void PSO(Particula *enjambre, int tam_enjambre, int num_variables, int iteraciones, float **Lista_metricas, float margen_diferencia, float c1, float c2){
    float **lbestVecindario = (float **)malloc(tam_enjambre * sizeof(float *));
    for(int i = 0; i < tam_enjambre; i++){
        lbestVecindario[i] = (float *)malloc(num_variables * sizeof(float));
    }

    for(int a = 0; a < iteraciones; a++){
        printf("\nIteración %d", a + 1);

        //actualizamos el mejor lbest para cada partícula
        for(int i = 0; i < tam_enjambre; i++){
            actualiza_lbest(i, tam_enjambre, num_variables, enjambre, lbestVecindario[i]);
        }

        //actualizamos la velocidad y posición
        actualiza_velocidad(enjambre, lbestVecindario, tam_enjambre, num_variables, c1, c2);
        actualiza_posicion(enjambre, tam_enjambre, num_variables, Lista_metricas[0][0], Lista_metricas[0][1]);

        //mandamos a evaluar la función objetivo para cada partícula
        for(int i = 0; i < tam_enjambre; i++){
            evaluaFO(enjambre[i].posicion, num_variables);
            if(enjambre[i].posicion[num_variables] > enjambre[i].mejorFO){
                enjambre[i].mejorFO = enjambre[i].posicion[num_variables];
                for(int j = 0; j < num_variables; j++){
                    enjambre[i].mejorPosicion[j] = enjambre[i].posicion[j];
                }
            }
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
    archivo = argv[6]; //tiene las metricas para inicializar la posición

    FILE *metricas=fopen(archivo, "r");
    if(!metricas){
        perror("Error al abrir el archivo");
        return -1;
    }
    //margen de diferencia 30%
    float margen_diferencia;
    fscanf(metricas, "%f", &margen_diferencia);

    printf("Margen de diferencia: %.2f \n", margen_diferencia);

    //leer las metricas para los vecindarios
    int num_vecindarios = 0;
    float **Lista_metricas = NULL;
    while(!feof(metricas)){
        Lista_metricas = (float **)realloc(Lista_metricas,(num_vecindarios+1)*sizeof(float *));
        Lista_metricas[num_vecindarios] = (float *)malloc(num_variables*sizeof(float));
        if(fscanf(metricas, "%f %f %f %f",&Lista_metricas[num_vecindarios][0],&Lista_metricas[num_vecindarios][1],&Lista_metricas[num_vecindarios][2], &Lista_metricas[num_vecindarios][3])==num_variables){
            printf("Metricas[%d]: %.4f, %.4f, %.4f %.4f\n", num_vecindarios+1,Lista_metricas[num_vecindarios][0],Lista_metricas[num_vecindarios][1],Lista_metricas[num_vecindarios][2], Lista_metricas[num_vecindarios][3]);
            num_vecindarios++;
        }
    }
    fclose(metricas);

    printf("Numero de vecindarios: %d\n", num_vecindarios);

    //crear el enjambre
    Particula *enjambre = (Particula *)malloc(tam_enjambre * sizeof(Particula));
    for(int i = 0; i<tam_enjambre; i++){
        enjambre[i].posicion = (float *)malloc(num_variables * sizeof(float));
        enjambre[i].velocidad = (float *)malloc(num_variables * sizeof(float));
        enjambre[i].mejorPosicion = (float *)malloc(num_variables * sizeof(float));
    }


    iteracion_cero(tam_enjambre, num_vecindarios, num_variables, enjambre, Lista_metricas, margen_diferencia);

    //imprimir el enjambre con vecindarios y posicion
    for(int i=0; i<tam_enjambre; i++){
        int vecindario = i % num_vecindarios;
        printf("Particula %d (vecindario %d): ", i+1, vecindario+1);
        for(int j=0; j<num_variables; j++)
            printf("%.4f ", enjambre[i].posicion[j]);
        printf("\n");
    }

    PSO(enjambre, tam_enjambre, num_variables, iteraciones, Lista_metricas, margen_diferencia, c1, c2);

    printf("\nLa mejor solución final es:\n");
    for(int i=0; i<tam_enjambre; i++){
        printf("Partícula %d: FO = %f, Mejor Posición = (", i, enjambre[i].mejorFO);
        for(int j=0; j<num_variables; j++){
            printf("%f", enjambre[i].mejorPosicion[j]);
            if(j < num_variables - 1) printf(", ");
        }
        printf(")\n");
    }
}

