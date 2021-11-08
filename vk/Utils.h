#pragma once
#include <stdexcept>

#define VK_THROW_NOT_OK(res)                                                   \
  if ((res) != VK_SUCCESS) {                                                   \
    throw std::runtime_error("Vulkan error: " + res);                          \
  }
