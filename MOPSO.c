#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int TAM_ENJAMBRE;
int ITERACIONES;
#define NUM_VARIABLES 4
#define ARCHIVO_PARETO 100

float pesos[] = {2.0, 1.0, 0.2, 0.5};
float limites_max[] = {50.0, 10.0, 1.0, -67.0};
float limites_min[] = {0.0, 0.0, 0.0, -100.0};

float instancia[8][NUM_VARIABLES];

typedef struct{
    float *posicion;
    float *velocidad;
    float *mejorPosicion;
    float objetivos[NUM_VARIABLES];
    float calidadServicio;
}Particula;

typedef struct{
    float posicion[NUM_VARIABLES];
    float objetivos[NUM_VARIABLES];
    float calidadServicio;
}SolucionPareto;

SolucionPareto archivoPareto[ARCHIVO_PARETO];
int tamArchivoPareto = 0;

void generaaleatorio(float *elemento, float inferior, float superior){
    float val = (float)rand() / RAND_MAX;
    *elemento = inferior + ((superior - inferior) * val);
}

void evaluaFO(float *individuo, float *objetivos, float *calidadServicio){
    objetivos[0] = fmin(1.0 / fmax(individuo[0], 0.01), 1000.0);
    objetivos[1] = fmin(1.0 / fmax(individuo[1], 0.01), 1000.0);
    objetivos[2] = fmin(1.0 / fmax(individuo[2], 0.01), 1000.0);
    objetivos[3] = individuo[3];

    *calidadServicio = pesos[0] * objetivos[0] +
                       pesos[1] * objetivos[1] +
                       pesos[2] * objetivos[2] +
                       pesos[3] * objetivos[3];
}


int domina(float *a, float *b){
    int mejor = 0;
    for(int i = 0; i < NUM_VARIABLES; i++){
        if(a[i] > b[i]) mejor = 1;
        else if(a[i] < b[i]) return 0;
    }
    return mejor;
}

void actualizarArchivoPareto(Particula *enjambre){
    for(int i = 0; i < TAM_ENJAMBRE; i++){
        int esDominado = 0;
        for(int j = 0; j < tamArchivoPareto; j++){
            if(domina(archivoPareto[j].objetivos, enjambre[i].objetivos)){
                esDominado = 1;
                break;
            }
        }
        if(!esDominado && tamArchivoPareto < ARCHIVO_PARETO){
            for(int k = 0; k < NUM_VARIABLES; k++){
                archivoPareto[tamArchivoPareto].posicion[k] = enjambre[i].posicion[k];
                archivoPareto[tamArchivoPareto].objetivos[k] = enjambre[i].objetivos[k];
            }
            archivoPareto[tamArchivoPareto].calidadServicio = enjambre[i].calidadServicio;
            tamArchivoPareto++;
        }
    }
}

void seleccionarGuia(float *lbest){
    if(tamArchivoPareto == 0) return;
    int idx = rand() % tamArchivoPareto;
    for(int j = 0; j < NUM_VARIABLES; j++){
        lbest[j] = archivoPareto[idx].posicion[j];
    }
}

void limitarPosicion(Particula *enjambre){
    for(int i = 0; i < TAM_ENJAMBRE; i++){
        for(int j = 0; j < NUM_VARIABLES; j++){
            if(enjambre[i].posicion[j] < limites_min[j]) enjambre[i].posicion[j] = limites_min[j];
            if(enjambre[i].posicion[j] > limites_max[j]) enjambre[i].posicion[j] = limites_max[j];
        }
    }
}

void limitarVelocidad(Particula *enjambre, float vmax){
    for(int i = 0; i < TAM_ENJAMBRE; i++){
        for(int j = 0; j < NUM_VARIABLES; j++){
            if(enjambre[i].velocidad[j] > vmax) enjambre[i].velocidad[j] = vmax;
            if(enjambre[i].velocidad[j] < -vmax) enjambre[i].velocidad[j] = -vmax;
        }
    }
}


