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

        let mut counter = -1i32;
        while win.poll() {
            let mut idx = 0;
            counter = (counter + 1) % 3;
            let frame = counter as usize;
            vk::AcquireNextImageKHR(swapchain, u64::MAX, acquire_semaphore[frame], 0, &mut idx);
            vk::WaitForFences(1, &fence[frame], 1, u64::MAX);
            vk::ResetFences(1, &fence[frame]);
            vk::ResetCommandBuffer(cmd_buffer[frame], vk::COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);
            vk::BeginCommandBuffer(cmd_buffer[frame], &vk::CommandBufferBeginInfo::new().flags(vk::COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT));

            let clear_values = [vk::ClearValue::new().color([0.4, 0.4, 0.5, 1.]), vk::ClearValue::depth(1., 0.) ];
            let pass_info =  vk::RenderPassBeginInfo::new()
                .renderPass(renderpass)
                .framebuffer(framebuffer[frame])
                .renderArea(vk::Rect2D::new().extent(win.extent()))
                .clearValueCount(2)
                .pClearValues(&clear_values as _);

            vk::CmdBeginRenderPass(cmd_buffer[frame], &pass_info, vk::SUBPASS_CONTENTS_INLINE);

            vk::CmdBindPipeline(cmd_buffer[frame], vk::PIPELINE_BIND_POINT_GRAPHICS, pipeline);
            let offset = 0;
            vk::CmdBindVertexBuffers(cmd_buffer[frame], 0, 1, &buffer.0, &offset);
            vk::CmdDraw(cmd_buffer[frame], 6, 1, 0, 0);
            vk::CmdEndRenderPass(cmd_buffer[frame]);
            vk::EndCommandBuffer(cmd_buffer[frame]);

            let submit_info = vk::SubmitInfo::new()
                .waitSemaphoreCount(1)
                .signalSemaphoreCount(1)
                .commandBufferCount(1)
                .pCommandBuffers(&cmd_buffer[frame])
                .pWaitDstStageMask(&vk::PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT)
                .pWaitSemaphores(&acquire_semaphore[frame])
                .pSignalSemaphores(&present_semaphore[frame]);

            vk::QueueSubmit(1, &submit_info, fence[frame]);

            let present_info = vk::PresentInfoKHR::new()
                .waitSemaphoreCount(1)
                .pWaitSemaphores(&present_semaphore[frame])
                .swapchainCount(1)
                .pSwapchains(&swapchain)
                .pImageIndices(&counter);
            vk::QueuePresentKHR(&present_info);
        }
        
        println!("Success!");
    }
}
