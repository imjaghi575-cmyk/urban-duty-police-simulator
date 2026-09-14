#include "Engine/Renderer/Vulkan/VulkanCapabilities.hpp"

#include <cassert>

int main() {
    using namespace urbanduty::renderer::vulkan;

    assert(select_supported_api_version(VK_API_VERSION_1_3, VK_API_VERSION_1_3, 33) == VK_API_VERSION_1_3);
    assert(select_supported_api_version(VK_API_VERSION_1_3, VK_API_VERSION_1_1, 33) == VK_API_VERSION_1_1);
    assert(select_supported_api_version(VK_API_VERSION_1_3, VK_API_VERSION_1_3, 29) == VK_API_VERSION_1_1);
    assert(select_supported_api_version(VK_API_VERSION_1_3, VK_API_VERSION_1_0, 24) == VK_API_VERSION_1_0);
    return 0;
}
