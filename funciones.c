#include <stdio.h>
#include <string.h>
#include "funciones.h"

#define LIMITE_CO2 500.0
#define LIMITE_SO2 20.0
#define LIMITE_NO2 40.0
#define LIMITE_PM25 25.0

// Variables globales para la fecha del reporte
int diaReporte = 0, mesReporte = 0, anioReporte = 0;

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

// Función auxiliar para pedir un entero dentro de un rango y validar la entrada
int pedirEnteroRango(const char *mensaje, int min, int max) {
    int valor;
    char c; // para detectar basura en buffer

    while (1) {
        printf("%s", mensaje);

        if (scanf("%d", &valor) != 1) {
            printf("Entrada inválida. Por favor ingrese un número entero.\n");
            // Limpiar buffer hasta el fin de línea
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }

        // Limpiar buffer para evitar lecturas erróneas
        while ((c = getchar()) != '\n' && c != EOF);

        if (valor < min || valor > max) {
            printf("Valor fuera del rango %d - %d. Intente de nuevo.\n", min, max);
            continue;
        }

        return valor;
    }
}

// Función pedirFechaReporte corregida y robusta
void pedirFechaReporte() {
    int d, m, a;
    while (1) {
        printf("Ingrese la fecha del reporte:\n");
        d = pedirEnteroRango("Día (1-31): ", 1, 31);
        m = pedirEnteroRango("Mes (1-12): ", 1, 12);
        a = pedirEnteroRango("Año (>= 2000): ", 2000, 9999);

        int diasMes[] = {31,28,31,30,31,30,31,31,30,31,30,31};
        // Validar año bisiesto
        if ((a % 400 == 0) || (a % 4 == 0 && a % 100 != 0)) {
            diasMes[1] = 29;
        }

        if (d > diasMes[m - 1]) {
            printf("Día %d no válido para el mes %d del año %d. Intente de nuevo.\n", d, m, a);
            continue;
        }

        diaReporte = d;
        mesReporte = m;
        anioReporte = a;
        break;
    }
}

void ingresarDatosActuales(Zona zonas[], int n) {
    pedirFechaReporte();

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
    float suma = 0, totalPesos = 0;
    for (int i = 0; i < 30; i++) {
        int peso = 30 - i;
        suma += valores[i] * peso;
        totalPesos += peso;
    }
    return suma / totalPesos;
}

float promedio(float valores[]) {
    float suma = 0;
    for (int i = 0; i < 30; i++) {
        suma += valores[i];
    }
    return suma / 30;
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
    int hayAlerta = 0;

    for (int i = 0; i < n; i++) {
        int alertaZona = 0;

        if (zonas[i].co2Actual > LIMITE_CO2 || zonas[i].co2Predicho > LIMITE_CO2) {
            if (!alertaZona) {
                printf("Zona: %s\n", zonas[i].nombreZona);
                alertaZona = 1;
                hayAlerta = 1;
            }
            printf("  Alerta: CO2 elevado (Actual: %.2f ppm, Predicción: %.2f ppm)\n", zonas[i].co2Actual, zonas[i].co2Predicho);
        }
        if (zonas[i].so2Actual > LIMITE_SO2 || zonas[i].so2Predicho > LIMITE_SO2) {
            if (!alertaZona) {
                printf("Zona: %s\n", zonas[i].nombreZona);
                alertaZona = 1;
                hayAlerta = 1;
            }
            printf("  Alerta: SO2 elevado (Actual: %.2f µg/m³, Predicción: %.2f µg/m³)\n", zonas[i].so2Actual, zonas[i].so2Predicho);
        }
        if (zonas[i].no2Actual > LIMITE_NO2 || zonas[i].no2Predicho > LIMITE_NO2) {
            if (!alertaZona) {
                printf("Zona: %s\n", zonas[i].nombreZona);
                alertaZona = 1;
                hayAlerta = 1;
            }
            printf("  Alerta: NO2 elevado (Actual: %.2f µg/m³, Predicción: %.2f µg/m³)\n", zonas[i].no2Actual, zonas[i].no2Predicho);
        }
        if (zonas[i].pm25Actual > LIMITE_PM25 || zonas[i].pm25Predicho > LIMITE_PM25) {
            if (!alertaZona) {
                printf("Zona: %s\n", zonas[i].nombreZona);
                alertaZona = 1;
                hayAlerta = 1;
            }
            printf("  Alerta: PM2.5 elevado (Actual: %.2f µg/m³, Predicción: %.2f µg/m³)\n", zonas[i].pm25Actual, zonas[i].pm25Predicho);
        }
    }

    if (!hayAlerta) {
        printf("No se detectaron niveles de contaminación por encima de los límites en ninguna zona.\n");
    }
}

void mostrarRecomendaciones(Zona zonas[], int n) {
    for (int i = 0; i < n; i++) {
        printf("\n--- Recomendaciones para %s ---\n", zonas[i].nombreZona);
        if (zonas[i].pm25Actual > LIMITE_PM25 || zonas[i].pm25Predicho > LIMITE_PM25 ||
            zonas[i].no2Actual > LIMITE_NO2 || zonas[i].no2Predicho > LIMITE_NO2)
            printf("-> Suspender actividades al aire libre.\n");
        if (zonas[i].co2Actual > LIMITE_CO2 || zonas[i].co2Predicho > LIMITE_CO2)
            printf("-> Restringir uso de vehículos privados.\n");
        if (zonas[i].so2Actual > LIMITE_SO2 || zonas[i].so2Predicho > LIMITE_SO2)
            printf("-> Limitar producción industrial temporalmente.\n");
    }
}

