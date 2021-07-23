#include "hero.h"


REFLECT(hero);

hero::hero()
{
	std::cout << "hero()" << std::endl;
}

hero::~hero()
{
	std::cout << "~hero()" << std::endl;
}

void hero::show()
{
	std::cout << "this is hero" << std::endl;
}
