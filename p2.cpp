#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstring>
#include <sstream>
#include <sys/stat.h>
#include <filesystem>

using namespace std;

struct Alumno{
	char codigo[5];
	char nombre[11];
	char apellidos[20];
	char carrera[15];

     int ciclo;
     float mensualidad;

     int nextDel = -1;
};



class FixedRecord{
private:
     string fileName;
public:
     FixedRecord(string fileName) : fileName(fileName){
          ofstream file(fileName, ios::trunc);
          file.close();
     };
     void add(Alumno alumno){
          
          
          if(this->headerData()==-2){
               fstream file(fileName, ios::app | ios::binary);
               file.write((alumno.codigo), 5);
               file.write((alumno.nombre), 11);
               file.write((alumno.apellidos), 20);
               file.write((alumno.carrera), 15);
               file.write((char *)(&alumno.ciclo), sizeof(int));
               file.write((char *)(&alumno.mensualidad), sizeof(float));
               file.write((char *)(&alumno.nextDel), sizeof(int));
               file << '\n';

               file.close();
          }else{
               //Recuperamos el ultimo eliminado del registro desactivado
               fstream file(fileName, ios::binary | ios::in | ios::out);
               int aux_1;file.seekp(64*headerData()+5+59);
               file.read((char *)&aux_1, sizeof(int));

               //Volcamos el nuevo registro sobre el antiguo
               file.seekp(64*headerData()+5, ios::beg);
               file.write((alumno.codigo), 5);
               file.write((alumno.nombre), 11);
               file.write((alumno.apellidos), 20);
               file.write((alumno.carrera), 15);
               file.write((char *)(&alumno.ciclo), sizeof(int));
               file.write((char *)(&alumno.mensualidad), sizeof(float));
               file.write((char *)(&alumno.nextDel), sizeof(int));

               //Cambiamos el header con el aux_1
               file.seekp(0);
               file.write((char *)&aux_1, sizeof(int));
               
               file.close();
          }
          

          
     }
     vector<Alumno> loadAll(){
          vector<Alumno> students;
          Alumno student;
          ifstream file(fileName,std::ifstream::ate | ios::binary);
          
          int registerSize = file.tellg()/64;
          file.seekg(0, ios::beg);

          for (int i = 0; i < registerSize; i++){
               file.seekg(sizeof(student)*i+5, ios::beg);

               file.read(student.codigo, sizeof(student.codigo));
               file.read(student.nombre, sizeof(student.nombre));
               file.read(student.apellidos, sizeof(student.apellidos));
               file.read(student.carrera, sizeof(student.carrera));
               file.read((char *)&student.ciclo, sizeof(int));
               file.read((char *)&student.mensualidad, sizeof(float));
               file.read((char *)&student.nextDel, sizeof(int));

               students.push_back(student);
          }          
          file.close();
          return students;
     }
     Alumno readRecord(int pos){
          Alumno student;
          ifstream file(fileName,std::ifstream::ate | ios::binary);
          int registerSize = file.tellg()/sizeof(student);file.seekg(0, std::ios::beg);
          if(registerSize >= pos){          
               file.seekg(sizeof(student)*pos, ios::beg);

               file.read(student.codigo, sizeof(student.codigo));
               file.read(student.nombre, sizeof(student.nombre));
               file.read(student.apellidos, sizeof(student.apellidos));
               file.read(student.carrera, sizeof(student.carrera));
               file.read((char *)&student.ciclo, sizeof(int));
               file.read((char *)&student.mensualidad, sizeof(float));

          }else{
               cout<<"Index out of range"<<endl;
          }


          return student;
     }

     int headerData(){
          ifstream mak(fileName,std::ifstream::ate | ios::binary);
          mak.seekg(0, ios::beg);
          int aux;mak.read((char *)&aux, sizeof(int));
          mak.close();
          return aux;
     }

     bool _delete(int pos){
          //Recuperamos el header
          int aux = headerData();

          //Escribimos la posicion del valor a eliminar. 
          fstream file(fileName, ios::binary | ios::in | ios::out);
          //cout<<"Size register:  "<<file.tellg()/sizeof(Alumno);file.seekg(0, std::ios::beg);
          file.seekp(0);
          file.write((char *)&pos, sizeof(int));
          
          //Escribimos en la posicion eliminada el contenido del header
          file.seekp(sizeof(Alumno)*pos+5+59);
          file.write((char *)&aux, sizeof(int));

          file.close();
          return true;
     }

     void setHeader(){
          ifstream in(fileName,std::ifstream::ate | ios::binary);
          int registerSize = in.tellg();in.seekg(0, std::ios::beg);
          in.close();
          ofstream out(fileName, ios::app | ios::binary);int fisrDelete = -2;
          out.write((char*)(&fisrDelete), sizeof(int));
          out << '\n';
          out.close();
     }
     int getTotalSize(){
          ifstream in(fileName,std::ifstream::ate | ios::binary);
          int registerSize = in.tellg();in.seekg(0, std::ios::beg);
          in.close();
          return registerSize;
     }

     vector<Alumno> load(){
          vector<Alumno> students;
          Alumno student;
          ifstream file(fileName,std::ifstream::ate | ios::binary);
          
          int registerSize = file.tellg()/64;
          file.seekg(0, ios::beg);

          for (int i = 0; i < registerSize; i++){
               file.seekg(sizeof(student)*i+5, ios::beg);

               file.read(student.codigo, sizeof(student.codigo));
               file.read(student.nombre, sizeof(student.nombre));
               file.read(student.apellidos, sizeof(student.apellidos));
               file.read(student.carrera, sizeof(student.carrera));

               file.read((char *)&student.ciclo, sizeof(int));
               file.read((char *)&student.mensualidad, sizeof(float));
               file.read((char *)&student.nextDel, sizeof(int));

               if(student.nextDel == -1){
                    students.push_back(student);
               }
          }          
          file.close();
          return students;
     }



};

std::string aleatorio(int longitud_maxima) {
    std::string caracteres_permitidos = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int longitud = longitud_maxima;
    std::string texto = "";
    for (int i = 0; i < longitud; i++) {
        int indice = rand() % caracteres_permitidos.length();
        texto += caracteres_permitidos[indice];
    }
    return texto;
}
