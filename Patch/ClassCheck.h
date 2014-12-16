class ClassChecker
{
public:
	ClassChecker();
	bool CheckClassSizes();
private:
	static const int MAX_CLASSES = 128;
	size_t class_sizes[MAX_CLASSES];
};