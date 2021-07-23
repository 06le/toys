#pragma once
#include "biology.h"

class monster : public biology
{
public:
	monster();
	virtual ~monster();
	virtual void show();
};

