#pragma once

#include <memory>
#include <vulkan/vulkan.h>

namespace toffoo::vk {
class Device;

class VertexBuffer {
private:
  VkBuffer vertexBuffer;
  VkDeviceMemory vertexBufferMemory;

  size_t buffer_size;

  std::shared_ptr<Device> device;

public:
  VertexBuffer(std::shared_ptr<Device> device, size_t size);

  VkBuffer handle() { return vertexBuffer; }

  void fill_from(void *data);

  virtual ~VertexBuffer();
};

std::shared_ptr<VertexBuffer> createVertexBuffer(std::shared_ptr<Device> device,
                                                 size_t size);
} // namespace toffoo::vk