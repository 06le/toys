/*
class Employee {
public:
    Employee();
    Employee(const string& name);

    const int id() const { return id_; }

private:
    string name_;
    int id_;
    static int s_increment;
};
int Employee::s_increment = 0;

Employee::Employee()
{
    id_ = s_increment++;
}

Employee::Employee(const string& name)
{
    id_ = s_increment++;
    name_ = name;
}*/
/*13.1 �������캯����һ�����캯��,����ͬ���͵�һ�������ʼ����һ������ʱ��ʲô.
       ��һ�������Ƕ������͵����ã����������������Ĭ��ֵ
       ʹ������
       *��һ�����󸳸���һ������ʱ
       *�����β��Ƿ���������
       *�������������Ƿ���������
       *�������б��ʼ��һ�������е�Ԫ�ػ�һ���ۺ����еĳ�Ա��
        */

/*13.2 rhs�ǲ����Ĳ���,�����ô��ε�ʱ��Ҫ��ֵ,��ֵ�͵��ÿ������캯��,�������캯����Ҫ��ֵ,��ֵ�ֵ��ÿ������캯��...
       �γ�������ѭ����*/

/*13.3 �����Ǹ���strblobʱ�������ptr��Ա��use-count���һ����
       �����Ǹ���strblobprtsʱ����ָ���Ա��ʹ�ü���������ġ�����Ϊ�ü������ڹ���ָ�룩*/

/*13.4  Point global;
        Point foo_bar(Point arg) // 1
        {
            Point local = arg, *heap = new Point(global); // 2, 3
            *heap = local;
            Point pa[ 4 ] = { local, *heap }; // 4, 5
        return *heap; // 6
        } */

/*13.5 Hasptr(const &Hasptr ex) : ps(new string (*ex.ps)), i(ex.i) {} */

/*13.6 ������ֵ������������Ǻ���������һ��������������ͬ���͵Ĳ���
       ��һ�������ֵ����һ��ͬ���Ͷ�����õ�
       �ϳɿ�����������Ҳ�����ÿ����static��Ա�������������Ӧ�ĳ�Ա
       ��δ�����Լ��Ŀ���������ͻ����ɺϳɿ��������*/

/*13.7 ���ᷢ��ǳ������������13.3��ͬ*/

/*13.8 Hasptr operator=(const &Hasptr ex) {ps = *ex.ps; i = *ex.i,; return *this;}*/

/*13.9 ���������ǳ�Ա�����������ܲ������޷���ֵ���������أ���ִ�к����壬�����ٳ�Ա��
       ����ĳЩ�࣬�ϳ�����������������֯�����͵Ķ������٣�����ϳ�����������Ϊ�գ����ٳ�Ա��
       */

/*13.10 ��strblob��������ʱ����̬�����ʹ�ü��������١�����ö�̬����û�й���ָ�룬�������ͷš�
        ��strblobpter��������ʱ����̬����Ķ��󽫲��ᱻ�ͷš�*/

/*13.11 ~Hasptr(){delete ps;}*/

/*13.12 3�� accum, item1, item2*/

/*13.13 #include <iostream>
#include <vector>
using namespace std;
struct X {
    X() {cout << "���캯��" << endl;}
    X(const X&) {cout << "��������" << endl;}
    ~X() {cout << "��������" << endl;}
    X& operator= (const X& ) {cout << "��ֵ�����" << endl; return *this;};

};
void fun (X& x)
{
    cout << "��������" << endl;
}
int main()
{
    X x1;
    vector <X> ex1;
    ex1.push_back(x1);
    auto a2 = new X (x1);
    delete a2;
    fun(x1);
    return 0;
}*/

/*13.14 �����ͬmysn*/

/*13.15 ��ı���һ���е��õ�����������Ϊ���Ƕ�����һ���������캯���������µ���š���������������ͬ��mysn��*/

/*13.16 ��ı䣬��Ϊ���������ã����ø�ֵ����������Ǻ���û���κθ�ֵ�����������������ͬ�ġ�*/

/*13.17 // from Github pezy
//13.14
#include <iostream>
class numbered {
public:
    numbered()
    {
        static int unique = 10;
        mysn = unique++;
    }
    int mysn;
};
void f(numbered s)
{
    std::cout << s.mysn << std::endl;
}
int main()
{
    numbered a, b = a, c = b;
    f(a);
    f(b);
    f(c);
}
//13.15
#include <iostream>
class numbered {
public:
    numbered()
    {
        static int unique = 10;
        mysn = unique++;
    }
    numbered(const numbered& n) { mysn = n.mysn + 1; }
    int mysn;
};
void f(numbered s)
{
    std::cout << s.mysn << std::endl;
}
int main()
{
    numbered a, b = a, c = b;
    f(a);
    f(b);
    f(c);
}
//13.16
#include <iostream>
class numbered {
public:
    numbered()
    {
        static int unique = 10;
        mysn = unique++;
    }
    numbered(const numbered& n) { mysn = n.mysn + 1; }
    int mysn;
};
void f(const numbered& s)
{
    std::cout << s.mysn << std::endl;
}
int main()
{
    numbered a, b = a, c = b;
    f(a);
    f(b);
    f(c);
}*/

/*13.18
#include <iostream>

using namespace std;

class Employee{
public:
    Employee();
    Employee(const string &n1) : name(n1) {pe_card = tem++;}
private:
    string name;
    int pe_card;
    static int tem;
};
int Employee::tem = 0;
Employee::Employee() {pe_card = tem++;}
int main()
{

    return 0;
}*/

/*13.19 ����Ҫ����Ϊ��ʵ����Ա���޷�����*/

