#include <cstring>
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

    bool checkValidationLayerSupport(uint32_t amountOfValidationLayers,const char * const * ppValidationLayrs)
    {
        uint32_t amountOfLayers = 0;
        VkResult result = vkEnumerateInstanceLayerProperties(&amountOfLayers, nullptr);
        EvaluteVulkanResult(result);
        VkLayerProperties *layers = new VkLayerProperties[amountOfLayers];
        result = vkEnumerateInstanceLayerProperties(&amountOfLayers, layers);
        EvaluteVulkanResult(result);

        uint32_t count = 0;

        for(uint32_t i = 0; i < amountOfValidationLayers; i++)
        {
            for(uint32_t k = 0; k < amountOfLayers; k++)
            {
                if(strcmp(ppValidationLayrs[i], layers[k].layerName) == 0)
                {
                    count++;
                    break;
                }
            }
        }

        delete layers;

        if(count != amountOfValidationLayers)
            return false;
        else
            return true;
    }

    bool checkInstanceExtensions()
    {
        uint32_t amountOfExtensions = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &amountOfExtensions, nullptr);
        VkExtensionProperties *extensionProperties = new VkExtensionProperties[amountOfExtensions];
        vkEnumerateInstanceExtensionProperties(nullptr, &amountOfExtensions, extensionProperties);
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

        uint32_t amountOfValidationLayers = 1;
        const char * const * validationLayers = new const char*[amountOfValidationLayers] { "VK_LAYER_KHRONOS_validation" };

        if(!checkValidationLayerSupport(amountOfValidationLayers, validationLayers))
        {
            std::cout << "Validation Layer not Supported!\n";
            return -1;
        }

        VkInstanceCreateInfo instanceCreateInfo;
        instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instanceCreateInfo.pNext = nullptr;
        instanceCreateInfo.flags = 0;
        instanceCreateInfo.pApplicationInfo = &applicationInfo;
        instanceCreateInfo.enabledLayerCount = amountOfValidationLayers;
        instanceCreateInfo.ppEnabledLayerNames = validationLayers;
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

        uint32_t amountOfQueueFamilyProperties = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevices[0], &amountOfQueueFamilyProperties, nullptr);
        VkQueueFamilyProperties* queueFamilyProperties = new VkQueueFamilyProperties[amountOfQueueFamilyProperties];
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevices[0], &amountOfQueueFamilyProperties, queueFamilyProperties);

        float *queuePriorities = new float[4] {1.0, 1.0, 1.0, 1.0};

        VkDeviceQueueCreateInfo deviceQueueCreateInfo;
        deviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        deviceQueueCreateInfo.pNext = nullptr;
        deviceQueueCreateInfo.flags = 0;
        deviceQueueCreateInfo.queueFamilyIndex = 0; //Todo Check for Queue Families
        deviceQueueCreateInfo.queueCount = 4;
        deviceQueueCreateInfo.pQueuePriorities = queuePriorities;

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

        delete queuePriorities;
        delete physicalDevices;
        delete validationLayers;

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