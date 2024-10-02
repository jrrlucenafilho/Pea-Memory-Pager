#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

vector<int> ReadInstance(string instance_path, int& page_quant)
{
    vector<int> page_refs;
    int number;
    ifstream fp(instance_path);

    if(!fp){
        cerr << "Error opening file" << instance_path;
        return page_refs;
    }

    fp >> page_quant;

    while(fp >> number){
        page_refs.push_back(number);
    }

    fp.close();

    return page_refs;
}

int main(void)
{
    int page_misses = 0;
    int page_quant;
    string instance_path;
    vector<int> page_refs;

    cin >> instance_path;

    page_refs = ReadInstance(instance_path, page_quant);

    if(page_refs.empty()){
        return 1;
    }

    cout << "Page quant: " << page_quant << '\n' << "Pages: ";

    for(int page : page_refs){
        cout << page << ' ';
    }

    return 0;
}
