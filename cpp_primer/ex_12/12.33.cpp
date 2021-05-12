#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <memory>
#include <sstream>
#include <iterator>
using namespace std;
using line_no = vector<string>::size_type;

class QueryResult;
class TextQuery
{
public:
    TextQuery(ifstream&);
    QueryResult query(const string&) const;

private:
    shared_ptr<vector<string>> file;
    map<string, shared_ptr<set<line_no>>> wm;
};

class QueryResult
{
using re_iterator = set<line_no>::iterator;
friend ostream& print(ostream&, const QueryResult&);
public:
    QueryResult(string i,
                shared_ptr<set<line_no>> j,
                shared_ptr<vector<string>> k) :
        sought(i), lines(j), file(k) {}

    re_iterator begin() {return lines -> begin();}
    re_iterator end() {return lines -> end();}

    shared_ptr<vector<string>> get_file() {return file;}

private:
    string sought;
    shared_ptr<set<line_no>> lines;
    shared_ptr<vector<string>> file;
};



TextQuery::TextQuery(ifstream& is) : file(new vector<string>)
{
    string text;
    while(getline(is, text))
    {
        file -> push_back(text);
        int n = file -> size() - 1;
        istringstream line(text);
        string word;
        while(line >> word)
        {
            auto &lines = wm[word];
            if(!lines)
                lines.reset(new set<line_no>);
            lines -> insert(n);
        }
    }
}

QueryResult TextQuery::query(const string& sought) const
{
    static shared_ptr<set<line_no>> nodata(new set<line_no>);
    auto re = wm.find(sought);
    if(re == wm.end())
        return QueryResult(sought, nodata, file);
    else
        return QueryResult(sought, re -> second, file);
}

ostream& print(ostream& os, const QueryResult& res)
{
    os << "The " << res.sought << "³öÏÖ" << res.lines -> size() << " "
       << ((res.lines -> size()) > 1 ? "times" : "time") << endl;
    for(auto num : *res.lines)
    {
        os << "\t(line " << num + 1 << ") " << *(res.file -> begin() + num) << endl;
    }
    return os;
}

void runQueries(ifstream &infile)
{
    TextQuery tq(infile);
    while(true)
    {
        cout << "enter word to look for, or q to quit: ";
        string s;
        if(!(cin >> s) || (s == "q")) break;
        print(cout, tq.query(s)) << endl;
    }
}

int main()
{
    ifstream file("C:/Users/Administrator/Desktop/Cpp-test.txt");
    runQueries(file);
    return 0;
}
