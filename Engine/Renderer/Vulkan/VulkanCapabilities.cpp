#include "Engine/Renderer/Vulkan/VulkanCapabilities.hpp"

namespace urbanduty::renderer::vulkan {

VkApiVersion select_supported_api_version(
    VkApiVersion requested,
    VkApiVersion loader_version,
    int android_api_level) noexcept {
    VkApiVersion candidate = requested < loader_version ? requested : loader_version;

    if (android_api_level >= 29 && candidate > VK_API_VERSION_1_1) {
        candidate = VK_API_VERSION_1_1;
    }

    if (candidate < VK_API_VERSION_1_0) {
        return VK_API_VERSION_1_0;
    }
    return candidate;
}

} // namespace urbanduty::renderer::vulkan
