#pragma once
#include <memory>
#include <vulkan/vulkan.h>

namespace toffoo::vk {
class WriteDescriptorSetWrapper {
public:
  WriteDescriptorSetWrapper(const VkWriteDescriptorSet &writeDescriptorSet,
                            const VkDescriptorImageInfo &imageInfo);

  WriteDescriptorSetWrapper(const VkWriteDescriptorSet &writeDescriptorSet,
                            const VkDescriptorBufferInfo &bufferInfo);

  const VkWriteDescriptorSet &GetWriteDescriptorSet() const;

private:
  VkWriteDescriptorSet writeDescriptorSet;
  std::unique_ptr<VkDescriptorImageInfo> imageInfo;
  std::unique_ptr<VkDescriptorBufferInfo> bufferInfo;
};
} // namespace toffoo::vk