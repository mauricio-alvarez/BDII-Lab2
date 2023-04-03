#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

struct Alumno 
{     
    string nombre;   
    string apellidos;   
    string carrera;   
    float mensualidad; 
};

// For writing a new record to the file
ostream& operator<<(ostream& os, const Alumno& student)
{
    os << student.nombre << '|';
    os << student.apellidos << '|';
    os << student.carrera << '|';
    os << student.mensualidad << '\n';
    os << flush;
    return os;
}

// For reading a new record from the file
istream& operator>>(istream& is, Alumno& student)
{
    getline(is, student.nombre, '|');
    getline(is, student.apellidos, '|');
    getline(is, student.carrera, '|');
    is >> student.mensualidad;
    return is;
}

template <template <typename...> class Container, typename T>
void print(const Container<T>& container, ostream& os = cout)
{
    for (const T& elem : container) {
        os << elem << " ";
    }
    os << "\n";
}

class VariableRecord
{
    public:
    VariableRecord(string fileName);
    Alumno readRecord(int pos);
    vector<Alumno> load();
    void add(Alumno record);

    private:
    string fileName;
};


VariableRecord::VariableRecord(string fileName) : fileName(fileName)
{

}

// O(n), n: number of records in the file
Alumno VariableRecord::readRecord(int pos)
{
    ifstream file;
    file.open(fileName);

    Alumno student;

    int currentPos = 0;
    int currentRecord = 1;

    while (currentRecord < pos && file >> student) {
        currentPos = file.tellg();
        currentRecord++;
    }

    if (currentRecord == pos && file >> student) {
        // Read the desired record from the file
    } else {
        throw invalid_argument("Index out of range");
    }

    file.close();

    return student;
}


// O(n), n: number of records in the file
vector<Alumno> VariableRecord::load()
{
    ifstream file;
    file.open(fileName);

    vector<Alumno> students;
    Alumno student;

    while (file >> student) {
        students.emplace_back(student);
    }

    file.close();

    return students;
}

// O(1)
void VariableRecord::add(Alumno record)
{
    ofstream file;
    file.open(fileName, ios::app);
    file << record;
    file.close();
}

int main() 
{
    string fileName = "datos2.txt";
    VariableRecord VR(fileName);

    vector<Alumno> students = VR.load();
    print(students);

    Alumno newStudent = {"Luis", "Mendez", "Computer Science", 2000};

    VR.add(newStudent);

    Alumno bufferStudent = VR.readRecord(6);
    cout << bufferStudent;

    return 0;
}