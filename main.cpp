#include "src\core\core.h"
#include <ctime>

int main()
{
	srand(static_cast<unsigned int>(time(NULL)));
	Core core;
	core.Run();
	return 0;
}