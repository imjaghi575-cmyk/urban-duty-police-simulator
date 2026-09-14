#pragma once

#include <cstdint>

namespace urbanduty::renderer::vulkan {

std::uint32_t select_supported_api_version(
    std::uint32_t requested,
    std::uint32_t loader_version,
    std::uint32_t physical_device_version,
    int android_api_level) noexcept;

} // namespace urbanduty::renderer::vulkan
