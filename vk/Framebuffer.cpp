#include "Framebuffer.h"
#include "Device.h"
#include "RenderPass.h"
#include "SwapChain.h"
#include "Utils.h"

namespace toffoo::vk {
Framebuffer::Framebuffer(std::shared_ptr<Device> device,
                         std::shared_ptr<SwapChain> swapchain,
                         std::shared_ptr<RenderPass> renderPass,
                         VkImageView imgView)
    : device(device), swapchain(swapchain), renderPass(renderPass) {
  VkFramebufferCreateInfo framebufferInfo{
      .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
      .renderPass = renderPass->handle(),
      .attachmentCount = 1,
      .pAttachments = &imgView,
      .width = swapchain->getExtent().width,
      .height = swapchain->getExtent().height,
      .layers = 1};

  VK_THROW_NOT_OK(vkCreateFramebuffer(device->handle(), &framebufferInfo,
                                      nullptr, &framebuffer));
}

VkFramebuffer Framebuffer::handle() { return framebuffer; }

Framebuffer::~Framebuffer() {
  vkDestroyFramebuffer(device->handle(), framebuffer, nullptr);
}

// TODO: create class to own all framebuffers instead of having multiple shared
// pointers to many framebuffers
std::vector<std::shared_ptr<Framebuffer>>
createFramebuffers(std::shared_ptr<Device> device,
                   std::shared_ptr<SwapChain> swapchain,
                   std::shared_ptr<RenderPass> renderPass) {
  auto &imgViews = swapchain->getImageViews();
  std::vector<std::shared_ptr<Framebuffer>> framebuffers(imgViews.size());
  for (size_t i = 0; i < imgViews.size(); ++i) {
    framebuffers[i] = std::make_shared<Framebuffer>(device, swapchain,
                                                    renderPass, imgViews[i]);
  }
  return framebuffers;
}
} // namespace toffoo::vk