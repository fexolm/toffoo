project(toffoo)

set(CMAKE_CXX_STANDARD 20)

add_executable(toffoo main.cpp vk/Instance.cpp vk/Device.cpp vk/Surface.cpp vk/SwapChain.cpp vk/Shader.cpp vk/Pipeline.cpp vk/RenderPass.cpp)
target_link_libraries(toffoo glfw vulkan)