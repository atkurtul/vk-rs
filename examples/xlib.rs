#![cfg(feature="xlib")]

use std::ffi::CString;
extern crate vk_rs;
use vk_rs::*;

fn main()
{
    unsafe {
        let surface_extension = CString::new("VK_KHR_xlib_surface").unwrap();
        vk::InitVulkanSimple(surface_extension.as_ptr());
        println!("Success!");

        //vk::CreateSwapchainKHR(pCreateInfo: *const SwapchainCreateInfoKHR, pSwapchain: *mut SwapchainKHR)
        //vk::CreateWin32SurfaceKHR(pCreateInfo: *const Win32SurfaceCreateInfoKHR, pSurface: *mut SurfaceKHR);
    }
}
