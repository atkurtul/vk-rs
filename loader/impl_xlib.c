#include "loader.h"
#include <X11/Xlib.h>
#include "vulkan/vulkan_xlib.h"

extern VkInstance			instance;
extern VkPhysicalDevice	    physicalDevice;

VkResult CreateXlibSurfaceKHR(
    const VkXlibSurfaceCreateInfoKHR*           pCreateInfo,
    VkSurfaceKHR*                               pSurface) {
        return ((PFN_vkCreateXlibSurfaceKHR)vkCreateXlibSurfaceKHR)(instance, pCreateInfo, 0, pSurface);
}

VkBool32 GetPhysicalDeviceXlibPresentationSupportKHR(
    uint32_t                                    queueFamilyIndex,
    Display*                                    dpy,
    VisualID                                    visualID) {
        return ((PFN_vkGetPhysicalDeviceXlibPresentationSupportKHR)vkGetPhysicalDeviceXlibPresentationSupportKHR)(physicalDevice, queueFamilyIndex, dpy, visualID);
}