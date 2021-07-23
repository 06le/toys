#include "reflector.h"

Reflector::Reflector(){ }

Reflector::~Reflector()
{
	map<string, ObjectFactory*>::iterator i = objectFactorys.begin();
	for (; i != objectFactorys.end(); ++i)
	{
		delete i->second;
	}
	objectFactorys.clear();
}

void Reflector::registerFactory(const string& classname, ObjectFactory* of)
{
	map<string, ObjectFactory*>::iterator i = objectFactorys.find(classname);
	if (i != objectFactorys.end())
		cout << "class has been in map";
	else
		objectFactorys[classname] = of;
}

ReflectorObject* Reflector::getNewInstance(const string& classname)
{
	map<string, ObjectFactory*>::iterator i = objectFactorys.find(classname);
	if (i != objectFactorys.end())
	{
		ObjectFactory* of = i->second;
		return of->newInstance();
	}
	return NULL;
}

Reflector& reflector()
{
	static Reflector reflector;
	return reflector;
}