void exportarReporte(Zona zonas[], int n) {
    char meses[12][15] = {"Enero", "Febrero", "Marzo", "Abril", "Mayo", "Junio",
                          "Julio", "Agosto", "Septiembre", "Octubre", "Noviembre", "Diciembre"};

    char nombreArchivo[100];
    if (diaReporte == 0 || mesReporte == 0 || anioReporte == 0) {
        // Si no se ha ingresado fecha, usar nombre por defecto
        strcpy(nombreArchivo, "reporte.txt");
    } else {
        sprintf(nombreArchivo, "reporte_%02d_de_%s_de_%d.txt", diaReporte, meses[mesReporte - 1], anioReporte);
    }

    FILE *file = fopen(nombreArchivo, "w");
    if (!file) {
        printf("Error al abrir archivo para exportar reporte.\n");
        return;
    }

    for (int i = 0; i < n; i++) {
        fprintf(file, "Zona: %s\n", zonas[i].nombreZona);
        fprintf(file, "CO2 actual: %.2f ppm - Predicción: %.2f ppm\n", zonas[i].co2Actual, zonas[i].co2Predicho);
        fprintf(file, "SO2 actual: %.2f µg/m³ - Predicción: %.2f µg/m³\n", zonas[i].so2Actual, zonas[i].so2Predicho);
        fprintf(file, "NO2 actual: %.2f µg/m³ - Predicción: %.2f µg/m³\n", zonas[i].no2Actual, zonas[i].no2Predicho);
        fprintf(file, "PM2.5 actual: %.2f µg/m³ - Predicción: %.2f µg/m³\n", zonas[i].pm25Actual, zonas[i].pm25Predicho);

        int alertas = 0;
        if (zonas[i].co2Actual > LIMITE_CO2 || zonas[i].co2Predicho > LIMITE_CO2) {
            fprintf(file, "Alerta: CO2 elevado (Actual: %.2f ppm, Predicción: %.2f ppm)\n", zonas[i].co2Actual, zonas[i].co2Predicho);
            alertas = 1;
        }
        if (zonas[i].so2Actual > LIMITE_SO2 || zonas[i].so2Predicho > LIMITE_SO2) {
            fprintf(file, "Alerta: SO2 elevado (Actual: %.2f µg/m³, Predicción: %.2f µg/m³)\n", zonas[i].so2Actual, zonas[i].so2Predicho);
            alertas = 1;
        }
        if (zonas[i].no2Actual > LIMITE_NO2 || zonas[i].no2Predicho > LIMITE_NO2) {
            fprintf(file, "Alerta: NO2 elevado (Actual: %.2f µg/m³, Predicción: %.2f µg/m³)\n", zonas[i].no2Actual, zonas[i].no2Predicho);
            alertas = 1;
        }
        if (zonas[i].pm25Actual > LIMITE_PM25 || zonas[i].pm25Predicho > LIMITE_PM25) {
            fprintf(file, "Alerta: PM2.5 elevado (Actual: %.2f µg/m³, Predicción: %.2f µg/m³)\n", zonas[i].pm25Actual, zonas[i].pm25Predicho);
            alertas = 1;
        }
        if (!alertas) {
            fprintf(file, "No se detectaron niveles de contaminación por encima de los límites.\n");
        }

        fprintf(file, "Recomendaciones:\n");
        if (zonas[i].pm25Actual > LIMITE_PM25 || zonas[i].pm25Predicho > LIMITE_PM25 ||
            zonas[i].no2Actual > LIMITE_NO2 || zonas[i].no2Predicho > LIMITE_NO2) {
            fprintf(file, "-> Suspender actividades al aire libre.\n");
        }
        if (zonas[i].co2Actual > LIMITE_CO2 || zonas[i].co2Predicho > LIMITE_CO2) {
            fprintf(file, "-> Restringir uso de vehículos privados.\n");
        }
        if (zonas[i].so2Actual > LIMITE_SO2 || zonas[i].so2Predicho > LIMITE_SO2) {
            fprintf(file, "-> Limitar producción industrial temporalmente.\n");
        }
        fprintf(file, "------------------------------------------------------------\n");
    }

    fclose(file);
    printf("Reporte exportado a '%s'\n", nombreArchivo);
}

void mostrarPromediosHistoricos(Zona zonas[], int n) {
    printf("\n--- Promedios Históricos (últimos 30 días) ---\n");
    for (int i = 0; i < n; i++) {
        float promCO2 = promedio(zonas[i].CO2);
        float promSO2 = promedio(zonas[i].SO2);
        float promNO2 = promedio(zonas[i].NO2);
        float promPM25 = promedio(zonas[i].PM25);

        printf("\n%s\n", zonas[i].nombreZona);
        printf("CO2: %.2f ppm (%s)\n", promCO2, promCO2 > LIMITE_CO2 ? "Excede límite" : "Dentro del límite");
        printf("SO2: %.2f µg/m³ (%s)\n", promSO2, promSO2 > LIMITE_SO2 ? "Excede límite" : "Dentro del límite");
        printf("NO2: %.2f µg/m³ (%s)\n", promNO2, promNO2 > LIMITE_NO2 ? "Excede límite" : "Dentro del límite");
        printf("PM2.5: %.2f µg/m³ (%s)\n", promPM25, promPM25 > LIMITE_PM25 ? "Excede límite" : "Dentro del límite");
    }
}