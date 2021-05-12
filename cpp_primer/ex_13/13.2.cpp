//13.22
#include <iostream>
#include <string>

using namespace std;

class Hasptr{
public:
    Hasptr(const string &s = string()) : ps(new string(s)), i(0) {} //接受一个string的构造函数
    Hasptr(const Hasptr& ex1) : i(ex1.i), ps(new string (*ex1.ps)) {}            //拷贝构造函数
    Hasptr &operator=(const Hasptr &ex1)                            //拷贝赋值运算符
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
    未定义析构函数：ps资源不会被释放，发生内存泄漏
    未定义拷贝构造函数:合成的拷贝构造函数只会拷贝ps地址，两个指针指向相同string。*/

//13.25
    拷贝构造函数和拷贝赋值运算符必须动态分配内存。而不是和右值共享对象。
    因为StrBlobPtr使用智能指针，会自动释放内存，所有不需要析构函数。*/

//13.27
#include <iostream>
#include <string>

using namespace std;

class Hasptr{
public:
    //构造函数
    Hasptr(const string& s = string()) : ps(new string(s)), i(0), use(new size_t(1)) {}
    //拷贝构造函数
    //如果没用构造函数初始值列表而是写在函数体里会有什么不同？
    //效率有区别。对于非内置类型，用初始化列表只需调用一次拷贝构造函数，而在构造函数内会先调用构造函数，再调用赋值运算函数。
    //Hasptr(Hasptr& hp)
    //{
    //    ps = hp.ps;
    //    i = hp.i;
    //    use = hp.use;
    //    ++*use;
    //}
    Hasptr(Hasptr& hp) : ps(hp.ps), i(hp.i), use(hp.use) {++*use;}
    //析构函数
    ~Hasptr();
    //拷贝赋值运算符
    Hasptr& operator=(const Hasptr& ex)
    {
       ++*ex.use; //递增右侧运算对象的引用计数
       if(--*use == 0) //递减本对象的引用计数
       {
           delete use;   //如果没有其他用户
           delete ps;   //释放本对象分配的成员
       }
       ps = ex.ps;
       i = ex.i;
       use = ex.use;
       return *this;    //返回本对象
    }
private:
    string *ps;
    int i;
    size_t *use;
};

Hasptr::~Hasptr()
{
    if(--*use == 0)     //如果引用计数变为0
    {
        delete ps;      //释放string内存
        delete use;     //释放计数器内存
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
    //默认构造函数
    TreeNode() : value(string()), count(1), left(nullptr), right(nullptr) {}
    //拷贝构造函数
    TreeNode(const TreeNode& ex) : value(ex.value), count(ex.count), left(ex.left), right(ex.right) {}
    //析构函数
    ~TreeNode() {delete left; delete right;}
    //拷贝赋值运算符
    TreeNode& operator=(const TreeNode& ex)
    {
        value = ex.value;
        count = ex.count;
        auto tem_le = ex.left;  //将left和right保存在临时变量，使自赋值情况赋值运算符也能运行
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
    //默认构造函数
    BinStrTree() : root(new TreeNode()) {}
    //拷贝构造函数
    BinStrTree(const BinStrTree& ex) : root(new TreeNode(*ex.root)) {}
    //析构函数
    ~BinStrTree() {delete root;}
    //拷贝赋值运算符
    BinStrTree& operator=(const BinStrTree& ex)
    {
        auto tem_ro = new TreeNode(*ex.root);
        delete ex.root;
        root = tem_ro;
        return *this;
    }
};
