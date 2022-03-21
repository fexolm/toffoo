#pragma once

#include <memory>
#include <vector>
#include <vulkan/vulkan.h>

namespace toffoo::vk {
class CommandPool;
class Device;
class RenderPass;
class Framebuffer;
class GraphicsPipeline;
class Semaphore;
class VertexBuffer;
class IndexBuffer;

class CommandBuffers {
private:
  std::vector<VkCommandBuffer> buffers;

  std::shared_ptr<Device> device;
  std::shared_ptr<CommandPool> pool;

public:
  CommandBuffers(std::shared_ptr<Device> device,
                 std::shared_ptr<CommandPool> pool, size_t size);

  const VkCommandBuffer &get(size_t idx);

  void begin(size_t idx);

  void beginRenderPass(size_t idx, std::shared_ptr<RenderPass> renderPass,
                       std::shared_ptr<Framebuffer> framebuffer,
                       VkExtent2D extent);

  void bindPipeline(size_t idx, std::shared_ptr<GraphicsPipeline> pipeline);

  void bindVertexBuffer(size_t idx, std::shared_ptr<VertexBuffer> vertexBuffer,
                        size_t binding);

  void bindIndexBuffer(size_t idx, std::shared_ptr<IndexBuffer> indexBuffer,
                       size_t binding);

  void draw(size_t idx, size_t indicesSize, size_t instanceCount,
            size_t firstIndex, size_t vertexOffset, size_t firstInstance);

  void endRenderPass(size_t idx);

  void end(size_t idx);

  void submit(size_t idx, Semaphore &waitSemaphore, Semaphore &signalSemaphore);
};

std::shared_ptr<CommandBuffers>
createCommandBuffers(std::shared_ptr<Device> device,
                     std::shared_ptr<CommandPool> pool, size_t size);
} // namespace toffoo::vk