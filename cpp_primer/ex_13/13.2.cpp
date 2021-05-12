//13.22
#include <iostream>
#include <string>

using namespace std;

class Hasptr{
public:
    Hasptr(const string &s = string()) : ps(new string(s)), i(0) {} //����һ��string�Ĺ��캯��
    Hasptr(const Hasptr& ex1) : i(ex1.i), ps(new string (*ex1.ps)) {}            //�������캯��
    Hasptr &operator=(const Hasptr &ex1)                            //������ֵ�����
    {
        i = ex1.i;
        auto newp = new string(*ex1.ps);
        delete ps;
        ps = newp;
        return *this;
    }
private:
    int i;
    string *ps;
};

//13.24
    δ��������������ps��Դ���ᱻ�ͷţ������ڴ�й©
    δ���忽�����캯��:�ϳɵĿ������캯��ֻ�´��ps��ַ������ָ��ָ����ͬstring��*/

//13.25
    �������캯���Ϳ�����ֵ��������붯̬�����ڴ档�����Ǻ���ֵ�������
    ��ΪStrBlobPtrʹ������ָ�룬���Զ��ͷ��ڴ棬���в���Ҫ����������*/

//13.27
#include <iostream>
#include <string>

using namespace std;

class Hasptr{
public:
    //���캯��
    Hasptr(const string& s = string()) : ps(new string(s)), i(0), use(new size_t(1)) {}
    //�������캯��
    //���û�ù��캯����ʼֵ�б����д�ں����������ʲô��ͬ��
    //Ч�������𡣶��ڷ��������ͣ��ó�ʼ���б�ֻ�����һ�ο������캯�������ڹ��캯���ڻ��ȵ��ù��캯�����ٵ��ø�ֵ���㺯����
    //Hasptr(Hasptr& hp)
    //{
    //    ps = hp.ps;
    //    i = hp.i;
    //    use = hp.use;
    //    ++*use;
    //}
    Hasptr(Hasptr& hp) : ps(hp.ps), i(hp.i), use(hp.use) {++*use;}
    //��������
    ~Hasptr();
    //������ֵ�����
    Hasptr& operator=(const Hasptr& ex)
    {
       ++*ex.use; //�����Ҳ������������ü���
       if(--*use == 0) //�ݼ�����������ü���
       {
           delete use;   //���û�������û�
           delete ps;   //�ͷű��������ĳ�Ա
       }
       ps = ex.ps;
       i = ex.i;
       use = ex.use;
       return *this;    //���ر�����
    }
private:
    string *ps;
    int i;
    size_t *use;
};

Hasptr::~Hasptr()
{
    if(--*use == 0)     //������ü�����Ϊ0
    {
        delete ps;      //�ͷ�string�ڴ�
        delete use;     //�ͷż������ڴ�
    }
}

//13.28
#include <iostream>
#include <string>

using namespace std;

class TreeNode {
private:
    string value;
    int count;
    TreeNode *left;
    TreeNode *right;
public:
    //Ĭ�Ϲ��캯��
    TreeNode() : value(string()), count(1), left(nullptr), right(nullptr) {}
    //�������캯��
    TreeNode(const TreeNode& ex) : value(ex.value), count(ex.count), left(ex.left), right(ex.right) {}
    //��������
    ~TreeNode() {delete left; delete right;}
    //������ֵ�����
    TreeNode& operator=(const TreeNode& ex)
    {
        value = ex.value;
        count = ex.count;
        auto tem_le = ex.left;  //��left��right��������ʱ������ʹ�Ը�ֵ�����ֵ�����Ҳ������
        delete ex.left;
        left = tem_le;
        auto tem_ri = ex.right;
        delete ex.right;
        right = ex.right;
        return *this;
    }
};

class BinStrTree
{
private:
    TreeNode *root;
public:
    //Ĭ�Ϲ��캯��
    BinStrTree() : root(new TreeNode()) {}
    //�������캯��
    BinStrTree(const BinStrTree& ex) : root(new TreeNode(*ex.root)) {}
    //��������
    ~BinStrTree() {delete root;}
    //������ֵ�����
    BinStrTree& operator=(const BinStrTree& ex)
    {
        auto tem_ro = new TreeNode(*ex.root);
        delete ex.root;
        root = tem_ro;
        return *this;
    }
};
