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

               cout<<"En el header: "<<aux_1<<endl;
               
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
          cout<<"Se encontro "<<students.size()<<", registros."<<endl;
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
          //Recuperamos lo que habia en el header
          int aux = headerData();

          //Escribimos la posicion del valor eliminado. 
          fstream file(fileName, ios::binary | ios::in | ios::out);
          file.seekp(0);
          file.write((char *)&pos, sizeof(int));
          
          //Escribimos en la posicion eliminada el contenido del header
          file.seekp(sizeof(Alumno)*pos+5+59);
          file.write((char *)&aux, sizeof(int));

          file.close();
          cout<<headerData()<<endl;
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
          cout<<"Se encontro "<<students.size()<<", registros."<<endl;
          return students;
     }



};

int main() {
     string _file = "datos1.txt";
     FixedRecord fr(_file);

     fr.setHeader();
     //cout<<"Total tienes: "<<fr.getTotalSize()<<endl;

     Alumno a1 = {"1231","AARON     ", "SANT               ", "CS            ",5,233.4};
     Alumno a2 = {"1232","RODRIGO   ", "AVENDAN            ", "MECATRONIC    ",2,2332.4};
     Alumno a3 = {"1233","TAVO      ", "LAMI LAMI LAMI     ", "PSICOLOGIA    ",4,2622.2};
     Alumno a4 = {"1234","ALVARO    ", "ALLAUCA VLADIMIR   ", "CS            ",1,22.2};
     Alumno a5 = {"1235","MARIO SUSA", "ALLAUCA ZOTO       ", "PSICOLOGIA    ",2,22.2};
     Alumno a6 = {"1236","ALVARITO  ", "ALLAUCA EDASON     ", "FISICA        ",3,22.2};
     Alumno a7 = {"1237","DANIELS   ", "ALLAUCA MARSOON    ", "INGENIERIA CIV",4,22.2};
     fr.add(a1);
     fr.add(a2);
     fr.add(a3);
     fr.add(a4);
     fr.add(a5);
     fr.add(a6);
     

     auto registers = fr.load();
     
     for (auto it = registers.begin(); it != registers.end(); it++){
          cout<<(*it).codigo<<"  "<<(*it).nombre<<"  "<<(*it).apellidos<<"  "<<(*it).carrera<<"  "<<(*it).ciclo<<"  "<<(*it).mensualidad<<" "<<(*it).nextDel<<endl;
     }
     fr._delete(2);

     fr.add(a7);

     registers = fr.loadAll();
     
     for (auto it = registers.begin(); it != registers.end(); it++){
          cout<<(*it).codigo<<"  "<<(*it).nombre<<"  "<<(*it).apellidos<<"  "<<(*it).carrera<<"  "<<(*it).ciclo<<"  "<<(*it).mensualidad<<" "<<(*it).nextDel<<endl;
     }

     /*
     fr._delete(2);
     fr._delete(4);

     registers = fr.loadAll();

     for (auto it = registers.begin(); it != registers.end(); it++){
          cout<<(*it).codigo<<"  "<<(*it).nombre<<"  "<<(*it).apellidos<<"  "<<(*it).carrera<<"  "<<(*it).ciclo<<"  "<<(*it).mensualidad<<" "<<(*it).nextDel<<endl;
     }

     registers = fr.load();

     for (auto it = registers.begin(); it != registers.end(); it++){
          cout<<(*it).codigo<<"  "<<(*it).nombre<<"  "<<(*it).apellidos<<"  "<<(*it).carrera<<"  "<<(*it).ciclo<<"  "<<(*it).mensualidad<<" "<<(*it).nextDel<<endl;
     }*/
     //fr.readRecord(2);


     



     return 0;
}
