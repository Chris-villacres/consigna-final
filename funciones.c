#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funciones.h"

float calcular_contaminacion(float co2, float so2, float no2, float pm25) {
    return (co2 / LIMITE_CO2 + so2 / LIMITE_SO2 + no2 / LIMITE_NO2 + pm25 / LIMITE_PM25) * 25.0;
}

void ingresar_niveles_actuales(Zona zonas[], int num_zonas) {
    for (int i = 0; i < num_zonas; i++) {
        printf("\nZona: %s\n", zonas[i].nombre); 
        printf("Ingrese los niveles actuales de contaminacion (CO2, SO2, NO2, PM2.5): ");
        for (int j = 0; j < PARAMETROS_CONTAMINACION; j++) {
            scanf("%f", &zonas[i].nivel_actual[j]);
        }
        zonas[i].contaminacion_actual = calcular_contaminacion(
            zonas[i].nivel_actual[0], zonas[i].nivel_actual[1],
            zonas[i].nivel_actual[2], zonas[i].nivel_actual[3]);
        printf("Contaminacion actual en %s: %.2f µg/m³\n", zonas[i].nombre, zonas[i].contaminacion_actual);
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

    printf("Ingrese el numero de zonas que desea analizar (maximo 5): ");
    scanf("%d", num_zonas);
    if (*num_zonas > 5) *num_zonas = 5;

    printf("Seleccione las zonas a analizar ingresando sus numeros separados por espacios (termine con -1):\n");
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
            suma += calcular_contaminacion(
                zonas[i].niveles[j][0], zonas[i].niveles[j][1],
                zonas[i].niveles[j][2], zonas[i].niveles[j][3]);
        }
        promedios[i] = suma / DIAS_HISTORICOS;
        printf("Promedio de contaminacion en %s: %.2f µg/m³\n", zonas[i].nombre, promedios[i]);
    }
}

void predecir_niveles(Zona zonas[], int num_zonas, float predicciones[]) {
    for (int i = 0; i < num_zonas; i++) {
        float suma = 0.0;
        float pesos = 0.0;

        for (int j = 0; j < DIAS_HISTORICOS; j++) {
            int peso = (j >= DIAS_HISTORICOS - 10) ? 2 : 1;
            suma += calcular_contaminacion(
                zonas[i].niveles[j][0], zonas[i].niveles[j][1],
                zonas[i].niveles[j][2], zonas[i].niveles[j][3]) * peso;
            pesos += peso;
        }
        predicciones[i] = suma / pesos;
        printf("Prediccion de contaminacion en %s: %.2f µg/m³\n", zonas[i].nombre, predicciones[i]);
    }
}

void emitir_alertas_y_recomendaciones(Zona zonas[], int num_zonas, float predicciones[]) {
    for (int i = 0; i < num_zonas; i++) {
        printf("\nZona: %s\n", zonas[i].nombre);
        int alerta = 0;

       
        for (int j = 0; j < PARAMETROS_CONTAMINACION; j++) {
            if (zonas[i].nivel_actual[j] > (j == 0 ? LIMITE_CO2 : j == 1 ? LIMITE_SO2 : j == 2 ? LIMITE_NO2 : LIMITE_PM25)) {
                alerta = 1;
                printf("⚠️ Alerta: %s supera el limite permitido.\n",
                       j == 0 ? "CO2" : j == 1 ? "SO2" : j == 2 ? "NO2" : "PM2.5");
            }
        }

        
        if (alerta) {
            printf("Recomendaciones para %s:\n", zonas[i].nombre);
            printf("- Evite actividades al aire libre.\n");
            printf("- Use mascarilla si es necesario.\n");
            printf("- Monitoree la calidad del aire regularmente.\n");
        } else {
            printf("La calidad del aire en %s esta dentro de los limites aceptables.\n", zonas[i].nombre);
        }

        printf("Prediccion para los proximos dias: %.2f µg/m³\n", predicciones[i]);
    }
}

void guardar_datos_a_archivo(Zona zonas[], int num_zonas, float promedios[], float predicciones[]) {
    FILE *fp = fopen("niveles_actuales.txt", "w");
    if (fp == NULL) {
        printf("Error al crear el archivo niveles_actuales.txt\n");
        return;
    }

    fprintf(fp, "Datos de contaminacion actuales, promedios, predicciones y alertas\n");
    for (int i = 0; i < num_zonas; i++) {
        fprintf(fp, "\nZona: %s\n", zonas[i].nombre);
        fprintf(fp, "Contaminacion actual: %.2f µg/m³\n", zonas[i].contaminacion_actual);
        fprintf(fp, "Promedio de los ultimos 30 dias: %.2f µg/m³\n", promedios[i]);
        fprintf(fp, "Prediccion para los proximos dias: %.2f µg/m³\n", predicciones[i]);

        int alerta = 0;
        for (int j = 0; j < PARAMETROS_CONTAMINACION; j++) {
            if (zonas[i].nivel_actual[j] > (j == 0 ? LIMITE_CO2 : j == 1 ? LIMITE_SO2 : j == 2 ? LIMITE_NO2 : LIMITE_PM25)) {
                alerta = 1;
                fprintf(fp, "⚠️ Alerta: %s supera el limite permitido.\n",
                        j == 0 ? "CO2" : j == 1 ? "SO2" : j == 2 ? "NO2" : "PM2.5");
            }
        }

        if (alerta) {
            fprintf(fp, "Recomendaciones:\n");
            fprintf(fp, "- Evite actividades al aire libre.\n");
            fprintf(fp, "- Use mascarilla si es necesario.\n");
            fprintf(fp, "- Monitoree la calidad del aire regularmente.\n");
        } else {
            fprintf(fp, "La calidad del aire está dentro de los limites aceptables.\n");
        }
    }

    fclose(fp);
    printf("Datos guardados en niveles_actuales.txt exitosamente.\n");
}

void editar_datos_historicos(Zona *zona) {
    printf("Editando datos historicos de la zona %s\n", zona->nombre);
    printf("Ingrese los nuevos datos historicos (CO2, SO2, NO2, PM2.5 para cada dia):\n");

    for (int i = 0; i < DIAS_HISTORICOS; i++) {
        printf("Día %d: ", i + 1);
        for (int j = 0; j < PARAMETROS_CONTAMINACION; j++) {
            scanf("%f", &zona->niveles[i][j]);
        }
    }
    printf("Datos historicos actualizados correctamente para la zona %s.\n", zona->nombre);
}