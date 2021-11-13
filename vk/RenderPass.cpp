#include "RenderPass.h"
#include "Device.h"
#include "SwapChain.h"
#include "Utils.h"

namespace toffoo::vk {
RenderPass::RenderPass(std::shared_ptr<SwapChain> swapchain,
                       std::shared_ptr<Device> device)
    : swapchain(swapchain), device(device) {
  VkAttachmentDescription colorAttachment{
      .format = swapchain->getImageFormat(),
      .samples = VK_SAMPLE_COUNT_1_BIT,
      .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
      .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
      .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
      .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
      .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
      .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR};

  VkAttachmentReference colorAttachmentRef{
      .attachment = 0, .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL};

  VkSubpassDescription subpass{.pipelineBindPoint =
                                   VK_PIPELINE_BIND_POINT_GRAPHICS,
                               .colorAttachmentCount = 1,
                               .pColorAttachments = &colorAttachmentRef};

  VkRenderPassCreateInfo renderPassInfo{
      .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
      .attachmentCount = 1,
      .pAttachments = &colorAttachment,
      .subpassCount = 1,
      .pSubpasses = &subpass,
  };

  VK_THROW_NOT_OK(vkCreateRenderPass(device->handle(), &renderPassInfo, nullptr,
                                     &renderPass));
}

VkRenderPass RenderPass::handle() { return renderPass; }

RenderPass::~RenderPass() {
  vkDestroyRenderPass(device->handle(), renderPass, nullptr);
}

std::shared_ptr<RenderPass>
craeteRenderPass(std::shared_ptr<SwapChain> swapchain,
                 std::shared_ptr<Device> device) {
  return std::make_shared<RenderPass>(swapchain, device);
}

} // namespace toffoo::vk