
#include <iostream>
#include <random>

int main()
{
	std::random_device rd;

	std::mt19937_64 mt(1);

	std::cout << "minimum mersenne twisteru je: " << mt.min() << std::endl;
	std::cout << "maximum mersenne twisteru je: " << mt.max() << std::endl;

	std::cout << "vygeneruj 10 nahodnych cisel:" << std::endl;
	for (int i = 0; i < 10; ++i) {
		std::cout << mt() << std::endl;
	}
	std::cin.ignore();
	return EXIT_SUCCESS;
}