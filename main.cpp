#include "vk/CommandBuffers.h"
#include "vk/CommandPool.h"
#include "vk/DescriptorSetPool.h"
#include "vk/DescriptorSets.h"
#include "vk/Device.h"
#include "vk/Framebuffer.h"
#include "vk/Image.h"
#include "vk/IndexBuffer.h"
#include "vk/Instance.h"
#include "vk/Pipeline.h"
#include "vk/RenderPass.h"
#include "vk/Semaphore.h"
#include "vk/Shader.h"
#include "vk/Surface.h"
#include "vk/SwapChain.h"
#include "vk/UniformBuffer.h"
#include "vk/VertexBuffer.h"
#include "vk/WriteDescriptorSetWrapper.h"

#include <GLFW/glfw3.h>
#include <chrono>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/projection.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "thirdparty/stb_image.h"

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
  glm::vec2 texCoord;

  static VkVertexInputBindingDescription getBindingDescription() {
    VkVertexInputBindingDescription bindingDescription{};
    bindingDescription.binding = 0;
    bindingDescription.stride = sizeof(Vertex);
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

    return bindingDescription;
  }

  static std::vector<VkVertexInputAttributeDescription>
  getAttributeDescriptions() {
    std::vector<VkVertexInputAttributeDescription> attributeDescriptions(3);

    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[0].offset = offsetof(Vertex, pos);

    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(Vertex, color);

    attributeDescriptions[2].binding = 0;
    attributeDescriptions[2].location = 2;
    attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

    return attributeDescriptions;
  }
};

struct UniformBufferObject {
  glm::mat4 model;
  glm::mat4 view;
  glm::mat4 proj;
};

void updateUniformBuffer(std::shared_ptr<toffoo::vk::UniformBuffer> buffer,
                         size_t width, size_t height) {
  static auto startTime = std::chrono::high_resolution_clock::now();

  auto currentTime = std::chrono::high_resolution_clock::now();
  float time = std::chrono::duration<float, std::chrono::seconds::period>(
                   currentTime - startTime)
                   .count();

  UniformBufferObject ubo{};
  ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f),
                          glm::vec3(0.0f, 0.0f, 1.0f));
  ubo.view =
      glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                  glm::vec3(0.0f, 0.0f, 1.0f));
  ubo.proj =
      glm::perspective(glm::radians(45.0f), width / (float)height, 0.1f, 10.0f);
  ubo.proj[1][1] *= -1;
  buffer->fill_from(&ubo);
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

  const std::vector<Vertex> vertices = {
      {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
      {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
      {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
      {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}};

  const std::vector<uint16_t> indices = {0, 1, 2, 2, 3, 0};

  auto bindingDescription = Vertex::getBindingDescription();
  auto attributeDescriptions = Vertex::getAttributeDescriptions();

  auto vertexBuffer =
      toffoo::vk::createVertexBuffer(device, sizeof(Vertex) * vertices.size());

  auto indexBuffer =
      toffoo::vk::createIndexBuffer(device, sizeof(uint16_t) * indices.size());

  vertexBuffer->fill_from((void *)vertices.data());
  indexBuffer->fill_from((void *)indices.data());

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
  pipelineBuilder.addDescritorSetLayoutBinding(
      toffoo::vk::UniformBuffer::getDescriptorSetLayoutBinding(0));
  pipelineBuilder.addDescritorSetLayoutBinding(
      toffoo::vk::Image::getDescriptorSetLayoutBinding(1));

  auto pipeline = pipelineBuilder.build();

  std::vector<std::shared_ptr<toffoo::vk::UniformBuffer>> uniformBuffers(
      framebuffers.size());

  for (auto &b : uniformBuffers) {
    b = toffoo::vk::createUniformBuffer(device, sizeof(UniformBufferObject));
  }

  auto descriptorPool =
      toffoo::vk::createDescriptorSetPool(device, framebuffers.size());

  auto descriptorSets = toffoo::vk::createDescriptorSets(
      device, descriptorPool, pipeline, framebuffers.size());

  int texWidth, texHeight, texChannels;
  auto texImg = stbi_load("texture.jpg", &texWidth, &texHeight, &texChannels,
                          STBI_rgb_alpha);

  auto image = toffoo::vk::Image::create(device, commandPool, texImg, texWidth,
                                         texHeight);

  for (int i = 0; i < framebuffers.size(); ++i) {
    descriptorSets->update(i, uniformBuffers[i], image);
  }

  toffoo::vk::Semaphore imageAvailable(device);
  toffoo::vk::Semaphore renderFinished(device);

  for (size_t i = 0; i < framebuffers.size(); ++i) {
    commandBuffers->begin(i);
    commandBuffers->beginRenderPass(i, renderPass, framebuffers[i],
                                    swapchain->getExtent());
    commandBuffers->bindPipeline(i, pipeline);
    commandBuffers->bindVertexBuffer(i, vertexBuffer, 0);
    commandBuffers->bindIndexBuffer(i, indexBuffer, 0);
    descriptorSets->bind(commandBuffers->get(i), i);
    commandBuffers->draw(i, indices.size(), 1, 0, 0, 0);
    commandBuffers->endRenderPass(i);
    commandBuffers->end(i);
  }

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    auto nextImg = swapchain->getNextImageIdx(imageAvailable);
    commandBuffers->submit(nextImg, imageAvailable, renderFinished);
    updateUniformBuffer(uniformBuffers[nextImg], 800, 600);
    swapchain->present(nextImg, renderFinished);
    device->waitPresentQueue();
  }

  device->waitIdle();

  glfwDestroyWindow(window);

  glfwTerminate();

  return 0;
}
