#pragma once

#include "Buffer.h"
#include <memory>

namespace toffoo::vk {
class Device;
class UniformBuffer : public Buffer {
public:
  UniformBuffer(std::shared_ptr<Device> device, size_t size);

  static VkDescriptorSetLayoutBinding
  getDescriptorSetLayoutBinding(int binding);
};

std::shared_ptr<UniformBuffer>
createUniformBuffer(std::shared_ptr<Device> device, size_t size);
} // namespace toffoo::vk