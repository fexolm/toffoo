#include "CommandPool.h"
#include "Device.h"
#include "Utils.h"
namespace toffoo::vk {
CommandPool::CommandPool(std::shared_ptr<Device> device) : device(device) {
  VkCommandPoolCreateInfo poolInfo{
      .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
      .flags = 0,
      .queueFamilyIndex = device->getGraphicsFamilyIdx(),
  };

  VK_THROW_NOT_OK(
      vkCreateCommandPool(device->handle(), &poolInfo, nullptr, &commandPool));
}

VkCommandPool CommandPool::handle() { return commandPool; }

CommandPool::~CommandPool() {
  vkDestroyCommandPool(device->handle(), commandPool, nullptr);
}

std::shared_ptr<CommandPool> createCommandPool(std::shared_ptr<Device> device) {
  return std::make_shared<CommandPool>(device);
}
} // namespace toffoo::vk