#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funciones.h"

// Función para calcular la contaminación del aire con la fórmula de Quito
float calcular_contaminacion(float co2, float so2, float no2, float pm25) {
    return (co2 / LIMITE_CO2 + so2 / LIMITE_SO2 + no2 / LIMITE_NO2 + pm25 / LIMITE_PM25) * 25.0;
}

void ingresar_niveles_actuales(Zona zonas[], int num_zonas) {
    for (int i = 0; i < num_zonas; i++) {
        printf("Ingrese los niveles actuales de contaminación para la zona %s (CO2, SO2, NO2, PM2.5): ", zonas[i].nombre);
        for (int j = 0; j < PARAMETROS_CONTAMINACION; j++) {
            scanf("%f", &zonas[i].nivel_actual[j]);
        }
        // Calcular contaminación actual con la fórmula de Quito
        zonas[i].contaminacion_actual = calcular_contaminacion(
            zonas[i].nivel_actual[0], zonas[i].nivel_actual[1],
            zonas[i].nivel_actual[2], zonas[i].nivel_actual[3]);
        printf("Contaminación actual en %s: %.2f µg/m³\n", zonas[i].nombre, zonas[i].contaminacion_actual);
    }
}

void cargar_datos_desde_archivos(Zona zonas[], int *num_zonas) {
    const char *archivos[] = {"Cumbaya.txt", "Lumbisi.txt", "Valle_de_los_Chillos.txt", "Norte.txt", "Sur.txt"};
    const char *nombres[] = {"Cumbaya", "Lumbisi", "Valle de los Chillos", "Norte", "Sur"};
    int seleccion[5];

    printf("Zonas disponibles:\n");
    for (int i = 0; i < 5; i++) {
        printf("%d. %s\n", i + 1, nombres[i]);
    }

    printf("Ingrese el número de zonas que desea analizar (máximo 5): ");
    scanf("%d", num_zonas);
    if (*num_zonas > 5) *num_zonas = 5;

    printf("Seleccione las zonas a analizar ingresando sus números separados por espacios (termine con -1):\n");
    for (int i = 0; i < *num_zonas; i++) {
        scanf("%d", &seleccion[i]);
        seleccion[i]--;
        strcpy(zonas[i].nombre, nombres[seleccion[i]]);

        FILE *fp = fopen(archivos[seleccion[i]], "r");
        if (fp == NULL) {
            printf("Error al abrir el archivo %s\n", archivos[seleccion[i]]);
            exit(EXIT_FAILURE);
        }

        for (int j = 0; j < DIAS_HISTORICOS; j++) {
            for (int k = 0; k < PARAMETROS_CONTAMINACION; k++) {
                fscanf(fp, "%f", &zonas[i].niveles[j][k]);
            }
        }

        fclose(fp);
    }
}

void calcular_promedios(Zona zonas[], int num_zonas, float promedios[]) {
    for (int i = 0; i < num_zonas; i++) {
        float suma = 0.0;
        for (int j = 0; j < DIAS_HISTORICOS; j++) {
            float contaminacion = calcular_contaminacion(
                zonas[i].niveles[j][0], zonas[i].niveles[j][1],
                zonas[i].niveles[j][2], zonas[i].niveles[j][3]);
            suma += contaminacion;
        }
        promedios[i] = suma / DIAS_HISTORICOS;
        printf("Promedio de contaminación en %s: %.2f µg/m³\n", zonas[i].nombre, promedios[i]);
    }
}

void predecir_niveles(Zona zonas[], int num_zonas, float predicciones[]) {
    for (int i = 0; i < num_zonas; i++) {
        float suma = 0.0;
        float pesos = 0.0;

        for (int j = 0; j < DIAS_HISTORICOS; j++) {
            int peso = (j >= DIAS_HISTORICOS - 10) ? 2 : 1;
            float contaminacion = calcular_contaminacion(
                zonas[i].niveles[j][0], zonas[i].niveles[j][1],
                zonas[i].niveles[j][2], zonas[i].niveles[j][3]);
            suma += contaminacion * peso;
            pesos += peso;
        }
        predicciones[i] = suma / pesos;
        printf("Predicción de contaminación en %s: %.2f µg/m³\n", zonas[i].nombre, predicciones[i]);
    }
}

void emitir_alertas_y_recomendaciones(Zona zonas[], int num_zonas, float predicciones[]) {
    for (int i = 0; i < num_zonas; i++) {
        if (predicciones[i] > 37) {
            printf("ALERTA: Los niveles de contaminación en %s son peligrosos.\n", zonas[i].nombre);
            printf("Recomendaciones:\n");
            printf("- Reducir el tráfico vehicular.\n");
            printf("- Cerrar temporalmente industrias.\n");
            printf("- Suspender actividades al aire libre.\n");
        } else {
            printf("Niveles de contaminación en %s no peligrosos.\n", zonas[i].nombre);
        }
    }
}

void guardar_datos_a_archivo(Zona zonas[], int num_zonas, float promedios[], float predicciones[]) {
    FILE *fp = fopen("niveles_actualizados.txt", "w");
    if (fp == NULL) {
        printf("Error al guardar los datos.\n");
        return;
    }

    for (int i = 0; i < num_zonas; i++) {
        fprintf(fp, "Zona: %s\n", zonas[i].nombre);
        fprintf(fp, "Contaminación actual: %.2f µg/m³\n", zonas[i].contaminacion_actual);
        fprintf(fp, "Promedio histórico: %.2f µg/m³\n", promedios[i]);
        fprintf(fp, "Predicción futura: %.2f µg/m³\n", predicciones[i]);
        if (predicciones[i] > 37) {
            fprintf(fp, "ALERTA: Niveles peligrosos.\n");
            fprintf(fp, "Recomendaciones:\n");
            fprintf(fp, "- Reducir el tráfico vehicular.\n");
            fprintf(fp, "- Cerrar temporalmente industrias.\n");
            fprintf(fp, "- Suspender actividades al aire libre.\n");
        } else {
            fprintf(fp, "Niveles no peligrosos.\n");
        }
        fprintf(fp, "\n");
    }

    fclose(fp);
    printf("Datos guardados exitosamente en 'niveles_actualizados.txt'\n");
}
