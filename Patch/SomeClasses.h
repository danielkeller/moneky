struct Foo
{
	Foo()
	{
		std::cout << "static ctor!\n";
	}

	int x;
};

struct Bar
{
	int x, y;
};