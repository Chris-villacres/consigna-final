#include <stdio.h>
#include "funciones.h"

int main() {
    int num_zonas;
    Zona zonas[5];

    float promedios[5];
    float predicciones[5];

    cargar_datos_desde_archivos(zonas, &num_zonas);
    ingresar_niveles_actuales(zonas, num_zonas);

    int opcion;
    do {
        printf("\n*** Menú de Opciones ***\n");
        printf("1. Calcular promedios de contaminación\n");
        printf("2. Predecir niveles de contaminación\n");
        printf("3. Emitir alertas y recomendaciones\n");
        printf("4. Guardar datos en archivo\n");
        printf("5. Salir\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                calcular_promedios(zonas, num_zonas, promedios);
                break;
            case 2:
                predecir_niveles(zonas, num_zonas, predicciones);
                break;
            case 3:
                emitir_alertas_y_recomendaciones(zonas, num_zonas, predicciones);
                break;
            case 4:
                guardar_datos_a_archivo(zonas, num_zonas, promedios, predicciones);
                break;
            case 5:
                printf("Saliendo del programa.\n");
                break;
            default:
                printf("Opción inválida.\n");
        }
    } while (opcion != 5);

    return 0;
}
