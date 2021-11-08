#include "Utils.h"
#include <cstring>

#include "Instance.h"

namespace toffoo::vk {
static const std::vector<const char *> validationLayers = {
    "VK_LAYER_KHRONOS_validation"};

bool checkValidationLayerSupport() { return true; }

Instance::Instance(const std::vector<const char *> &extensions,
                   bool enableValidationLayers) {
  if (enableValidationLayers && !checkValidationLayerSupport()) {
    throw std::runtime_error("validation layers requested, but not available!");
  }

  VkApplicationInfo appInfo{
      .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
      .pApplicationName = "Toffoo",
      .applicationVersion = VK_MAKE_VERSION(0, 1, 0),
      .pEngineName = "Toffoo Engine",
      .engineVersion = VK_MAKE_VERSION(0, 1, 0),
      .apiVersion = VK_API_VERSION_1_0,
  };
  VkInstanceCreateInfo createInfo{
      .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
      .pApplicationInfo = &appInfo,
      .enabledLayerCount = 0,
      .ppEnabledLayerNames = nullptr,
      .enabledExtensionCount = (uint32_t)extensions.size(),
      .ppEnabledExtensionNames = extensions.data()};

  if (enableValidationLayers) {
    createInfo.enabledLayerCount =
        static_cast<uint32_t>(validationLayers.size());
    createInfo.ppEnabledLayerNames = validationLayers.data();
  }

  VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);

  VK_THROW_NOT_OK(vkCreateInstance(&createInfo, nullptr, &instance));
}

Instance::~Instance() { vkDestroyInstance(instance, nullptr); }

VkInstance Instance::handle() { return instance; }

std::shared_ptr<Instance>
createInstance(const std::vector<const char *> &extensions,
               bool enableValidationLayers) {
  return std::make_shared<Instance>(extensions, enableValidationLayers);
}

} // namespace toffoo::vk