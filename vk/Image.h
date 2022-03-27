#pragma once

#include "WriteDescriptorSetWrapper.h"
#include <memory>
#include <vulkan/vulkan.h>
namespace toffoo::vk {
class Device;
class CommandPool;
class Buffer;

class Image {
private:
  VkImage textureImage;
  VkDeviceMemory textureImageMemory;
  VkImageView view;
  VkSampler sampler;

  std::shared_ptr<Device> device;

public:
  Image(std::shared_ptr<Device> device, size_t width, size_t height,
        VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
        VkMemoryPropertyFlags properties);

  const VkImage &handle();
  const VkImageView &getView();
  const VkSampler &getSampler();

  WriteDescriptorSetWrapper getWriteDescriptorSet(size_t binding);

  virtual ~Image();

  static void transitionImageLayout(std::shared_ptr<Device> device,
                                    std::shared_ptr<CommandPool> command_pool,
                                    VkImage image, VkFormat format,
                                    VkImageLayout oldLayout,
                                    VkImageLayout newLayout);

  static void copyBufferToImage(std::shared_ptr<Device> device,
                                std::shared_ptr<CommandPool> command_pool,
                                std::shared_ptr<Buffer> buffer,
                                std::shared_ptr<Image> image, uint32_t width,
                                uint32_t height);

  static VkDescriptorSetLayoutBinding
  getDescriptorSetLayoutBinding(int binding);

  static std::shared_ptr<Image>
  create(std::shared_ptr<Device> device,
         std::shared_ptr<CommandPool> command_pool, void *img, size_t width,
         size_t height);
};
} // namespace toffoo::vk