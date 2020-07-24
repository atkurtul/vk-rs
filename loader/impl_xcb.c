#include "loader.h"
#include <xcb/xcb.h>
#include "vulkan/vulkan_xcb.h"

extern VkInstance			instance;
extern VkPhysicalDevice	    physicalDevice;

VkResult CreateXcbSurfaceKHR(
    const VkXcbSurfaceCreateInfoKHR*            pCreateInfo,
    VkSurfaceKHR*                               pSurface) {
        return ((PFN_vkCreateXcbSurfaceKHR)vkCreateXcbSurfaceKHR)(instance, pCreateInfo, 0, pSurface);
}

VkBool32 GetPhysicalDeviceXcbPresentationSupportKHR(
    uint32_t                                    queueFamilyIndex,
    xcb_connection_t*                           connection,
    xcb_visualid_t                              visual_id) {
        return ((PFN_vkGetPhysicalDeviceXcbPresentationSupportKHR)vkGetPhysicalDeviceXcbPresentationSupportKHR)(physicalDevice, queueFamilyIndex, connection, visual_id);
}
