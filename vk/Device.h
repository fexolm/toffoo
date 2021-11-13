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

  uint32_t graphicsFamilyIdx;
  uint32_t presentFamilyIdx;

  VkQueue graphicsQueue;
  VkQueue presentQueue;

  std::shared_ptr<Instance> instance;
  std::shared_ptr<Surface> surface;

public:
  Device(std::shared_ptr<Instance> instance, std::shared_ptr<Surface> surface);

  VkDevice handle();

  uint32_t getGraphicsFamilyIdx();
  uint32_t getPresentFamilyIdx();

  VkPhysicalDevice getPhysicalDevice();

  std::shared_ptr<Surface> getSurface();

  VkQueue getGraphicsQueue();
  VkQueue getPresentQueue();

  void waitIdle();
  void waitPresentQueue();

  ~Device();
};

std::shared_ptr<Device> createDevice(std::shared_ptr<Instance> instance,
                                     std::shared_ptr<Surface> surface);
} // namespace toffoo::vk