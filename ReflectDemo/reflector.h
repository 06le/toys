#pragma once
#include <map>
#include <iostream>

using namespace std;

//mapֵ���� ����֧�ַ�����඼��Ҫ�̳������
class ReflectorObject
{
public:
	virtual ~ReflectorObject() {}
};

//���󹤳������࣬���ɶ���ʵ��
class ObjectFactory
{
public:
	ObjectFactory() { cout << "ObjectFactory()" << std::endl;}
	virtual ~ObjectFactory() { cout << "~ObjectFactory()" << std::endl; }
	virtual ReflectorObject* newInstance() = 0;
};

//���������������������������󹤳�����ӳ���ϵ
class Reflector
{
public:
	Reflector();
	~Reflector();
	void registerFactory(const string& classname, ObjectFactory* of);
	ReflectorObject* getNewInstance(const string& claaName);

private:
	map<string, ObjectFactory*> objectFactorys;
};

//������ʵ���� ȫ��Ψһ
Reflector& reflector();

//��Ҫ��������࣬��Ҫ�����Ӧ�� cpp �ļ��н��з�������
#define REFLECT(name)\
class ObjectFactory_##name : public ObjectFactory\
{\
	public:\
		ObjectFactory_##name() {cout << "ObjectFactory" << #name << "()" << endl;}\
		virtual ~ObjectFactory_##name() { cout << "~ObjectFactory_" << #name << "()" << endl;}\
		ReflectorObject* newInstance() {return new name();}\
};\
class Register_##name\
{\
	public:\
		Register_##name() { reflector().registerFactory(#name, new ObjectFactory_##name());}\
};\
Register_##name register_##name;


//����������ȡ����ʵ��
template<typename T>
T* getNewInstance(const string& classname)
{
	return dynamic_cast<T*>(reflector().getNewInstance(classname));
}

