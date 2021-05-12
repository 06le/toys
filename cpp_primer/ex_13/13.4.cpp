13.33
��Ϊfolder���message��ı䣬���ܶ����Folder��Message.*/

13.34
#include <iostream>
#include <set>
#include <string>
using namespace std;
class message{
    friend class folders;
public:
    explicit message(const string &str = "") : contents(str)) {}  //folders����ʽ��ʼ��Ϊ�ռ��� explicit message() : contents(" "), folders(nullptr) {} �кβ�ͬ��
    //�������Ƴ�Ա����������ָ��message��ָ��
    message(const message&);                  //�������캯��
    message& operator=(const message&);       //������ֵ�����
    ~message();                               //��������
    void save(Folder&);                       //�������Folder���һ��Message
    void remove(Folder&);                     //�������Folderɾ��һ��Messaage
    //
    //��һ����Щ�������������ڶ��壬��Щ���������ⶨ��?????
    //
void save(Folder& ex1)
{
    folders.insert(&ex1);       //������folder��ӵ����ǵ�folder�б���
    f.addMsg(this);             //����message��ӵ�ex1��message������
}
void remove(Folder& ex1)
{
    folders.erase(&ex1);       //������folder�����ǵ�folder�б���ɾ��
    f.remMsg(this);             //����message��ex1��message������ɾ��
}
//��ÿ��ָ��ԭmessage��folder���һ��ָ����message��ָ��
void add_to_Folders(const message &ex1)
{
    for(auto m : ex1.folders)       //��ÿ������m��folder
    {
        f->addMsg(this);            //���folder���һ��ָ��message��ָ��
    }
}
message(const message& ex1) : contents(ex1.contents), folders(ex1.folders)
{
    add_to_Folders(ex1);            //������Ϣ��ӵ�ָ��m��folder��
}
void remove_from_Folders()
{
    for(auto m : folders)       //�������� *this.folders???? ��folders�е�ÿ��ָ��
    {
        m->remMsg(this)          //�Ӹ�folder��ɾ����message
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
    //��ÿ����Ϣ��ָ�����ԭ�����ڵ�folder��ɾ��
    for (auto f : ex1.folders)
        f->remMsg(&ex1);
    for (auto f : ex2.folders)
        f->remMsg(&ex2);
    swap(ex1.contents, ex2.contents);
    swap(ex1.folders, ex2.folders);
    //��ÿ��message��ָ����ӵ�������folde��
    for (auto f : ex1.folders)
        f->addMsg(&ex1);
    for (auto f : ex2.folders)
        f->addMsg(&ex2);
}
private:
    string contents;         //ʵ����Ϣ�ı�
    set<Folder*> folders;   //������Message��Folder

    //�������캯����������ֵ�����������������ʹ�õĹ��ߺ���
    //����message��ӵ�ָ�������Folder��
    void add_to_Folders(const message&);
    //��folders�е�ÿ��Folder��ɾ����message
    void remove_from_Folders();
};

13.38
@Mooophy The copy and swap is an elegant way when working with dynamically allocated memory.
In the Message class ,nothing is allocated dynamically. Thus using this idiom makes no sense
and will make it more complicated to implement due to the pointers that point back.

@pezy In this case, swap function is special. It will be clear two Message's folders , then swap members, and added
themselves to each folders. But, Message assignment operator just clear itself, and copy the members, and added itself to
each folders. The rhs don't need to clear and add to folders. So, if using copy and swap to define, it will be very inefficiency.

