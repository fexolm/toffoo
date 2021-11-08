#include "Device.h"
#include "Instance.h"
#include "Surface.h"
#include "Utils.h"
#include <cassert>
#include <set>

namespace toffoo::vk {
const std::vector<const char *> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME};

bool checkDeviceExtensionSupport(VkPhysicalDevice device) {
  uint32_t extensionCount;
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount,
                                       nullptr);

  std::vector<VkExtensionProperties> availableExtensions(extensionCount);
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount,
                                       availableExtensions.data());

  std::set<std::string> requiredExtensions(deviceExtensions.begin(),
                                           deviceExtensions.end());

  for (const auto &extension : availableExtensions) {
    requiredExtensions.erase(extension.extensionName);
  }

  return requiredExtensions.empty();
}

std::vector<VkQueueFamilyProperties> getQueueFamilies(VkPhysicalDevice device) {
  uint32_t queueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

  std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount,
                                           queueFamilies.data());
  return queueFamilies;
}

bool findGraphicsFamily(VkPhysicalDevice device, uint32_t &idx) {
  auto families = getQueueFamilies(device);
  for (uint32_t i = 0; i < families.size(); ++i) {
    if (families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      idx = i;
      return true;
    }
  }
  return false;
}

bool findPresentFamily(VkPhysicalDevice device, VkSurfaceKHR surface,
                       uint32_t &idx) {
  auto families = getQueueFamilies(device);
  for (uint32_t i = 0; i < families.size(); ++i) {
    if (families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      VkBool32 presentSupport = false;
      vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
      idx = i;
      return true;
    }
  }
  return false;
}

bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface) {
  VkPhysicalDeviceProperties deviceProperties;
  vkGetPhysicalDeviceProperties(device, &deviceProperties);
  uint32_t idx;
  bool hasGraphicsQueue = findGraphicsFamily(device, idx);
  bool hasPresentQueue = findPresentFamily(device, surface, idx);
  bool extensionsSupported = checkDeviceExtensionSupport(device);

  // TODO: also verify that swapchain supports at least 1 format and at least 1
  // presetMode

  return (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU ||
          deviceProperties.deviceType ==
              VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU) &&
         hasGraphicsQueue && hasPresentQueue && extensionsSupported;
}

VkPhysicalDevice peekDevice(VkInstance instance, VkSurfaceKHR surface) {
  uint32_t deviceCount = 0;
  vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
  if (deviceCount == 0) {
    throw std::runtime_error("failed to find GPUs with Vulkan support!");
  }
  std::vector<VkPhysicalDevice> devices(deviceCount);
  vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

  VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
  for (const auto &device : devices) {
    if (isDeviceSuitable(device, surface)) {
      physicalDevice = device;
      break;
    }
  }

  if (physicalDevice == VK_NULL_HANDLE) {
    throw std::runtime_error("failed to find a suitable GPU!");
  }

  return physicalDevice;
}

Device::Device(std::shared_ptr<Instance> instance,
               std::shared_ptr<Surface> surface)
    : instance(instance), surface(surface) {
  physicalDevice = peekDevice(instance->handle(), surface->handle());
  uint32_t graphicsFamilyIdx;
  uint32_t presentFamilyIdx;

  findGraphicsFamily(physicalDevice, graphicsFamilyIdx);
  findPresentFamily(physicalDevice, surface->handle(), presentFamilyIdx);

  std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

  float queuePriority = 1.0f;
  queueCreateInfos.push_back(
      {.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
       .queueFamilyIndex = graphicsFamilyIdx,
       .queueCount = 1,
       .pQueuePriorities = &queuePriority});

  if (graphicsFamilyIdx != presentFamilyIdx) {
    queueCreateInfos.push_back(
        {.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
         .queueFamilyIndex = presentFamilyIdx,
         .queueCount = 1,
         .pQueuePriorities = &queuePriority});
  }

  VkPhysicalDeviceFeatures deviceFeatures{};

  VkDeviceCreateInfo createInfo{
      .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
      .queueCreateInfoCount = (uint32_t)queueCreateInfos.size(),
      .pQueueCreateInfos = queueCreateInfos.data(),
      .enabledExtensionCount = (uint32_t)deviceExtensions.size(),
      .ppEnabledExtensionNames = deviceExtensions.data(),
      .pEnabledFeatures = &deviceFeatures};

  VK_THROW_NOT_OK(
      vkCreateDevice(physicalDevice, &createInfo, nullptr, &device));

  vkGetDeviceQueue(device, graphicsFamilyIdx, 0, &graphicsQueue);
  vkGetDeviceQueue(device, presentFamilyIdx, 0, &presentQueue);

  // TODO: we currently expect them to be equal to use VK_SHARING_MODE_EXCLUSIVE
  // mode in swapchain but it is not always true
  assert(graphicsFamilyIdx == presentFamilyIdx);
}

VkDevice Device::handle() { return device; }

VkPhysicalDevice Device::getPhysicalDevice() { return physicalDevice; }

std::shared_ptr<Surface> Device::getSurface() { return surface; }

Device::~Device() { vkDestroyDevice(device, nullptr); }

std::shared_ptr<Device> createDevice(std::shared_ptr<Instance> instance,
                                     std::shared_ptr<Surface> surface) {
  return std::make_shared<Device>(instance, surface);
}
} // namespace toffoo::vk