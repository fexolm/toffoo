#include "Pipeline.h"
#include "Device.h"
#include "RenderPass.h"
#include "Shader.h"
#include "SwapChain.h"
#include "Utils.h"

namespace toffoo::vk {

GraphicsPipeline::GraphicsPipeline(GraphicsPipelineBuilder &builder)
    : device(builder.device), pipelineLayout(builder.pipelineLayout),
      renderPass(builder.renderPass) {
  VkPipelineShaderStageCreateInfo shaders[2] = {builder.vertShaderStageInfo,
                                                builder.fragShaderStageInfo};
  VkGraphicsPipelineCreateInfo pipelineInfo{
      .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
      .stageCount = 2,
      .pStages = shaders,
      .pVertexInputState = &builder.vertexInputStateInfo,
      .pInputAssemblyState = &builder.inputAssemblyStateInfo,
      .pViewportState = &builder.viewportStateInfo,
      .pRasterizationState = &builder.rasterizerStateInfo,
      .pMultisampleState = &builder.miltisamplingStateInfo,
      .pDepthStencilState = nullptr,
      .pColorBlendState = &builder.colorBlendingStateInfo,
      .pDynamicState = nullptr, // &builder.dynamicStateInfo,
      .layout = builder.pipelineLayout->handle(),
      .renderPass = builder.renderPass->handle(),
      .subpass = 0,
      .basePipelineHandle = VK_NULL_HANDLE,
      .basePipelineIndex = -1};

  VK_THROW_NOT_OK(vkCreateGraphicsPipelines(device->handle(), VK_NULL_HANDLE, 1,
                                            &pipelineInfo, nullptr, &pipeline));
}

VkPipeline GraphicsPipeline::handle() { return pipeline; }

GraphicsPipeline::~GraphicsPipeline() {
  vkDestroyPipeline(device->handle(), pipeline, nullptr);
}

GraphicsPipelineBuilder::GraphicsPipelineBuilder(
    std::shared_ptr<Device> device, std::shared_ptr<RenderPass> renderPass)
    : device(device), renderPass(renderPass) {}

void GraphicsPipelineBuilder::addVertexShader(std::shared_ptr<Shader> shader) {
  vertShader = shader;
  vertShaderStageInfo = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
      .stage = VK_SHADER_STAGE_VERTEX_BIT,
      .module = shader->handle(),
      .pName = "main"};
}

void GraphicsPipelineBuilder::addFragmentShader(
    std::shared_ptr<Shader> shader) {
  fragShader = shader;
  fragShaderStageInfo = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
      .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
      .module = shader->handle(),
      .pName = "main"};
}

void GraphicsPipelineBuilder::addVertexInputState(
    const std::vector<VkVertexInputAttributeDescription> &attributeDescriptions,
    const VkVertexInputBindingDescription &bindingDescription) {
  vertexInputStateInfo = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
      .vertexBindingDescriptionCount = 1,
      .pVertexBindingDescriptions = &bindingDescription,
      .vertexAttributeDescriptionCount =
          static_cast<uint32_t>(attributeDescriptions.size()),
      .pVertexAttributeDescriptions = attributeDescriptions.data()};
}

void GraphicsPipelineBuilder::addInputAssemblyState() {
  inputAssemblyStateInfo = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
      .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
      .primitiveRestartEnable = VK_FALSE};
}

void GraphicsPipelineBuilder::addViewportState(
    std::shared_ptr<SwapChain> swapchain, float width, float height) {
  viewport = {.x = 0.0f,
              .y = 0.0f,
              .width = width,
              .height = height,
              .minDepth = 0.0f,
              .maxDepth = 1.0f};

  scissor = {
      .offset = {0, 0},
      .extent = swapchain->getExtent(),
  };

  viewportStateInfo = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
      .viewportCount = 1,
      .pViewports = &viewport,
      .scissorCount = 1,
      .pScissors = &scissor,
  };
}

void GraphicsPipelineBuilder::addRasterizationState() {
  rasterizerStateInfo = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
      .depthClampEnable = VK_FALSE,
      .rasterizerDiscardEnable = VK_FALSE,
      .polygonMode = VK_POLYGON_MODE_FILL,
      .cullMode = VK_CULL_MODE_BACK_BIT,
      .frontFace = VK_FRONT_FACE_CLOCKWISE,
      .depthBiasEnable = VK_FALSE,
      .depthBiasConstantFactor = 0.0f,
      .depthBiasClamp = 0.0f,
      .depthBiasSlopeFactor = 0.0f,
      .lineWidth = 1.0f,
  };
}

void GraphicsPipelineBuilder::addMultisamplintState() {
  miltisamplingStateInfo = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
      .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
      .sampleShadingEnable = VK_FALSE,
      .minSampleShading = 1.0,
      .pSampleMask = nullptr,
      .alphaToCoverageEnable = VK_FALSE,
      .alphaToOneEnable = VK_FALSE};
}

void GraphicsPipelineBuilder::addColorBlendState() {
  colorBlendAttachment = {
      .blendEnable = VK_FALSE,
      .srcColorBlendFactor = VK_BLEND_FACTOR_ONE,
      .dstColorBlendFactor = VK_BLEND_FACTOR_ZERO,
      .colorBlendOp = VK_BLEND_OP_ADD,
      .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
      .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
      .alphaBlendOp = VK_BLEND_OP_ADD,
      .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
                        VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT

  };

  colorBlendingStateInfo = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
      .logicOpEnable = VK_FALSE,
      .logicOp = VK_LOGIC_OP_COPY,
      .attachmentCount = 1,
      .pAttachments = &colorBlendAttachment,
      .blendConstants = {0.0f, 0.0f, 0.0f, 0.0f}};
}

void GraphicsPipelineBuilder::addDynamicState() {
  static VkDynamicState dynamicStates[] = {VK_DYNAMIC_STATE_VIEWPORT,
                                           VK_DYNAMIC_STATE_LINE_WIDTH};
  dynamicStateInfo = {.sType =
                          VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
                      .dynamicStateCount = 2,
                      .pDynamicStates = dynamicStates};
}

void GraphicsPipelineBuilder::setupPipelineLayout() {
  pipelineLayout = std::make_shared<PipelineLayout>(device);
}

PipelineLayout::PipelineLayout(std::shared_ptr<Device> device)
    : device(device), layout(layout) {
  VkPipelineLayoutCreateInfo pipelineLayoutInfo = {
      .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
      .setLayoutCount = 0,
      .pSetLayouts = nullptr,
      .pushConstantRangeCount = 0,
      .pPushConstantRanges = nullptr};

  VK_THROW_NOT_OK(vkCreatePipelineLayout(device->handle(), &pipelineLayoutInfo,
                                         nullptr, &layout));
}

VkPipelineLayout PipelineLayout::handle() { return layout; }

PipelineLayout::~PipelineLayout() {
  vkDestroyPipelineLayout(device->handle(), layout, nullptr);
}

std::shared_ptr<GraphicsPipeline> GraphicsPipelineBuilder::build() {
  return std::make_shared<GraphicsPipeline>(*this);
}

} // namespace toffoo::vk