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
#include "vk/VertexBuffer.h"

#include <GLFW/glfw3.h>
#include <vector>

#include <glm/glm.hpp>

std::vector<const char *> getRequiredExtensions() {
  uint32_t glfwExtensionCount = 0;
  auto glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

  std::vector<const char *> extensions(glfwExtensions,
                                       glfwExtensions + glfwExtensionCount);
  return extensions;
}

struct Vertex {
  glm::vec2 pos;
  glm::vec3 color;

  static VkVertexInputBindingDescription getBindingDescription() {
    VkVertexInputBindingDescription bindingDescription{};
    bindingDescription.binding = 0;
    bindingDescription.stride = sizeof(Vertex);
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    return bindingDescription;
  }

  static std::vector<VkVertexInputAttributeDescription>
  getAttributeDescriptions() {
    std::vector<VkVertexInputAttributeDescription> attributeDescriptions(2);
    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[0].offset = offsetof(Vertex, pos);

    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(Vertex, color);
    return attributeDescriptions;
  }
};

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

  const std::vector<Vertex> vertices = {{{0.0f, -0.5f}, {1.0f, 1.0f, 1.0f}},
                                        {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
                                        {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}};

  auto bindingDescription = Vertex::getBindingDescription();
  auto attributeDescriptions = Vertex::getAttributeDescriptions();

  auto vertexBuffer =
      toffoo::vk::createVertexBuffer(device, sizeof(Vertex) * vertices.size());

  vertexBuffer->fill_from((void *)vertices.data());

  toffoo::vk::GraphicsPipelineBuilder pipelineBuilder(device, renderPass);

  pipelineBuilder.addVertexShader(toffoo::vk::createShader(device, "vert.spv"));
  pipelineBuilder.addFragmentShader(
      toffoo::vk::createShader(device, "frag.spv"));

  pipelineBuilder.addVertexInputState(attributeDescriptions,
                                      bindingDescription);
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
    commandBuffers->bindVertexBuffer(i, vertexBuffer, 0);
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
