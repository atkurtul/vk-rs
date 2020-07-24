
extern crate vk_rs;
use vk_rs::*;
use std::ffi::CString;
use std::ptr;

pub unsafe fn init_vulkan() {
    let extensions : Vec<CString> =  ["VK_KHR_surface", "VK_KHR_xlib_surface"].iter().map(|s| CString::new(*s).unwrap()).collect();
    let layers : Vec<CString> =  ["VK_LAYER_KHRONOS_validation"].iter().map(|s| CString::new(*s).unwrap()).collect();
    let device_ext : Vec<CString> =  ["VK_KHR_swapchain"].iter().map(|s| CString::new(*s).unwrap()).collect();

    let extensions_raw : Vec<*const i8> = extensions.iter().map(|s| s.as_ptr()).collect();
    let layers_raw : Vec<*const i8> = layers.iter().map(|s| s.as_ptr()).collect();
    let device_ext_raw : Vec<*const i8> = device_ext.iter().map(|s| s.as_ptr()).collect();
	let mut appinfo  = vk::ApplicationInfo::default();
	appinfo.engineVersion = vk::make_version(1, 2, 0);
	appinfo.applicationVersion = vk::make_version(1, 2, 0);
    appinfo.apiVersion = vk::make_version(1, 2, 0);
    
    let mut info = vk::InstanceCreateInfo::default();
	info.pApplicationInfo = &appinfo;
	info.enabledLayerCount = layers_raw.len() as _;
	info.ppEnabledLayerNames = layers_raw.as_ptr();
	info.enabledExtensionCount = extensions_raw.len() as _;
    info.ppEnabledExtensionNames = extensions_raw.as_ptr();

    let res = vk::CreateInstance(&info);
    println!("Instance created {}", res);
    let mut count = 0;
    let res = vk::EnumeratePhysicalDevices(&mut count, ptr::null_mut());
    println!("Enumerate devices {}", res);
    let devices = vec![vk::PhysicalDevice::default(); count as _];
    let res = vk::EnumeratePhysicalDevices(&mut count, devices.as_ptr() as _);
    println!("Enumerate devices {}", res);
    
    let mut props = vk::PhysicalDeviceProperties::default();
    let pdev = devices.iter().cloned().find(|d| {
        vk::SetPhysicalDevice(*d);
        vk::GetPhysicalDeviceProperties(&mut props);
        props.deviceType == vk::PHYSICAL_DEVICE_TYPE_DISCRETE_GPU
    }).unwrap();
    vk::SetPhysicalDevice(pdev);
    println!("Found device {}", pdev);
    
    let prio = 1f32;
	let mut qinfo = vk::DeviceQueueCreateInfo::default();
	qinfo.queueCount = 1;
	qinfo.pQueuePriorities = &prio;

    let mut features = vk::PhysicalDeviceFeatures::default();
	features.fillModeNonSolid = 1;
	features.sampleRateShading = 1;
	features.vertexPipelineStoresAndAtomics = 1;
    features.samplerAnisotropy = 1;
    
    let mut ext_features = vk::PhysicalDeviceDescriptorIndexingFeatures::default();
	ext_features.descriptorBindingPartiallyBound = 1;
    ext_features.descriptorBindingVariableDescriptorCount = 1;
    
    let mut device_info = vk::DeviceCreateInfo::default();
	device_info.queueCreateInfoCount = 1;
	device_info.pNext = &ext_features as *const _ as _;
	device_info.pQueueCreateInfos = &qinfo;
	device_info.pEnabledFeatures = &features;
	device_info.enabledExtensionCount = device_ext_raw.len() as _;
	device_info.ppEnabledExtensionNames = device_ext_raw.as_ptr();
	device_info.enabledLayerCount = layers_raw.len() as _;
    device_info.ppEnabledLayerNames = layers_raw.as_ptr();
    
    let res = vk::CreateDevice(&device_info);
    println!("Device created {}", res);
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

pub fn make_renderpass(ms : i32) -> vk::RenderPass{
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
    unsafe { println!("RenderPass created {:?}", vk::CreateRenderPass(&info, &mut pass)) };
    pass
}
