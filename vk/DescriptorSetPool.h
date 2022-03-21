#pragma once

#include <memory>
#include <vulkan/vulkan.h>

namespace toffoo::vk {
class Device;

class DescriptorSetPool {
private:
  VkDescriptorPool pool;

  std::shared_ptr<Device> device;

public:
  DescriptorSetPool(std::shared_ptr<Device> device, size_t size);

  const VkDescriptorPool &handle();

  ~DescriptorSetPool();
};

std::shared_ptr<DescriptorSetPool>
createDescriptorSetPool(std::shared_ptr<Device> device, size_t size);
}; // namespace toffoo::vk