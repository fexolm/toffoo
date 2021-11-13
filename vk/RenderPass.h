#pragma once
#include <memory>
#include <vulkan/vulkan.h>

namespace toffoo::vk {
class SwapChain;
class Device;
class RenderPass {
private:
  VkRenderPass renderPass;
  std::shared_ptr<SwapChain> swapchain;
  std::shared_ptr<Device> device;

public:
  RenderPass(std::shared_ptr<SwapChain> swapchain,
             std::shared_ptr<Device> device);

  VkRenderPass handle();

  ~RenderPass();
};

std::shared_ptr<RenderPass>
craeteRenderPass(std::shared_ptr<SwapChain> swapchain,
                 std::shared_ptr<Device> device);
} // namespace toffoo::vk