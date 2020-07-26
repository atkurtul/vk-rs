
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
    let appinfo  = vk::ApplicationInfo::new()
        .engineVersion(vk::make_version(1, 2, 0))
	    .applicationVersion(vk::make_version(1, 2, 0))
        .apiVersion(vk::make_version(1, 2, 0));

    let info = vk::InstanceCreateInfo::new()
	    .pApplicationInfo(&appinfo)
	    .enabledLayerCount(layers_raw.len() as _)
	    .ppEnabledLayerNames(layers_raw.as_ptr())
	    .enabledExtensionCount(extensions_raw.len() as _)
        .ppEnabledExtensionNames(extensions_raw.as_ptr());

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
	let qinfo = vk::DeviceQueueCreateInfo::new()
	    .queueCount(1)
	    .pQueuePriorities(&prio);

    let features = vk::PhysicalDeviceFeatures::new()
	    .fillModeNonSolid(1)
	    .sampleRateShading(1)
	    .vertexPipelineStoresAndAtomics(1)
        .samplerAnisotropy(1);
    
    let ext_features = vk::PhysicalDeviceDescriptorIndexingFeatures::new()
	    .descriptorBindingPartiallyBound(1)
        .descriptorBindingVariableDescriptorCount(1);
    
    let device_info = vk::DeviceCreateInfo::new()
        .queueCreateInfoCount(1)
	    .pNext(&ext_features as *const _ as _)
	    .pQueueCreateInfos(&qinfo)
	    .pEnabledFeatures(&features)
	    .enabledExtensionCount(device_ext_raw.len() as _)
	    .ppEnabledExtensionNames(device_ext_raw.as_ptr())
	    .enabledLayerCount(layers_raw.len() as _)
        .ppEnabledLayerNames(layers_raw.as_ptr());
    
    let res = vk::CreateDevice(&device_info);
    vk::SetDeviceQueue(0, 0);
    println!("Device created {}", res);
}

pub unsafe fn create_swapchain(extent : vk::Extent2D, surface : vk::SurfaceKHR) -> vk::SwapchainKHR {
    use vk::*;
    let info = SwapchainCreateInfoKHR::new()
        .surface(surface)
        .minImageCount(3)
        .imageFormat(FORMAT_B8G8R8A8_UNORM)
        .imageColorSpace(COLOR_SPACE_SRGB_NONLINEAR_KHR)
        .imageExtent(extent)
        .imageArrayLayers(1)
        .imageUsage(IMAGE_USAGE_COLOR_ATTACHMENT_BIT as _)
        .imageSharingMode(SHARING_MODE_EXCLUSIVE)
        .preTransform(SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
        .compositeAlpha(COMPOSITE_ALPHA_OPAQUE_BIT_KHR)
        .presentMode(PRESENT_MODE_IMMEDIATE_KHR)
        .clipped(1);
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
    let subpass =  make_subpass_desc(1)
        .pColorAttachments(&refs[0])
        .pDepthStencilAttachment(&refs[1]);

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
        let layout_info = vk::PipelineLayoutCreateInfo::new();
        let mut layout = 0;
        vk::CreatePipelineLayout(&layout_info, &mut layout);
        let stage = stage_info();
        let input_state = vertex_input_state();
        let assembler = assembler_info();
        let viewport = viewport_info(800, 600);
        let rasterizer = rasterizer_info();
        let ms = ms_info();
        let depth = depth_info();
        let blend = blend_info();
        let mut pipe : vk::Pipeline = 0;
        let info = vk::GraphicsPipelineCreateInfo::new()
            .stageCount(2)
            .pStages(stage.0.as_ptr())
            .pVertexInputState(&input_state.0)
            .pInputAssemblyState(&assembler)
            .pViewportState(&viewport.0)
            .pRasterizationState(&rasterizer)
            .pMultisampleState(&ms)
            .pDepthStencilState(&depth)
            .pColorBlendState(&blend.0)
            .renderPass(pass)
            .layout(layout);
        vk::CreateGraphicsPipelines(0, 1, &info, &mut pipe);
        pipe
    }
}

fn blend_info() -> (vk::PipelineColorBlendStateCreateInfo, Box<vk::PipelineColorBlendAttachmentState>) {
    let attachment = Box::new(vk::PipelineColorBlendAttachmentState::new()
        .colorWriteMask((vk::COLOR_COMPONENT_R_BIT |
        vk::COLOR_COMPONENT_G_BIT | vk::COLOR_COMPONENT_B_BIT | vk::COLOR_COMPONENT_A_BIT) as _));
    (vk::PipelineColorBlendStateCreateInfo::new()
    .attachmentCount(1)
    .pAttachments(&*attachment), attachment)
}

fn depth_info() -> vk::PipelineDepthStencilStateCreateInfo {
	vk::PipelineDepthStencilStateCreateInfo::new()
	    .depthTestEnable(1)
	    .depthWriteEnable(1)
	    .depthCompareOp(vk::COMPARE_OP_LESS)
}

fn ms_info() -> vk::PipelineMultisampleStateCreateInfo {
	vk::PipelineMultisampleStateCreateInfo::new().rasterizationSamples(1)
}

fn rasterizer_info() -> vk::PipelineRasterizationStateCreateInfo{
	vk::PipelineRasterizationStateCreateInfo::new()
	    .polygonMode(vk::POLYGON_MODE_FILL)
	    .lineWidth(1f32)
	    .frontFace(vk::FRONT_FACE_CLOCKWISE)
	    .cullMode(vk::CULL_MODE_NONE as _)
}

