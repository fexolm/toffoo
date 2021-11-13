#pragma once
#include <memory>
#include <vector>
#include <vulkan/vulkan.h>
namespace toffoo::vk {
class Device;
class RenderPass;
class Semaphore;
class SwapChain {
private:
  VkSwapchainKHR swapchain;

  std::shared_ptr<Device> device;

  std::vector<VkImageView> imageViews;

  VkFormat imageFormat;
  VkExtent2D extent;

  void createImageViews(const std::vector<VkImage> &images,
                        VkFormat imageFormat);

public:
  SwapChain(std::shared_ptr<Device> device, int width, int height);

  VkSwapchainKHR handle();

  VkExtent2D getExtent();

  VkFormat getImageFormat();

  std::vector<VkImageView> &getImageViews();

  uint32_t getNextImageIdx(Semaphore &semaphore);

  void present(uint32_t imgIdx, Semaphore &wait);

  ~SwapChain();
};

std::shared_ptr<SwapChain> createSwapchain(std::shared_ptr<Device> device,
                                           int width, int height);
} // namespace toffoo::vk