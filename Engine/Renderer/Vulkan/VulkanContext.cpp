#include "Engine/Renderer/Vulkan/VulkanContext.hpp"

#if defined(URBANDUTY_HAS_VULKAN)
#include <vulkan/vulkan.h>
#endif

namespace urbanduty::renderer {

VulkanContext::~VulkanContext() {
    shutdown();
}

bool VulkanContext::initialize(const VulkanContextConfig& config, std::string& error) {
    shutdown();

#if defined(URBANDUTY_HAS_VULKAN)
    std::uint32_t api_version = VK_API_VERSION_1_0;
    const VkResult version_result = vkEnumerateInstanceVersion(&api_version);
    if (version_result != VK_SUCCESS) {
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

    VkPhysicalDevice device = VK_NULL_HANDLE;
    std::uint32_t graphics_queue_family = 0;
    for (std::uint32_t i = 0; i < device_count && device == VK_NULL_HANDLE; ++i) {
        VkPhysicalDevice candidate = VK_NULL_HANDLE;
        if (vkEnumeratePhysicalDevices(instance, &device_count, &candidate) != VK_SUCCESS) {
            break;
        }
        // The compact foundation only needs a deterministic graphics-capable device.
        std::uint32_t family_count = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(candidate, &family_count, nullptr);
        std::vector<VkQueueFamilyProperties> families(family_count);
        vkGetPhysicalDeviceQueueFamilyProperties(candidate, &family_count, families.data());
        for (std::uint32_t family = 0; family < family_count; ++family) {
            if ((families[family].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0) {
                device = candidate;
                graphics_queue_family = family;
                break;
            }
        }
    }

    (void)graphics_queue_family;
    capabilities_.graphics_queue_available = device != VK_NULL_HANDLE;
    capabilities_.present_queue_available = false; // Requires an SDL-created presentation surface.
    if (!capabilities_.graphics_queue_available) {
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
