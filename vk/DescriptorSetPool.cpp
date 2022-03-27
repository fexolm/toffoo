#include "DescriptorSetPool.h"
#include "Device.h"

namespace toffoo::vk {
DescriptorSetPool::DescriptorSetPool(std::shared_ptr<Device> device,
                                     size_t size)
    : device(device) {
  std::array<VkDescriptorPoolSize, 2> poolSizes{};

  // Allocate many descriptors
  poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  poolSizes[0].descriptorCount = 4096;

  poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
  poolSizes[1].descriptorCount = 2048;

  VkDescriptorPoolCreateInfo poolInfo{};
  poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
  poolInfo.pPoolSizes = poolSizes.data();
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