#pragma once

#include <memory>
#include <vector>
#include <vulkan/vulkan.h>

namespace toffoo::vk {
class Device;
class GraphicsPipeline;
class DescriptorSetPool;
class Buffer;

class DescriptorSets {
private:
  std::vector<VkDescriptorSet> sets;

  std::shared_ptr<Device> device;
  std::shared_ptr<GraphicsPipeline> pipeline;
  std::shared_ptr<DescriptorSetPool> pool;

public:
  DescriptorSets(std::shared_ptr<Device> device,
                 std::shared_ptr<DescriptorSetPool> pool,
                 std::shared_ptr<GraphicsPipeline> pipeline, size_t size);

  const VkDescriptorSet &get(size_t idx);

  void update(size_t idx, std::shared_ptr<Buffer> buffer);

  void bind(VkCommandBuffer cb, size_t idx);
};

std::shared_ptr<DescriptorSets>
createDescriptorSets(std::shared_ptr<Device> device,
                     std::shared_ptr<DescriptorSetPool> pool,
                     std::shared_ptr<GraphicsPipeline> pipeline, size_t size);
} // namespace toffoo::vk
