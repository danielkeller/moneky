#include "stdafx.h"
#include "ClassCheck.h"
#include "SomeClasses.h"

#define HOTSWAP_CLASS(cls) case __COUNTER__ - base: return sizeof(cls)

size_t ClassSize(int cls)
{
	static const int base = __COUNTER__;
	switch (cls)
	{
		HOTSWAP_CLASS(Bar);
		HOTSWAP_CLASS(Foo);
	default: return __COUNTER__ - base;
	}
}

ClassChecker::ClassChecker()
{
	size_t num_classes = ClassSize(-1);
	for (size_t i = 0; i < num_classes && i < MAX_CLASSES; ++i)
		class_sizes[i] = ClassSize(i);
}

bool ClassChecker::CheckClassSizes()
{
	size_t num_classes = ClassSize(-1);
	for (size_t i = 0; i < num_classes && i < MAX_CLASSES; ++i)
		if (class_sizes[i] != ClassSize(i))
			return false;
	return true;
}