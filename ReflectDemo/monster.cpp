#include "monster.h"


REFLECT(monster);
monster::monster()
{
	std::cout << "monster()" << std::endl;
}

monster::~monster()
{
	std::cout << "~monster()" << std::endl;
}

void monster::show()
{
	std::cout << "this is monster" << std::endl;
}