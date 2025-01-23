#ifndef FUNCIONES_H
#define FUNCIONES_H

#define DIAS_HISTORICOS 30
#define PARAMETROS_CONTAMINACION 4
#define LIMITE_CO2 1000.0
#define LIMITE_SO2 500.0
#define LIMITE_NO2 200.0
#define LIMITE_PM25 25.0

typedef struct {
    char nombre[50];
    float niveles[DIAS_HISTORICOS][PARAMETROS_CONTAMINACION];
    float nivel_actual[PARAMETROS_CONTAMINACION];
    float contaminacion_actual;
} Zona;

void ingresar_niveles_actuales(Zona zonas[], int num_zonas);
void cargar_datos_desde_archivos(Zona zonas[], int *num_zonas);
float calcular_contaminacion(float co2, float so2, float no2, float pm25);
void calcular_promedios(Zona zonas[], int num_zonas, float promedios[]);
void predecir_niveles(Zona zonas[], int num_zonas, float predicciones[]);
void emitir_alertas_y_recomendaciones(Zona zonas[], int num_zonas, float predicciones[]);
void guardar_datos_a_archivo(Zona zonas[], int num_zonas, float promedios[], float predicciones[]);
#endif
