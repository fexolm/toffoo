#pragma once

#include "Buffer.h"
#include <memory>
#include <vulkan/vulkan.h>

namespace toffoo::vk {
class Device;

class VertexBuffer : public Buffer {
public:
  VertexBuffer(std::shared_ptr<Device> device, size_t size);
};

std::shared_ptr<VertexBuffer> createVertexBuffer(std::shared_ptr<Device> device,
                                                 size_t size);
} // namespace toffoo::vk