extern crate vk_rs;
use vk_rs::*;
mod util;
use util::*;

#[cfg(feature="win32")]
mod win32_window;
#[cfg(feature="win32")]
use win32_window::*;

#[cfg(feature="xlib")]
mod xlib_window;
#[cfg(feature="xlib")]
use xlib_window::*;


fn main()
{
    unsafe {
        #[cfg(feature="win32")]
        init_vulkan("VK_KHR_win32_surface");
        #[cfg(feature="xlib")]
        init_vulkan("VK_KHR_xlib_surface");

        let win = Window::new(800, 600);
        let swapchain = create_swapchain(win.extent(), win.surface);
        let renderpass = make_renderpass(1);
        let pipeline = create_pipeline(renderpass);
        let (cmd_pool, cmd_buffer, fence, acquire_semaphore, present_semaphore, views, framebuffer) = 
                create_resources(swapchain, renderpass, win.extent().width, win.extent().height);

        let vert : Vec<f32> = vec![
            -1., 1.,    1., 0., 0.,
             0.,-1.,    0., 1., 0.,
             1., 1.,    0., 0., 1.,
        ];

        let buffer = create_buffer(&vert);

        let mut frame = 0;
        let mut counter = -1i32;
        while win.poll() {
            let mut idx = 0;
            counter = (counter + 1) % 3;
            frame = counter as _;
            vk::AcquireNextImageKHR(swapchain, u64::MAX, acquire_semaphore[frame], 0, &mut idx);
            vk::WaitForFences(1, &fence[frame], 1, u64::MAX);
            vk::ResetFences(1, &fence[frame]);
            vk::ResetCommandBuffer(cmd_buffer[frame], vk::COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);
            let mut cmd_begin_info = vk::CommandBufferBeginInfo::default();
            cmd_begin_info.flags = vk::COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
            vk::BeginCommandBuffer(cmd_buffer[frame], &cmd_begin_info);


            let clear_values = [vk::ClearValue::new().color([0.4, 0.4, 0.5, 1.]), vk::ClearValue::depth(1., 0.) ];
            let mut pass_info =  vk::RenderPassBeginInfo::default();
            pass_info.renderPass = renderpass;
            pass_info.framebuffer = framebuffer[frame];
            pass_info.renderArea.extent = win.extent(); 
            pass_info.clearValueCount = 2;
            pass_info.pClearValues = &clear_values as _;
            vk::CmdBeginRenderPass(cmd_buffer[frame], &pass_info, vk::SUBPASS_CONTENTS_INLINE);

            vk::CmdBindPipeline(cmd_buffer[frame], vk::PIPELINE_BIND_POINT_GRAPHICS, pipeline);
            let offset = 0;
            vk::CmdBindVertexBuffers(cmd_buffer[frame], 0, 1, &buffer.0, &offset);
            vk::CmdDraw(cmd_buffer[frame], 6, 1, 0, 0);
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
            present_info.pSwapchains = &swapchain;
            present_info.pImageIndices = &counter;
            vk::QueuePresentKHR(&present_info);
        }
        
        println!("Success!");
    }
}
