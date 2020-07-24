#![allow(dead_code)]
#![allow(unused_variables)]
#![allow(unused_imports)]

extern crate winapi;
extern crate vk_rs;
use vk_rs::*;
use std::ptr::*;
use std::ffi::CString;

fn zero_mem<T>() -> T { unsafe { std::mem::zeroed::<T>() } }
fn typeid<T: std::any::Any>(_: &T) {
    println!("{:?}", std::any::type_name::<T>());
}

fn main()
{
    unsafe {
        let win = Window::new(800, 600);
        vk::InitVulkan();
        let swapchain = Swapchain::new_win32(&win);
        let renderpass = make_renderpass(1);
        while Window::poll() {
            //println!("{:?}", win.mpos());
        }
    }
}

fn make_attachment(
    format : vk::Format, 
    ms : i32, 
    load_op : vk::AttachmentLoadOp, 
    store_op : vk::AttachmentStoreOp, 
    layout : vk::ImageLayout) -> vk::AttachmentDescription {
        vk::AttachmentDescription {
            format : format, 
            samples : ms,
            loadOp : load_op,
            storeOp : store_op,
            initialLayout : vk::IMAGE_LAYOUT_UNDEFINED,
            finalLayout : layout,
            .. Default::default()
        }
}

fn make_depth_attachment(ms : i32) -> vk::AttachmentDescription {
    make_attachment(vk::FORMAT_D32_SFLOAT, ms, vk::ATTACHMENT_LOAD_OP_CLEAR, vk::ATTACHMENT_STORE_OP_STORE, vk::IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL)
}

fn make_color_attachment(ms : i32, present : bool, op : vk::AttachmentLoadOp) -> vk::AttachmentDescription {
    let mut layout = vk::IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    if present {
        layout = vk::IMAGE_LAYOUT_PRESENT_SRC_KHR;
    }
    make_attachment(vk::FORMAT_B8G8R8A8_UNORM, ms, op, vk::ATTACHMENT_STORE_OP_STORE, layout)
}

fn  make_color_ref(slot : u32) -> vk::AttachmentReference
{
    vk::AttachmentReference { 
        attachment : slot, 
        layout : vk::IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL 
    }
}

fn  make_depth_ref(slot : u32) -> vk::AttachmentReference
{
    vk::AttachmentReference { 
        attachment : slot, 
        layout : vk::IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL 
    }
}

fn make_subpass_desc(ncolor : u32) -> vk::SubpassDescription {

    vk::SubpassDescription {
        pipelineBindPoint : vk::PIPELINE_BIND_POINT_GRAPHICS,
        colorAttachmentCount : ncolor,
        .. Default::default()
    }
}

fn make_renderpass(ms : i32) -> vk::RenderPass{
    let attachments : [vk::AttachmentDescription;2]= [
        make_color_attachment(1, true, vk::ATTACHMENT_LOAD_OP_CLEAR),
        make_depth_attachment(1),
    ];
    let refs = [ make_color_ref(0), make_depth_ref(1) ];
    let mut subpass =  make_subpass_desc(1);
    subpass.pColorAttachments = &refs[0];
    subpass.pDepthStencilAttachment = &refs[1];

    let info = vk::RenderPassCreateInfo {
        attachmentCount : attachments.len() as _,
        pAttachments : attachments.as_ptr(),
        subpassCount : 1,
        pSubpasses : &subpass,
        .. Default::default()
    };
    let mut pass = 0;
    unsafe { println!("RenderPass {:?}", vk::CreateRenderPass(&info, &mut pass)) };
    pass
}

struct Swapchain
{
    surface : vk::SurfaceKHR,
    handle  : vk::SwapchainKHR,
}

impl Swapchain {
    unsafe
    fn new_win32(win : &Window) ->Swapchain {
        use vk::*;
        let mut surface = 0;
        let mut handle = 0;
        let mut surface_info = Win32SurfaceCreateInfoKHR::default();
        surface_info.hwnd = win.hwnd;
        surface_info.hinstance = win.hinstance;
        CreateWin32SurfaceKHR(&surface_info,  &mut surface);
        let mut supported = 0;
        GetPhysicalDeviceSurfaceSupportKHR(0, surface, &mut supported);
        let mut info = SwapchainCreateInfoKHR::default();
        info.surface = surface;
        info.minImageCount = 3;
        info.imageFormat = FORMAT_B8G8R8A8_UNORM;
        info.imageColorSpace = COLOR_SPACE_SRGB_NONLINEAR_KHR;
        info.imageExtent = win.extent();
        info.imageArrayLayers = 1;
        info.imageUsage = IMAGE_USAGE_COLOR_ATTACHMENT_BIT as _;
        info.imageSharingMode = SHARING_MODE_EXCLUSIVE;
        info.preTransform = SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
        info.compositeAlpha = COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        info.presentMode = PRESENT_MODE_MAILBOX_KHR;
        info.clipped = 1;
        let result = CreateSwapchainKHR(&info, &mut handle);
        println!("swapchain created {}", result);
        Swapchain {
            surface : surface,
            handle : handle,
        }
    }
}


