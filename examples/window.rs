
extern crate vk_rs;
use vk_rs::*;

#[derive(Copy, Clone)]
pub struct Window {
    pub display : u64,
    pub window : u64,
    pub surface : vk::SurfaceKHR,
    pub extent : vk::Extent2D,
}

impl Window {
    pub unsafe fn new(width : u32, height : u32) -> Window {
        use x11::*;
        let display = xlib::XOpenDisplay(std::ptr::null());
        let screen = xlib::XDefaultScreen(display);
        let root = xlib::XRootWindow(display, screen);
        let mut attributes: xlib::XSetWindowAttributes = std::mem::uninitialized();
        attributes.background_pixel = xlib::XWhitePixel(display, screen);
        let window = xlib::XCreateWindow(display, root,
            0, 0, width, height,
            0, 0,
            xlib::InputOutput as _, std::ptr::null_mut(),
            xlib::CWBackPixel, &mut attributes);
        xlib::XSelectInput(display, window, xlib::ExposureMask | xlib::KeyPressMask);
        xlib::XMapWindow(display, window);

        let mut surface_info = vk::XlibSurfaceCreateInfoKHR::default();
        surface_info.dpy = display as _;
        surface_info.window = window;
        let mut surface = 0;
        vk::CreateXlibSurfaceKHR(&surface_info,  &mut surface);
        let mut supported = 0;
        vk::GetPhysicalDeviceSurfaceSupportKHR(0, surface, &mut supported);

        Window { display : display as _, window, surface, extent : vk::Extent2D {width, height} }
    }

    pub unsafe fn poll(&self) -> bool {
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