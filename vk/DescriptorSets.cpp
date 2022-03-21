#include "DescriptorSets.h"
#include "Buffer.h"
#include "DescriptorSetPool.h"
#include "Device.h"
#include "Pipeline.h"

namespace toffoo::vk {
DescriptorSets::DescriptorSets(std::shared_ptr<Device> device,
                               std::shared_ptr<DescriptorSetPool> pool,
                               std::shared_ptr<GraphicsPipeline> pipeline,
                               size_t size)
    : device(device), pipeline(pipeline), pool(pool) {
  std::vector<VkDescriptorSetLayout> layouts(
      size, pipeline->getDescriptorSetLayout()->handle());
  VkDescriptorSetAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
  allocInfo.descriptorPool = pool->handle();
  allocInfo.descriptorSetCount = size;
  allocInfo.pSetLayouts = layouts.data();

  sets.resize(size);

  if (vkAllocateDescriptorSets(device->handle(), &allocInfo, sets.data()) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to allocate descriptor sets!");
  }
}

const VkDescriptorSet &DescriptorSets::get(size_t idx) { return sets[idx]; }

void DescriptorSets::update(size_t idx, std::shared_ptr<Buffer> buffer) {
  VkDescriptorBufferInfo bufferInfo{};
  bufferInfo.buffer = buffer->handle();
  bufferInfo.offset = 0;
  bufferInfo.range = VK_WHOLE_SIZE;

  VkWriteDescriptorSet descriptorWrite{};
  descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  descriptorWrite.dstSet = sets[idx];
  descriptorWrite.dstBinding = 0;
  descriptorWrite.dstArrayElement = 0;
  descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  descriptorWrite.descriptorCount = 1;
  descriptorWrite.pBufferInfo = &bufferInfo;
  descriptorWrite.pImageInfo = nullptr;
  descriptorWrite.pTexelBufferView = nullptr;

  vkUpdateDescriptorSets(device->handle(), 1, &descriptorWrite, 0, nullptr);
}

void DescriptorSets::bind(VkCommandBuffer cb, size_t idx) {
  vkCmdBindDescriptorSets(cb, VK_PIPELINE_BIND_POINT_GRAPHICS,
                          pipeline->getLayout()->handle(), 0, 1, &sets[idx], 0,
                          nullptr);
}

std::shared_ptr<DescriptorSets>
createDescriptorSets(std::shared_ptr<Device> device,
                     std::shared_ptr<DescriptorSetPool> pool,
                     std::shared_ptr<GraphicsPipeline> pipeline, size_t size) {
  return std::make_shared<DescriptorSets>(device, pool, pipeline, size);
}
} // namespace toffoo::vk