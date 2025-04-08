import pandas as pd
import matplotlib.pyplot as plt

#cargamos el archivo CSV
df = pd.read_csv("historial_configuraciones.csv")

#mostramos las primeras filas para verificar
print(df.head())

#grafica 1: Evolución del fitness
plt.figure()
plt.plot(df["Generacion"], df["Fitness"], marker='o')
plt.title("Evolución del Fitness")
plt.xlabel("Generación")
plt.ylabel("Fitness")
plt.grid(True)
plt.savefig("evolucion_fitness.png")
plt.show()

#grafica 2: Parámetros c1 y c2 a lo largo de las generaciones
plt.figure()
plt.plot(df["Generacion"], df["c1"], label="c1", marker='o')
plt.plot(df["Generacion"], df["c2"], label="c2", marker='x')
plt.title("Evolución de c1 y c2")
plt.xlabel("Generación")
plt.ylabel("Valor")
plt.legend()
plt.grid(True)
plt.savefig("evolucion_c1_c2.png")
plt.show()

#grafica 3: Tamaño de enjambre vs Fitness
plt.figure()
plt.scatter(df["TamEnjambre"], df["Fitness"], alpha=0.7)
plt.title("Tamaño del Enjambre vs Fitness")
plt.xlabel("Tamaño del Enjambre")
plt.ylabel("Fitness")
plt.grid(True)
plt.savefig("enjambre_vs_fitness.png")
plt.show()

#grafica 4: Iteraciones vs Fitness
plt.figure()
plt.scatter(df["Iteraciones"], df["Fitness"], alpha=0.7, color="green")
plt.title("Iteraciones vs Fitness")
plt.xlabel("Iteraciones")
plt.ylabel("Fitness")
plt.grid(True)
plt.savefig("iteraciones_vs_fitness.png")
plt.show()
