#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>

using namespace std;

struct Alumno{
	char codigo[5];
	char nombre[11];
	char apellidos[20];
	char carrera[15];
};

void print(Alumno alumno){
	cout <<	"Codigo: " << alumno.codigo <<
	"\nNombre: " << alumno.nombre <<
	"\nApellidos: " << alumno.apellidos <<
	"\nCarrera: " << alumno.carrera << endl;	
}

class FixedRecord{
private:
	ifstream inFile;
	int length;

public:
	FixedRecord() = default;
	FixedRecord(string filename) {
		inFile.open(filename);	
		inFile.seekg(0,inFile.end);
		this->length = inFile.tellg();
		inFile.seekg(0,inFile.beg);
	};
	
	vector<Alumno> load(){
		vector<Alumno> alumnos;
		if (inFile.is_open()){
            // allocate mem
            int cantity = this->length / 51;
            for (int i =0;i<cantity;i++){
                Alumno a;
                inFile.seekg(51*i+(2*i));
                inFile.get(a.codigo,5,'\0');
                inFile.get(a.nombre, 11,'\0');
                inFile.get(a.apellidos,20,'\0');
                inFile.get(a.carrera,15,'\0');    
                alumnos.push_back(a);
            }
			inFile.close();			
		}else {	
			cout <<"File Cant be Read" <<endl;
		}
		return alumnos;
	}
	void add(Alumno record){
		return;
	}
	Alumno readRecord(int pos){
		Alumno temp;
		return temp;
	}
};

void ejercicio_b(vector<Alumno> vec){
    for (auto alumno: vec){
		print(alumno);
	};
}

int main(){
    FixedRecord file("datos1.txt");
	vector<Alumno> vec = file.load();
	ejercicio_b(vec);
    
	
	return 0;
}
