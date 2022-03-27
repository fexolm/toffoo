#include "WriteDescriptorSetWrapper.h"

namespace toffoo::vk {
WriteDescriptorSetWrapper::WriteDescriptorSetWrapper(
    const VkWriteDescriptorSet &writeDescriptorSet,
    const VkDescriptorImageInfo &imageInfo)
    : writeDescriptorSet(writeDescriptorSet),
      imageInfo(std::make_unique<VkDescriptorImageInfo>(imageInfo)) {
  this->writeDescriptorSet.pImageInfo = this->imageInfo.get();
  this->writeDescriptorSet.pBufferInfo = nullptr;
}

WriteDescriptorSetWrapper::WriteDescriptorSetWrapper(
    const VkWriteDescriptorSet &writeDescriptorSet,
    const VkDescriptorBufferInfo &bufferInfo)
    : writeDescriptorSet(writeDescriptorSet),
      bufferInfo(std::make_unique<VkDescriptorBufferInfo>(bufferInfo)) {
  this->writeDescriptorSet.pBufferInfo = this->bufferInfo.get();
  this->writeDescriptorSet.pImageInfo = nullptr;
}

const VkWriteDescriptorSet &
WriteDescriptorSetWrapper::GetWriteDescriptorSet() const {
  return writeDescriptorSet;
}
} // namespace toffoo::vk