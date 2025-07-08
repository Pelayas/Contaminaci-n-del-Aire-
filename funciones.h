typedef struct {
    char nombreZona[50];
    float CO2[30], SO2[30], NO2[30], PM25[30];
    float co2Actual, so2Actual, no2Actual, pm25Actual;
    float temperatura, viento, humedad;
    float co2Predicho, so2Predicho, no2Predicho, pm25Predicho;
} Zona;

typedef struct {
    int dia;
    int mes;
    int anio;
} Fecha;

void cargarDatos(Zona zonas[], int n);
void guardarDatos(Zona zonas[], int n);
void ingresarDatosActuales(Zona zonas[], int n);
void predecirContaminacion(Zona zonas[], int n);
void generarAlertas(Zona zonas[], int n);
void mostrarRecomendaciones(Zona zonas[], int n);
void exportarReporte(Zona zonas[], int n);
void mostrarPromediosHistoricos(Zona zonas[], int n);  // <--- NUEVA FUNCIÓN
float promedio(float valores[]);                       // <--- FUNC AUXILIAR
float promedioPonderado(float valores[]);              // <--- FUNC DE PREDICCIÓN