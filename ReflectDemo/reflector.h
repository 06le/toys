#pragma once
#include <map>
#include <iostream>

using namespace std;

//map值类型 所有支持反射的类都需要继承这个类
class ReflectorObject
{
public:
	virtual ~ReflectorObject() {}
};

//对象工厂抽象类，生成对象实例
class ObjectFactory
{
public:
	ObjectFactory() { cout << "ObjectFactory()" << std::endl;}
	virtual ~ObjectFactory() { cout << "~ObjectFactory()" << std::endl; }
	virtual ReflectorObject* newInstance() = 0;
};

//反射器，用来管理（对象名，对象工厂）的映射关系
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

//反射器实例， 全局唯一
Reflector& reflector();

//需要被反射的类，需要在其对应的 cpp 文件中进行反射声明
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


//根据类名获取对象实例
template<typename T>
T* getNewInstance(const string& classname)
{
	return dynamic_cast<T*>(reflector().getNewInstance(classname));
}

