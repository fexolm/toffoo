#pragma once
#include <memory>
#include <vector>
#include <vulkan/vulkan.h>

namespace toffoo::vk {
class Device;
class Shader {
private:
  std::shared_ptr<Device> device;
  VkShaderModule shader;

public:
  Shader(std::shared_ptr<Device> device, const std::vector<char> &code);

  VkShaderModule handle();

  ~Shader();
};

std::shared_ptr<Shader> createShader(std::shared_ptr<Device> device,
                                     const char *filename);
} // namespace toffoo::vk