import numpy as np
from sklearn.preprocessing import StandardScaler

#leer archivo y cargar datos
filename = "metricas.txt"
with open(filename, 'r') as file:
    lines = file.readlines()

#ignorar el margen de diferencia
margen_diferencia = float(lines[0].strip())
print(f"Margen de diferencia: {margen_diferencia}")

#leer las metricas
datos = []
for line in lines[1:]:
    row = list(map(float, line.strip().split()))
    datos.append(row)

#convertir a un arreglo
datos = np.array(datos)

#normalizar los datos
scaler = StandardScaler()
datos_normalizados = scaler.fit_transform(datos)

#guardar el margen de diferencia y los datos normalizados
archivo_final = "metricas_normalizadas.txt"
with open(archivo_final, 'w') as file:
    file.write(f"{margen_diferencia:.1f}\n")
    np.savetxt(file, datos_normalizados, fmt="%.4f")

print(f"Datos normalizados guardados en {archivo_final}")
