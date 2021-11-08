#pragma once

#include <memory>
#include <vector>
#include <vulkan/vulkan.h>

namespace toffoo::vk {

class Instance {
private:
  VkInstance instance;

public:
  Instance(const std::vector<const char *> &extensions,
           bool enableValidationLayers);

  VkInstance handle();

  ~Instance();
};

std::shared_ptr<Instance>
createInstance(const std::vector<const char *> &extensions,
               bool enableValidationLayers);

} // namespace toffoo::vk