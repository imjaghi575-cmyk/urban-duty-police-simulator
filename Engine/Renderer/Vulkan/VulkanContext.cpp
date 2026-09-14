#include "Engine/Renderer/Vulkan/VulkanContext.hpp"

#if defined(URBANDUTY_HAS_VULKAN)
#include <vulkan/vulkan.h>
#include <vector>
#endif

namespace urbanduty::renderer {

VulkanContext::~VulkanContext() {
    shutdown();
}

bool VulkanContext::initialize(const VulkanContextConfig& config, std::string& error) {
    shutdown();

#if defined(URBANDUTY_HAS_VULKAN)
    std::uint32_t api_version = VK_API_VERSION_1_0;
    if (vkEnumerateInstanceVersion(&api_version) != VK_SUCCESS) {
        error = "Unable to query Vulkan instance version";
        return false;
    }

    capabilities_.api_version = api_version;
    if (config.required_api_version != 0 && api_version < config.required_api_version) {
        error = "Installed Vulkan runtime is below the required API version";
        return false;
    }

    VkApplicationInfo app_info{};
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName = config.application_name;
    app_info.applicationVersion = config.application_version;
    app_info.pEngineName = "UrbanDuty Native Engine";
    app_info.engineVersion = 1;
    app_info.apiVersion = api_version;

    VkInstanceCreateInfo create_info{};
    create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    create_info.pApplicationInfo = &app_info;

    VkInstance instance = VK_NULL_HANDLE;
    if (vkCreateInstance(&create_info, nullptr, &instance) != VK_SUCCESS) {
        error = "Unable to create Vulkan instance";
        return false;
    }
    instance_ = reinterpret_cast<void*>(instance);

    std::uint32_t layer_count = 0;
    vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
    capabilities_.validation_layers_available = layer_count > 0;

    std::uint32_t device_count = 0;
    if (vkEnumeratePhysicalDevices(instance, &device_count, nullptr) != VK_SUCCESS || device_count == 0) {
        error = "No Vulkan physical device is available";
        shutdown();
        return false;
    }

    std::vector<VkPhysicalDevice> devices(device_count);
    if (vkEnumeratePhysicalDevices(instance, &device_count, devices.data()) != VK_SUCCESS) {
        error = "Unable to enumerate Vulkan physical devices";
        shutdown();
        return false;
    }

    bool graphics_queue_found = false;
    for (VkPhysicalDevice candidate : devices) {
        std::uint32_t family_count = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(candidate, &family_count, nullptr);
        std::vector<VkQueueFamilyProperties> families(family_count);
        vkGetPhysicalDeviceQueueFamilyProperties(candidate, &family_count, families.data());
        for (const VkQueueFamilyProperties& family : families) {
            if ((family.queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0) {
                graphics_queue_found = true;
                break;
            }
        }
        if (graphics_queue_found) {
            break;
        }
    }

    capabilities_.graphics_queue_available = graphics_queue_found;
    capabilities_.present_queue_available = false; // Requires an SDL-created presentation surface.
    if (!graphics_queue_found) {
        error = "No graphics-capable Vulkan queue family is available";
        shutdown();
        return false;
    }

    initialized_ = true;
    return true;
#else
    (void)config;
    error = "Vulkan support was not found by CMake";
    return false;
#endif
}

void VulkanContext::shutdown() noexcept {
#if defined(URBANDUTY_HAS_VULKAN)
    if (instance_ != nullptr) {
        vkDestroyInstance(reinterpret_cast<VkInstance>(instance_), nullptr);
    }
#endif
    instance_ = nullptr;
    initialized_ = false;
    capabilities_ = {};
}

} // namespace urbanduty::renderer
