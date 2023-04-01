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

ostream& operator<<(ostream& stream, Alumno& a){
	stream << "\n";
	stream << a.codigo;
	stream << a.nombre;
	stream << a.apellidos;
	stream << a.carrera;
	stream << '\0';
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
	for (int i=0; i<temp.size(); i++) {
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
		if (outFile.is_open()){
			// ask from console
			cout <<"Codigo: "; readFromConsole(record.codigo,5);
			cout <<"Nombres: "; readFromConsole(record.nombre,11);
			cout <<"Apellidos: "; readFromConsole(record.apellidos,20);
			cout <<"Carrera: "; readFromConsole(record.carrera,15);
			//outFile << record;
			outFile << "\n";
			outFile.write(record.codigo, 5);
			outFile.write(record.nombre,11);
			outFile.write(record.apellidos, 20);
			outFile.write(record.carrera, 15);
			outFile<<"\0"; outFile<<flush;
			outFile.close();
		}
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
	Alumno alumno;
	file.add(alumno);
	vector<Alumno> vec = file.load();
	ejercicio_b(vec);

	return 0;
}
