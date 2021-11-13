#pragma once
#include <memory>
#include <vulkan/vulkan.h>

namespace toffoo::vk {
class Device;
class CommandPool {
private:
  VkCommandPool commandPool;

  std::shared_ptr<Device> device;

public:
  CommandPool(std::shared_ptr<Device> device);

  VkCommandPool handle();

  ~CommandPool();
};

std::shared_ptr<CommandPool> createCommandPool(std::shared_ptr<Device> device);
} // namespace toffoo::vk