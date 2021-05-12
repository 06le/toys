#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;
void readfile(const string& file, vector<string>& v1)
{
    ifstream in(file);
    if(in)
    {
        string line;
        while(getline(in, line))
            v1.push_back(line);
    }
}

int main()
{
    vector<string> vec;
    readfile("C:/Users/Administrator/Desktop/C.txt", vec);
    for(auto p : vec)
        cout << p;
    return 0;
}
