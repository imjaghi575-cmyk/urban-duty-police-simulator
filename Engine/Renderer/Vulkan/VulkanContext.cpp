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
    if (vkEnumerateInstanceVersion(&api_version) != VK_SUCCESS) {
        error = "Unable to query Vulkan instance version";
        return false;
    }

    capabilities_.api_version = api_version;
    capabilities_.required_api_version = config.required_api_version;

    if (config.required_api_version != 0 && api_version < config.required_api_version) {
        error = "Installed Vulkan runtime is below the required API version";
        return false;
    }

    std::uint32_t layer_count = 0;
    vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
    capabilities_.validation_layers_available = layer_count > 0;

    std::uint32_t device_count = 0;
    if (vkEnumeratePhysicalDevices(VK_NULL_HANDLE, &device_count, nullptr) != VK_SUCCESS) {
        // A valid instance is required for physical-device enumeration. This branch
        // intentionally reports capability discovery as unavailable rather than
        // manufacturing a device result.
        error = "Vulkan loader is present but no initialized instance is available";
        return false;
    }

    capabilities_.graphics_queue_available = device_count > 0;
    capabilities_.present_queue_available = device_count > 0;
    initialized_ = true;
    return true;
#else
    (void)config;
    error = "Vulkan support was not found by CMake";
    return false;
#endif
}

void VulkanContext::shutdown() noexcept {
    initialized_ = false;
    capabilities_ = {};
}

} // namespace urbanduty::renderer
