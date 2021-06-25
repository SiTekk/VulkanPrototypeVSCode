#include <iostream>
#include "../lib/testLib.hpp"

int main(int, char**) 
{
    std::cout << VulkanPrototype::testFunction() << std::endl;
    std::cout << "Hello, world!\n";
    std::cout << "Test if everything worked!\n";

    return 0;
}