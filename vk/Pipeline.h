#pragma once
#include <memory>
#include <vector>
#include <vulkan/vulkan.h>

namespace toffoo::vk {
class Shader;
class SwapChain;
class Device;
class GraphicsPipelineBuilder;
class RenderPass;
class PipelineLayout;

class GraphicsPipeline {

private:
  VkPipeline pipeline;

  std::shared_ptr<Device> device;

  std::shared_ptr<PipelineLayout> pipelineLayout;

  std::shared_ptr<RenderPass> renderPass;

public:
  GraphicsPipeline(GraphicsPipelineBuilder &builder);

  VkPipeline handle();

  ~GraphicsPipeline();
};

class PipelineLayout {
private:
  std::shared_ptr<Device> device;
  VkPipelineLayout layout;

public:
  PipelineLayout(std::shared_ptr<Device> device);

  VkPipelineLayout handle();

  ~PipelineLayout();
};

class GraphicsPipelineBuilder {
private:
  std::shared_ptr<Device> device;
  std::shared_ptr<RenderPass> renderPass;

  VkPipelineShaderStageCreateInfo vertShaderStageInfo;
  VkPipelineShaderStageCreateInfo fragShaderStageInfo;
  VkPipelineVertexInputStateCreateInfo vertexInputStateInfo;
  VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateInfo;

  VkViewport viewport;
  VkRect2D scissor;
  VkPipelineViewportStateCreateInfo viewportStateInfo;

  VkPipelineRasterizationStateCreateInfo rasterizerStateInfo;
  VkPipelineMultisampleStateCreateInfo miltisamplingStateInfo;

  VkPipelineColorBlendAttachmentState colorBlendAttachment;
  VkPipelineColorBlendStateCreateInfo colorBlendingStateInfo;

  VkPipelineDynamicStateCreateInfo dynamicStateInfo;

  std::shared_ptr<PipelineLayout> pipelineLayout;

  std::shared_ptr<Shader> vertShader;
  std::shared_ptr<Shader> fragShader;

public:
  GraphicsPipelineBuilder(std::shared_ptr<Device> device,
                          std::shared_ptr<RenderPass> renderPass);

  void addVertexShader(std::shared_ptr<Shader> shader);
  void addFragmentShader(std::shared_ptr<Shader> shader);
  void addVertexInputState(
      const std::vector<VkVertexInputAttributeDescription>
          &attributeDescriptions,
      const VkVertexInputBindingDescription &bindingDescription);
  void addInputAssemblyState();
  void addViewportState(std::shared_ptr<SwapChain> swapchain, float width,
                        float height);
  void addRasterizationState();
  void addMultisamplintState();

  void addColorBlendState();
  void addDynamicState();

  void setupPipelineLayout();

  std::shared_ptr<GraphicsPipeline> build();

  friend class GraphicsPipeline;
};
} // namespace toffoo::vk