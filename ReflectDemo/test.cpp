#include "biology.h"
#include "reflector.h"

int main()
{

	biology* b1 = getNewInstance<biology>("hero");
	biology* b2 = getNewInstance<biology>("monster");
	b1->show();
	b2->show();

	delete b1;
	delete b2;
	return 0;
}