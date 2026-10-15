#include <iostream>
#include <cstring>
using namespace std;



/***************************ESTRUCTURAS UTILIZADAS**************************************/

struct ComandaHistorica {
	char fecha[11]; 
	char nombreMozo[50]; 
	int codigoProducto; int cantidad; float comision;
};

struct Comanda { 
	int idMozo; 
	int codigoProducto; 
	int cantidad; 
	float comision; 
};

struct Mozo { 
	int idMozo; 
	char nombre[50]; 
	char password[20]; 
	float totalComision; 
};

struct Producto{
	int codigo; 
	char descripcion[50]; 
	float precio; 
	int stockActual;
};

/***********************FIN ESTRUCTURAS UTILIZADAS**************************************/



/*********************************FUNCIONES*********************************************/

void armar_lista_de_mozos();
void guardarMozo(ComandaHistorica comanda_aux, int &id,FILE* mozos_aux);
void armar_comandas();
int tamanio(FILE* f);
void ordenar_por_fecha(ComandaHistorica* &arr,int len);
bool fecha_es_menor(ComandaHistorica* arr, int actual, int min);
void separar_por_fecha(ComandaHistorica* arr,int len);
void ordenar_por_mozo(ComandaHistorica* &aux,int len);
int id(char nombre[50],Mozo* &arr,int len);
void crear_archivo(ComandaHistorica* aux,char fecha[],int len);
void actualizar_inventario(int cod, int cant);

/*******************************FIN FUNCIONES*******************************************/



/************************************MAIN***********************************************/

int main(int argc, char** argv) {
	
	armar_lista_de_mozos();
	armar_comandas();
	
	return 0;
}

/**********************************FIN MAIN*********************************************/



/*********************************FUNCIONES*********************************************/

void armar_lista_de_mozos(){
	int id=1555;
	FILE* historial = fopen("comandas_historicas.dat","rb");
	ComandaHistorica comanda_aux;
	
	FILE* mozos_aux = fopen("mozos.dat","rb+");
    
    if (!mozos_aux) mozos_aux = fopen("mozos.dat","wb+"); 
	
	while(fread(&comanda_aux,sizeof(ComandaHistorica),1,historial)==1){
		guardarMozo(comanda_aux,id,mozos_aux);
	}
	
	fclose(historial);
	fclose(mozos_aux);
}

void guardarMozo(ComandaHistorica comanda_aux,int &id, FILE* mozos_aux){

	Mozo mozo_aux; 
	int encontrado=0;
	
	fseek(mozos_aux,0,SEEK_SET);

	while(fread(&mozo_aux,sizeof(Mozo),1,mozos_aux)==1 && encontrado!=1){
	
		if(strcmp(mozo_aux.nombre,comanda_aux.nombreMozo)==0){	
		
			fseek(mozos_aux,-(long)sizeof(Mozo),SEEK_CUR);
			mozo_aux.totalComision+=comanda_aux.comision;
	
			fwrite(&mozo_aux,sizeof(Mozo),1,mozos_aux);
			encontrado=1;
			break;
		}
	}

	if(encontrado==0){

		Mozo nuevo;
	
		nuevo.idMozo=id;id++;
		nuevo.totalComision=comanda_aux.comision;
		strcpy(nuevo.nombre,comanda_aux.nombreMozo);
		
		char pwdn[20]="";
		cout<<"Se esta registrando un nuevo mozo con id: "<<nuevo.idMozo<<"."<<endl;
		cout<<"Establezca su contrase�a: ";cin>>pwdn;
	 
		int i=0;
		while (pwdn[i]!='\0'){
			pwdn[i]=pwdn[i]+5;
			i++;
		}

		strcpy(nuevo.password,pwdn);
		
		fwrite(&nuevo,sizeof(Mozo),1,mozos_aux);
	}	

}
	
void armar_comandas(){

	
	FILE* historial = fopen("comandas_historicas.dat","rb");
	
	if(historial == NULL){
    	cout << "No se pudo abrir el archivo de comandas." << endl;
    	return;
	}
	
	fseek(historial,0,SEEK_SET);
	
	int cantidad_comandas = tamanio(historial);
	
	ComandaHistorica* arr = new ComandaHistorica[cantidad_comandas];
	
	fseek(historial,0,SEEK_SET);
	
	ComandaHistorica aux;
	int i=0;
	
	while(fread(&aux,sizeof(ComandaHistorica),1,historial)==1){
		
		arr[i]=aux; 
		i++;
	}
	
	ordenar_por_fecha(arr,cantidad_comandas);
	
	separar_por_fecha(arr,cantidad_comandas);
	fclose(historial);
}

int tamanio(FILE* f){
	
	fseek(f,0,SEEK_END);
	
	long cap= ftell(f);
	long estructura=sizeof(ComandaHistorica);
	
	int tam=cap/estructura;
	
	return tam;
}

void ordenar_por_fecha(ComandaHistorica* &arr,int len){
	for(int i=0;i<len-1;i++){
		int min=i;
		for(int j=i+1;j<len;j++){
		
			if(fecha_es_menor(arr,j,min)){
				min=j;

			}
		}
		ComandaHistorica aux = arr[i];
		arr[i]=arr[min];
		arr[min]=aux;
	}
}

