#ifndef VULKANPROTOTYPE_H
#define VULKANPROTOTYPE_H

#include "vulkan/vulkan.h"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <cstring>
#include <iostream>
#include <vector>

#define VULKANPROTOTYPE_VERSION VK_MAKE_VERSION(0, 1, 0)

namespace VulkanPrototype
{
    class VulkanPrototype
    {

    public:
        VulkanPrototype();

        void EvaluteVulkanResult(VkResult result);
        int Run();

        GLFWwindow *window;
        VkDevice Device;
        VkInstance Instance;
        VkQueue Queue;
        VkSurfaceKHR Surface;

    private:
        bool checkInstanceExtensions();
        bool checkValidationLayerSupport(std::vector<const char *> validationLayers);
        int cleanupGlfw();
        int cleanupVulkan();
        int initializeGlfw();
        int initializeVulkan();
        int mainLoop();

        // std::vector<VkPhysicalDevice> physicalDevices;
        // std::vector<VkQueueFamilyProperties> queueFamilyProperties;
        // std::vector<float> queuePriorities = {1.0, 1.0, 1.0, 1.0};

        uint32_t xSize, ySize;

        // VkDeviceCreateInfo deviceCreateInfo;
        // VkDeviceQueueCreateInfo deviceQueueCreateInfo;
        // VkInstanceCreateInfo instanceCreateInfo;
        // VkPhysicalDeviceFeatures physicalDeviceFeatures;
        // VkResult result;
    };
}

#endif
