#include "Shader.h"
#include "Device.h"
#include "Utils.h"
#include <fstream>
namespace toffoo::vk {

Shader::Shader(std::shared_ptr<Device> device, const std::vector<char> &code)
    : device(device) {
  VkShaderModuleCreateInfo createInfo{
      .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
      .codeSize = code.size(),
      .pCode = reinterpret_cast<const uint32_t *>(code.data())};

  VK_THROW_NOT_OK(
      vkCreateShaderModule(device->handle(), &createInfo, nullptr, &shader));
}

VkShaderModule Shader::handle() { return shader; }

Shader::~Shader() { vkDestroyShaderModule(device->handle(), shader, nullptr); }

std::shared_ptr<Shader> createShader(std::shared_ptr<Device> device,
                                     const char *filename) {
  std::ifstream file(filename, std::ios::ate | std::ios::binary);

  size_t fileSize = (size_t)file.tellg();
  std::vector<char> buffer(fileSize);
  file.seekg(0);
  file.read(buffer.data(), fileSize);

  return std::make_shared<Shader>(device, buffer);
}
} // namespace toffoo::vk