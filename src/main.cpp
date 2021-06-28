#include <iostream>
#include "vulkan/vulkan.h"

#define VULKANPROTOTYPE_VERSION VK_MAKE_VERSION(0, 1, 0)

namespace VulkanPrototype
{

    VkInstance instance;

    void EvaluteVulkanResult(VkResult result)
    {
        if (result != VK_SUCCESS)
        {
#ifdef _WIN32
            __debugbreak();
#endif
            std::cout << result;
        }
    }

    void printPhysicalDeviceStats(VkPhysicalDevice *device)
    {
        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(*device, &properties);
        std::cout << "API Version: " << VK_VERSION_MAJOR(properties.apiVersion) << "." << VK_VERSION_MINOR(properties.apiVersion) << "." << VK_VERSION_PATCH(properties.apiVersion) << std::endl;
        std::cout << "Driver Version: " << VK_VERSION_MAJOR(properties.driverVersion) << "." << VK_VERSION_MINOR(properties.driverVersion) << "." << VK_VERSION_PATCH(properties.driverVersion) << std::endl;
        std::cout << "Vendor ID: " << properties.vendorID << std::endl;
        std::cout << "Device ID: " << properties.deviceID << std::endl;
        std::cout << "Device Type: " << properties.deviceType << std::endl;
        std::cout << "Device Name: " << properties.deviceName << std::endl;

        VkPhysicalDeviceFeatures features;
        vkGetPhysicalDeviceFeatures(*device, &features);
        std::cout << "\nGeometry Shader: " << features.geometryShader << std::endl;

        VkPhysicalDeviceMemoryProperties memoryProperties;
        vkGetPhysicalDeviceMemoryProperties(*device, &memoryProperties);
        std::cout << "\nMemory Type Count: " << memoryProperties.memoryTypeCount << std::endl;
        std::cout << "Memory Heap Count: " << memoryProperties.memoryHeapCount << std::endl;

        uint32_t amountOfQueueFamilies = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(*device, &amountOfQueueFamilies, nullptr);
        VkQueueFamilyProperties *familyProperties = new VkQueueFamilyProperties[amountOfQueueFamilies];
        vkGetPhysicalDeviceQueueFamilyProperties(*device, &amountOfQueueFamilies, familyProperties);

        std::cout << "\nAmount of Queue Families: " << amountOfQueueFamilies << std::endl << std::endl;

        for (int i = 0; i < amountOfQueueFamilies; i++)
        {
            std::cout << "Queue Family Number: " << i << std::endl;
            std::cout << "Queue Graphics Bit: " << (familyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) << std::endl;
            std::cout << "Queue Compute Bit: " << (familyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT) << std::endl;
            std::cout << "Queue Transpher Bit: " << (familyProperties[i].queueFlags & VK_QUEUE_TRANSFER_BIT) << std::endl;
            std::cout << std::endl;
        }

        delete familyProperties;
    }

    int main(int argc, char *argv[])
    {
        VkApplicationInfo applicationInfo;
        applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        applicationInfo.pNext = nullptr;
        applicationInfo.pApplicationName = "VulkanPrototype";
        applicationInfo.applicationVersion = VULKANPROTOTYPE_VERSION;
        applicationInfo.pEngineName = nullptr;
        applicationInfo.engineVersion = 0;
        applicationInfo.apiVersion = VK_API_VERSION_1_2;

        VkInstanceCreateInfo instanceCreateInfo;
        instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instanceCreateInfo.pNext = nullptr;
        instanceCreateInfo.flags = 0;
        instanceCreateInfo.pApplicationInfo = &applicationInfo;
        instanceCreateInfo.enabledLayerCount = 0;
        instanceCreateInfo.ppEnabledLayerNames = nullptr;
        instanceCreateInfo.enabledExtensionCount = 0;
        instanceCreateInfo.ppEnabledExtensionNames = nullptr;

        VkResult result = vkCreateInstance(&instanceCreateInfo, nullptr, &instance);
        EvaluteVulkanResult(result);

        uint32_t amountOfPhysicalDevices = 0;
        result = vkEnumeratePhysicalDevices(instance, &amountOfPhysicalDevices, nullptr);
        EvaluteVulkanResult(result);

        VkPhysicalDevice *physicalDevices = new VkPhysicalDevice[amountOfPhysicalDevices];
        result = vkEnumeratePhysicalDevices(instance, &amountOfPhysicalDevices, physicalDevices);
        EvaluteVulkanResult(result);

        for (int i = 0; i < amountOfPhysicalDevices; i++)
        {
            printPhysicalDeviceStats(physicalDevices);
            std::cout << "\n---------------------------------------\n\n";
        }

        delete physicalDevices;

        return 0;
    }
}

int main(int argc, char *argv[])
{
    VulkanPrototype::main(argc, argv);

    std::cout << "\nPress Enter to close console: ";
    getchar();

    return 0;
}