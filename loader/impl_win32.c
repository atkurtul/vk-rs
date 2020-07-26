#include "loader.h"
#include "windows.h"
#include "vulkan/vulkan_win32.h"

extern VkInstance			instance;
extern VkDevice			    device;
extern VkPhysicalDevice	    physicalDevice;

VkResult CreateWin32SurfaceKHR(
    const struct VkWin32SurfaceCreateInfoKHR*          pCreateInfo,
    VkSurfaceKHR*                                      pSurface) {
        return ((PFN_vkCreateWin32SurfaceKHR)vkCreateWin32SurfaceKHR)(instance, pCreateInfo, 0, pSurface);
}
VkBool32 GetPhysicalDeviceWin32PresentationSupportKHR() {
        return ((PFN_vkGetPhysicalDeviceWin32PresentationSupportKHR)vkGetPhysicalDeviceWin32PresentationSupportKHR)(physicalDevice, 0);
}
VkResult GetMemoryWin32HandleKHR(
    const struct VkMemoryGetWin32HandleInfoKHR*        pGetWin32HandleInfo,
    HANDLE*                                            pHandle) {
        return ((PFN_vkGetMemoryWin32HandleKHR)vkGetMemoryWin32HandleKHR)(device, pGetWin32HandleInfo, pHandle);
}
VkResult GetMemoryWin32HandlePropertiesKHR(
    VkExternalMemoryHandleTypeFlagBits          handleType,
    HANDLE                                      handle,
    VkMemoryWin32HandlePropertiesKHR*           pMemoryWin32HandleProperties) {
        return ((PFN_vkGetMemoryWin32HandlePropertiesKHR)vkGetMemoryWin32HandlePropertiesKHR)(device, handleType, handle, pMemoryWin32HandleProperties);
}
VkResult ImportSemaphoreWin32HandleKHR(
    const VkImportSemaphoreWin32HandleInfoKHR*  pImportSemaphoreWin32HandleInfo) {
        return ((PFN_vkImportSemaphoreWin32HandleKHR)vkImportSemaphoreWin32HandleKHR)(device, pImportSemaphoreWin32HandleInfo);
}
VkResult GetSemaphoreWin32HandleKHR(
    const VkSemaphoreGetWin32HandleInfoKHR*     pGetWin32HandleInfo,
    HANDLE*                                     pHandle) {
        return ((PFN_vkGetSemaphoreWin32HandleKHR)vkGetSemaphoreWin32HandleKHR)(device, pGetWin32HandleInfo, pHandle);
}
VkResult ImportFenceWin32HandleKHR(
    const VkImportFenceWin32HandleInfoKHR*      pImportFenceWin32HandleInfo) {
        return ((PFN_vkImportFenceWin32HandleKHR)vkImportFenceWin32HandleKHR)(device, pImportFenceWin32HandleInfo);
}
VkResult GetFenceWin32HandleKHR(
    const VkFenceGetWin32HandleInfoKHR*         pGetWin32HandleInfo,
    HANDLE*                                     pHandle) {
        return ((PFN_vkGetFenceWin32HandleKHR)vkGetFenceWin32HandleKHR)(device, pGetWin32HandleInfo, pHandle);
}
VkResult GetMemoryWin32HandleNV(
    VkDeviceMemory                              memory,
    VkExternalMemoryHandleTypeFlagsNV           handleType,
    HANDLE*                                     pHandle) {
        return ((PFN_vkGetMemoryWin32HandleNV)vkGetMemoryWin32HandleNV)(device, memory, handleType, pHandle);
}
VkResult GetPhysicalDeviceSurfacePresentModes2EXT(
    const VkPhysicalDeviceSurfaceInfo2KHR*      pSurfaceInfo,
    uint32_t*                                   pPresentModeCount,
    VkPresentModeKHR*                           pPresentModes) {
        return ((PFN_vkGetPhysicalDeviceSurfacePresentModes2EXT)vkGetPhysicalDeviceSurfacePresentModes2EXT)(physicalDevice, pSurfaceInfo, pPresentModeCount, pPresentModes);
}
VkResult AcquireFullScreenExclusiveModeEXT(
    VkSwapchainKHR                              swapchain) {
        return ((PFN_vkAcquireFullScreenExclusiveModeEXT)vkAcquireFullScreenExclusiveModeEXT)(device, swapchain);
}
VkResult ReleaseFullScreenExclusiveModeEXT(
    VkSwapchainKHR                              swapchain) {
        return ((PFN_vkReleaseFullScreenExclusiveModeEXT)vkReleaseFullScreenExclusiveModeEXT)(device, swapchain);
}
VkResult GetDeviceGroupSurfacePresentModes2EXT(
    const VkPhysicalDeviceSurfaceInfo2KHR*      pSurfaceInfo,
    VkDeviceGroupPresentModeFlagsKHR*           pModes) {
        return ((PFN_vkGetDeviceGroupSurfacePresentModes2EXT)vkGetDeviceGroupSurfacePresentModes2EXT)(device, pSurfaceInfo, pModes);
}