#include "Buffer.h"
#include "CommandBuffers.h"
#include "CommandPool.h"
#include "Device.h"
#include "Utils.h"
#include <cstring>

namespace toffoo::vk {

Buffer::Buffer(std::shared_ptr<Device> device, size_t size,
               VkBufferUsageFlags usage, VkMemoryPropertyFlags properties)
    : buffer_size(size), device(device) {
  VkBufferCreateInfo bufferInfo{};
  bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  bufferInfo.size = size;
  bufferInfo.usage = usage;
  bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  if (vkCreateBuffer(device->handle(), &bufferInfo, nullptr, &bufferHandle) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to create vertex buffer!");
  }

  VkMemoryRequirements memRequirements;
  vkGetBufferMemoryRequirements(device->handle(), bufferHandle,
                                &memRequirements);

  VkMemoryAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  allocInfo.allocationSize = memRequirements.size;
  allocInfo.memoryTypeIndex = findMemoryType(
      device->getPhysicalDevice(), memRequirements.memoryTypeBits,
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | properties);

  if (vkAllocateMemory(device->handle(), &allocInfo, nullptr,
                       &vertexBufferMemory) != VK_SUCCESS) {
    throw std::runtime_error("failed to allocate vertex buffer memory!");
  }

  vkBindBufferMemory(device->handle(), bufferHandle, vertexBufferMemory, 0);
}

VkBuffer Buffer::handle() { return bufferHandle; }

size_t Buffer::size() { return buffer_size; }

void Buffer::fill_from(void *data) {
  void *mapped_buf;
  vkMapMemory(device->handle(), vertexBufferMemory, 0, buffer_size, 0,
              &mapped_buf);
  memcpy(mapped_buf, data, buffer_size);
  vkUnmapMemory(device->handle(), vertexBufferMemory);
}

Buffer::~Buffer() {
  vkDestroyBuffer(device->handle(), bufferHandle, nullptr);
  vkFreeMemory(device->handle(), vertexBufferMemory, nullptr);
}

void Buffer::copy(std::shared_ptr<Device> device,
                  std::shared_ptr<CommandPool> command_pool,
                  std::shared_ptr<Buffer> src, std::shared_ptr<Buffer> dst) {
  auto cb = createCommandBuffers(device, command_pool, 1);
  cb->begin(0);

  VkBufferCopy copyRegion{};
  copyRegion.size = src->size();
  vkCmdCopyBuffer(cb->get(0), src->handle(), dst->handle(), 1, &copyRegion);

  cb->end(0);
  cb->submit(0);
}
} // namespace toffoo::vk