#include <iostream>
#include <fstream>
#include <cstring>
#include <ostream>
#include <vector>

using namespace std;

struct Alumno{
	char codigo[5];
	char nombre[11];
	char apellidos[20];
	char carrera[15];
};

ostream& operator<<(ostream& stream, Alumno& record){
	stream << "\n";
	stream.write(record.codigo, 5);
    stream.write(record.nombre,11);
    stream.write(record.apellidos, 20);
    stream.write(record.carrera, 15);
    stream<<"\0";
    stream << flush;
    return stream;
}

void print(Alumno alumno){
	cout <<	"Codigo: " << alumno.codigo <<
	"\nNombre: " << alumno.nombre <<
	"\nApellidos: " << alumno.apellidos <<
	"\nCarrera: " << alumno.carrera << endl;	
}

void readFromConsole(char buffer[], int size){
	string temp;
	cin >> temp;
	for (int i=0; i<size+1; i++) {
		buffer[i] = (i<temp.size())? temp[i]:' ';
	}
	buffer[size-1] = '\0';
	cin.clear();
}

class FixedRecord{
private:
	ifstream inFile;
	ofstream outFile;
	int length;

public:
	FixedRecord() = default;
	FixedRecord(string filename) {
		inFile.open(filename);
		outFile.open(filename,fstream::app);
		
	};
	
	vector<Alumno> load(){
		vector<Alumno> alumnos;
		if (inFile.is_open()){
            inFile.seekg(0,inFile.end);
    		this->length = inFile.tellg();
    		inFile.seekg(0,inFile.beg);
            // allocate mem
            int cantity = this->length / 51;
            for (int i =0;i<cantity;i++){
                Alumno a;
                inFile.seekg(51*i+(2*i)-(i*1/7)*(i-6));
                inFile.get(a.codigo,5,'\0');
                inFile.seekg((int)inFile.tellg()+1);
                inFile.get(a.nombre, 11,'\0');
                inFile.seekg((int)inFile.tellg()+1);
                inFile.get(a.apellidos,20,'\0');
                inFile.seekg((int)inFile.tellg()+1);
                inFile.get(a.carrera,15,'\0');    
                alumnos.push_back(a);
            }		
		}else {	
			cout <<"File Cant be Read" <<endl;
		}
		return alumnos;
	}
	void add(Alumno record){
		if (outFile.is_open()){
			// ask from console
			cout <<"Codigo: "; readFromConsole(record.codigo,5);
			cout <<"Nombres: "; readFromConsole(record.nombre,11);
			cout <<"Apellidos: "; readFromConsole(record.apellidos,20);
			cout <<"Carrera: "; readFromConsole(record.carrera,15);
			outFile << record;
		}
	}
	Alumno readRecord(int pos){
		Alumno temp; pos-=1;
        inFile.seekg(51*pos+(2*pos)-(pos*1/7)*(pos-6));
        inFile.get(temp.codigo,5,'\0');
        inFile.seekg((int)inFile.tellg()+1);
        inFile.get(temp.nombre, 11,'\0');
        inFile.seekg((int)inFile.tellg()+1);
        inFile.get(temp.apellidos,20,'\0');
        inFile.seekg((int)inFile.tellg()+1);
        inFile.get(temp.carrera,15,'\0');  
		return temp;
	}
    ~FixedRecord(){
        outFile.close();
        inFile.close();
    }
};


int main(){
	FixedRecord file("datos1.txt");
    cout << "--------Menu Pregunta 1---------"<<endl;
    int opc;
    do{
        cout <<"1. Leer todos los registros del archivo"<<endl;
        cout <<"2. Agregar un nuevo registro"<<endl;
        cout <<"3. Elegir posicion de un registro para leer"<<endl;
        cout <<"4. Salir"<<endl;
        
        cout<< "Escoja una opción: "<<endl;
        cin >> opc;

        switch(opc){
            case 1: {
                vector<Alumno> vec = file.load();
	            for (auto alumno: vec){
		            print(alumno);
	            };
                break;
            }
            case 2:{
                Alumno alumno;
                file.add(alumno);
                break;
            }
            case 3:{
                int pos;
                cout <<"Ingrese la posicion para leer:"<<endl;
                cin>>pos;
                Alumno alumno1 = file.readRecord(pos);
                cout<<"\n";
                print(alumno1);
                break;
            }
            case 4: break;                
        }        
    }while(opc != 4);
    
	return 0;
}
