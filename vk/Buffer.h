#pragma once
#include <memory>
#include <vulkan/vulkan.h>

namespace toffoo::vk {

class Device;
class CommandPool;
class Buffer {
protected:
  VkBuffer bufferHandle;
  VkDeviceMemory vertexBufferMemory;

  size_t bufferSize;

  std::shared_ptr<Device> device;

public:
  Buffer(std::shared_ptr<Device> device, size_t size, VkBufferUsageFlags usage,
         VkMemoryPropertyFlags properties);

  VkBuffer handle();

  void fill_from(void *data);

  size_t size();

  virtual ~Buffer();

  static void copy(std::shared_ptr<Device> device,
                   std::shared_ptr<CommandPool> command_pool,
                   std::shared_ptr<Buffer> src, std::shared_ptr<Buffer> dst);
};

class CommandPool;

} // namespace toffoo::vk