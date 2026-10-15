#include <iostream>
#include <cstring>
using namespace std;

struct Producto {int codigo; char descripcion[50]; float precio; int stockActual;};

struct Mozo { int idMozo; char nombre[50]; char password[20]; float totalComision;};

struct Comanda { int idMozo; int codigoProducto; int cantidad; float comision;};

struct Node {Comanda comanda; Node* next;};

void dayManager(char comandas[50]);
bool mozoMaster2000(int mozoActual, char psw[20]);
bool inventoryCheck(int codigoProducto);
void comandaCarga(char archivo[50], Node* start, int mozo, int idProducto, int cantidad);
Node* loadList (Node* start, Comanda venta);

int main(int argc, char** argv) {	
	char comandas[50] = "comandas_";
	dayManager(comandas);
	FILE* fcoman = fopen(comandas,"ab");
	if (fcoman == NULL) {cout<<"No se pudo abrir el archivo."<< endl;return 1;};
	fclose(fcoman);
	int eleccion = 1;
	while (eleccion == 1) {
		int mozoActual;
		char psw[20];
		Node* start = NULL;
		cout<<"Ingrese su ID: "<<endl;cin>>mozoActual;
		cout<<"Ingrese la contrasenia:"<<endl;cin>> psw;
		bool verified = mozoMaster2000(mozoActual, psw);
		if (verified) {
			int codigoIngresado;
			int cantidad;
			float total;
			cout<<"Ingrese el codigo del producto:"<<endl;cin>> codigoIngresado;
			bool verifiedProduct= inventoryCheck(codigoIngresado);
			cout<<"Ingrese la cantidad:"<<endl;cin>>cantidad;
			if (verifiedProduct){
				comandaCarga(comandas,start,mozoActual,codigoIngresado, cantidad);
			};
		} else{
		eleccion = 0;
		fclose(fcoman);
		cout<<"Cerrando el programa";
		return 0;	
		};
		cout<<"Elija una opcion: 1.Cargar una venta. 0.Salir del programa"<<endl;
		cin>>eleccion;
		delete start;
	};
	return 0;
	};	
	

void dayManager (char fileName[50]) {
	char fecha[15] = "";
	cout<<"Por favor, ingrese la fecha en formato DD-MM-AAAA (Dia, Mes, Anio):"<<endl;
	cin>>fecha;
	char format[10]= ".dat";
	strcat(fileName,fecha);
	strcat(fileName, format);
	};

bool mozoMaster2000(int mozoIngresado, char psw[20]) {
		FILE* fmozo = fopen("mozos.dat","rb");
		if (fmozo == NULL) { cout<<"No se pudo abrir el archivo:mozos.dat."<< endl;	return false;};
		Mozo regMozo;
		while (fread(&regMozo, sizeof(Mozo), 1, fmozo) == 1) {
			if (mozoIngresado == regMozo.idMozo) {
				for (int i = 0; i<strlen(psw); i++){
					if(strlen(psw)!=strlen(regMozo.password)){
						cout<<"El usuario o contrasenia son incorrectos."<<endl;
						fclose(fmozo);
						return false;
					};
					int aux = (int)psw[i] + 5;
					if(aux!=(int)regMozo.password[i]){
						cout<<"El usuario o contrasenia son incorrectos."<<endl;
						fclose(fmozo);
						return false;
					};
				};
				cout<<"Bienvenido/a "<< regMozo.nombre<<endl;
				fclose(fmozo);
				return true;
			};
		};
		fclose(fmozo);
		cout<<"El usuario o contrasenia son incorrectos."<<endl;
		return false;
		};

bool inventoryCheck(int codigoProducto){
	FILE* finventario = fopen("inventario.dat", "rb");
	if (finventario == NULL) {cout<<"No se pudo abrir el archivo: inventario.dat."<< endl;return false;};
	Producto regInventario;
	while (fread(&regInventario, sizeof(Producto), 1, finventario) == 1) {
		if(codigoProducto == regInventario.codigo){		
			cout<<"Producto encontrado."<<endl;
			fclose(finventario);
			return true;
		};
	};
	cout<<"El producto no ha sido encontrado."<<endl;
	fclose(finventario);
	return false;
};

void comandaCarga(char archivo[50], Node* start, int mozo, int idProducto, int cantidad){
			FILE* fcomanRead = fopen(archivo,"rb");
			if (fcomanRead == NULL) {cout<<"No se pudo abrir el archivo."<< endl;return;};
			FILE* finventario = fopen("inventario.dat","rb+");
			if (finventario == NULL) {cout<<"No se pudo abrir el archivo."<< endl;return;};
			Producto regInventario;
			float comision = 0;
			float porcentaje = 0.10;
			while (fread(&regInventario, sizeof(Producto), 1, finventario) == 1) {
				if(idProducto == regInventario.codigo){		
				comision = regInventario.precio*cantidad*porcentaje;
				regInventario.stockActual-=cantidad;
            	fseek(finventario,-(long)sizeof(Producto),SEEK_CUR);
            	fwrite(&regInventario,sizeof(Producto),1,finventario);
            	fclose(finventario);
			};
			};
			fclose(finventario);
			Comanda regComanda;
			while (fread(&regComanda, sizeof(Comanda), 1, fcomanRead) == 1){
				start = loadList(start, regComanda);
			};
			Comanda addedComanda = {mozo, idProducto, cantidad, comision};
			start = loadList(start, addedComanda);
			fclose(fcomanRead);
			FILE* fcomanWrite = fopen(archivo,"wb");
			while(start !=NULL){
				fwrite(&start->comanda, sizeof(Comanda),1,fcomanWrite);
				Node* aux = start;
				start = start->next;
				delete aux;
			};
			fclose(fcomanWrite);
		cout<<"Comanda cargada."<<endl;
		return;
};

Node* createNode (Comanda value) {
	Node* newNode = new Node();
	newNode->comanda = value;
	newNode->next = NULL;
	return newNode;
};

Node* loadList (Node* start, Comanda venta){
	Node* newNode = createNode(venta);
	Node* aux;
	Node* curr = start;
	if (start ==NULL || start->comanda.idMozo > venta.idMozo){
		newNode->next = start;
		return newNode;
	} 
	while(curr->next !=NULL && curr->next->comanda.idMozo <venta.idMozo){
		curr = curr->next;
	};
	newNode->next = curr->next;
	curr->next = newNode;
	return start;
};


