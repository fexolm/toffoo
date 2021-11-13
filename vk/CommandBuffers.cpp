#include "CommandBuffers.h"
#include "CommandPool.h"
#include "Device.h"
#include "Framebuffer.h"
#include "Pipeline.h"
#include "RenderPass.h"
#include "Semaphore.h"
#include "Utils.h"

namespace toffoo::vk {
CommandBuffers::CommandBuffers(std::shared_ptr<Device> device,
                               std::shared_ptr<CommandPool> pool, size_t size)
    : device(device), pool(pool) {
  buffers.resize(size);
  VkCommandBufferAllocateInfo allocInfo{
      .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
      .commandPool = pool->handle(),
      .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
      .commandBufferCount = (uint32_t)buffers.size()};

  VK_THROW_NOT_OK(
      vkAllocateCommandBuffers(device->handle(), &allocInfo, buffers.data()));
}

std::vector<VkCommandBuffer> &CommandBuffers::getBuffers() { return buffers; }

void CommandBuffers::begin(size_t idx) {
  VkCommandBufferBeginInfo beginInfo{
      .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
      .flags = 0,
      .pInheritanceInfo = nullptr};

  VK_THROW_NOT_OK(vkBeginCommandBuffer(buffers[idx], &beginInfo));
}

void CommandBuffers::beginRenderPass(size_t idx,
                                     std::shared_ptr<RenderPass> renderPass,
                                     std::shared_ptr<Framebuffer> framebuffer,
                                     VkExtent2D extent) {
  VkClearValue clearColor = {{0.0f, 0.0f, 0.0f, 1.0f}};
  VkRenderPassBeginInfo renderPassInfo{
      .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
      .renderPass = renderPass->handle(),
      .framebuffer = framebuffer->handle(),
      .renderArea = {.offset = {0, 0}, .extent = extent},
      .clearValueCount = 1,
      .pClearValues = &clearColor};

  vkCmdBeginRenderPass(buffers[idx], &renderPassInfo,
                       VK_SUBPASS_CONTENTS_INLINE);
}

void CommandBuffers::bindPipeline(size_t idx,
                                  std::shared_ptr<GraphicsPipeline> pipeline) {
  vkCmdBindPipeline(buffers[idx], VK_PIPELINE_BIND_POINT_GRAPHICS,
                    pipeline->handle());
}

void CommandBuffers::draw(size_t idx, size_t vertexCount, size_t instanceCount,
                          size_t firstVertex, size_t firstInstance) {
  vkCmdDraw(buffers[idx], vertexCount, instanceCount, firstVertex,
            firstInstance);
}

void CommandBuffers::endRenderPass(size_t idx) {
  vkCmdEndRenderPass(buffers[idx]);
}

void CommandBuffers::end(size_t idx) {
  VK_THROW_NOT_OK(vkEndCommandBuffer(buffers[idx]));
}

void CommandBuffers::submit(size_t idx, Semaphore &waitSemaphore,
                            Semaphore &signalSemaphore) {

  VkSemaphore waitHandle = waitSemaphore.handle();
  VkSemaphore signalHandle = signalSemaphore.handle();
  VkPipelineStageFlags waitStage =
      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

  VkSubmitInfo submitInfo{.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
                          .waitSemaphoreCount = 1,
                          .pWaitSemaphores = &waitHandle,
                          .pWaitDstStageMask = &waitStage,
                          .commandBufferCount = 1,
                          .pCommandBuffers = &buffers[idx],
                          .signalSemaphoreCount = 1,
                          .pSignalSemaphores = &signalHandle};

  VK_THROW_NOT_OK(vkQueueSubmit(device->getGraphicsQueue(), 1, &submitInfo,
                                VK_NULL_HANDLE));
}

std::shared_ptr<CommandBuffers>
createCommandBuffers(std::shared_ptr<Device> device,
                     std::shared_ptr<CommandPool> pool, size_t size) {
  return std::make_shared<CommandBuffers>(device, pool, size);
}

} // namespace toffoo::vk