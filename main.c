#include <stdio.h>
#include "funciones.h"

int main() {
    int num_zonas = 5; 
    Zona zonas[5] = {
        {"Cumbaya"},
        {"Lumbisi"},
        {"Valle de los Chillos"},
        {"Norte"},
        {"Sur"}
    };

    float promedios[5];
    float predicciones[5];

    
    printf("\n** Ingresar niveles actuales de contaminacion **\n");
    ingresar_niveles_actuales(zonas, num_zonas);

   
    printf("\n** Seleccionar zonas para analisis **\n");
    cargar_datos_desde_archivos(zonas, &num_zonas);

   
    int opcion;
    do {
        printf("\n** Menu de Opciones **\n");
        printf("1. Calcular promedios de contaminacion\n");
        printf("2. Predecir niveles de contaminacion\n");
        printf("3. Emitir alertas y recomendaciones\n");
        printf("4. Guardar datos en archivo\n");
        printf("5. Editar datos historicos\n");
        printf("6. Salir\n");
        printf("Seleccione una opcion: ");
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
            case 5: {
                int zona_seleccionada;
                printf("Seleccione la zona para editar (1-%d): ", num_zonas);
                scanf("%d", &zona_seleccionada);
                if (zona_seleccionada >= 1 && zona_seleccionada <= num_zonas) {
                    editar_datos_historicos(&zonas[zona_seleccionada - 1]);
                } else {
                    printf("Zona no valida.\n");
                }
                break;
            }
            case 6:
                printf("Saliendo del programa.\n");
                break;
            default:
                printf("Opcion invalida.\n");
        }
    } while (opcion != 6);

    return 0;
}