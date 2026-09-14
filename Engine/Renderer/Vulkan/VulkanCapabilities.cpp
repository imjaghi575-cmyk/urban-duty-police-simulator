#include "Engine/Renderer/Vulkan/VulkanCapabilities.hpp"

#include <vulkan/vulkan.h>

namespace urbanduty::renderer::vulkan {

std::uint32_t select_supported_api_version(
    std::uint32_t requested,
    std::uint32_t loader_version,
    std::uint32_t physical_device_version,
    int android_api_level) noexcept {
    std::uint32_t candidate = requested == 0 ? loader_version : (requested < loader_version ? requested : loader_version);

    if (physical_device_version != 0 && candidate > physical_device_version) {
        candidate = physical_device_version;
    }

    if (android_api_level >= 24 && android_api_level < 29 && candidate > VK_API_VERSION_1_0) {
        candidate = VK_API_VERSION_1_0;
    } else if (android_api_level >= 29 && android_api_level < 33 && candidate > VK_API_VERSION_1_1) {
        candidate = VK_API_VERSION_1_1;
    } else if (android_api_level >= 33 && candidate > VK_API_VERSION_1_3) {
        candidate = VK_API_VERSION_1_3;
    }

    if (candidate < VK_API_VERSION_1_0) {
        return VK_API_VERSION_1_0;
    }
    return candidate;
}

} // namespace urbanduty::renderer::vulkan
