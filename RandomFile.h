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
    void readIndex(){
          ifstream dataFile(this->fileName,ios::ate | ios::binary);
          if (!dataFile) {cout<< "No se pudo abrir el archivo\n";}

          int registerSize = dataFile.tellg()/54;
          dataFile.seekg(0,ios::beg);
          Record auxiliarRecord;

          for (int i = 0; i < registerSize; i++){
               dataFile.read((char*)(&auxiliarRecord.nombre),sizeof(auxiliarRecord.nombre));
               dataFile.read((char*)(&auxiliarRecord.carrera),sizeof(auxiliarRecord.carrera));
               dataFile.read((char*)(&auxiliarRecord.ciclo),sizeof(int));
               this->index[auxiliarRecord.getKey()] = i*54;
          }
          
          dataFile.close();
          
    }
    void writeIndex(){
        ofstream indexFile(this->indexName, ios::binary);   
        for (auto& entry: index){
            indexFile.write(entry.first.c_str(), 30);
            indexFile.write((char*)&entry.second, sizeof(long));
        }
        indexFile.close();
    }

    void write_record(Record record) {
        fstream dataFile(this->fileName, ios::app | ios::binary);
        
        long posFisica = dataFile.tellp();

        dataFile.write((char*)&record.nombre, sizeof(record.nombre));
        dataFile.write((char*)&record.carrera, sizeof(record.carrera));
        dataFile.write((char*)&record.ciclo, sizeof(int));

        this->index[record.getKey()] = posFisica;
        dataFile.close();
    }
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

    void scanAll() {
        cout<<"SCAN"<<endl;
       ifstream dataFile(this->fileName,ios::ate | ios::binary);
          if (!dataFile) {cout<< "No se pudo abrir el archivo\n";}
          int registerSize = dataFile.tellg()/54;
          dataFile.seekg(0,ios::beg);
          Record auxiliarRecord;

          for (int i = 0; i < registerSize; i++){
               dataFile.read((char*)(&auxiliarRecord.nombre),sizeof(auxiliarRecord.nombre));
               dataFile.read((char*)(&auxiliarRecord.carrera),sizeof(auxiliarRecord.carrera));
               dataFile.read((char*)(&auxiliarRecord.ciclo),sizeof(int));
               auxiliarRecord.showData();
          }
          dataFile.close();
    }

    /*
   * Muestra todos los registros de acuerdo a como estan ordenados en el indice
   */
    void scanAllByIndex() {
        cout<<"SCAN BY INDEX"<<endl;
        ifstream dataFile(this->fileName,ios::ate | ios::binary);
        if (!dataFile) {cout<< "No se pudo abrir el archivo\n";}

        Record auxiliarRecord;
        
        for (auto it = index.begin(); it != index.end(); it++){
            dataFile.seekg((*it).second, ios::beg);
            dataFile.read((char*)(&auxiliarRecord.nombre),sizeof(auxiliarRecord.nombre));
            dataFile.read((char*)(&auxiliarRecord.carrera),sizeof(auxiliarRecord.carrera));
            dataFile.read((char*)(&auxiliarRecord.ciclo),sizeof(int));
            auxiliarRecord.showData();
        }
    }

};
