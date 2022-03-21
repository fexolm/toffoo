#pragma once
#include <stdexcept>
#include <vulkan/vulkan.h>

#define VK_THROW_NOT_OK(res)                                                   \
  if ((res) != VK_SUCCESS) {                                                   \
    throw std::runtime_error("Vulkan error: " + res);                          \
  }

inline uint32_t findMemoryType(VkPhysicalDevice physicalDevice,
                               uint32_t typeFilter,
                               VkMemoryPropertyFlags properties) {
  VkPhysicalDeviceMemoryProperties memProperties;
  vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

  for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
    if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags &
                                    properties) == properties) {
      return i;
    }
  }

  throw std::runtime_error("failed to find suitable memory type!");
}