void actualizarVelocidadYPosicion(Particula *enjambre, float c1, float c2, float vmax){
    for(int i = 0; i < TAM_ENJAMBRE; i++){
        float lbest[NUM_VARIABLES];
        seleccionarGuia(lbest);

        for(int j = 0; j < NUM_VARIABLES; j++){
            float r1 = (float)rand() / RAND_MAX;
            float r2 = (float)rand() / RAND_MAX;
            enjambre[i].velocidad[j] += c1 * r1 * (enjambre[i].mejorPosicion[j] - enjambre[i].posicion[j]) +
                                        c2 * r2 * (lbest[j] - enjambre[i].posicion[j]);
            enjambre[i].posicion[j] += enjambre[i].velocidad[j];
        }
    }

    limitarVelocidad(enjambre, vmax);
    limitarPosicion(enjambre);
}

void cargarInstancia(const char *archivo_instancia){
    FILE *file = fopen(archivo_instancia, "r");
    if(!file){
        printf("No se pudo abrir el archivo %s\n", archivo_instancia);
        exit(1);
    }
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < NUM_VARIABLES; j++){
            fscanf(file, "%f", &instancia[i][j]);
        }
    }
    fclose(file);
    //printf("Instancia cargada correctamente desde %s\n", archivo_instancia);
}

void MOPSO(Particula *enjambre, int iteraciones, float c1, float c2){
    FILE *archivo = fopen("pareto_results.txt", "w");
    if(!archivo){
        printf("Error al abrir el archivo de resultados.\n");
        return;
    }
    float vmax = 0.1 * (limites_max[0] - limites_min[0]);  // Velocidad máxima basada en el rango

    for(int a = 0; a < iteraciones; a++){
        //printf("\nIteracion %d", a + 1);
        for(int i = 0; i < TAM_ENJAMBRE; i++){
            evaluaFO(enjambre[i].posicion, enjambre[i].objetivos, &enjambre[i].calidadServicio);
        }
        actualizarArchivoPareto(enjambre);
        actualizarVelocidadYPosicion(enjambre, c1, c2,vmax);
    }

    for(int i = 0; i < tamArchivoPareto; i++){
        fprintf(archivo, "%f %f %f %f\n", archivoPareto[i].objetivos[0], archivoPareto[i].objetivos[1], archivoPareto[i].objetivos[2], archivoPareto[i].objetivos[3]);
    }
    fclose(archivo);
}

int main(int argc, char * argv[]){
    srand(time(NULL));

    TAM_ENJAMBRE = atoi(argv[1]);
    ITERACIONES = atoi(argv[2]);
    float c1 = atof(argv[3]);
    float c2 = atof(argv[4]);
    Particula enjambre[TAM_ENJAMBRE];
    cargarInstancia("instanciaJose.txt");
    for(int i = 0; i < TAM_ENJAMBRE; i++){
        enjambre[i].posicion = (float *)malloc(NUM_VARIABLES * sizeof(float));
        enjambre[i].velocidad = (float *)malloc(NUM_VARIABLES * sizeof(float));
        enjambre[i].mejorPosicion = (float *)malloc(NUM_VARIABLES * sizeof(float));
        for(int j = 0; j < NUM_VARIABLES; j++){
            generaaleatorio(&enjambre[i].posicion[j], limites_min[j], limites_max[j]);
            enjambre[i].velocidad[j] = 0.0;
            enjambre[i].mejorPosicion[j] = enjambre[i].posicion[j];
        }
    }
    MOPSO(enjambre, ITERACIONES, c1, c2);

//printf("\nProceso terminado.\n");
for(int i = 0; i < TAM_ENJAMBRE; i++){
    free(enjambre[i].posicion);
    free(enjambre[i].velocidad);
    free(enjambre[i].mejorPosicion);
}

   //calculamos el fitness promedio del archivo Pareto
   float fitness_total = 0.0;
   for(int i = 0; i < tamArchivoPareto; i++)
     fitness_total += archivoPareto[i].calidadServicio;
   float fitness_promedio = (tamArchivoPareto > 0) ? fitness_total / tamArchivoPareto : 0.0;
   //escribimos en resultado.txt
   FILE *res = fopen("resultado.txt", "w");
   if(res){
     fprintf(res, "%f\n", fitness_promedio);
     //printf("%f\n", fitness_promedio);
     fclose(res);
   }
    return 0;
}

