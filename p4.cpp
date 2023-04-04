#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

struct Matricula 
{   
    string codigo;    
    int ciclo;   
    float mensualidad;   
    string observaciones;
};

// out: write
ostream& operator<<(ostream& os, const Matricula& matricula)
{
    // matricula field
    int size = matricula.codigo.size();
    os.write((char*)&size, sizeof(int)); // :9
    os.write(matricula.codigo.c_str(), size); // 202110216

    // ciclo field
    size = to_string(matricula.ciclo).size();
    os.write((char*)&size, sizeof(int)); // :1
    os.write((char*)&matricula.ciclo, size); // 5

    // ciclo field
    size = to_string(matricula.mensualidad).size();
    os.write((char*)&size, sizeof(int)); // :7? :11
    os.write((char*)&matricula.mensualidad, size); // 2300.50

    // observaciones field
    size = matricula.observaciones.size();
    os.write((char*)&size, sizeof(int)); // :17
    os.write(matricula.observaciones.c_str(), size); // Sin observaciones

    return os;
}

// in: read
istream& operator>>(istream& is, Matricula& matricula)
{
    int size = matricula.codigo.size();
    is.read((char*)&matricula.codigo, size);


    return is;
}


class VariableRecordBin
{
    public:
    VariableRecordBin(string fileName);
    Matricula readRecord(int pos);
    vector<Matricula> load();
    void add(const Matricula& record);

    private:
    string fileName;
};

VariableRecordBin::VariableRecordBin(string fileName) : fileName(fileName)
{

}

vector<Matricula> VariableRecordBin::load()
{
    return vector<Matricula>();
}


Matricula VariableRecordBin::readRecord(int pos)
{
    return Matricula();
}

void VariableRecordBin::add(const Matricula& record)
{
    ofstream file;
    file.open(fileName, ios::app | ios::binary);
    file << record;
    file.close();
}

int main() 
{
    VariableRecordBin file("test.bin");

    Matricula m1 = {"202110216", 5, 2300.50, "Sin observaciones"};
    Matricula m2 = {"201895245", 10, 1950, "Sin observaciones"};

    file.add(m1);
    file.add(m2);

    return 0;
}