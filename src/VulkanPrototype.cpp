#include "VulkanPrototype.h"

namespace VulkanPrototype
{
    VulkanPrototype::VulkanPrototype()
    {
        xSize = 1280;
        ySize = 720;
    }

    void VulkanPrototype::EvaluteVulkanResult(VkResult result)
    {
        if (result != VK_SUCCESS)
        {
#ifdef _WIN32
            __debugbreak();
#endif
            std::cout << result;
        }
    }

    int VulkanPrototype::Run()
    {
        initializeGlfw();
        initializeVulkan();
        mainLoop();
        cleanupVulkan();
        cleanupGlfw();
        return 0;
    }

    bool VulkanPrototype::checkInstanceExtensions(uint32_t amountOfGlfwExtensions, const char **requiredGlfwExtensions)
    {
        uint32_t amountOfExtensions = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &amountOfExtensions, nullptr);
        std::vector<VkExtensionProperties> extensionProperties;
        extensionProperties.resize(amountOfExtensions);
        vkEnumerateInstanceExtensionProperties(nullptr, &amountOfExtensions, extensionProperties.data());

        uint32_t count = 0;

        for (uint32_t i = 0; i < amountOfGlfwExtensions; i++)
        {
            for (uint32_t k = 0; k < amountOfExtensions; k++)
            {
                if (strcmp(requiredGlfwExtensions[i], extensionProperties[k].extensionName) == 0)
                {
                    count++;
                    break;
                }
            }
        }

        if (count != amountOfGlfwExtensions)
            return false;
        else
            return true;
    }

    bool VulkanPrototype::checkValidationLayerSupport(std::vector<const char *> validationLayers)
    {
        uint32_t amountOfLayers = 0;
        VkResult result = vkEnumerateInstanceLayerProperties(&amountOfLayers, nullptr);
        EvaluteVulkanResult(result);
        std::vector<VkLayerProperties> layers;
        layers.resize(amountOfLayers);
        result = vkEnumerateInstanceLayerProperties(&amountOfLayers, layers.data());
        EvaluteVulkanResult(result);

        uint32_t count = 0;

        for (uint32_t i = 0; i < validationLayers.size(); i++)
        {
            for (uint32_t k = 0; k < amountOfLayers; k++)
            {
                if (strcmp(validationLayers[i], layers[k].layerName) == 0)
                {
                    count++;
                    break;
                }
            }
        }

        if (count != validationLayers.size())
            return false;
        else
            return true;
    }

    int VulkanPrototype::cleanupGlfw()
    {
        glfwDestroyWindow(Window);
        return 0;
    }

    int VulkanPrototype::cleanupVulkan()
    {
        vkDeviceWaitIdle(Device);

        vkDestroyDevice(Device, nullptr);
        vkDestroySurfaceKHR(Instance, Surface, nullptr);
        vkDestroyInstance(Instance, nullptr);

        return 0;
    }

    int VulkanPrototype::initializeGlfw()
    {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        Window = glfwCreateWindow(xSize, ySize, "VulkanPrototype", nullptr, nullptr);

        return 0;
    }

    int VulkanPrototype::initializeVulkan()
    {
        VkApplicationInfo applicationInfo;
        applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        applicationInfo.pNext = nullptr;
        applicationInfo.pApplicationName = "VulkanPrototype";
        applicationInfo.applicationVersion = VULKANPROTOTYPE_VERSION;
        applicationInfo.pEngineName = nullptr;
        applicationInfo.engineVersion = 0;
        applicationInfo.apiVersion = VK_API_VERSION_1_2;

        std::vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"};

        if (!checkValidationLayerSupport(validationLayers))
        {
            std::cout << "Validation Layer not Supported!\n";
            return -1;
        }

        std::vector<const char *> usedExtensions = {};

        uint32_t amountOfGlfwExtensions = 0;
        const char **requiredGlfwExtensions = glfwGetRequiredInstanceExtensions(&amountOfGlfwExtensions);

        if (!checkInstanceExtensions(amountOfGlfwExtensions, requiredGlfwExtensions))
        {
            std::cout << "Extension not Supported!\n";
            return -1;
        }

        VkInstanceCreateInfo instanceCreateInfo;
        instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instanceCreateInfo.pNext = nullptr;
        instanceCreateInfo.flags = 0;
        instanceCreateInfo.pApplicationInfo = &applicationInfo;
        instanceCreateInfo.enabledLayerCount = validationLayers.size();
        instanceCreateInfo.ppEnabledLayerNames = validationLayers.data();
        instanceCreateInfo.enabledExtensionCount = amountOfGlfwExtensions;
        instanceCreateInfo.ppEnabledExtensionNames = requiredGlfwExtensions;

        VkResult result = vkCreateInstance(&instanceCreateInfo, nullptr, &Instance);
        EvaluteVulkanResult(result);

        result = glfwCreateWindowSurface(Instance, Window, nullptr, &Surface);
        EvaluteVulkanResult(result);

        uint32_t amountOfPhysicalDevices = 0;
        result = vkEnumeratePhysicalDevices(Instance, &amountOfPhysicalDevices, nullptr);
        EvaluteVulkanResult(result);
        std::vector<VkPhysicalDevice> physicalDevices;
        physicalDevices.resize(amountOfPhysicalDevices);
        result = vkEnumeratePhysicalDevices(Instance, &amountOfPhysicalDevices, physicalDevices.data());
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
        deviceQueueCreateInfo.queueCount = 1;
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
        result = vkCreateDevice(physicalDevices[0], &deviceCreateInfo, nullptr, &Device);
        EvaluteVulkanResult(result);

        VkQueue queue;
        vkGetDeviceQueue(Device, 0, 0, &queue);

        return 0;
    }

    int VulkanPrototype::mainLoop()
    {
        while (!glfwWindowShouldClose(Window))
        {
            glfwPollEvents();
        }

        return 0;
    }

}