#pragma once

#include <memory>
#include <vulkan/vulkan.h>
namespace toffoo::vk {
class Instance;
class Surface;

class Device {
private:
  VkDevice device;
  VkPhysicalDevice physicalDevice;

  VkQueue graphicsQueue;
  VkQueue presentQueue;

  std::shared_ptr<Instance> instance;
  std::shared_ptr<Surface> surface;

public:
  Device(std::shared_ptr<Instance> instance, std::shared_ptr<Surface> surface);

  VkDevice handle();

  VkPhysicalDevice getPhysicalDevice();

  std::shared_ptr<Surface> getSurface();

  ~Device();
};

std::shared_ptr<Device> createDevice(std::shared_ptr<Instance> instance,
                                     std::shared_ptr<Surface> surface);
} // namespace toffoo::vk