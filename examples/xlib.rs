//#![cfg(feature="xlib")]

extern crate vk_rs;
use vk_rs::*;
extern crate x11;
mod util;
use util::*;

fn main()
{
    unsafe {
        init_vulkan("VK_KHR_xlib_surface");
        let win = Window::new(800, 600);
        let swapchain = Swapchain::new_xlib(win);
        let renderpass = make_renderpass(1);
        let pipeline = create_pipeline(renderpass);
        let (cmd_pool, cmd_buffer, fence, acquire_semaphore, present_semaphore, views, framebuffer) = 
                create_resources(swapchain.handle, renderpass, 800, 600);

        const IDX : [u32;3] = [0, 1, 2];
        let mut frame = -1i32 as usize;
        let mut counter = -1i32;
        while(win.poll()) {
            let mut idx = 0;
            counter = (counter + 1) % 3;
            frame = counter as _;
            vk::AcquireNextImageKHR(swapchain.handle, u64::MAX, acquire_semaphore[frame], 0, &mut idx);
            vk::WaitForFences(1, &fence[frame], 1, u64::MAX);
            vk::ResetFences(1, &fence[frame]);
            vk::ResetCommandBuffer(cmd_buffer[frame], vk::COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);
            let mut cmd_begin_info = vk::CommandBufferBeginInfo::default();
            cmd_begin_info.flags = vk::COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
            vk::BeginCommandBuffer(cmd_buffer[frame], &cmd_begin_info);


            let clear_values = [vk::ClearValue::color(0.4, 0.1, 0.5, 1.), vk::ClearValue::depth(1., 0.) ];
            let mut pass_info =  vk::RenderPassBeginInfo::default();
            pass_info.renderPass = renderpass;
            pass_info.framebuffer = framebuffer[frame];
            pass_info.renderArea.extent = vk::Extent2D{width:800, height:600 }; 
            pass_info.clearValueCount = 2;
            pass_info.pClearValues = &clear_values as _;
            vk::CmdBeginRenderPass(cmd_buffer[frame], &pass_info, vk::SUBPASS_CONTENTS_INLINE);


            vk::CmdEndRenderPass(cmd_buffer[frame]);
            vk::EndCommandBuffer(cmd_buffer[frame]);

            let mut submit_info = vk::SubmitInfo::default();
            submit_info.waitSemaphoreCount = 1;
            submit_info.signalSemaphoreCount = 1;
            submit_info.commandBufferCount = 1;
            submit_info.pCommandBuffers = &cmd_buffer[frame];
            submit_info.pWaitDstStageMask = &vk::PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            submit_info.pWaitSemaphores = &acquire_semaphore[frame];
            submit_info.pSignalSemaphores = &present_semaphore[frame];

            vk::QueueSubmit(1, &submit_info, fence[frame]);

            let mut present_info = vk::PresentInfoKHR::default();
            present_info.waitSemaphoreCount = 1;
            present_info.pWaitSemaphores = &present_semaphore[frame];
            present_info.swapchainCount = 1;
            present_info.pSwapchains = &swapchain.handle;
            present_info.pImageIndices = &IDX[frame];
            vk::QueuePresentKHR(&present_info);
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
        info.presentMode = PRESENT_MODE_FIFO_KHR;
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
    unsafe fn new(width : u32, height : u32) -> Window {
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
        Window { display : display as _, window, extent : vk::Extent2D {width, height} }
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