#include "Engine/Renderer/Vulkan/VulkanCapabilities.hpp"

namespace urbanduty::renderer::vulkan {

VkApiVersion select_supported_api_version(
    VkApiVersion requested,
    VkApiVersion loader_version,
    int android_api_level) noexcept {
    VkApiVersion candidate = requested == 0 ? loader_version : (requested < loader_version ? requested : loader_version);

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
