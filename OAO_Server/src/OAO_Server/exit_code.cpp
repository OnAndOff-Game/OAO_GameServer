#include <iostream>
#include "exit_code.h"

auto OAO::exit(exit_code code) -> void
{
	std::cout << "Please press enter to quit..." << std::endl;
	std::cin.get();
}