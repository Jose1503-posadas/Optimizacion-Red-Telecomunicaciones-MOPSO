import numpy as np
import matplotlib.pyplot as plt

archivo_pareto = "pareto_results.txt" 

#cargamos los datos del archivo
pareto_data = np.loadtxt(archivo_pareto)


latencia = pareto_data[:, 0]
jitter = pareto_data[:, 1]
perdida_paquetes = pareto_data[:, 2]
cobertura = pareto_data[:, 3]

fig = plt.figure(figsize=(10, 7))
ax = fig.add_subplot(111, projection='3d')

ax.scatter(latencia, jitter, perdida_paquetes, c=cobertura, cmap='viridis', s=50, label="Soluciones Pareto")
ax.set_xlabel("Latencia")
ax.set_ylabel("Jitter")
ax.set_zlabel("Pérdida de Paquetes")
ax.set_title("Frente de Pareto en 3D (Calidad de Servicio en Redes)")

plt.legend()
plt.show()

