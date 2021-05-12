//13.29
����Ҫ���ÿ������캯��������ֱ�ӽ���ָ�룬���Բ��ᵼ�µݹ�ѭ��*/

//13.30 --  13.31
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

class Hasptr{
    friend void swap(Hasptr&, Hasptr&);
    friend bool operator<(const Hasptr&, const Hasptr&);
public:
    Hasptr(const string &s = string()) : i(0), ps(new string(s)) {}                  //����һ��string�Ĺ��캯��
    Hasptr(const Hasptr& ex1) : i(ex1.i), ps(new string (*ex1.ps)) {}                //�������캯��
    Hasptr &operator=(const Hasptr &ex1)                                             //������ֵ�����
    {
        i = ex1.i;
        auto newp = new string(*ex1.ps);
        delete ps;
        ps = newp;
        return *this;
    }
    void swap(Hasptr& ex1, Hasptr& ex2) //Hasptr�汾swap����
    {
        using std::swap;
        swap(ex1.i, ex2.i);
        swap(ex1.ps, ex2.ps);
        cout << "�����������ض���swap�汾��ƥ��̶�������std�ж���İ汾����øú���" <<endl;
    }

private:
    int i;
    string *ps;
};
bool operator<(const Hasptr& ex1, const Hasptr& ex2)
{
    return *ex1.ps < *ex2.ps;
}

//13.32
���ᣬswap�����Ǳ���ֵ���ж���Ŀ����������ܣ���ָ��汾ֱ�ӽ���ָ�룬û�н��ж��࿽��������
