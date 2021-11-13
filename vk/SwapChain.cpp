#include "SwapChain.h"
#include "Device.h"
#include "RenderPass.h"
#include "Semaphore.h"
#include "Surface.h"
#include "Utils.h"
#include <algorithm>
#include <vector>

namespace toffoo::vk {
struct SwapChainSupportDetails {
  VkSurfaceCapabilitiesKHR capabilities;
  std::vector<VkSurfaceFormatKHR> formats;
  std::vector<VkPresentModeKHR> presentModes;
};

SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device,
                                              VkSurfaceKHR surface) {
  SwapChainSupportDetails details;

  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface,
                                            &details.capabilities);

  uint32_t formatCount;
  vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

  if (formatCount != 0) {
    details.formats.resize(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount,
                                         details.formats.data());
  }

  uint32_t presentModeCount;
  vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount,
                                            nullptr);

  if (presentModeCount != 0) {
    details.presentModes.resize(presentModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(
        device, surface, &presentModeCount, details.presentModes.data());
  }

  return details;
}

VkSurfaceFormatKHR chooseSwapSurfaceFormat(
    const std::vector<VkSurfaceFormatKHR> &availableFormats) {
  for (const auto &availableFormat : availableFormats) {
    if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
        availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
      return availableFormat;
    }
  }

  return availableFormats[0];
}

VkPresentModeKHR chooseSwapPresentMode(
    const std::vector<VkPresentModeKHR> &availablePresentModes) {
  for (const auto &availablePresentMode : availablePresentModes) {
    if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
      return availablePresentMode;
    }
  }

  return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities,
                            int width, int height) {
  if (capabilities.currentExtent.width != UINT32_MAX) {
    return capabilities.currentExtent;
  } else {
    VkExtent2D actualExtent = {static_cast<uint32_t>(width),
                               static_cast<uint32_t>(height)};

    actualExtent.width =
        std::clamp(actualExtent.width, capabilities.minImageExtent.width,
                   capabilities.maxImageExtent.width);
    actualExtent.height =
        std::clamp(actualExtent.height, capabilities.minImageExtent.height,
                   capabilities.maxImageExtent.height);

    return actualExtent;
  }
}

SwapChain::SwapChain(std::shared_ptr<Device> device, int width, int height)
    : device(device) {
  SwapChainSupportDetails swapChainSupport = querySwapChainSupport(
      device->getPhysicalDevice(), device->getSurface()->handle());

  VkSurfaceFormatKHR surfaceFormat =
      chooseSwapSurfaceFormat(swapChainSupport.formats);
  VkPresentModeKHR presentMode =
      chooseSwapPresentMode(swapChainSupport.presentModes);
  extent = chooseSwapExtent(swapChainSupport.capabilities, width, height);

  uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
  if (swapChainSupport.capabilities.maxImageCount > 0 &&
      imageCount > swapChainSupport.capabilities.maxImageCount) {
    imageCount = swapChainSupport.capabilities.maxImageCount;
  }

  VkSwapchainCreateInfoKHR createInfo{
      .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
      .surface = device->getSurface()->handle(),
      .minImageCount = imageCount,
      .imageFormat = surfaceFormat.format,
      .imageColorSpace = surfaceFormat.colorSpace,
      .imageExtent = extent,
      .imageArrayLayers = 1,
      .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
      .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
      .queueFamilyIndexCount = 0,
      .pQueueFamilyIndices = nullptr,
      .preTransform = swapChainSupport.capabilities.currentTransform,
      .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
      .presentMode = presentMode,
      .clipped = VK_TRUE,
      .oldSwapchain = VK_NULL_HANDLE};

  VK_THROW_NOT_OK(
      vkCreateSwapchainKHR(device->handle(), &createInfo, nullptr, &swapchain));

  vkGetSwapchainImagesKHR(device->handle(), swapchain, &imageCount, nullptr);
  std::vector<VkImage> images;
  images.resize(imageCount);
  vkGetSwapchainImagesKHR(device->handle(), swapchain, &imageCount,
                          images.data());

  createImageViews(images, surfaceFormat.format);

  imageFormat = surfaceFormat.format;
}

uint32_t SwapChain::getNextImageIdx(Semaphore &semaphore) {
  uint32_t idx;
  vkAcquireNextImageKHR(device->handle(), swapchain, UINT64_MAX,
                        semaphore.handle(), VK_NULL_HANDLE, &idx);
  return idx;
}

void SwapChain::present(uint32_t imgIdx, Semaphore &wait) {
  VkSemaphore waitHandle = wait.handle();
  VkPresentInfoKHR presentInfo{.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
                               .waitSemaphoreCount = 1,
                               .pWaitSemaphores = &waitHandle,
                               .swapchainCount = 1,
                               .pSwapchains = &swapchain,
                               .pImageIndices = &imgIdx,
                               .pResults = nullptr};
  vkQueuePresentKHR(device->getPresentQueue(), &presentInfo);
}

SwapChain::~SwapChain() {
  for (auto &img : imageViews) {
    vkDestroyImageView(device->handle(), img, nullptr);
  }
  vkDestroySwapchainKHR(device->handle(), swapchain, nullptr);
}

VkSwapchainKHR SwapChain::handle() { return swapchain; }

VkExtent2D SwapChain::getExtent() { return extent; }

VkFormat SwapChain::getImageFormat() { return imageFormat; }

void SwapChain::createImageViews(const std::vector<VkImage> &images,
                                 VkFormat imageFormat) {
  imageViews.resize(images.size());
  for (size_t i = 0; i < images.size(); i++) {
    VkImageViewCreateInfo createInfo{
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .image = images[i],
        .viewType = VK_IMAGE_VIEW_TYPE_2D,
        .format = imageFormat,
        .components = VkComponentMapping{.r = VK_COMPONENT_SWIZZLE_IDENTITY,
                                         .g = VK_COMPONENT_SWIZZLE_IDENTITY,
                                         .b = VK_COMPONENT_SWIZZLE_IDENTITY,
                                         .a = VK_COMPONENT_SWIZZLE_IDENTITY},

        .subresourceRange =
            VkImageSubresourceRange{.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                                    .baseMipLevel = 0,
                                    .levelCount = 1,
                                    .baseArrayLayer = 0,
                                    .layerCount = 1}};

    VK_THROW_NOT_OK(vkCreateImageView(device->handle(), &createInfo, nullptr,
                                      &imageViews[i]));
  }
}

std::vector<VkImageView> &SwapChain::getImageViews() { return imageViews; }

std::shared_ptr<SwapChain> createSwapchain(std::shared_ptr<Device> device,
                                           int width, int height) {
  return std::make_shared<SwapChain>(device, width, height);
}

} // namespace toffoo::vk