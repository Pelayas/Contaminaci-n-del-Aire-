#include <stdio.h>
#include <string.h>
#include "funciones.h"

void cargarDatos(Zona zonas[], int n) {
    FILE *file = fopen("datos.txt", "r");
    if (!file) {
        printf("No se encontró archivo de datos. Se crearán datos nuevos.\n");
        for (int i = 0; i < n; i++) {
            sprintf(zonas[i].nombreZona, "Zona_%d", i + 1);
            for (int j = 0; j < 30; j++) {
                zonas[i].CO2[j] = zonas[i].SO2[j] = zonas[i].NO2[j] = zonas[i].PM25[j] = 0;
            }
        }
        return;
    }

    for (int i = 0; i < n; i++) {
        fscanf(file, "%s", zonas[i].nombreZona);
        for (int j = 0; j < 30; j++)
            fscanf(file, "%f %f %f %f", &zonas[i].CO2[j], &zonas[i].SO2[j], &zonas[i].NO2[j], &zonas[i].PM25[j]);
    }
    fclose(file);
}

void guardarDatos(Zona zonas[], int n) {
    FILE *file = fopen("datos.txt", "w");
    for (int i = 0; i < n; i++) {
        fprintf(file, "%s\n", zonas[i].nombreZona);
        for (int j = 0; j < 30; j++)
            fprintf(file, "%.2f %.2f %.2f %.2f\n", zonas[i].CO2[j], zonas[i].SO2[j], zonas[i].NO2[j], zonas[i].PM25[j]);
    }
    fclose(file);
}

float pedirFlotanteValido(const char *mensaje) {
    float valor;
    int resultado;
    do {
        printf("%s", mensaje);
        resultado = scanf("%f", &valor);
        while(getchar() != '\n'); // limpiar buffer
        if (resultado != 1) {
            printf("Entrada inválida. Por favor ingrese un número.\n");
        } else if (valor < 0) {
            printf("No se permiten valores negativos.\n");
        }
    } while (resultado != 1 || valor < 0);
    return valor;
}

void ingresarDatosActuales(Zona zonas[], int n) {
    for (int i = 0; i < n; i++) {
        printf("\n--- %s ---\n", zonas[i].nombreZona);
        zonas[i].co2Actual = pedirFlotanteValido("Ingrese CO2 actual: ");
        zonas[i].so2Actual = pedirFlotanteValido("Ingrese SO2 actual: ");
        zonas[i].no2Actual = pedirFlotanteValido("Ingrese NO2 actual: ");
        zonas[i].pm25Actual = pedirFlotanteValido("Ingrese PM2.5 actual: ");
        zonas[i].temperatura = pedirFlotanteValido("Temperatura (°C): ");
        zonas[i].viento = pedirFlotanteValido("Viento (km/h): ");
        zonas[i].humedad = pedirFlotanteValido("Humedad (%): ");

        for (int j = 29; j > 0; j--) {
            zonas[i].CO2[j] = zonas[i].CO2[j - 1];
            zonas[i].SO2[j] = zonas[i].SO2[j - 1];
            zonas[i].NO2[j] = zonas[i].NO2[j - 1];
            zonas[i].PM25[j] = zonas[i].PM25[j - 1];
        }

        zonas[i].CO2[0] = zonas[i].co2Actual;
        zonas[i].SO2[0] = zonas[i].so2Actual;
        zonas[i].NO2[0] = zonas[i].no2Actual;
        zonas[i].PM25[0] = zonas[i].pm25Actual;
    }
}

float promedioPonderado(float valores[]) {
    return valores[0];
}

void predecirContaminacion(Zona zonas[], int n) {
    for (int i = 0; i < n; i++) {
        zonas[i].co2Predicho = promedioPonderado(zonas[i].CO2);
        zonas[i].so2Predicho = promedioPonderado(zonas[i].SO2);
        zonas[i].no2Predicho = promedioPonderado(zonas[i].NO2);
        zonas[i].pm25Predicho = promedioPonderado(zonas[i].PM25);
        printf("\n--- Predicción %s ---\n", zonas[i].nombreZona);
        printf("CO2: %.2f | SO2: %.2f | NO2: %.2f | PM2.5: %.2f\n",
               zonas[i].co2Predicho, zonas[i].so2Predicho,
               zonas[i].no2Predicho, zonas[i].pm25Predicho);
    }
}

void generarAlertas(Zona zonas[], int n) {
    int hayAlerta = 0; // bandera para saber si se imprimió alguna alerta

    for (int i = 0; i < n; i++) {
        int alertaZona = 0; // bandera para esta zona

        if (zonas[i].co2Predicho > 500) {
            if (!alertaZona) {
                printf("Zona: %s\n", zonas[i].nombreZona);
                alertaZona = 1;
                hayAlerta = 1;
            }
            printf("  Alerta: CO2 elevado (%.2f ppm)\n", zonas[i].co2Predicho);
        }
        if (zonas[i].so2Predicho > 20) {
            if (!alertaZona) {
                printf("Zona: %s\n", zonas[i].nombreZona);
                alertaZona = 1;
                hayAlerta = 1;
            }
            printf("  Alerta: SO2 elevado (%.2f µg/m³)\n", zonas[i].so2Predicho);
        }
        if (zonas[i].no2Predicho > 40) {
            if (!alertaZona) {
                printf("Zona: %s\n", zonas[i].nombreZona);
                alertaZona = 1;
                hayAlerta = 1;
            }
            printf("  Alerta: NO2 elevado (%.2f µg/m³)\n", zonas[i].no2Predicho);
        }
        if (zonas[i].pm25Predicho > 25) {
            if (!alertaZona) {
                printf("Zona: %s\n", zonas[i].nombreZona);
                alertaZona = 1;
                hayAlerta = 1;
            }
            printf("  Alerta: PM2.5 elevado (%.2f µg/m³)\n", zonas[i].pm25Predicho);
        }
    }

    if (!hayAlerta) {
        printf("No se detectaron niveles de contaminacion por encima de los limites en ninguna zona.\n");
    }
}

void mostrarRecomendaciones(Zona zonas[], int n) {
    for (int i = 0; i < n; i++) {
        printf("\n--- Recomendaciones para %s ---\n", zonas[i].nombreZona);
        if (zonas[i].pm25Predicho > 25 || zonas[i].no2Predicho > 40)
            printf("-> Suspender actividades al aire libre.\n");
        if (zonas[i].co2Predicho > 500)
            printf("-> Restringir uso de vehículos privados.\n");
        if (zonas[i].so2Predicho > 20)
            printf("-> Limitar producción industrial temporalmente.\n");
    }
}

void exportarReporte(Zona zonas[], int n) {
    FILE *file = fopen("reporte.txt", "w");
    for (int i = 0; i < n; i++) {
        fprintf(file, "Zona: %s\n", zonas[i].nombreZona);
        fprintf(file, "CO2 actual: %.2f - Predicción: %.2f\n", zonas[i].co2Actual, zonas[i].co2Predicho);
        fprintf(file, "SO2 actual: %.2f - Predicción: %.2f\n", zonas[i].so2Actual, zonas[i].so2Predicho);
        fprintf(file, "NO2 actual: %.2f - Predicción: %.2f\n", zonas[i].no2Actual, zonas[i].no2Predicho);
        fprintf(file, "PM2.5 actual: %.2f - Predicción: %.2f\n\n", zonas[i].pm25Actual, zonas[i].pm25Predicho);
    }
    fclose(file);
    printf("Reporte exportado a 'reporte.txt'\n");
}
