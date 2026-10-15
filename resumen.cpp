
#include <iostream>
#include <cstdio>      
#include <cstring>     

using namespace std;

struct Comanda {
    int idMozo;
    int codigoProducto;
    int cantidad;
    float comision;
};


void solicitar_nombre_archivo(char nombreArchivo[]);
void mostrar_mozo(int idMozo, int productosVendidos, float totalComision);
void mostrar_resumen_semanal(const char nombreArchivoComandas[]);

int main() {
    char nombreArchivoComandas[50];

    cout << "=== RESUMEN DEL CIERRE SEMANAL ===" << endl;
    solicitar_nombre_archivo(nombreArchivoComandas);

    mostrar_resumen_semanal(nombreArchivoComandas);

    return 0;
}


void solicitar_nombre_archivo(char nombreArchivo[]) {
    int semana;        
    char mes[10];      

    cout << "Ingrese el numero de semana (X): ";
    cin >> semana;
    cout << "Ingrese el mes (mm): ";
    cin >> mes;

    
    strcpy(nombreArchivo, "comandas_semana_s0-00.dat");
    nombreArchivo[17] = '0' + semana;   
    nombreArchivo[19] = mes[0];         
    nombreArchivo[20] = mes[1];         

    cout << "Se va a leer el archivo: " << nombreArchivo << endl;
}


void mostrar_mozo(int idMozo, int productosVendidos, float totalComision) {
    
    long pesos = (long) totalComision;                           
    int centavos = (int) ((totalComision - pesos) * 100 + 0.5);  
    if (centavos == 100) {                                       
        pesos++;
        centavos = 0;
    }

    cout << "Mozo ID: " << idMozo << endl;
    cout << " - Productos vendidos en la semana: " << productosVendidos << endl;
    cout << " - Comision a pagar: $" << pesos << ".";
    if (centavos < 10) {                                         
        cout << "0";
    }
    cout << centavos << endl;
    cout << "------------------------------------------------------------" << endl;
}


void mostrar_resumen_semanal(const char nombreArchivoComandas[]) {
    FILE *archComandas = fopen(nombreArchivoComandas, "rb");
    if (archComandas == NULL) {
        cout << "Error: No se pudo abrir el archivo " << nombreArchivoComandas << ". Verifique la semana y el mes, o si el cierre fue ejecutado." << endl;
        return;
    }

    cout << "\n------------------------------------------------------------" << endl;
    cout << "                    RESUMEN DE VENTAS POR MOZO              " << endl;
    cout << "------------------------------------------------------------" << endl;

    Comanda comanda;
    int totalProductosBuffet = 0;

    int idMozoActual = 0;
    int productosVendidos = 0;
    float totalComision = 0.0;
    bool hayDatos = false;

    while (fread(&comanda, sizeof(Comanda), 1, archComandas) == 1) {
        
        if (hayDatos && comanda.idMozo != idMozoActual) {
            mostrar_mozo(idMozoActual, productosVendidos, totalComision);
            productosVendidos = 0;
            totalComision = 0.0f;
        }

        idMozoActual = comanda.idMozo;
        hayDatos = true;

        productosVendidos += comanda.cantidad;
        totalComision += comanda.comision;
        totalProductosBuffet += comanda.cantidad;
    }

    if (hayDatos) {
        mostrar_mozo(idMozoActual, productosVendidos, totalComision);
    }
    else {
        cout << "El archivo no tiene ventas registradas." << endl;
    }

    cout << "\n============================================================" << endl;
    cout << " TOTAL GENERAL DE PRODUCTOS VENDIDOS POR EL BUFFET: " << totalProductosBuffet << endl;
    cout << "============================================================" << endl;

    fclose(archComandas);
}
