

#include <iostream>    
#include <cstdio>      
#include <cstring>     
using namespace std;


struct Comanda {
    int   idMozo;           
    int   codigoProducto;   
    int   cantidad;         
    float comision;         
};

const int DIAS_SEMANA = 7;  

void armarNombreSemana(char nombre[], int semana, const char mes[]) {
    
    strcpy(nombre, "comandas_semana_s0-00.dat"); 

    nombre[17] = '0' + semana;   
    nombre[19] = mes[0];         
    nombre[20] = mes[1];         
}

void armarNombreDia(char nombre[], int d, const char mes[], const char anio[]) { 
    
    strcpy(nombre, "comandas_00-00-0000.dat");

    nombre[9]  = '0' + d / 10;   
    nombre[10] = '0' + d % 10;   
    nombre[12] = mes[0];         
    nombre[13] = mes[1];         
    nombre[15] = anio[0];        
    nombre[16] = anio[1];
    nombre[17] = anio[2];
    nombre[18] = anio[3];
}

void abrirDia(const char nombre[], FILE* dias[], Comanda actual[], int leido[], int& cantDias) {
    FILE* f = fopen(nombre, "rb");                
    if (f == NULL) {                              
        cout << "  " << nombre << ": no existe, se saltea.\n";   
        return;                                   
    }
    cout << "  " << nombre << ": OK\n";           
    dias[cantDias] = f;                           
    leido[cantDias] = fread(&actual[cantDias], sizeof(Comanda), 1, f);   
    cantDias++;                                   
}


int abrirSemana(int semana, const char mes[], const char anio[],FILE* dias[], Comanda actual[], int leido[]) {
    int cantDias = 0;                             
    int primerDia = 7 * (semana - 1) + 1;         
    int ultimoDia = 7 * semana;                   
    if (ultimoDia > 31) {                         
        ultimoDia = 31;                           
    }

    char nombreDia[30];                         
    for (int i = primerDia; i <= ultimoDia; i++) {           
        armarNombreDia(nombreDia, i, mes, anio);              
        abrirDia(nombreDia, dias, actual, leido, cantDias);   
    }
    return cantDias;                             
}


int buscarMenor(Comanda actual[], int leido[], int cantDias) {
    int menor = -1;                               
    for (int i = 0; i < cantDias; i++) {          
        if (leido[i] == 1 && (menor == -1 || actual[i].idMozo < actual[menor].idMozo)) {   
            menor = i;                           
        }
    }
    return menor;     
};

int aparear(const char nombreSemana[], FILE* dias[], Comanda actual[], int leido[], int cantDias) {
    FILE* archSemana = fopen(nombreSemana, "wb");     
    if (archSemana == NULL) {                         
        cout << "No se pudo crear " << nombreSemana << "\n";
        return -1;                                    
    }

    int totalRegistros = 0;                           
    int menor = buscarMenor(actual, leido, cantDias); 
    while (menor != -1) { 
        fwrite(&actual[menor], sizeof(Comanda), 1, archSemana);  
        totalRegistros++;                             
        leido[menor] = fread(&actual[menor], sizeof(Comanda), 1, dias[menor]); 
                                                     
        menor = buscarMenor(actual, leido, cantDias);
    }
    fclose(archSemana);                              
    return totalRegistros;                       
};

void cerrarDias(FILE* dias[], int cantDias) {
    for (int i = 0; i < cantDias; i++) {          
        fclose(dias[i]);                          
    }
};

int main() {
    FILE*   dias[DIAS_SEMANA];      
    Comanda actual[DIAS_SEMANA];    
    int     leido[DIAS_SEMANA];     

    int  semana;                   
    char mes[10];                   
    char anio[10];                 
    cout << "Semana del mes (1 a 5): ";
    cin >> semana;
    cout << "Mes (mm, ej: 07): ";
    cin >> mes;
    cout << "Año (aaaa, ej: 2026): ";
    cin >> anio;

    char nombreSemana[30];                            
    armarNombreSemana(nombreSemana, semana, mes);     

    cout << "\n";
    int cantDias = abrirSemana(semana, mes, anio, dias, actual, leido);   

    int total = aparear(nombreSemana, dias, actual, leido, cantDias);    

    cerrarDias(dias, cantDias);                       

    if (total == -1) {
        return 1;
    }
    cout << "\nCierre OK: " << cantDias << " planilla(s) diaria(s) unida(s) en "
         << nombreSemana << " (" << total << " ventas, ordenadas por mozo).\n";
    return 0;
};
