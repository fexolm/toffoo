#pragma once
#include <memory>
#include <vector>
#include <vulkan/vulkan.hpp>

namespace toffoo::vk {
class SwapChain;
class RenderPass;
class Device;
class Framebuffer {
private:
  VkFramebuffer framebuffer;

  std::shared_ptr<Device> device;
  std::shared_ptr<SwapChain> swapchain; // swapchain own images
  std::shared_ptr<RenderPass> renderPass;

public:
  Framebuffer(std::shared_ptr<Device> device,
              std::shared_ptr<SwapChain> swapchain,
              std::shared_ptr<RenderPass> renderPass, VkImageView imageView);

  VkFramebuffer handle();

  ~Framebuffer();
};

std::vector<std::shared_ptr<Framebuffer>>
createFramebuffers(std::shared_ptr<Device> device,
                   std::shared_ptr<SwapChain> swapchain,
                   std::shared_ptr<RenderPass> renderPass);
} // namespace toffoo::vk