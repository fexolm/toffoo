#pragma once

#include <memory>
#include <vulkan/vulkan.h>
namespace toffoo::vk {
class Device;
class Semaphore {
private:
  VkSemaphore semaphore;

  std::shared_ptr<Device> device;

public:
  Semaphore(std::shared_ptr<Device> device);
  VkSemaphore handle();
  ~Semaphore();
};
} // namespace toffoo::vk