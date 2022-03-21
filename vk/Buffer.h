#pragma once
#include <memory>
#include <vulkan/vulkan.h>

namespace toffoo::vk {

class Device;
class Buffer {
protected:
  VkBuffer bufferHandle;
  VkDeviceMemory vertexBufferMemory;

  size_t buffer_size;

  std::shared_ptr<Device> device;

public:
  Buffer(std::shared_ptr<Device> device, size_t size, VkBufferUsageFlags usage,
         VkMemoryPropertyFlags properties);

  VkBuffer handle();

  void fill_from(void *data);

  virtual ~Buffer();
};
} // namespace toffoo::vk