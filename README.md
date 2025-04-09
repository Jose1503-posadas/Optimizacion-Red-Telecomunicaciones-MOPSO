# Análisis de la Calidad de Servicio en Redes de Telecomunicaciones Utilizando Algoritmos de Enjambre de Partículas
Este proyecto fue desarrollado como parte del **Proyecto Terminal** para la obtención del título de **Ingeniero en Computación** en la **Universidad Autónoma Metropolitana (UAM)**.  
El trabajo fue realizado por **José Alberto Posadas Gudiño** alumno de esta institución, bajo la asesoría y supervisión académica de:
- **Dra. Karen Samara Miranda Campos**  
- **Dr. Edwin Montes Orozco**

El objetivo de este proyecto es desarrollar y validar un enfoque basado en algoritmos de enjambre de partículas para optimizar la calidad de servicio (QoS) en redes de telecomunicaciones, mejorando métricas clave como latencia, jitter, pérdida de paquetes y cobertura

## Como usarlo:

1. Clona este repositorio:

   - `gh repo clone Jose1503-posadas/Optimizacion-Red-Telecomunicaciones-MOPSO`
   
2. Asegurate de tener intaladas las bibliotecas necesarias
   - **Python**-> Pandas, Matplotlib, Numpy
   - **C** -> math.h, time.h

3. Compilar el algoritmo de **MOPSO.c** para poder tener el ejecutable para posteriormente ocuparlo en la evolución diferencial(DE.c)

   - `gcc -o MOPSO MOPSO.c -lm`

4. En caso de querer compilar solamente el MOPSO se tiene que realizar de la siguiente manera:

   - `./mopso <TAM_ENJAMBRE> <ITERACIONES> <C1> <C2>`

   Por ejemplo:

   - `./mopso 30 100 0.8 0.5`
     
5. Compilar el archivo **DE.c**, el cual implementa el algoritmo de Evolución Diferencial. Este algoritmo se encarga de encontrar los mejores valores de parámetros para ejecutar el archivo MOPSO, de acuerdo con las métricas establecidas.

    - `gcc -o DE DE.c -lm`

7. Ejecuta el archivo ejecutable de **DE**
   
    - `./DE`

8. En caso de querer visualizar el frente de pareto que se obtiene del MOPSO se compila de la siguiente manera:
    - `python3 FrentePareto.py`

9. En caso de querer visualizar los resultados que se obtienen del MOPSO se compila de la siguiente manera:
    - `python3 .py`

