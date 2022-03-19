#include "VertexBuffer.h"
#include "Device.h"

#include <cstring>

namespace toffoo::vk {

VertexBuffer::VertexBuffer(std::shared_ptr<Device> device, size_t size)
    : Buffer(device, size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
             VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                 VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) {}

std::shared_ptr<VertexBuffer> createVertexBuffer(std::shared_ptr<Device> device,
                                                 size_t size) {
  return std::make_shared<VertexBuffer>(device, size);
}
} // namespace toffoo::vk