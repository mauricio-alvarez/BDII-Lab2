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
	
	vector<Alumno> load_a(){
		vector<Alumno> alumnos;
		if (inFile.is_open()){
			Alumno temp;
			while (!inFile.eof()) {
				cout << temp.nombre << endl;
				alumnos.push_back(temp);
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

void print(Alumno alumno){
	cout <<	"Codigo: " << alumno.codigo <<
	"\nNombre: " << alumno.nombre <<
	"\nApellidos: " << alumno.apellidos <<
	"\nCarrera: " << alumno.carrera << endl;
	
}
int main(){
	/*FixedRecord file("datos1.txt");
	vector<Alumno> alumnos = file.load();
	cout << "Data Loaded!!" << endl;
	for (auto alumno: alumnos){
		cout << "Codigo: " << alumno.codigo <<
		", Nombre: " << alumno.nombre <<
		", Apellidos: " << alumno.apellidos <<
		", Carrera: " << alumno.carrera << endl;
	};
	*/
	ifstream file("datos1.txt");
	file.seekg(0,file.end);
	int length = file.tellg();
	file.seekg(0,file.beg);
	// allocate mem
	char* buffer = new char[length];
	
	//file.read(buffer, length);
	int cantity = length / 51;
	int n=0,let=0;	
	file.seekg(n);
	//for (int i=0; i<cantity;i++){
		Alumno a;
		file.get(a.codigo,5);
		file.get(a.nombre, 11);
		file.get(a.apellidos,20);
		file.get(a.carrera,15);
		print(a);
		char del[5];
		file.seekg(150);
		file.get(del,5,'\0');	
		file.get(del,5);
		cout<<del<<endl;
		cout<<file.tellg()<<endl;
	
	file.close();
//	cout.write(buffer, length);
	delete [] buffer;
	return 0;
}
