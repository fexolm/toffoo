#include "Surface.h"
#include "Instance.h"
#include "Utils.h"

namespace toffoo::vk {

Surface::Surface(std::shared_ptr<Instance> instance, VkSurfaceKHR handle)
    : instance(instance), surface(handle) {}

VkSurfaceKHR Surface::handle() { return surface; }

Surface::~Surface() {
  vkDestroySurfaceKHR(instance->handle(), surface, nullptr);
}

std::shared_ptr<Surface> createGlfwSurface(std::shared_ptr<Instance> instance,
                                           GLFWwindow *window) {
  VkSurfaceKHR surfaceHandle;
  VK_THROW_NOT_OK(glfwCreateWindowSurface(instance->handle(), window, nullptr,
                                          &surfaceHandle));
  return std::make_shared<Surface>(instance, surfaceHandle);
}
} // namespace toffoo::vk