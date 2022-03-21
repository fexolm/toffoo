#include "DescriptorSetPool.h"
#include "Device.h"

namespace toffoo::vk {
DescriptorSetPool::DescriptorSetPool(std::shared_ptr<Device> device,
                                     size_t size)
    : device(device) {
  VkDescriptorPoolSize poolSize{};
  poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  poolSize.descriptorCount = size;

  VkDescriptorPoolCreateInfo poolInfo{};
  poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  poolInfo.poolSizeCount = 1;
  poolInfo.pPoolSizes = &poolSize;
  poolInfo.maxSets = size;

  if (vkCreateDescriptorPool(device->handle(), &poolInfo, nullptr, &pool) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to create descriptor pool!");
  }
}

const VkDescriptorPool &DescriptorSetPool::handle() { return pool; }

DescriptorSetPool::~DescriptorSetPool() {
  vkDestroyDescriptorPool(device->handle(), pool, nullptr);
}

std::shared_ptr<DescriptorSetPool>
createDescriptorSetPool(std::shared_ptr<Device> device, size_t size) {
  return std::make_shared<DescriptorSetPool>(device, size);
}
}; // namespace toffoo::vk