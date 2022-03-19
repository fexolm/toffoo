#pragma once
#include "Buffer.h"
#include <memory>
#include <vulkan/vulkan.h>

namespace toffoo::vk {
class IndexBuffer : public Buffer {
public:
  IndexBuffer(std::shared_ptr<Device> device, size_t size);
};

std::shared_ptr<IndexBuffer> createIndexBuffer(std::shared_ptr<Device> device,
                                               size_t size);
} // namespace toffoo::vk