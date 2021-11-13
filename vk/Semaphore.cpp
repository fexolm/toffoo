#include "Semaphore.h"
#include "Device.h"
#include "Utils.h"
namespace toffoo::vk {

Semaphore::Semaphore(std::shared_ptr<Device> device) : device(device) {
  VkSemaphoreCreateInfo semaphoreInfo{
      .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};
  VK_THROW_NOT_OK(
      vkCreateSemaphore(device->handle(), &semaphoreInfo, nullptr, &semaphore));
}

VkSemaphore Semaphore::handle() { return semaphore; }

Semaphore::~Semaphore() {
  vkDestroySemaphore(device->handle(), semaphore, nullptr);
}
} // namespace toffoo::vk