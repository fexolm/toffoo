#include "vk/CommandBuffers.h"
#include "vk/CommandPool.h"
#include "vk/Device.h"
#include "vk/Framebuffer.h"
#include "vk/Instance.h"
#include "vk/Pipeline.h"
#include "vk/RenderPass.h"
#include "vk/Semaphore.h"
#include "vk/Shader.h"
#include "vk/Surface.h"
#include "vk/SwapChain.h"

#include <GLFW/glfw3.h>
#include <vector>

std::vector<const char *> getRequiredExtensions() {
  uint32_t glfwExtensionCount = 0;
  auto glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

  std::vector<const char *> extensions(glfwExtensions,
                                       glfwExtensions + glfwExtensionCount);
  return extensions;
}

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

  GLFWwindow *window =
      glfwCreateWindow(800, 600, "Vulkan window", nullptr, nullptr);

  auto extensions = getRequiredExtensions();

  auto instance = toffoo::vk::createInstance(extensions, true);

  auto surface = toffoo::vk::createGlfwSurface(instance, window);

  auto device = toffoo::vk::createDevice(instance, surface);

  int width, height;
  glfwGetFramebufferSize(window, &width, &height);

  auto swapchain = toffoo::vk::createSwapchain(device, width, height);

  auto renderPass = toffoo::vk::createRenderPass(swapchain, device);

  auto framebuffers =
      toffoo::vk::createFramebuffers(device, swapchain, renderPass);

  auto commandPool = toffoo::vk::createCommandPool(device);

  auto commandBuffers = toffoo::vk::createCommandBuffers(device, commandPool,
                                                         framebuffers.size());

  toffoo::vk::GraphicsPipelineBuilder pipelineBuilder(device, renderPass);

  pipelineBuilder.addVertexShader(toffoo::vk::createShader(device, "vert.spv"));
  pipelineBuilder.addFragmentShader(
      toffoo::vk::createShader(device, "frag.spv"));

  pipelineBuilder.addVertexInputState();
  pipelineBuilder.addInputAssemblyState();
  pipelineBuilder.addViewportState(swapchain, width, height);
  pipelineBuilder.addRasterizationState();
  pipelineBuilder.addMultisamplintState();
  pipelineBuilder.addColorBlendState();
  pipelineBuilder.addDynamicState();
  pipelineBuilder.setupPipelineLayout();

  auto pipeline = pipelineBuilder.build();

  toffoo::vk::Semaphore imageAvailable(device);
  toffoo::vk::Semaphore renderFinished(device);

  for (size_t i = 0; i < framebuffers.size(); ++i) {
    commandBuffers->begin(i);
    commandBuffers->beginRenderPass(i, renderPass, framebuffers[i],
                                    swapchain->getExtent());
    commandBuffers->bindPipeline(i, pipeline);
    commandBuffers->draw(i, 3, 1, 0, 0);
    commandBuffers->endRenderPass(i);
    commandBuffers->end(i);
  }

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    auto nextImg = swapchain->getNextImageIdx(imageAvailable);
    commandBuffers->submit(nextImg, imageAvailable, renderFinished);
    swapchain->present(nextImg, renderFinished);
    device->waitPresentQueue();
  }

  device->waitIdle();

  glfwDestroyWindow(window);

  glfwTerminate();

  return 0;
}
