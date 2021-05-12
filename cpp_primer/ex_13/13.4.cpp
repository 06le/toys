13.33
因为folder里的message会改变，不能定义成Folder或Message.*/

13.34
#include <iostream>
#include <set>
#include <string>
using namespace std;
class message{
    friend class folders;
public:
    explicit message(const string &str = "") : contents(str)) {}  //folders被隐式初始化为空集合 explicit message() : contents(" "), folders(nullptr) {} 有何不同？
    //拷贝控制成员，用来管理指向本message的指针
    message(const message&);                  //拷贝构造函数
    message& operator=(const message&);       //拷贝赋值运算符
    ~message();                               //析构函数
    void save(Folder&);                       //向给定的Folder添加一条Message
    void remove(Folder&);                     //向给定的Folder删除一条Messaage
    //
    //试一下哪些函数必须在类内定义，哪些可以在类外定义?????
    //
void save(Folder& ex1)
{
    folders.insert(&ex1);       //将给定folder添加到我们的folder列表中
    f.addMsg(this);             //将本message添加到ex1的message集合中
}
void remove(Folder& ex1)
{
    folders.erase(&ex1);       //将给定folder从我们的folder列表中删除
    f.remMsg(this);             //将本message从ex1的message集合中删除
}
//对每个指向原message的folder添加一个指向新message的指针
void add_to_Folders(const message &ex1)
{
    for(auto m : ex1.folders)       //对每个包含m的folder
    {
        f->addMsg(this);            //向该folder添加一个指向本message的指针
    }
}
message(const message& ex1) : contents(ex1.contents), folders(ex1.folders)
{
    add_to_Folders(ex1);            //将本消息添加到指向m的folder中
}
void remove_from_Folders()
{
    for(auto m : folders)       //有区别吗 *this.folders???? 对folders中的每个指针
    {
        m->remMsg(this)          //从该folder中删除本message
    }
}
~message()
{
    remove_from_Folders();
}
message& operator=(const message& ex1)
{
    remove_from_Folders(this);
    contents(ex1.contents);
    folders(ex1.folders);
    add_to_Folders(ex1);
    return *this;
}
void swap(message &ex1, message &ex2)
{
    using std::swap;
    //将每个消息的指针从它原来所在的folder中删除
    for (auto f : ex1.folders)
        f->remMsg(&ex1);
    for (auto f : ex2.folders)
        f->remMsg(&ex2);
    swap(ex1.contents, ex2.contents);
    swap(ex1.folders, ex2.folders);
    //将每个message的指针添加到它的新folde中
    for (auto f : ex1.folders)
        f->addMsg(&ex1);
    for (auto f : ex2.folders)
        f->addMsg(&ex2);
}
private:
    string contents;         //实际消息文本
    set<Folder*> folders;   //包含本Message的Folder

    //拷贝构造函数，拷贝赋值运算符和析构函数所使用的工具函数
    //将本message添加到指向参数的Folder中
    void add_to_Folders(const message&);
    //从folders中的每个Folder中删除本message
    void remove_from_Folders();
};

13.38
@Mooophy The copy and swap is an elegant way when working with dynamically allocated memory.
In the Message class ,nothing is allocated dynamically. Thus using this idiom makes no sense
and will make it more complicated to implement due to the pointers that point back.

@pezy In this case, swap function is special. It will be clear two Message's folders , then swap members, and added
themselves to each folders. But, Message assignment operator just clear itself, and copy the members, and added itself to
each folders. The rhs don't need to clear and add to folders. So, if using copy and swap to define, it will be very inefficiency.

