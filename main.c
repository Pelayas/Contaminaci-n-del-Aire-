#include <stdio.h>
#include <string.h>
#include "funciones.h"

#define NUM_ZONAS 5

int main() {
    Zona zonas[NUM_ZONAS];
    int opcion;
    int datosIngresados = 0;
    int prediccionHecha = 0;

    cargarDatos(zonas, NUM_ZONAS);

    do {
        printf("\n--- MENU ---\n");
        printf("1. Ingresar datos actuales\n");
        printf("2. Ver predicción 24h\n");
        printf("3. Ver alertas\n");
        printf("4. Ver recomendaciones\n");
        printf("5. Exportar reporte\n");
        printf("6. Ver promedios históricos (30 días)\n");
        printf("7. Salir\n");  // Salir como última opción
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);
        getchar();

        switch(opcion) {
            case 1:
                ingresarDatosActuales(zonas, NUM_ZONAS);
                datosIngresados = 1;
                prediccionHecha = 1;
                break;

            case 2:
                if (!datosIngresados) {
                    printf("Primero debe ingresar datos actuales (opción 1).\n");
                } else {
                    predecirContaminacion(zonas, NUM_ZONAS);
                    prediccionHecha = 1;
                }
                break;

            case 3:
                if (!prediccionHecha) {
                    printf("Primero debe ingresar datos y ver la predicción (opciones 1 o 2).\n");
                } else {
                    generarAlertas(zonas, NUM_ZONAS);
                }
                break;

            case 4:
                if (!prediccionHecha) {
                    printf("Primero debe ingresar datos y ver la predicción (opciones 1 o 2).\n");
                } else {
                    mostrarRecomendaciones(zonas, NUM_ZONAS);
                }
                break;

            case 5:
                if (!datosIngresados) {
                    printf("Primero debe ingresar datos actuales (opción 1).\n");
                } else {
                    exportarReporte(zonas, NUM_ZONAS);
                }
                break;

            case 6:
                mostrarPromediosHistoricos(zonas, NUM_ZONAS);
                break;

            case 7:
                guardarDatos(zonas, NUM_ZONAS);
                printf("Saliendo...\n");
                break;

            default:
                printf("Opción inválida\n");
        }
    } while(opcion != 7);

    return 0;
}
