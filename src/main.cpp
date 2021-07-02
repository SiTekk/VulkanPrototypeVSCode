#include <iostream>
#include "vulkan/vulkan.h"

#define VULKANPROTOTYPE_VERSION VK_MAKE_VERSION(0, 1, 0)

namespace VulkanPrototype
{

    VkInstance instance;
    VkDevice device;

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

    //Test Commit 3

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

        uint32_t amountOfLayers = 0;
        vkEnumerateInstanceLayerProperties(&amountOfLayers, nullptr);
        VkLayerProperties *layers = new VkLayerProperties[amountOfLayers];
        vkEnumerateInstanceLayerProperties(&amountOfLayers, layers);

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

        VkDeviceQueueCreateInfo deviceQueueCreateInfo;
        deviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        deviceQueueCreateInfo.pNext = nullptr;
        deviceQueueCreateInfo.flags = 0;
        deviceQueueCreateInfo.queueFamilyIndex = 0; //Todo Check for Queue Families
        deviceQueueCreateInfo.queueCount = 4;
        deviceQueueCreateInfo.pQueuePriorities = nullptr;

        VkPhysicalDeviceFeatures physicalDeviceFeatures = {};

        VkDeviceCreateInfo deviceCreateInfo;
        deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        deviceCreateInfo.pNext = nullptr;
        deviceCreateInfo.flags = 0;
        deviceCreateInfo.queueCreateInfoCount = 1;
        deviceCreateInfo.pQueueCreateInfos = &deviceQueueCreateInfo;
        deviceCreateInfo.enabledLayerCount = 0;
        deviceCreateInfo.ppEnabledLayerNames = nullptr;
        deviceCreateInfo.enabledExtensionCount = 0;
        deviceCreateInfo.ppEnabledExtensionNames = nullptr;
        deviceCreateInfo.pEnabledFeatures = &physicalDeviceFeatures;

        //Pick fitting Device
        result = vkCreateDevice(physicalDevices[0], &deviceCreateInfo, nullptr, &device);
        EvaluteVulkanResult(result);

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