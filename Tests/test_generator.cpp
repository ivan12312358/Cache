#include <iostream>
#include <cmath>
#include <ctime>

int main()
{
	size_t size{0}, cache_size{0};
	std::cin >> cache_size >> size ;
	std::cout << cache_size << std::endl << size << std::endl;

	std::srand(std::time(nullptr));

	for(int i = 0; i < size; i++)
	{
		std::cout << rand() % 5000 - 1000;

		std::cout << std::endl;
	}

	return 0;
}