bool fecha_es_menor(ComandaHistorica* arr, int actual, int min){
	
	int anio_actual = (arr[actual].fecha[8] - 48) * 10 + (arr[actual].fecha[9] - 48);
	int anio_min = (arr[min].fecha[8] - 48) * 10 + (arr[min].fecha[9] - 48);
	
	int mes_actual = (arr[actual].fecha[3] - 48) * 10 + (arr[actual].fecha[4] - 48);
	int mes_min = (arr[min].fecha[3] - 48) * 10 + (arr[min].fecha[4] - 48);
	
	int dia_actual = (arr[actual].fecha[0] - 48) * 10 + (arr[actual].fecha[1] - 48);
	int dia_min = (arr[min].fecha[0] - 48) * 10 + (arr[min].fecha[1] - 48);
	
	int total_actual=anio_actual*10000+mes_actual*100+dia_actual;
	int total_min=anio_min*10000+mes_min*100+dia_min;
	
	if(total_actual<total_min){
		return true;
	}
	else{
		return false;
	}
	
}

void separar_por_fecha(ComandaHistorica* arr,int len){

	int i=0, k=0;
	
	while(i<len){
		
		char fecha[11]="";
		strcpy(fecha,arr[i].fecha); 
		
		k=0;
		
		while( i<len && strcmp(arr[i].fecha,fecha)==0){
			i++;
			k++;
		}
		
		ComandaHistorica* aux = new ComandaHistorica[k];
		
		for(int l=0;l<k;l++){
			aux[l]=arr[l+(i-k)]; 
		}
		
		
		ordenar_por_mozo(aux,k);
		crear_archivo(aux,fecha,k);
		delete[] aux;
	}
}

void ordenar_por_mozo(ComandaHistorica* &aux,int len){
	
	FILE* mozos = fopen("mozos.dat","rb");
	
	if(mozos==NULL){
		cout<<"Error al abrir archivo de mozos"<<endl;
		return;
	}
	

	fseek(mozos,0,SEEK_END);
	int longitud = ftell(mozos)/sizeof(Mozo);
	Mozo* arr_mozos = new Mozo[longitud];
	fseek(mozos,0,SEEK_SET);
	
	Mozo mozo_aux; int i=0;
	while(i<longitud && fread(&mozo_aux,sizeof(Mozo),1,mozos)==1){
		arr_mozos[i]=mozo_aux;
		i++;
	}
	fclose(mozos);
	
	
	for(int i=0;i<len-1;i++){
		int min=i;
		for(int j=i+1;j<len;j++){
		
			if(id(aux[j].nombreMozo,arr_mozos,longitud)<id(aux[min].nombreMozo,arr_mozos,longitud)){
				min=j;
			
			}
		}
		ComandaHistorica comanda = aux[i];
		aux[i]=aux[min];
		aux[min]=comanda;
	}
	
	delete[] arr_mozos;
}

int id(char nombre[50],Mozo* &arr,int len){
	
	int codigo=0;
	
	for(int i=0;i<len;i++){
		if(strcmp(arr[i].nombre,nombre)==0){
			codigo=arr[i].idMozo;
		}
	}
	return codigo;
}

void crear_archivo(ComandaHistorica* aux, char fecha[],int len){
	
	Comanda* aux2 = new Comanda[len];
	
	FILE* mozos = fopen("mozos.dat","rb");
	
	fseek(mozos,0,SEEK_END);
	int longitud = ftell(mozos)/sizeof(Mozo);
	Mozo* arr_mozos = new Mozo[longitud];
	fseek(mozos,0,SEEK_SET);
	
	Mozo mozo_aux; int i=0;
	while(i<longitud && fread(&mozo_aux,sizeof(Mozo),1,mozos)==1){
		arr_mozos[i]=mozo_aux;
		i++;
	}
	fclose(mozos);
	
	for(int i=0;i<len;i++){
		
		aux2[i].idMozo=id(aux[i].nombreMozo,arr_mozos,longitud);
		 
		aux2[i].codigoProducto=aux[i].codigoProducto;
		
		aux2[i].cantidad=aux[i].cantidad;
		
		aux2[i].comision=aux[i].comision;
		

		actualizar_inventario(aux[i].codigoProducto,aux[i].cantidad);
	}
	
	char comanda_fecha[24]="comandas_";
	strcat(comanda_fecha,fecha);
	strcat(comanda_fecha,".dat");
	
	FILE* comanda_diaria = fopen(comanda_fecha,"wb");
	fwrite(aux2,sizeof(Comanda),len,comanda_diaria);
	fclose(comanda_diaria);
	
	delete[] aux2;
	delete[] arr_mozos;
}

void actualizar_inventario(int cod, int cant){
	FILE* inventario = fopen("inventario.dat","rb+");
	Producto aux;
	
	while(fread(&aux,sizeof(Producto),1,inventario)==1){
		if(aux.codigo==cod){
			aux.stockActual-=cant;
			fseek(inventario,-(long)sizeof(Producto),SEEK_CUR);
			fwrite(&aux,sizeof(Producto),1,inventario);
			fclose(inventario);
			return;
		}
	}
}
/*******************************FIN FUNCIONES*******************************************/
