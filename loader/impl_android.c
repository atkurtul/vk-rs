#include "loader.h"
#include "vulkan/vulkan_android.h"

extern VkInstance			instance;
extern VkDevice	            device;

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