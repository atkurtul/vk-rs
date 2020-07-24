//#![cfg(feature="xlib")]

extern crate vk_rs;
use vk_rs::*;
extern crate x11;
mod util;
use util::*;

fn main()
{
    unsafe {
        init_vulkan();
        let win = Window::new();
        let swapchain = Swapchain::new_xlib(win);
        let renderpass = make_renderpass(1);
        println!("{:?}", stage_info());
        while(win.poll()) {
        }
        println!("Success!");
    }
}


struct Swapchain
{
    surface : vk::SurfaceKHR,
    handle  : vk::SwapchainKHR,
}

impl Swapchain {
    unsafe fn new_xlib(win : Window) ->Swapchain {
        use vk::*;
        let mut surface = 0;
        let mut handle = 0;
        let mut surface_info = XlibSurfaceCreateInfoKHR::default();
        surface_info.dpy = win.display;
        surface_info.window = win.window;
        CreateXlibSurfaceKHR(&surface_info,  &mut surface);
        let mut supported = 0;
        GetPhysicalDeviceSurfaceSupportKHR(0, surface, &mut supported);
        let mut info = SwapchainCreateInfoKHR::default();
        info.surface = surface;
        info.minImageCount = 3;
        info.imageFormat = FORMAT_B8G8R8A8_UNORM;
        info.imageColorSpace = COLOR_SPACE_SRGB_NONLINEAR_KHR;
        info.imageExtent = win.extent;
        info.imageArrayLayers = 1;
        info.imageUsage = IMAGE_USAGE_COLOR_ATTACHMENT_BIT as _;
        info.imageSharingMode = SHARING_MODE_EXCLUSIVE;
        info.preTransform = SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
        info.compositeAlpha = COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        info.presentMode = PRESENT_MODE_IMMEDIATE_KHR;
        info.clipped = 1;
        let result = CreateSwapchainKHR(&info, &mut handle);
        println!("Swapchain created {}", result);
        Swapchain {
            surface : surface,
            handle : handle,
        }
    }
}

#[derive(Copy, Clone)]
struct Window {
    display : u64,
    window : u64,
    extent : vk::Extent2D,
}

impl Window {
    unsafe fn new() -> Window {
        use x11::*;
        let display = xlib::XOpenDisplay(std::ptr::null());
        let screen = xlib::XDefaultScreen(display);
        let root = xlib::XRootWindow(display, screen);
        let mut attributes: xlib::XSetWindowAttributes = std::mem::uninitialized();
        attributes.background_pixel = xlib::XWhitePixel(display, screen);
        let window = xlib::XCreateWindow(display, root,
            0, 0, 800, 600,
            0, 0,
            xlib::InputOutput as _, std::ptr::null_mut(),
            xlib::CWBackPixel, &mut attributes);
        xlib::XSelectInput(display, window, xlib::ExposureMask | xlib::KeyPressMask);
        xlib::XMapWindow(display, window);
        Window { display : display as _, window, extent : vk::Extent2D {width : 800, height: 600} }
    }

    unsafe fn poll(&self) -> bool {
        use x11::*;
        use x11::xlib::*;
        if XPending(self.display as _) > 0 {
            let mut event: x11::xlib::XEvent = std::mem::uninitialized();
            XNextEvent(self.display as _, &mut event);  
            match event.get_type() {
                KeyPress => { 
                    println!("{}", event.key.keycode); 
                    if event.key.keycode == 9 { 
                        return false; 
                    }
                }
                _ => ()
            }
        }
        true
    } 
}