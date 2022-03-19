#include "VertexBuffer.h"
#include "Device.h"

#include <cstring>

namespace toffoo::vk {

uint32_t findMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter,
                        VkMemoryPropertyFlags properties) {
  VkPhysicalDeviceMemoryProperties memProperties;
  vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

  for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
    if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags &
                                    properties) == properties) {
      return i;
    }
  }

  throw std::runtime_error("failed to find suitable memory type!");
}

VertexBuffer::VertexBuffer(std::shared_ptr<Device> device, size_t size)
    : buffer_size(size), device(device) {
  VkBufferCreateInfo bufferInfo{};
  bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  bufferInfo.size = size;
  bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
  bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  if (vkCreateBuffer(device->handle(), &bufferInfo, nullptr, &vertexBuffer) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to create vertex buffer!");
  }

  VkMemoryRequirements memRequirements;
  vkGetBufferMemoryRequirements(device->handle(), vertexBuffer,
                                &memRequirements);

  VkMemoryAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  allocInfo.allocationSize = memRequirements.size;
  allocInfo.memoryTypeIndex = findMemoryType(
      device->getPhysicalDevice(), memRequirements.memoryTypeBits,
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
          VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

  if (vkAllocateMemory(device->handle(), &allocInfo, nullptr,
                       &vertexBufferMemory) != VK_SUCCESS) {
    throw std::runtime_error("failed to allocate vertex buffer memory!");
  }

  vkBindBufferMemory(device->handle(), vertexBuffer, vertexBufferMemory, 0);
}

void VertexBuffer::fill_from(void *data) {
  void *mapped_buf;
  vkMapMemory(device->handle(), vertexBufferMemory, 0, buffer_size, 0,
              &mapped_buf);
  memcpy(mapped_buf, data, buffer_size);
  vkUnmapMemory(device->handle(), vertexBufferMemory);
}

VertexBuffer::~VertexBuffer() {
  vkDestroyBuffer(device->handle(), vertexBuffer, nullptr);
  vkFreeMemory(device->handle(), vertexBufferMemory, nullptr);
}

std::shared_ptr<VertexBuffer> createVertexBuffer(std::shared_ptr<Device> device,
                                                 size_t size) {
  return std::make_shared<VertexBuffer>(device, size);
}
} // namespace toffoo::vk