struct InputState {
    pos     : [f32;2],
    state   : i32,
    time    : f64,
}

struct IO {
    input : [InputState; 259],
    size  : [f32;2],
    mpos  : [f32;2],
}

impl IO {
    fn lparam_to_pos(lparam : isize) -> [f32;2] {
        let x = (lparam & 65535) as i16;
        let y = (lparam >> 16) as i16;
        [x as f32, y as f32]
    }

    fn set_input_down(&mut self, wparam : usize) {
        self.input[wparam].state += 1 ;
    }

    fn set_input_up(&mut self, wparam : usize) {
        self.input[wparam].state  = 0;
    }

    fn new() -> Box::<IO> {
        unsafe { Box::<IO>::new(std::mem::zeroed::<IO>()) }
    }

    fn resize(&mut self, lparam : isize) {
        self.size = IO::lparam_to_pos(lparam);
    }

    fn extent(&self) -> vk::Extent2D
    {
        vk::Extent2D { 
            width : self.size[0] as u32, 
            height : self.size[1] as u32
        }
    }
}

struct Window {
    hwnd        : u64,
    hinstance   : u64,
    io          : Box::<IO>,
}

impl Window
{  
    unsafe extern "system" 
    fn wnd_proc(hwnd : *mut winapi::shared::windef::HWND__, msg : u32, wparam : usize, lparam: isize) -> isize 
    {
        use winapi::um::winuser::*; 
        let io = &mut *(GetWindowLongPtrA(hwnd, GWLP_USERDATA) as *mut IO);
        match msg {
            WM_NCCREATE => {
                let params = (*(lparam as *mut CREATESTRUCTA)).lpCreateParams as isize;
                SetWindowLongPtrA(hwnd, GWLP_USERDATA, params);
            }
            WM_KEYDOWN => {
                io.set_input_down(wparam);
                if wparam == VK_ESCAPE as usize {
                    PostQuitMessage(1)
                }
            }
            WM_MOUSEMOVE => io.mpos = IO::lparam_to_pos(lparam),
            WM_SIZE => io.resize(lparam),
            WM_KEYUP => io.set_input_up(wparam),
            WM_LBUTTONDOWN => io.set_input_down(256),
            WM_RBUTTONDOWN => io.set_input_down(257),
            WM_MBUTTONDOWN => io.set_input_down(258),
            WM_LBUTTONUP =>   io.set_input_up(256),
            WM_RBUTTONUP =>   io.set_input_up(257),
            WM_MBUTTONUP =>   io.set_input_up(258),
        _ => (),
        }  
        DefWindowProcA(hwnd, msg, wparam, lparam)
    }  

    unsafe 
    fn new(x : i32, y : i32) -> Window
    {
        use std::ptr::*;
        use std::ffi::*;
        let mut io = Box::<IO>::new(std::mem::zeroed::<IO>());
        let io_ptr = &mut *io as *mut IO;
        use winapi::um::winuser::*;
        let hinstance = winapi::um::libloaderapi::GetModuleHandleA(null_mut());
        let name = CString::new("wc").unwrap();
        let wc = WNDCLASSA {
            style : CS_OWNDC | CS_HREDRAW | CS_VREDRAW,
            lpfnWndProc : Some(Window::wnd_proc),
            hInstance : hinstance,
            lpszClassName : name.as_ptr(),
            cbClsExtra : 0,
            cbWndExtra : 0,
            hIcon: null_mut(),
            hCursor: null_mut(),
            hbrBackground: null_mut(),
            lpszMenuName: null_mut(),
        };
        RegisterClassA(&wc);
        let hwnd = CreateWindowExA(WS_EX_ACCEPTFILES, name.as_ptr(), name.as_ptr(),
            WS_VISIBLE | WS_SIZEBOX | WS_SYSMENU,
            0, 0, x, y,
            null_mut(), null_mut(), hinstance, io_ptr as _);     
        ShowWindow(hwnd, SW_SHOW);
        UpdateWindow(hwnd);  

        Window { hwnd : hwnd as u64, hinstance : hinstance as u64, io : io }
    }

    unsafe
    fn poll() -> bool
    {
        use std::ptr::*;
        use winapi::um::winuser::*;
        let mut msg = std::mem::zeroed::<MSG>();
        while PeekMessageA(&mut msg, null_mut(), 0, 0, PM_REMOVE) != 0
        {
            TranslateMessage(&msg);
            DispatchMessageA(&msg);
            if msg.message == WM_QUIT { 
                return false;
            }
        }
        return true;
    }

    fn extent(&self) -> vk::Extent2D {
        self.io.extent()
    }

    fn mpos(&self) -> [f32;2] {
        self.io.mpos
    }
}