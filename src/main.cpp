#include "VulkanPrototype.h"

int main(int argc, char *argv[])
{
    VulkanPrototype::VulkanPrototype vulkanPrototype;
    vulkanPrototype.Run();

    std::cout << "\nPress Enter to close console: ";
    getchar();

    return 0;
}