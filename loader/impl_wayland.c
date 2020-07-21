#include "loader.h"
#include "vulkan/vulkan_wayland.h"

extern VkInstance			instance;
extern VkPhysicalDevice	    physicalDevice;

VkResult CreateWaylandSurfaceKHR(
    const VkWaylandSurfaceCreateInfoKHR*        pCreateInfo,
    VkSurfaceKHR*                               pSurface) {
        return ((PFN_vkCreateWaylandSurfaceKHR)vkCreateWaylandSurfaceKHR)(instance, pCreateInfo, 0, pSurface);
}

VkBool32 GetPhysicalDeviceWaylandPresentationSupportKHR(
    uint32_t                                    queueFamilyIndex,
    struct wl_display*                          display) {
        return ((PFN_vkGetPhysicalDeviceWaylandPresentationSupportKHR)vkGetPhysicalDeviceWaylandPresentationSupportKHR)(physicalDevice, queueFamilyIndex, display);
}
