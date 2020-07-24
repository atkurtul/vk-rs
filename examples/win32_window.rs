#![cfg(feature="win32")]

extern crate winapi;
extern crate vk_rs;
use vk_rs::*;

fn main() {

}

struct InputState {
    pos     : [f32;2],
    state   : i32,
    time    : f64,
}

pub struct IO {
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

pub struct Window {
    pub hwnd        : u64,
    pub hinstance   : u64,
    pub surface     : vk::SurfaceKHR,
    pub io          : Box::<IO>,
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

    pub unsafe 
    fn new(x : u32, y : u32) -> Window
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
            0, 0, x as _, y as _,
            null_mut(), null_mut(), hinstance, io_ptr as _);     
        ShowWindow(hwnd, SW_SHOW);
        UpdateWindow(hwnd);  

        let mut surface = 0;
        let mut surface_info = vk::Win32SurfaceCreateInfoKHR::default();
        surface_info.hwnd = hwnd as _;
        surface_info.hinstance = hinstance as _;
        vk::CreateWin32SurfaceKHR(&surface_info,  &mut surface);
        let mut supported = 0;
        vk::GetPhysicalDeviceSurfaceSupportKHR(0, surface, &mut supported);

        Window { hwnd : hwnd as u64, hinstance : hinstance as u64, surface, io : io }
    }

    pub unsafe
    fn poll(&self) -> bool
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

    pub fn extent(&self) -> vk::Extent2D {
        self.io.extent()
    }

    fn mpos(&self) -> [f32;2] {
        self.io.mpos
    }
}