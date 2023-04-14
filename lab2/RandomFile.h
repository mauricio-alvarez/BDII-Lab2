#include <iostream>
#include<fstream>
#include<cstdio>
#include <map>
using namespace std;

struct Record
{
    char nombre[30];
    char carrera[20];
    int ciclo;

    void setData() {
        cout << "Alumno:";
        cin >> nombre;
        cout << "Carrera: ";
        cin >> carrera;
        cout << "Ciclo: ";
        cin >> ciclo;
    }

    void showData() {
        cout << "\nNombre: " << nombre;
        cout << "\nCarrera: " << carrera;
        cout << "\nCiclo : " << ciclo;
    }    
    
    string getKey(){
        return nombre;
    }
};

class RandomFile {
private:
    string fileName;
    string indexName;
    //map: mantiene ordenado las entradas
    map<string, long> index;

public:
    RandomFile(string _fileName) {
        this->fileName = _fileName;
        this->indexName = _fileName + "_ind";
        readIndex();
    }

    ~RandomFile(){
        writeIndex();
    }

    /*
    * leer el indice desde disco
    */
    void readIndex()
    {
        ifstream indexFile(this->indexName, ios::binary);
        // for 0 -> final
        //      leo 30 guardo en la primera entrada
        //      leo sizeof(long), guardo en la segunda entrada
      
    }

    /*
    * Regresa el indice al disco
    */
    void writeIndex(){
        ofstream indexFile(this->indexName, ios::binary);   
        for (auto& entry: index){
            indexFile.write(entry.first.c_str(), 30);
            indexFile.write((char*)&entry.second, sizeof(long));
        }
        indexFile.close();
    }

    /*
    * Escribe el registro al final del archivo de datos. Se actualiza el indice. 
    */
    void write_record(Record record) {
       ofstream dataFile;
       dataFile.open(this->fileName, ios::app | ios::binary);
       long posFisica = dataFile.tellp();
       dataFile.write((char*)&record, sizeof(Record));
       this->index[record.getKey()] = posFisica;
       dataFile.close();
    }


    /*
    * Busca un registro que coincida con la key
    */
    Record* search(string key) {
        Record* result = nullptr;
        // Inicializamos el iterador
        map<string,long>::iterator it;
        // Buscamos en el index
        it= index.find(key);

        if (it == index.end())
            // No ha sido encontrado
            return nullptr;
        else {
            // Guardamos la pos 
            long ind = it->second;
            // Abrimos el archivo
            ifstream file(this->fileName, ios::binary);
            // Nos posicionamos correctamente
            file.seekg(ind);
            // Guardamos los datos directamente pues es de tamaño fijo
            file.read((char*)&result, sizeof(Record));
        }

        return result;
    }

    /*
   * Muestra todos los registros de acuerdo como fueron insertados en el archivo de datos
   */
    void scanAll() {
       
    }

    /*
   * Muestra todos los registros de acuerdo a como estan ordenados en el indice
   */
    void scanAllByIndex() {
       
    }

};


