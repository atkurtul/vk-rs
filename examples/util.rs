
extern crate vk_rs;
use vk_rs::*;
use std::ffi::CString;
use std::ptr;

pub unsafe fn init_vulkan(surface_extension : &str) {
    let extensions : Vec<CString> =  ["VK_KHR_surface", surface_extension].iter().map(|s| CString::new(*s).unwrap()).collect();
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
    vk::SetDeviceQueue(0, 0);
    println!("Device created {}", res);
}

pub unsafe fn create_swapchain(extent : vk::Extent2D, surface : vk::SurfaceKHR) -> vk::SwapchainKHR {
    use vk::*;
    let mut info = SwapchainCreateInfoKHR::default();
    info.surface = surface;
    info.minImageCount = 3;
    info.imageFormat = FORMAT_B8G8R8A8_UNORM;
    info.imageColorSpace = COLOR_SPACE_SRGB_NONLINEAR_KHR;
    info.imageExtent = extent;
    info.imageArrayLayers = 1;
    info.imageUsage = IMAGE_USAGE_COLOR_ATTACHMENT_BIT as _;
    info.imageSharingMode = SHARING_MODE_EXCLUSIVE;
    info.preTransform = SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    info.compositeAlpha = COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    info.presentMode = PRESENT_MODE_FIFO_KHR;
    info.clipped = 1;
    let mut swapchain = 0;
    let result = CreateSwapchainKHR(&info, &mut swapchain);
    println!("Swapchain created {}", result);
    swapchain
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

fn  make_color_ref(slot : u32) -> vk::AttachmentReference {
    vk::AttachmentReference { 
        attachment : slot, 
        layout : vk::IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL 
    }
}

fn  make_depth_ref(slot : u32) -> vk::AttachmentReference {
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

pub fn create_pipeline(pass : vk::RenderPass) -> vk::Pipeline {
    unsafe {
        let stage = stage_info();
        let input_state = vertex_input_state();
        let assembler = assembler_info();
        let viewport = viewport_info(800, 600);
        let rasterizer = rasterizer_info();
        let ms = ms_info();
        let depth = depth_info();
        let blend = blend_info();
        let mut pipe : vk::Pipeline = 0;
        let mut info = vk::GraphicsPipelineCreateInfo::default();
        info.stageCount = 2;
        info.pStages = stage.0.as_ptr();
        info.pVertexInputState = &input_state.0;
        info.pInputAssemblyState = &assembler;
        info.pViewportState = &viewport.0;
        info.pRasterizationState = &rasterizer;
        info.pMultisampleState = &ms;
        info.pDepthStencilState = &depth;
        info.pColorBlendState = &blend.0;
        info.renderPass = pass;
        let layout_info = vk::PipelineLayoutCreateInfo::default();
        vk::CreatePipelineLayout(&layout_info, &mut info.layout);
        vk::CreateGraphicsPipelines(0, 1, &info, &mut pipe);
        pipe
    }
}

fn blend_info() -> (vk::PipelineColorBlendStateCreateInfo, Box<vk::PipelineColorBlendAttachmentState>) {
	let mut info = vk::PipelineColorBlendStateCreateInfo::default();
	let mut attachment = Box::new(vk::PipelineColorBlendAttachmentState::default());
	info.attachmentCount = 1;
	info.pAttachments = &*attachment;
	attachment.colorWriteMask = (vk::COLOR_COMPONENT_R_BIT |
    vk::COLOR_COMPONENT_G_BIT | vk::COLOR_COMPONENT_B_BIT | vk::COLOR_COMPONENT_A_BIT) as _;
    (info, attachment)
}

fn depth_info() -> vk::PipelineDepthStencilStateCreateInfo {
	let mut info = vk::PipelineDepthStencilStateCreateInfo::default();
	info.depthTestEnable = 1;
	info.depthWriteEnable = 1;
	info.depthCompareOp = vk::COMPARE_OP_LESS;
	info
}

fn ms_info() -> vk::PipelineMultisampleStateCreateInfo {
	let mut info = vk::PipelineMultisampleStateCreateInfo::default();
	info.rasterizationSamples = 1;
	info
}

fn rasterizer_info() -> vk::PipelineRasterizationStateCreateInfo{
	let mut info = vk::PipelineRasterizationStateCreateInfo::default();
	info.polygonMode = vk::POLYGON_MODE_FILL;
	info.lineWidth = 1f32;
	info.frontFace = vk::FRONT_FACE_CLOCKWISE;
	info.cullMode = vk::CULL_MODE_NONE as _;
    info
}

fn viewport_info(width : u32, height : u32) -> 
        (vk::PipelineViewportStateCreateInfo,
        Box<vk::Viewport>,
        Box<vk::Rect2D>) {
	let mut viewport = Box::new(vk::Viewport::default());
	let mut scissor = Box::new(vk::Rect2D::default());

	viewport.width	= width as _;
	viewport.height	= height as _;
	viewport.maxDepth = 1.;

	scissor.extent = vk::Extent2D { width, height };

	let mut info = vk::PipelineViewportStateCreateInfo::default();
	info.viewportCount = 1;
	info.scissorCount = 1;
	info.pScissors = &*scissor;
	info.pViewports = &*viewport;

	(info, viewport, scissor)
}

fn assembler_info() -> vk::PipelineInputAssemblyStateCreateInfo {
	let mut info = vk::PipelineInputAssemblyStateCreateInfo::default();
	info.topology = vk::PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	return info;
}

fn vertex_input_state() -> (vk::PipelineVertexInputStateCreateInfo, 
    Box<vk::VertexInputBindingDescription>, 
    Box<[vk::VertexInputAttributeDescription; 2]>) {   
    let mut info    = vk::PipelineVertexInputStateCreateInfo::default();
    let mut binding = Box::new(vk::VertexInputBindingDescription::default());
    let mut attrib  = Box::new([vk::VertexInputAttributeDescription::default(); 2]);
    binding.stride = 20;
    attrib[0].location = 0;
    attrib[1].location = 1;
    attrib[0].format = vk::FORMAT_R32G32_SFLOAT;
    attrib[1].format = vk::FORMAT_R32G32B32_SFLOAT;
    attrib[0].offset = 0;
    attrib[1].offset = 8;
    info.vertexBindingDescriptionCount = 1;
    info.pVertexAttributeDescriptions = &*attrib as _;
    info.pVertexBindingDescriptions = &*binding;
    info.vertexAttributeDescriptionCount = 2;
    info.vertexBindingDescriptionCount = 1;
    (info, binding, attrib)
}

unsafe fn stage_info() -> ([vk::PipelineShaderStageCreateInfo; 2], CString)  {

    let vert = std::fs::read("examples/shaders/vert.spv").unwrap();
    let frag = std::fs::read("examples/shaders/frag.spv").unwrap();

    let mut shader_info = <[vk::ShaderModuleCreateInfo;2]>::default();
    shader_info[0].pCode = vert.as_ptr() as _;
    shader_info[1].pCode = frag.as_ptr() as _;
    shader_info[0].codeSize = vert.len() as _;
    shader_info[1].codeSize = frag.len() as _;

    let entry = CString::new("main").unwrap();
    let mut info = <[vk::PipelineShaderStageCreateInfo; 2]>::default();
    info[0].pName = entry.as_ptr();
    info[1].pName = entry.as_ptr();
    info[0].stage = vk::SHADER_STAGE_VERTEX_BIT;
    info[1].stage = vk::SHADER_STAGE_FRAGMENT_BIT;
    vk::CreateShaderModule(&shader_info[0], &mut info[0].module);
    vk::CreateShaderModule(&shader_info[1], &mut info[1].module);
    (info, entry)
}

pub fn create_resources(swapchain : vk::SwapchainKHR, renderpass : vk::RenderPass, width : u32, height : u32) -> 
    (vk::CommandPool, 
    Vec<vk::CommandBuffer>, 
    Vec<vk::Fence>,
    Vec<vk::Semaphore>,
    Vec<vk::Semaphore>,
    Vec<vk::ImageView>,
    Vec<vk::Framebuffer>) {
    unsafe {
        let mut pool = 0;
        let mut cmd_buffers = vec![0u64;3];
        let mut fence = vec![0u64;3];
        let mut acquire_semaphore = vec![0u64;3];
        let mut present_semaphore = vec![0u64;3];
        let mut views = vec![0u64;3];
        let mut framebuffer = vec![0u64;3];

        let mut pool_info = vk::CommandPoolCreateInfo::default();
        pool_info.flags = vk::COMMAND_POOL_CREATE_TRANSIENT_BIT | vk::COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;   
        vk::CreateCommandPool(&pool_info, &mut pool);

        let mut alloc_info = vk::CommandBufferAllocateInfo::default();
        alloc_info.commandPool = pool;
        alloc_info.level = vk::COMMAND_BUFFER_LEVEL_PRIMARY;
        alloc_info.commandBufferCount = 3;
        vk::AllocateCommandBuffers(&alloc_info, cmd_buffers.as_mut_ptr());
        let mut fence_info = vk::FenceCreateInfo::default();
        fence_info.flags = vk::FENCE_CREATE_SIGNALED_BIT;
        fence.iter_mut().for_each(|f| {vk::CreateFence(&fence_info, f);});
        let semaphore_info = vk::SemaphoreCreateInfo::default();
        acquire_semaphore.iter_mut().for_each(|s| { vk::CreateSemaphore(&semaphore_info, s); });
        present_semaphore.iter_mut().for_each(|s| { vk::CreateSemaphore(&semaphore_info, s); });

        let depth_buffer = create_image_and_view(vk::FORMAT_D32_SFLOAT, vk::IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT |
            vk::IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT, width, height, vk::IMAGE_ASPECT_DEPTH_BIT);
        
        let mut count = 0;
        vk::GetSwapchainImagesKHR(swapchain, &mut count, ptr::null_mut());
        let mut images = vec![0u64; count as _];
        vk::GetSwapchainImagesKHR(swapchain, &mut count, images.as_mut_ptr());

        views.iter_mut().zip(images.iter()).for_each(|(v, i)| *v = create_image_view(*i, vk::FORMAT_B8G8R8A8_UNORM, vk::IMAGE_ASPECT_COLOR_BIT) );
        framebuffer.iter_mut().zip(views.iter()).for_each(|(f, v)| *f = create_framebuffer(renderpass, vec![*v, depth_buffer.2], width, height));
        (pool, cmd_buffers, fence, acquire_semaphore, present_semaphore, views, framebuffer)
    }
}

fn create_image(format : vk::Format, usage : vk::ImageUsageFlags, width : u32, height : u32) -> vk::Image {
	let mut info = vk::ImageCreateInfo::default();
	info.imageType = vk::IMAGE_TYPE_2D;
	info.extent.width = width;
	info.extent.height = height;
	info.extent.depth = 1;
	info.mipLevels = 1;
	info.arrayLayers = 1;
	info.tiling = vk::IMAGE_TILING_OPTIMAL;
	info.sharingMode = vk::SHARING_MODE_EXCLUSIVE;
	info.samples = 1;
	info.format = format;
    info.usage = usage; 
    let mut image = 0;
    unsafe { vk::CreateImage(&info, &mut image); }
	image
}

fn find_memory_type_index(req : vk::MemoryRequirements, flags : vk::MemoryPropertyFlags) -> u32 
{
    let mut props = vk::PhysicalDeviceMemoryProperties::default();
    unsafe { vk::GetPhysicalDeviceMemoryProperties(&mut props); }
    props.memoryTypes.iter().enumerate().find(|t| (t.1.propertyFlags & flags) == flags).unwrap().0 as _
}

fn alloc_image_memory(image : vk::Image) -> vk::DeviceMemory {
    unsafe {
        let mut req = vk::MemoryRequirements::default();
        vk::GetImageMemoryRequirements(image, &mut req);
        let mut info = vk::MemoryAllocateInfo::default();
        info.allocationSize = req.size;
        info.memoryTypeIndex = find_memory_type_index(req, vk::MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
        let mut memory = 0;
        vk::AllocateMemory(&info, &mut memory);
        vk::BindImageMemory(image, memory, 0);
        memory
    }
}

fn create_image_view(img : vk::Image, format : vk::Format, aspect : vk::ImageAspectFlags) -> vk::ImageView
{
	let mut info = vk::ImageViewCreateInfo::default();
	info.image = img;
	info.viewType = vk::IMAGE_VIEW_TYPE_2D;
	info.format = format;
	info.subresourceRange.aspectMask = aspect;
	info.subresourceRange.levelCount = 1;
    info.subresourceRange.layerCount = 1;
    let mut view = 0;
    unsafe { vk::CreateImageView(&info, &mut view); }
    view
}

fn create_image_and_view(format : vk::Format, usage : vk::ImageUsageFlags, width : u32, height : u32, aspect : vk::ImageAspectFlags) -> (vk::Image, vk::DeviceMemory, vk::ImageView)
{
    unsafe {
        let img = create_image(format, usage, width, height);
        let memory = alloc_image_memory(img);
        let view = create_image_view(img, format, aspect);
        (img, memory, view)
    }
}

fn create_framebuffer(pass : vk::RenderPass, attachments : Vec<vk::ImageView>, width : u32, height : u32) -> vk::Framebuffer
{
	let mut info = vk::FramebufferCreateInfo::default();
	info.renderPass = pass;
	info.attachmentCount = attachments.len() as _;
	info.pAttachments = attachments.as_ptr();
	info.width = width;
	info.height = height;
    info.layers = 1;
    let mut framebuffer = 0;
    let mut res;
    unsafe { res = vk::CreateFramebuffer(&info, &mut framebuffer); }
    if res != 0 { panic!("Framebuffer"); }
    framebuffer
}

fn alloc_buffer_memory(buffer : vk::Buffer) -> (vk::DeviceMemory, *mut f32) {
    unsafe {
        let mut req = vk::MemoryRequirements::default();
        vk::GetBufferMemoryRequirements(buffer, &mut req);
        let mut info = vk::MemoryAllocateInfo::default();
        info.allocationSize = req.size;
        info.memoryTypeIndex = find_memory_type_index(req, vk::MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                 vk::MEMORY_PROPERTY_HOST_COHERENT_BIT);
        let mut memory = 0;
        vk::AllocateMemory(&info, &mut memory);
        vk::BindBufferMemory(buffer, memory, 0);
        let mut map : *mut f32 = std::ptr::null_mut();
        vk::MapMemory(memory,  0, req.size, 0, &mut map);
        (memory, map as _)
    }
}

pub fn create_buffer(vert : &Vec<f32>) -> (vk::Buffer, vk::DeviceMemory, *mut f32) {
    unsafe {
        let mut info = vk::BufferCreateInfo::default();
        info.usage = vk::BUFFER_USAGE_VERTEX_BUFFER_BIT;
        info.sharingMode = vk::SHARING_MODE_EXCLUSIVE;
        info.size = (vert.len() * 4usize) as _;
        let mut buffer = 0;
        vk::CreateBuffer(&info, &mut buffer); 
        let memory = alloc_buffer_memory(buffer);
        std::ptr::copy_nonoverlapping(vert.as_ptr(), memory.1, info.size as _);
        (buffer, memory.0, memory.1)
    }
}

fn main() {

}