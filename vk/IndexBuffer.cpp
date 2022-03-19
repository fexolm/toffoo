#include "IndexBuffer.h"

namespace toffoo::vk {
IndexBuffer::IndexBuffer(std::shared_ptr<Device> device, size_t size)
    : Buffer(device, size, VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
             VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                 VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) {}

std::shared_ptr<IndexBuffer> createIndexBuffer(std::shared_ptr<Device> device,
                                               size_t size) {
  return std::make_shared<IndexBuffer>(device, size);
}
} // namespace toffoo::vk