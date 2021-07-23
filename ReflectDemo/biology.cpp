#include "biology.h"
#include <iostream>


biology::biology()
{
	std::cout << "biology()" << std::endl;
}

biology::~biology()
{
	std::cout << "~biology()" << std::endl;
}

void biology::show()
{
	std::cout << "this is biology" << std::endl;
}