fn viewport_info(width : u32, height : u32) -> 
        (vk::PipelineViewportStateCreateInfo,
        Box<vk::Viewport>,
        Box<vk::Rect2D>) {
	let viewport = Box::new(vk::Viewport::new().width(width as _).height(height as _).maxDepth(1.));
	let scissor = Box::new(vk::Rect2D::new().extent(vk::Extent2D{width, height}));

	(vk::PipelineViewportStateCreateInfo::new()
	    .viewportCount(1)
	    .scissorCount(1)
	    .pScissors(&*scissor)
	    .pViewports(&*viewport), viewport, scissor)
}

fn assembler_info() -> vk::PipelineInputAssemblyStateCreateInfo {
	vk::PipelineInputAssemblyStateCreateInfo::new().topology(vk::PRIMITIVE_TOPOLOGY_TRIANGLE_LIST)
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

        vk::CreateCommandPool(&vk::CommandPoolCreateInfo::new()
            .flags(vk::COMMAND_POOL_CREATE_TRANSIENT_BIT | vk::COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT), &mut pool);

        let alloc_info = vk::CommandBufferAllocateInfo::new()
            .commandPool(pool)
            .level(vk::COMMAND_BUFFER_LEVEL_PRIMARY)
            .commandBufferCount(3);

        vk::AllocateCommandBuffers(&alloc_info, cmd_buffers.as_mut_ptr());
        fence.iter_mut().for_each(|f| {vk::CreateFence(&vk::FenceCreateInfo::new().flags(vk::FENCE_CREATE_SIGNALED_BIT), f);});
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
	let info = vk::ImageCreateInfo::new()
	    .imageType(vk::IMAGE_TYPE_2D)
	    .extent(vk::Extent3D{width, height, depth : 1})
	    .mipLevels(1)
	    .arrayLayers(1)
	    .tiling(vk::IMAGE_TILING_OPTIMAL)
	    .sharingMode(vk::SHARING_MODE_EXCLUSIVE)
	    .samples(1)
	    .format(format)
        .usage(usage);
    let mut image = 0;
    unsafe { vk::CreateImage(&info, &mut image); }
	image
}

fn find_memory_type_index(req : vk::MemoryRequirements, flags : vk::MemoryPropertyFlags) -> u32 
{
    let mut props = <_>::default();
    unsafe { vk::GetPhysicalDeviceMemoryProperties(&mut props); }
    props.memoryTypes.iter().enumerate().find(|t| (t.1.propertyFlags & flags) == flags).unwrap().0 as _
}

fn alloc_image_memory(image : vk::Image) -> vk::DeviceMemory {
    unsafe {
        let mut req = vk::MemoryRequirements::default();
        vk::GetImageMemoryRequirements(image, &mut req);
        let info = vk::MemoryAllocateInfo::new()
            .allocationSize(req.size)
            .memoryTypeIndex(find_memory_type_index(req, vk::MEMORY_PROPERTY_DEVICE_LOCAL_BIT));
        let mut memory = 0;
        vk::AllocateMemory(&info, &mut memory);
        vk::BindImageMemory(image, memory, 0);
        memory
    }
}

fn create_image_view(img : vk::Image, format : vk::Format, aspect : vk::ImageAspectFlags) -> vk::ImageView
{
	let info = vk::ImageViewCreateInfo::new()
	    .image(img)
	    .viewType(vk::IMAGE_VIEW_TYPE_2D)
	    .format(format)
	    .subresourceRange(
            vk::ImageSubresourceRange::new().aspectMask(aspect)
	        .levelCount(1)
            .layerCount(1));
    let mut view = 0;
    unsafe { vk::CreateImageView(&info, &mut view); }
    view
}

fn create_image_and_view(format : vk::Format, usage : vk::ImageUsageFlags, width : u32, height : u32, aspect : vk::ImageAspectFlags) -> (vk::Image, vk::DeviceMemory, vk::ImageView)
{
    let img = create_image(format, usage, width, height);
    let memory = alloc_image_memory(img);
    let view = create_image_view(img, format, aspect);
    (img, memory, view)
}

fn create_framebuffer(pass : vk::RenderPass, attachments : Vec<vk::ImageView>, width : u32, height : u32) -> vk::Framebuffer
{
	let info = vk::FramebufferCreateInfo::new()
	    .renderPass(pass)
	    .attachmentCount(attachments.len() as _)
	    .pAttachments(attachments.as_ptr())
	    .width(width)
	    .height(height)
        .layers(1);
    let mut framebuffer = 0;
    unsafe { vk::CreateFramebuffer(&info, &mut framebuffer); }
    framebuffer
}

fn alloc_buffer_memory(buffer : vk::Buffer) -> (vk::DeviceMemory, *mut f32) {
    unsafe {
        let mut req = vk::MemoryRequirements::default();
        vk::GetBufferMemoryRequirements(buffer, &mut req);
        let info = vk::MemoryAllocateInfo::new()
            .allocationSize(req.size)
            .memoryTypeIndex(
                    find_memory_type_index(req,  vk::MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                                        vk::MEMORY_PROPERTY_HOST_COHERENT_BIT));
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
        let info = vk::BufferCreateInfo::new()
            .usage(vk::BUFFER_USAGE_VERTEX_BUFFER_BIT)
            .sharingMode(vk::SHARING_MODE_EXCLUSIVE)
            .size((vert.len() * 4usize) as _);
        let mut buffer = 0;
        vk::CreateBuffer(&info, &mut buffer); 
        let memory = alloc_buffer_memory(buffer);
        std::ptr::copy_nonoverlapping(vert.as_ptr(), memory.1, info.size as _);
        (buffer, memory.0, memory.1)
    }
}