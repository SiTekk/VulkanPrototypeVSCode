#include <cstring>
#include <iostream>
#include <vector>
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

    bool checkValidationLayerSupport(std::vector<const char*> validationLayers)
    {
        uint32_t amountOfLayers = 0;
        VkResult result = vkEnumerateInstanceLayerProperties(&amountOfLayers, nullptr);
        EvaluteVulkanResult(result);
        std::vector<VkLayerProperties> layers;
        layers.resize(amountOfLayers);
        result = vkEnumerateInstanceLayerProperties(&amountOfLayers, layers.data());
        EvaluteVulkanResult(result);

        uint32_t count = 0;

        for(uint32_t i = 0; i < validationLayers.size(); i++)
        {
            for(uint32_t k = 0; k < amountOfLayers; k++)
            {
                if(strcmp(validationLayers[i], layers[k].layerName) == 0)
                {
                    count++;
                    break;
                }
            }
        }

        if(count != validationLayers.size())
            return false;
        else
            return true;
    }

    bool checkInstanceExtensions()
    {
        uint32_t amountOfExtensions = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &amountOfExtensions, nullptr);
        std::vector<VkExtensionProperties> extensionProperties;
        extensionProperties.resize(amountOfExtensions);
        vkEnumerateInstanceExtensionProperties(nullptr, &amountOfExtensions, extensionProperties.data());
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


        std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };

        if(!checkValidationLayerSupport(validationLayers))
        {
            std::cout << "Validation Layer not Supported!\n";
            return -1;
        }

        VkInstanceCreateInfo instanceCreateInfo;
        instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instanceCreateInfo.pNext = nullptr;
        instanceCreateInfo.flags = 0;
        instanceCreateInfo.pApplicationInfo = &applicationInfo;
        instanceCreateInfo.enabledLayerCount = validationLayers.size();
        instanceCreateInfo.ppEnabledLayerNames = validationLayers.data();
        instanceCreateInfo.enabledExtensionCount = 0;
        instanceCreateInfo.ppEnabledExtensionNames = nullptr;

        VkResult result = vkCreateInstance(&instanceCreateInfo, nullptr, &instance);
        EvaluteVulkanResult(result);

        uint32_t amountOfPhysicalDevices = 0;
        result = vkEnumeratePhysicalDevices(instance, &amountOfPhysicalDevices, nullptr);
        EvaluteVulkanResult(result);
        std::vector<VkPhysicalDevice> physicalDevices;
        physicalDevices.resize(amountOfPhysicalDevices);
        result = vkEnumeratePhysicalDevices(instance, &amountOfPhysicalDevices, physicalDevices.data());
        EvaluteVulkanResult(result);

        uint32_t amountOfQueueFamilyProperties = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevices[0], &amountOfQueueFamilyProperties, nullptr);
        std::vector<VkQueueFamilyProperties> queueFamilyProperties;
        queueFamilyProperties.resize(amountOfQueueFamilyProperties);
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevices[0], &amountOfQueueFamilyProperties, queueFamilyProperties.data());

        std::vector<float> queuePriorities = {1.0, 1.0, 1.0, 1.0};

        VkDeviceQueueCreateInfo deviceQueueCreateInfo;
        deviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        deviceQueueCreateInfo.pNext = nullptr;
        deviceQueueCreateInfo.flags = 0;
        deviceQueueCreateInfo.queueFamilyIndex = 0; //Todo Check for Queue Families
        deviceQueueCreateInfo.queueCount = 4;
        deviceQueueCreateInfo.pQueuePriorities = queuePriorities.data();

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

        vkDeviceWaitIdle(device);
        vkDestroyDevice(device, nullptr);
        vkDestroyInstance(instance, nullptr);

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