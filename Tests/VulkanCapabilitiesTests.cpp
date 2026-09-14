#include "Engine/Renderer/Vulkan/VulkanCapabilities.hpp"

#include <cassert>

int main() {
    using namespace urbanduty::renderer::vulkan;

    // The selected version must never exceed the Android platform, loader,
    // or physical-device API version. The physical-device limit was missing.
    assert(select_supported_api_version(
        VK_API_VERSION_1_3, VK_API_VERSION_1_3, VK_API_VERSION_1_3, 33) == VK_API_VERSION_1_3);
    assert(select_supported_api_version(
        VK_API_VERSION_1_3, VK_API_VERSION_1_3, VK_API_VERSION_1_1, 33) == VK_API_VERSION_1_1);
    assert(select_supported_api_version(
        VK_API_VERSION_1_3, VK_API_VERSION_1_3, VK_API_VERSION_1_3, 29) == VK_API_VERSION_1_1);
    assert(select_supported_api_version(
        VK_API_VERSION_1_3, VK_API_VERSION_1_3, VK_API_VERSION_1_3, 24) == VK_API_VERSION_1_0);
    assert(select_supported_api_version(
        VK_API_VERSION_1_3, VK_API_VERSION_1_1, VK_API_VERSION_1_3, 33) == VK_API_VERSION_1_1);
    assert(select_supported_api_version(
        VK_API_VERSION_1_3, VK_API_VERSION_1_3, VK_API_VERSION_1_0, 33) == VK_API_VERSION_1_0);
    return 0;
}
