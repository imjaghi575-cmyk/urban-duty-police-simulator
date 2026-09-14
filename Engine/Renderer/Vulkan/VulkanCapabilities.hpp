#pragma once

#include <vulkan/vulkan.h>

namespace urbanduty::renderer::vulkan {

VkApiVersion select_supported_api_version(
    VkApiVersion requested,
    VkApiVersion loader_version,
    VkApiVersion physical_device_version,
    int android_api_level) noexcept;

} // namespace urbanduty::renderer::vulkan
