#include "loader.h"
#include "vulkan/vulkan_android.h"

extern VkInstance			instance;
extern VkDevice	            device;
extern VkPhysicalDevice	    physicalDevice;

VkResult CreateAndroidSurfaceKHR(
    const VkAndroidSurfaceCreateInfoKHR*        pCreateInfo,
    VkSurfaceKHR*                               pSurface) {
        return ((PFN_vkCreateAndroidSurfaceKHR)vkCreateAndroidSurfaceKHR)(instance, pCreateInfo, 0, pSurface);
}

VkResult GetAndroidHardwareBufferPropertiesANDROID(
    const struct AHardwareBuffer*               buffer,
    VkAndroidHardwareBufferPropertiesANDROID*   pProperties) {
        return ((PFN_vkGetAndroidHardwareBufferPropertiesANDROID)vkGetAndroidHardwareBufferPropertiesANDROID)(device, buffer, pProperties);
}

VkResult GetMemoryAndroidHardwareBufferANDROID(
    const VkMemoryGetAndroidHardwareBufferInfoANDROID* pInfo,
    struct AHardwareBuffer**                    pBuffer) {
        return ((PFN_vkGetMemoryAndroidHardwareBufferANDROID)vkGetMemoryAndroidHardwareBufferANDROID)(device, pInfo, pBuffer);
}

typedef VkFlags VkWaylandSurfaceCreateFlagsKHR;
typedef struct VkWaylandSurfaceCreateInfoKHR {
    VkStructureType                   sType;
    const void*                       pNext;
    VkWaylandSurfaceCreateFlagsKHR    flags;
    struct wl_display*                display;
    struct wl_surface*                surface;
} VkWaylandSurfaceCreateInfoKHR;

typedef VkResult (VKAPI_PTR *PFN_vkCreateWaylandSurfaceKHR)(VkInstance instance, const VkWaylandSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
typedef VkBool32 (VKAPI_PTR *PFN_vkGetPhysicalDeviceWaylandPresentationSupportKHR)(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, struct wl_display* display);

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

typedef unsigned xcb_window_t;

typedef VkFlags VkXcbSurfaceCreateFlagsKHR;
typedef struct VkXcbSurfaceCreateInfoKHR {
    VkStructureType               sType;
    const void*                   pNext;
    VkXcbSurfaceCreateFlagsKHR    flags;
    struct xcb_connection_t*             connection;
    uint                          window;
} VkXcbSurfaceCreateInfoKHR;

typedef VkResult (VKAPI_PTR *PFN_vkCreateXcbSurfaceKHR)(VkInstance instance, const VkXcbSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
typedef VkBool32 (VKAPI_PTR *PFN_vkGetPhysicalDeviceXcbPresentationSupportKHR)(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, struct xcb_connection_t* connection, uint visual_id);


VkResult CreateXcbSurfaceKHR(
    const VkXcbSurfaceCreateInfoKHR*            pCreateInfo,
    VkSurfaceKHR*                               pSurface) {
        return ((PFN_vkCreateXcbSurfaceKHR)vkCreateXcbSurfaceKHR)(instance, pCreateInfo, 0, pSurface);
}

VkBool32 GetPhysicalDeviceXcbPresentationSupportKHR(
    uint32_t                                    queueFamilyIndex,
    struct xcb_connection_t*                    connection,
    uint                                        visual_id) {
        return ((PFN_vkGetPhysicalDeviceXcbPresentationSupportKHR)vkGetPhysicalDeviceXcbPresentationSupportKHR)(physicalDevice, queueFamilyIndex, connection, visual_id);
}

typedef VkFlags VkXlibSurfaceCreateFlagsKHR;
typedef struct VkXlibSurfaceCreateInfoKHR {
    VkStructureType                sType;
    const void*                    pNext;
    VkXlibSurfaceCreateFlagsKHR    flags;
    void*                          dpy;
    uint64_t                       window;
} VkXlibSurfaceCreateInfoKHR;

typedef VkResult (VKAPI_PTR *PFN_vkCreateXlibSurfaceKHR)(VkInstance instance, const VkXlibSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface);
typedef VkBool32 (VKAPI_PTR *PFN_vkGetPhysicalDeviceXlibPresentationSupportKHR)(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, void* dpy, uint64_t visualID);


VkResult CreateXlibSurfaceKHR(
    const VkXlibSurfaceCreateInfoKHR*           pCreateInfo,
    VkSurfaceKHR*                               pSurface) {
        return ((PFN_vkCreateXlibSurfaceKHR)vkCreateXlibSurfaceKHR)(instance, pCreateInfo, 0, pSurface);
}

VkBool32 GetPhysicalDeviceXlibPresentationSupportKHR(
    uint32_t                                    queueFamilyIndex,
    void*                                       dpy,
    uint64_t                                    visualID) {
        return ((PFN_vkGetPhysicalDeviceXlibPresentationSupportKHR)vkGetPhysicalDeviceXlibPresentationSupportKHR)(physicalDevice, queueFamilyIndex, dpy, visualID);
}


typedef struct SECURITY_ATTRIBUTES SECURITY_ATTRIBUTES;
typedef void *HANDLE;
typedef void *HINSTANCE;
typedef void *HWND;
typedef const wchar_t *LPCWSTR;
typedef unsigned long DWORD;
typedef void *HMONITOR;
#include "vulkan/vulkan_win32.h";

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