#include "vk/Device.h"
#include "vk/Instance.h"
#include "vk/Surface.h"
#include "vk/SwapChain.h"

#include <GLFW/glfw3.h>
#include <vector>

std::vector<const char *> getRequiredExtensions() {
  uint32_t glfwExtensionCount = 0;
  auto glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

  std::vector<const char *> extensions(glfwExtensions,
                                       glfwExtensions + glfwExtensionCount);
  return extensions;
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

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
  }

  glfwDestroyWindow(window);

  glfwTerminate();

  return 0;
}
