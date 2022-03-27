#include "UniformBuffer.h"

namespace toffoo::vk {
UniformBuffer::UniformBuffer(std::shared_ptr<Device> device, size_t size)
    : Buffer(device, size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
             VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                 VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) {}

VkDescriptorSetLayoutBinding
UniformBuffer::getDescriptorSetLayoutBinding(int binding) {
  VkDescriptorSetLayoutBinding uboLayoutBinding{};
  uboLayoutBinding.binding = binding;
  uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  uboLayoutBinding.descriptorCount = 1;
  uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
  uboLayoutBinding.pImmutableSamplers = nullptr;
  return uboLayoutBinding;
}

WriteDescriptorSetWrapper UniformBuffer::getWriteDescriptorSet(size_t binding) {
  VkDescriptorBufferInfo bufferInfo = {};
  bufferInfo.buffer = bufferHandle;
  bufferInfo.offset = 0;
  bufferInfo.range = bufferSize;

  VkWriteDescriptorSet descriptorWrite = {};
  descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  descriptorWrite.dstBinding = binding;
  descriptorWrite.dstArrayElement = 0;
  descriptorWrite.descriptorCount = 1;
  descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  return {descriptorWrite, bufferInfo};
}

std::shared_ptr<UniformBuffer>
createUniformBuffer(std::shared_ptr<Device> device, size_t size) {
  return std::make_shared<UniformBuffer>(device, size);
}

} // namespace toffoo::vk