#pragma once

#include <cstdint>
#include <string>

namespace urbanduty::renderer {

struct VulkanContextConfig {
    const char* application_name = "Urban Duty: Police Simulator";
    std::uint32_t application_version = 1;
    std::uint32_t required_api_version = 0;
};

struct VulkanCapabilities {
    std::uint32_t api_version = 0;
    bool validation_layers_available = false;
    bool graphics_queue_available = false;
    bool present_queue_available = false;
};

class VulkanContext final {
public:
    VulkanContext() = default;
    ~VulkanContext();

    VulkanContext(const VulkanContext&) = delete;
    VulkanContext& operator=(const VulkanContext&) = delete;

    bool initialize(const VulkanContextConfig& config, std::string& error);
    void shutdown() noexcept;

    bool initialized() const noexcept { return initialized_; }
    const VulkanCapabilities& capabilities() const noexcept { return capabilities_; }

private:
    bool initialized_{false};
    VulkanCapabilities capabilities_{};
};

} // namespace urbanduty::renderer
