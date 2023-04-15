#include "RandomFile.h"

using namespace std;

void writeFile(string filename){
    RandomFile file(filename);
    Record record;
    for (int i = 0; i < 1; i++)
    {
        record.setData();
        file.write_record(record);
    }   
}

void readFile(string filename){
    RandomFile file(filename);
    cout<<"--------- show all data -----------\n";
    file.scanAll();
    cout<<"\n--------- show all sorted data -----------\n";
    file.scanAllByIndex();   
    cout<<"\n--------- search for a specific name -------\n";
    Record *temp = file.search("Alex");
    temp->showData();
    cout<<endl;
    cout << temp->ciclo<<endl;
    
}

int main(){
    writeFile("data.dat");
    readFile("data.dat");
    return 0;
}
