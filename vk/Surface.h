#pragma once
#include "Instance.h"

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

namespace toffoo::vk {
class Instance;

class Surface {
private:
  std::shared_ptr<Instance> instance;
  VkSurfaceKHR surface;

public:
  Surface(std::shared_ptr<Instance> instance, VkSurfaceKHR handle);

  VkSurfaceKHR handle();

  ~Surface();
};

std::shared_ptr<Surface> createGlfwSurface(std::shared_ptr<Instance> instance,
                                           GLFWwindow *window);
} // namespace toffoo::vk