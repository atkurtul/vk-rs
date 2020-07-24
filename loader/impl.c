#include "loader.h"
#include "vulkan/vulkan_core.h"

VkInstance			instance;
VkDevice			device;
VkPhysicalDevice	physicalDevice;
uint				memory_idx[2];
VkQueue				queue;
VkCommandPool       transient_pool;

VkResult CreateInstance(
    const VkInstanceCreateInfo*                 pCreateInfo) {
        load_fn();
        VkResult result = vkCreateInstance(pCreateInfo, 0, &instance);
        load_instance_fn(instance);
        return result;
}

void SetPhysicalDevice(VkPhysicalDevice pdev) {
    physicalDevice = pdev;
}

VkResult CreateDevice(
    const VkDeviceCreateInfo*                   pCreateInfo) {
        VkResult result =  vkCreateDevice(physicalDevice, pCreateInfo, 0, &device);
        load_device_fn(device);
        return result;
}

void DestroyInstance() {
        vkDestroyInstance(instance, 0);
}

VkResult EnumeratePhysicalDevices(
    uint32_t*                                   pPhysicalDeviceCount,
    VkPhysicalDevice*                           pPhysicalDevices) {
        return vkEnumeratePhysicalDevices(instance, pPhysicalDeviceCount, pPhysicalDevices);
}

void GetPhysicalDeviceFeatures(
    VkPhysicalDeviceFeatures*                   pFeatures) {
        vkGetPhysicalDeviceFeatures(physicalDevice, pFeatures);
}

void GetPhysicalDeviceFormatProperties(
    VkFormat                                    format,
    VkFormatProperties*                         pFormatProperties) {
        vkGetPhysicalDeviceFormatProperties(physicalDevice, format, pFormatProperties);
}

VkResult GetPhysicalDeviceImageFormatProperties(
    VkFormat                                    format,
    VkImageType                                 type,
    VkImageTiling                               tiling,
    VkImageUsageFlags                           usage,
    VkImageCreateFlags                          flags,
    VkImageFormatProperties*                    pImageFormatProperties) {
        return vkGetPhysicalDeviceImageFormatProperties(physicalDevice, format, type, tiling, usage, flags, pImageFormatProperties);
}

void GetPhysicalDeviceProperties(
    VkPhysicalDeviceProperties*                 pProperties) {
        vkGetPhysicalDeviceProperties(physicalDevice, pProperties);
}

void GetPhysicalDeviceQueueFamilyProperties(
    uint32_t*                                   pQueueFamilyPropertyCount,
    VkQueueFamilyProperties*                    pQueueFamilyProperties) {
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties);
}

void GetPhysicalDeviceMemoryProperties(
    VkPhysicalDeviceMemoryProperties*           pMemoryProperties) {
        vkGetPhysicalDeviceMemoryProperties(physicalDevice, pMemoryProperties);
}

PFN_vkVoidFunction GetInstanceProcAddr(
    const char*                                 pName) {
        return vkGetInstanceProcAddr(instance, pName);
}

PFN_vkVoidFunction GetDeviceProcAddr(
    const char*                                 pName) {
        return vkGetDeviceProcAddr(device, pName);
}

void DestroyDevice() {
        vkDestroyDevice(device, 0);
}

VkResult EnumerateInstanceExtensionProperties(
    const char*                                 pLayerName,
    uint32_t*                                   pPropertyCount,
    VkExtensionProperties*                      pProperties) {
        return vkEnumerateInstanceExtensionProperties(pLayerName, pPropertyCount, pProperties);
}

VkResult EnumerateDeviceExtensionProperties(
    const char*                                 pLayerName,
    uint32_t*                                   pPropertyCount,
    VkExtensionProperties*                      pProperties) {
        return vkEnumerateDeviceExtensionProperties(physicalDevice, pLayerName, pPropertyCount, pProperties);
}

VkResult EnumerateInstanceLayerProperties(
    uint32_t*                                   pPropertyCount,
    VkLayerProperties*                          pProperties) {
        return vkEnumerateInstanceLayerProperties(pPropertyCount, pProperties);
}

VkResult EnumerateDeviceLayerProperties(
    uint32_t*                                   pPropertyCount,
    VkLayerProperties*                          pProperties) {
        return vkEnumerateDeviceLayerProperties(physicalDevice, pPropertyCount, pProperties);
}

void GetDeviceQueue(
    uint32_t                                    queueFamilyIndex,
    uint32_t                                    queueIndex,
    VkQueue*                                    pQueue) {
        vkGetDeviceQueue(device, queueFamilyIndex, queueIndex, pQueue);
}

void SetDeviceQueue(    
    uint32_t                                    queueFamilyIndex,
    uint32_t                                    queueIndex) {
    GetDeviceQueue(queueFamilyIndex, queueIndex, &queue);
}

VkResult QueueSubmit(
    uint32_t                                    submitCount,
    const VkSubmitInfo*                         pSubmits,
    VkFence                                     fence) {
        return vkQueueSubmit(queue, submitCount, pSubmits, fence);
}

VkResult QueueWaitIdle(
    VkQueue                                     queue) {
        return vkQueueWaitIdle(queue);
}

VkResult DeviceWaitIdle() {
        return vkDeviceWaitIdle(device);
}

VkResult AllocateMemory(
    const VkMemoryAllocateInfo*                 pAllocateInfo,
    VkDeviceMemory*                             pMemory) {
        return vkAllocateMemory(device, pAllocateInfo, 0, pMemory);
}

void FreeMemory(
    VkDeviceMemory                              memory) {
        vkFreeMemory(device, memory, 0);
}

VkResult MapMemory(
    VkDeviceMemory                              memory,
    VkDeviceSize                                offset,
    VkDeviceSize                                size,
    VkMemoryMapFlags                            flags,
    void**                                      ppData) {
        return vkMapMemory(device, memory, offset, size, flags, ppData);
}

void UnmapMemory(
    VkDeviceMemory                              memory) {
        vkUnmapMemory(device, memory);
}

VkResult FlushMappedMemoryRanges(
    uint32_t                                    memoryRangeCount,
    const VkMappedMemoryRange*                  pMemoryRanges) {
        return vkFlushMappedMemoryRanges(device, memoryRangeCount, pMemoryRanges);
}

VkResult InvalidateMappedMemoryRanges(
    uint32_t                                    memoryRangeCount,
    const VkMappedMemoryRange*                  pMemoryRanges) {
        return vkInvalidateMappedMemoryRanges(device, memoryRangeCount, pMemoryRanges);
}

void GetDeviceMemoryCommitment(
    VkDeviceMemory                              memory,
    VkDeviceSize*                               pCommittedMemoryInBytes) {
        vkGetDeviceMemoryCommitment(device, memory, pCommittedMemoryInBytes);
}

VkResult BindBufferMemory(
    VkBuffer                                    buffer,
    VkDeviceMemory                              memory,
    VkDeviceSize                                memoryOffset) {
        return vkBindBufferMemory(device, buffer, memory, memoryOffset);
}

VkResult BindImageMemory(
    VkImage                                     image,
    VkDeviceMemory                              memory,
    VkDeviceSize                                memoryOffset) {
        return vkBindImageMemory(device, image, memory, memoryOffset);
}

void GetBufferMemoryRequirements(
    VkBuffer                                    buffer,
    VkMemoryRequirements*                       pMemoryRequirements) {
        vkGetBufferMemoryRequirements(device, buffer, pMemoryRequirements);
}

void GetImageMemoryRequirements(
    VkImage                                     image,
    VkMemoryRequirements*                       pMemoryRequirements) {
        vkGetImageMemoryRequirements(device, image, pMemoryRequirements);
}

void GetImageSparseMemoryRequirements(
    VkImage                                     image,
    uint32_t*                                   pSparseMemoryRequirementCount,
    VkSparseImageMemoryRequirements*            pSparseMemoryRequirements) {
        vkGetImageSparseMemoryRequirements(device, image, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
}

void GetPhysicalDeviceSparseImageFormatProperties(
    VkFormat                                    format,
    VkImageType                                 type,
    VkSampleCountFlagBits                       samples,
    VkImageUsageFlags                           usage,
    VkImageTiling                               tiling,
    uint32_t*                                   pPropertyCount,
    VkSparseImageFormatProperties*              pProperties) {
        vkGetPhysicalDeviceSparseImageFormatProperties(physicalDevice, format, type, samples, usage, tiling, pPropertyCount, pProperties);
}

VkResult QueueBindSparse(
    VkQueue                                     queue,
    uint32_t                                    bindInfoCount,
    const VkBindSparseInfo*                     pBindInfo,
    VkFence                                     fence) {
        return vkQueueBindSparse(queue, bindInfoCount, pBindInfo, fence);
}

VkResult CreateFence(
    const VkFenceCreateInfo*                    pCreateInfo,
    VkFence*                                    pFence) {
        return vkCreateFence(device, pCreateInfo, 0, pFence);
}

void DestroyFence(
    VkFence                                     fence) {
        vkDestroyFence(device, fence, 0);
}

VkResult ResetFences(
    uint32_t                                    fenceCount,
    const VkFence*                              pFences) {
        return vkResetFences(device, fenceCount, pFences);
}

VkResult GetFenceStatus(
    VkFence                                     fence) {
        return vkGetFenceStatus(device, fence);
}

VkResult WaitForFences(
    uint32_t                                    fenceCount,
    const VkFence*                              pFences,
    VkBool32                                    waitAll,
    uint64_t                                    timeout) {
        return vkWaitForFences(device, fenceCount, pFences, waitAll, timeout);
}

VkResult CreateSemaphore(
    const VkSemaphoreCreateInfo*                pCreateInfo,
    VkSemaphore*                                pSemaphore) {
        return vkCreateSemaphore(device, pCreateInfo, 0, pSemaphore);
}

void DestroySemaphore(
    VkSemaphore                                 semaphore) {
        vkDestroySemaphore(device, semaphore, 0);
}

VkResult CreateEvent(
    const VkEventCreateInfo*                    pCreateInfo,
    VkEvent*                                    pEvent) {
        return vkCreateEvent(device, pCreateInfo, 0, pEvent);
}

void DestroyEvent(
    VkEvent                                     event) {
        vkDestroyEvent(device, event, 0);
}

VkResult GetEventStatus(
    VkEvent                                     event) {
        return vkGetEventStatus(device, event);
}

VkResult SetEvent(
    VkEvent                                     event) {
        return vkSetEvent(device, event);
}

VkResult ResetEvent(
    VkEvent                                     event) {
        return vkResetEvent(device, event);
}

VkResult CreateQueryPool(
    const VkQueryPoolCreateInfo*                pCreateInfo,
    VkQueryPool*                                pQueryPool) {
        return vkCreateQueryPool(device, pCreateInfo, 0, pQueryPool);
}

void DestroyQueryPool(
    VkQueryPool                                 queryPool) {
        vkDestroyQueryPool(device, queryPool, 0);
}

VkResult GetQueryPoolResults(
    VkQueryPool                                 queryPool,
    uint32_t                                    firstQuery,
    uint32_t                                    queryCount,
    size_t                                      dataSize,
    void*                                       pData,
    VkDeviceSize                                stride,
    VkQueryResultFlags                          flags) {
        return vkGetQueryPoolResults(device, queryPool, firstQuery, queryCount, dataSize, pData, stride, flags);
}

VkResult CreateBuffer(
    const VkBufferCreateInfo*                   pCreateInfo,
    VkBuffer*                                   pBuffer) {
        return vkCreateBuffer(device, pCreateInfo, 0, pBuffer);
}

void DestroyBuffer(
    VkBuffer                                    buffer) {
        vkDestroyBuffer(device, buffer, 0);
}

VkResult CreateBufferView(
    const VkBufferViewCreateInfo*               pCreateInfo,
    VkBufferView*                               pView) {
        return vkCreateBufferView(device, pCreateInfo, 0, pView);
}

void DestroyBufferView(
    VkBufferView                                bufferView) {
        vkDestroyBufferView(device, bufferView, 0);
}

VkResult CreateImage(
    const VkImageCreateInfo*                    pCreateInfo,
    VkImage*                                    pImage) {
        return vkCreateImage(device, pCreateInfo, 0, pImage);
}

void DestroyImage(
    VkImage                                     image) {
        vkDestroyImage(device, image, 0);
}

void GetImageSubresourceLayout(
    VkImage                                     image,
    const VkImageSubresource*                   pSubresource,
    VkSubresourceLayout*                        pLayout) {
        vkGetImageSubresourceLayout(device, image, pSubresource, pLayout);
}

VkResult CreateImageView(
    const VkImageViewCreateInfo*                pCreateInfo,
    VkImageView*                                pView) {
        return vkCreateImageView(device, pCreateInfo, 0, pView);
}

void DestroyImageView(
    VkImageView                                 imageView) {
        vkDestroyImageView(device, imageView, 0);
}

VkResult CreateShaderModule(
    const VkShaderModuleCreateInfo*             pCreateInfo,
    VkShaderModule*                             pShaderModule) {
        return vkCreateShaderModule(device, pCreateInfo, 0, pShaderModule);
}

void DestroyShaderModule(
    VkShaderModule                              shaderModule) {
        vkDestroyShaderModule(device, shaderModule, 0);
}

VkResult CreatePipelineCache(
    const VkPipelineCacheCreateInfo*            pCreateInfo,
    VkPipelineCache*                            pPipelineCache) {
        return vkCreatePipelineCache(device, pCreateInfo, 0, pPipelineCache);
}

void DestroyPipelineCache(
    VkPipelineCache                             pipelineCache) {
        vkDestroyPipelineCache(device, pipelineCache, 0);
}

VkResult GetPipelineCacheData(
    VkPipelineCache                             pipelineCache,
    size_t*                                     pDataSize,
    void*                                       pData) {
        return vkGetPipelineCacheData(device, pipelineCache, pDataSize, pData);
}

VkResult MergePipelineCaches(
    VkPipelineCache                             dstCache,
    uint32_t                                    srcCacheCount,
    const VkPipelineCache*                      pSrcCaches) {
        return vkMergePipelineCaches(device, dstCache, srcCacheCount, pSrcCaches);
}

VkResult CreateGraphicsPipelines(
    VkPipelineCache                             pipelineCache,
    uint32_t                                    createInfoCount,
    const VkGraphicsPipelineCreateInfo*         pCreateInfos,
    VkPipeline*                                 pPipelines) {
        return vkCreateGraphicsPipelines(device, pipelineCache, createInfoCount, pCreateInfos, 0, pPipelines);
}

VkResult CreateComputePipelines(
    VkPipelineCache                             pipelineCache,
    uint32_t                                    createInfoCount,
    const VkComputePipelineCreateInfo*          pCreateInfos,
    VkPipeline*                                 pPipelines) {
        return vkCreateComputePipelines(device, pipelineCache, createInfoCount, pCreateInfos, 0, pPipelines);
}

void DestroyPipeline(
    VkPipeline                                  pipeline) {
        vkDestroyPipeline(device, pipeline, 0);
}

VkResult CreatePipelineLayout(
    const VkPipelineLayoutCreateInfo*           pCreateInfo,
    VkPipelineLayout*                           pPipelineLayout) {
        return vkCreatePipelineLayout(device, pCreateInfo, 0, pPipelineLayout);
}

void DestroyPipelineLayout(
    VkPipelineLayout                            pipelineLayout) {
        vkDestroyPipelineLayout(device, pipelineLayout, 0);
}

VkResult CreateSampler(
    const VkSamplerCreateInfo*                  pCreateInfo,
    VkSampler*                                  pSampler) {
        return vkCreateSampler(device, pCreateInfo, 0, pSampler);
}

void DestroySampler(
    VkSampler                                   sampler) {
        vkDestroySampler(device, sampler, 0);
}

VkResult CreateDescriptorSetLayout(
    const VkDescriptorSetLayoutCreateInfo*      pCreateInfo,
    VkDescriptorSetLayout*                      pSetLayout) {
        return vkCreateDescriptorSetLayout(device, pCreateInfo, 0, pSetLayout);
}

void DestroyDescriptorSetLayout(
    VkDescriptorSetLayout                       descriptorSetLayout) {
        vkDestroyDescriptorSetLayout(device, descriptorSetLayout, 0);
}

VkResult CreateDescriptorPool(
    const VkDescriptorPoolCreateInfo*           pCreateInfo,
    VkDescriptorPool*                           pDescriptorPool) {
        return vkCreateDescriptorPool(device, pCreateInfo, 0, pDescriptorPool);
}

void DestroyDescriptorPool(
    VkDescriptorPool                            descriptorPool) {
        vkDestroyDescriptorPool(device, descriptorPool, 0);
}

VkResult ResetDescriptorPool(
    VkDescriptorPool                            descriptorPool,
    VkDescriptorPoolResetFlags                  flags) {
        return vkResetDescriptorPool(device, descriptorPool, flags);
}

VkResult AllocateDescriptorSets(
    const VkDescriptorSetAllocateInfo*          pAllocateInfo,
    VkDescriptorSet*                            pDescriptorSets) {
        return vkAllocateDescriptorSets(device, pAllocateInfo, pDescriptorSets);
}

VkResult FreeDescriptorSets(
    VkDescriptorPool                            descriptorPool,
    uint32_t                                    descriptorSetCount,
    const VkDescriptorSet*                      pDescriptorSets) {
        return vkFreeDescriptorSets(device, descriptorPool, descriptorSetCount, pDescriptorSets);
}

void UpdateDescriptorSets(
    uint32_t                                    descriptorWriteCount,
    const VkWriteDescriptorSet*                 pDescriptorWrites,
    uint32_t                                    descriptorCopyCount,
    const VkCopyDescriptorSet*                  pDescriptorCopies) {
        vkUpdateDescriptorSets(device, descriptorWriteCount, pDescriptorWrites, descriptorCopyCount, pDescriptorCopies);
}

VkResult CreateFramebuffer(
    const VkFramebufferCreateInfo*              pCreateInfo,
    VkFramebuffer*                              pFramebuffer) {
        return vkCreateFramebuffer(device, pCreateInfo, 0, pFramebuffer);
}

void DestroyFramebuffer(
    VkFramebuffer                               framebuffer) {
        vkDestroyFramebuffer(device, framebuffer, 0);
}

VkResult CreateRenderPass(
    const VkRenderPassCreateInfo*               pCreateInfo,
    VkRenderPass*                               pRenderPass) {
        return vkCreateRenderPass(device, pCreateInfo, 0, pRenderPass);
}

void DestroyRenderPass(
    VkRenderPass                                renderPass) {
        vkDestroyRenderPass(device, renderPass, 0);
}

void GetRenderAreaGranularity(
    VkRenderPass                                renderPass,
    VkExtent2D*                                 pGranularity) {
        vkGetRenderAreaGranularity(device, renderPass, pGranularity);
}

VkResult CreateCommandPool(
    const VkCommandPoolCreateInfo*              pCreateInfo,
    VkCommandPool*                              pCommandPool) {
        return vkCreateCommandPool(device, pCreateInfo, 0, pCommandPool);
}

void DestroyCommandPool(
    VkCommandPool                               commandPool) {
        vkDestroyCommandPool(device, commandPool, 0);
}

VkResult ResetCommandPool(
    VkCommandPool                               commandPool,
    VkCommandPoolResetFlags                     flags) {
        return vkResetCommandPool(device, commandPool, flags);
}

VkResult AllocateCommandBuffers(
    const VkCommandBufferAllocateInfo*          pAllocateInfo,
    VkCommandBuffer*                            pCommandBuffers) {
        return vkAllocateCommandBuffers(device, pAllocateInfo, pCommandBuffers);
}

void FreeCommandBuffers(
    VkCommandPool                               commandPool,
    uint32_t                                    commandBufferCount,
    const VkCommandBuffer*                      pCommandBuffers) {
        vkFreeCommandBuffers(device, commandPool, commandBufferCount, pCommandBuffers);
}

VkResult BeginCommandBuffer(
    VkCommandBuffer                             commandBuffer,
    const VkCommandBufferBeginInfo*             pBeginInfo) {
        return vkBeginCommandBuffer(commandBuffer, pBeginInfo);
}

VkResult EndCommandBuffer(
    VkCommandBuffer                             commandBuffer) {
        return vkEndCommandBuffer(commandBuffer);
}

VkResult ResetCommandBuffer(
    VkCommandBuffer                             commandBuffer,
    VkCommandBufferResetFlags                   flags) {
        return vkResetCommandBuffer(commandBuffer, flags);
}

void CmdBindPipeline(
    VkCommandBuffer                             commandBuffer,
    VkPipelineBindPoint                         pipelineBindPoint,
    VkPipeline                                  pipeline) {
        vkCmdBindPipeline(commandBuffer, pipelineBindPoint, pipeline);
}

void CmdSetViewport(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    firstViewport,
    uint32_t                                    viewportCount,
    const VkViewport*                           pViewports) {
        vkCmdSetViewport(commandBuffer, firstViewport, viewportCount, pViewports);
}

void CmdSetScissor(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    firstScissor,
    uint32_t                                    scissorCount,
    const VkRect2D*                             pScissors) {
        vkCmdSetScissor(commandBuffer, firstScissor, scissorCount, pScissors);
}

void CmdSetLineWidth(
    VkCommandBuffer                             commandBuffer,
    float                                       lineWidth) {
        vkCmdSetLineWidth(commandBuffer, lineWidth);
}

void CmdSetDepthBias(
    VkCommandBuffer                             commandBuffer,
    float                                       depthBiasConstantFactor,
    float                                       depthBiasClamp,
    float                                       depthBiasSlopeFactor) {
        vkCmdSetDepthBias(commandBuffer, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor);
}

void CmdSetBlendConstants(
    VkCommandBuffer                             commandBuffer,
    const float                                 blendConstants[4]) {
        vkCmdSetBlendConstants(commandBuffer, blendConstants);
}

void CmdSetDepthBounds(
    VkCommandBuffer                             commandBuffer,
    float                                       minDepthBounds,
    float                                       maxDepthBounds) {
        vkCmdSetDepthBounds(commandBuffer, minDepthBounds, maxDepthBounds);
}

void CmdSetStencilCompareMask(
    VkCommandBuffer                             commandBuffer,
    VkStencilFaceFlags                          faceMask,
    uint32_t                                    compareMask) {
        vkCmdSetStencilCompareMask(commandBuffer, faceMask, compareMask);
}

void CmdSetStencilWriteMask(
    VkCommandBuffer                             commandBuffer,
    VkStencilFaceFlags                          faceMask,
    uint32_t                                    writeMask) {
        vkCmdSetStencilWriteMask(commandBuffer, faceMask, writeMask);
}

void CmdSetStencilReference(
    VkCommandBuffer                             commandBuffer,
    VkStencilFaceFlags                          faceMask,
    uint32_t                                    reference) {
        vkCmdSetStencilReference(commandBuffer, faceMask, reference);
}

void CmdBindDescriptorSets(
    VkCommandBuffer                             commandBuffer,
    VkPipelineBindPoint                         pipelineBindPoint,
    VkPipelineLayout                            layout,
    uint32_t                                    firstSet,
    uint32_t                                    descriptorSetCount,
    const VkDescriptorSet*                      pDescriptorSets,
    uint32_t                                    dynamicOffsetCount,
    const uint32_t*                             pDynamicOffsets) {
        vkCmdBindDescriptorSets(commandBuffer, pipelineBindPoint, layout, firstSet, descriptorSetCount, pDescriptorSets, dynamicOffsetCount, pDynamicOffsets);
}

void CmdBindIndexBuffer(
    VkCommandBuffer                             commandBuffer,
    VkBuffer                                    buffer,
    VkDeviceSize                                offset,
    VkIndexType                                 indexType) {
        vkCmdBindIndexBuffer(commandBuffer, buffer, offset, indexType);
}

void CmdBindVertexBuffers(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    firstBinding,
    uint32_t                                    bindingCount,
    const VkBuffer*                             pBuffers,
    const VkDeviceSize*                         pOffsets) {
        vkCmdBindVertexBuffers(commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets);
}

void CmdDraw(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    vertexCount,
    uint32_t                                    instanceCount,
    uint32_t                                    firstVertex,
    uint32_t                                    firstInstance) {
        vkCmdDraw(commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
}

void CmdDrawIndexed(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    indexCount,
    uint32_t                                    instanceCount,
    uint32_t                                    firstIndex,
    int32_t                                     vertexOffset,
    uint32_t                                    firstInstance) {
        vkCmdDrawIndexed(commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
}

void CmdDrawIndirect(
    VkCommandBuffer                             commandBuffer,
    VkBuffer                                    buffer,
    VkDeviceSize                                offset,
    uint32_t                                    drawCount,
    uint32_t                                    stride) {
        vkCmdDrawIndirect(commandBuffer, buffer, offset, drawCount, stride);
}

void CmdDrawIndexedIndirect(
    VkCommandBuffer                             commandBuffer,
    VkBuffer                                    buffer,
    VkDeviceSize                                offset,
    uint32_t                                    drawCount,
    uint32_t                                    stride) {
        vkCmdDrawIndexedIndirect(commandBuffer, buffer, offset, drawCount, stride);
}

void CmdDispatch(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    groupCountX,
    uint32_t                                    groupCountY,
    uint32_t                                    groupCountZ) {
        vkCmdDispatch(commandBuffer, groupCountX, groupCountY, groupCountZ);
}

void CmdDispatchIndirect(
    VkCommandBuffer                             commandBuffer,
    VkBuffer                                    buffer,
    VkDeviceSize                                offset) {
        vkCmdDispatchIndirect(commandBuffer, buffer, offset);
}

void CmdCopyBuffer(
    VkCommandBuffer                             commandBuffer,
    VkBuffer                                    srcBuffer,
    VkBuffer                                    dstBuffer,
    uint32_t                                    regionCount,
    const VkBufferCopy*                         pRegions) {
        vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, regionCount, pRegions);
}

void CmdCopyImage(
    VkCommandBuffer                             commandBuffer,
    VkImage                                     srcImage,
    VkImageLayout                               srcImageLayout,
    VkImage                                     dstImage,
    VkImageLayout                               dstImageLayout,
    uint32_t                                    regionCount,
    const VkImageCopy*                          pRegions) {
        vkCmdCopyImage(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
}

void CmdBlitImage(
    VkCommandBuffer                             commandBuffer,
    VkImage                                     srcImage,
    VkImageLayout                               srcImageLayout,
    VkImage                                     dstImage,
    VkImageLayout                               dstImageLayout,
    uint32_t                                    regionCount,
    const VkImageBlit*                          pRegions,
    VkFilter                                    filter) {
        vkCmdBlitImage(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions, filter);
}

void CmdCopyBufferToImage(
    VkCommandBuffer                             commandBuffer,
    VkBuffer                                    srcBuffer,
    VkImage                                     dstImage,
    VkImageLayout                               dstImageLayout,
    uint32_t                                    regionCount,
    const VkBufferImageCopy*                    pRegions) {
        vkCmdCopyBufferToImage(commandBuffer, srcBuffer, dstImage, dstImageLayout, regionCount, pRegions);
}

void CmdCopyImageToBuffer(
    VkCommandBuffer                             commandBuffer,
    VkImage                                     srcImage,
    VkImageLayout                               srcImageLayout,
    VkBuffer                                    dstBuffer,
    uint32_t                                    regionCount,
    const VkBufferImageCopy*                    pRegions) {
        vkCmdCopyImageToBuffer(commandBuffer, srcImage, srcImageLayout, dstBuffer, regionCount, pRegions);
}

void CmdUpdateBuffer(
    VkCommandBuffer                             commandBuffer,
    VkBuffer                                    dstBuffer,
    VkDeviceSize                                dstOffset,
    VkDeviceSize                                dataSize,
    const void*                                 pData) {
        vkCmdUpdateBuffer(commandBuffer, dstBuffer, dstOffset, dataSize, pData);
}

void CmdFillBuffer(
    VkCommandBuffer                             commandBuffer,
    VkBuffer                                    dstBuffer,
    VkDeviceSize                                dstOffset,
    VkDeviceSize                                size,
    uint32_t                                    data) {
        vkCmdFillBuffer(commandBuffer, dstBuffer, dstOffset, size, data);
}

void CmdClearColorImage(
    VkCommandBuffer                             commandBuffer,
    VkImage                                     image,
    VkImageLayout                               imageLayout,
    const VkClearColorValue*                    pColor,
    uint32_t                                    rangeCount,
    const VkImageSubresourceRange*              pRanges) {
        vkCmdClearColorImage(commandBuffer, image, imageLayout, pColor, rangeCount, pRanges);
}

void CmdClearDepthStencilImage(
    VkCommandBuffer                             commandBuffer,
    VkImage                                     image,
    VkImageLayout                               imageLayout,
    const VkClearDepthStencilValue*             pDepthStencil,
    uint32_t                                    rangeCount,
    const VkImageSubresourceRange*              pRanges) {
        vkCmdClearDepthStencilImage(commandBuffer, image, imageLayout, pDepthStencil, rangeCount, pRanges);
}

void CmdClearAttachments(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    attachmentCount,
    const VkClearAttachment*                    pAttachments,
    uint32_t                                    rectCount,
    const VkClearRect*                          pRects) {
        vkCmdClearAttachments(commandBuffer, attachmentCount, pAttachments, rectCount, pRects);
}

void CmdResolveImage(
    VkCommandBuffer                             commandBuffer,
    VkImage                                     srcImage,
    VkImageLayout                               srcImageLayout,
    VkImage                                     dstImage,
    VkImageLayout                               dstImageLayout,
    uint32_t                                    regionCount,
    const VkImageResolve*                       pRegions) {
        vkCmdResolveImage(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
}

void CmdSetEvent(
    VkCommandBuffer                             commandBuffer,
    VkEvent                                     event,
    VkPipelineStageFlags                        stageMask) {
        vkCmdSetEvent(commandBuffer, event, stageMask);
}

void CmdResetEvent(
    VkCommandBuffer                             commandBuffer,
    VkEvent                                     event,
    VkPipelineStageFlags                        stageMask) {
        vkCmdResetEvent(commandBuffer, event, stageMask);
}

void CmdWaitEvents(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    eventCount,
    const VkEvent*                              pEvents,
    VkPipelineStageFlags                        srcStageMask,
    VkPipelineStageFlags                        dstStageMask,
    uint32_t                                    memoryBarrierCount,
    const VkMemoryBarrier*                      pMemoryBarriers,
    uint32_t                                    bufferMemoryBarrierCount,
    const VkBufferMemoryBarrier*                pBufferMemoryBarriers,
    uint32_t                                    imageMemoryBarrierCount,
    const VkImageMemoryBarrier*                 pImageMemoryBarriers) {
        vkCmdWaitEvents(commandBuffer, eventCount, pEvents, srcStageMask, dstStageMask, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
}

void CmdPipelineBarrier(
    VkCommandBuffer                             commandBuffer,
    VkPipelineStageFlags                        srcStageMask,
    VkPipelineStageFlags                        dstStageMask,
    VkDependencyFlags                           dependencyFlags,
    uint32_t                                    memoryBarrierCount,
    const VkMemoryBarrier*                      pMemoryBarriers,
    uint32_t                                    bufferMemoryBarrierCount,
    const VkBufferMemoryBarrier*                pBufferMemoryBarriers,
    uint32_t                                    imageMemoryBarrierCount,
    const VkImageMemoryBarrier*                 pImageMemoryBarriers) {
        vkCmdPipelineBarrier(commandBuffer, srcStageMask, dstStageMask, dependencyFlags, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
}

void CmdBeginQuery(
    VkCommandBuffer                             commandBuffer,
    VkQueryPool                                 queryPool,
    uint32_t                                    query,
    VkQueryControlFlags                         flags) {
        vkCmdBeginQuery(commandBuffer, queryPool, query, flags);
}

void CmdEndQuery(
    VkCommandBuffer                             commandBuffer,
    VkQueryPool                                 queryPool,
    uint32_t                                    query) {
        vkCmdEndQuery(commandBuffer, queryPool, query);
}

void CmdResetQueryPool(
    VkCommandBuffer                             commandBuffer,
    VkQueryPool                                 queryPool,
    uint32_t                                    firstQuery,
    uint32_t                                    queryCount) {
        vkCmdResetQueryPool(commandBuffer, queryPool, firstQuery, queryCount);
}

void CmdWriteTimestamp(
    VkCommandBuffer                             commandBuffer,
    VkPipelineStageFlagBits                     pipelineStage,
    VkQueryPool                                 queryPool,
    uint32_t                                    query) {
        vkCmdWriteTimestamp(commandBuffer, pipelineStage, queryPool, query);
}

void CmdCopyQueryPoolResults(
    VkCommandBuffer                             commandBuffer,
    VkQueryPool                                 queryPool,
    uint32_t                                    firstQuery,
    uint32_t                                    queryCount,
    VkBuffer                                    dstBuffer,
    VkDeviceSize                                dstOffset,
    VkDeviceSize                                stride,
    VkQueryResultFlags                          flags) {
        vkCmdCopyQueryPoolResults(commandBuffer, queryPool, firstQuery, queryCount, dstBuffer, dstOffset, stride, flags);
}

void CmdPushConstants(
    VkCommandBuffer                             commandBuffer,
    VkPipelineLayout                            layout,
    VkShaderStageFlags                          stageFlags,
    uint32_t                                    offset,
    uint32_t                                    size,
    const void*                                 pValues) {
        vkCmdPushConstants(commandBuffer, layout, stageFlags, offset, size, pValues);
}

void CmdBeginRenderPass(
    VkCommandBuffer                             commandBuffer,
    const VkRenderPassBeginInfo*                pRenderPassBegin,
    VkSubpassContents                           contents) {
        vkCmdBeginRenderPass(commandBuffer, pRenderPassBegin, contents);
}

void CmdNextSubpass(
    VkCommandBuffer                             commandBuffer,
    VkSubpassContents                           contents) {
        vkCmdNextSubpass(commandBuffer, contents);
}

void CmdEndRenderPass(
    VkCommandBuffer                             commandBuffer) {
        vkCmdEndRenderPass(commandBuffer);
}

void CmdExecuteCommands(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    commandBufferCount,
    const VkCommandBuffer*                      pCommandBuffers) {
        vkCmdExecuteCommands(commandBuffer, commandBufferCount, pCommandBuffers);
}

VkResult EnumerateInstanceVersion(
    uint32_t*                                   pApiVersion) {
        return vkEnumerateInstanceVersion(pApiVersion);
}

VkResult BindBufferMemory2(
    uint32_t                                    bindInfoCount,
    const VkBindBufferMemoryInfo*               pBindInfos) {
        return vkBindBufferMemory2(device, bindInfoCount, pBindInfos);
}

VkResult BindImageMemory2(
    uint32_t                                    bindInfoCount,
    const VkBindImageMemoryInfo*                pBindInfos) {
        return vkBindImageMemory2(device, bindInfoCount, pBindInfos);
}

void GetDeviceGroupPeerMemoryFeatures(
    uint32_t                                    heapIndex,
    uint32_t                                    localDeviceIndex,
    uint32_t                                    remoteDeviceIndex,
    VkPeerMemoryFeatureFlags*                   pPeerMemoryFeatures) {
        vkGetDeviceGroupPeerMemoryFeatures(device, heapIndex, localDeviceIndex, remoteDeviceIndex, pPeerMemoryFeatures);
}

void CmdSetDeviceMask(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    deviceMask) {
        vkCmdSetDeviceMask(commandBuffer, deviceMask);
}

void CmdDispatchBase(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    baseGroupX,
    uint32_t                                    baseGroupY,
    uint32_t                                    baseGroupZ,
    uint32_t                                    groupCountX,
    uint32_t                                    groupCountY,
    uint32_t                                    groupCountZ) {
        return vkCmdDispatchBase(commandBuffer, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ);
}

VkResult EnumeratePhysicalDeviceGroups(
    uint32_t*                                   pPhysicalDeviceGroupCount,
    VkPhysicalDeviceGroupProperties*            pPhysicalDeviceGroupProperties) {
        return vkEnumeratePhysicalDeviceGroups(instance, pPhysicalDeviceGroupCount, pPhysicalDeviceGroupProperties);
}

void GetImageMemoryRequirements2(
    const VkImageMemoryRequirementsInfo2*       pInfo,
    VkMemoryRequirements2*                      pMemoryRequirements) {
        return vkGetImageMemoryRequirements2(device, pInfo, pMemoryRequirements);
}

void GetBufferMemoryRequirements2(
    const VkBufferMemoryRequirementsInfo2*      pInfo,
    VkMemoryRequirements2*                      pMemoryRequirements) {
        return vkGetBufferMemoryRequirements2(device, pInfo, pMemoryRequirements);
}

void GetImageSparseMemoryRequirements2(
    const VkImageSparseMemoryRequirementsInfo2* pInfo,
    uint32_t*                                   pSparseMemoryRequirementCount,
    VkSparseImageMemoryRequirements2*           pSparseMemoryRequirements) {
        return vkGetImageSparseMemoryRequirements2(device, pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
}

void GetPhysicalDeviceFeatures2(
    VkPhysicalDeviceFeatures2*                  pFeatures) {
        return vkGetPhysicalDeviceFeatures2(physicalDevice, pFeatures);
}

void GetPhysicalDeviceProperties2(
    VkPhysicalDeviceProperties2*                pProperties) {
        return vkGetPhysicalDeviceProperties2(physicalDevice, pProperties);
}

void GetPhysicalDeviceFormatProperties2(
    VkFormat                                    format,
    VkFormatProperties2*                        pFormatProperties) {
        return vkGetPhysicalDeviceFormatProperties2(physicalDevice, format, pFormatProperties);
}

VkResult GetPhysicalDeviceImageFormatProperties2(
    const VkPhysicalDeviceImageFormatInfo2*     pImageFormatInfo,
    VkImageFormatProperties2*                   pImageFormatProperties) {
        return vkGetPhysicalDeviceImageFormatProperties2(physicalDevice, pImageFormatInfo, pImageFormatProperties);
}

void GetPhysicalDeviceQueueFamilyProperties2(
    uint32_t*                                   pQueueFamilyPropertyCount,
    VkQueueFamilyProperties2*                   pQueueFamilyProperties) {
        return vkGetPhysicalDeviceQueueFamilyProperties2(physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties);
}

void GetPhysicalDeviceMemoryProperties2(
    VkPhysicalDeviceMemoryProperties2*          pMemoryProperties) {
        return vkGetPhysicalDeviceMemoryProperties2(physicalDevice, pMemoryProperties);
}

void GetPhysicalDeviceSparseImageFormatProperties2(
    const VkPhysicalDeviceSparseImageFormatInfo2* pFormatInfo,
    uint32_t*                                   pPropertyCount,
    VkSparseImageFormatProperties2*             pProperties) {
        return vkGetPhysicalDeviceSparseImageFormatProperties2(physicalDevice, pFormatInfo, pPropertyCount, pProperties);
}

void TrimCommandPool(
    VkCommandPool                               commandPool,
    VkCommandPoolTrimFlags                      flags) {
        return vkTrimCommandPool(device, commandPool, flags);
}

void GetDeviceQueue2(
    const VkDeviceQueueInfo2*                   pQueueInfo,
    VkQueue*                                    pQueue) {
        return vkGetDeviceQueue2(device, pQueueInfo, pQueue);
}

VkResult CreateSamplerYcbcrConversion(
    const VkSamplerYcbcrConversionCreateInfo*   pCreateInfo,
    VkSamplerYcbcrConversion*                   pYcbcrConversion) {
        return vkCreateSamplerYcbcrConversion(device, pCreateInfo, 0, pYcbcrConversion);
}

void DestroySamplerYcbcrConversion(
    VkSamplerYcbcrConversion                    ycbcrConversion) {
        return vkDestroySamplerYcbcrConversion(device, ycbcrConversion, 0);
}

VkResult CreateDescriptorUpdateTemplate(
    const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo,
    VkDescriptorUpdateTemplate*                 pDescriptorUpdateTemplate) {
        return vkCreateDescriptorUpdateTemplate(device, pCreateInfo, 0, pDescriptorUpdateTemplate);
}

void DestroyDescriptorUpdateTemplate(
    VkDescriptorUpdateTemplate                  descriptorUpdateTemplate) {
        return vkDestroyDescriptorUpdateTemplate(device, descriptorUpdateTemplate, 0);
}

void UpdateDescriptorSetWithTemplate(
    VkDescriptorSet                             descriptorSet,
    VkDescriptorUpdateTemplate                  descriptorUpdateTemplate,
    const void*                                 pData) {
        return vkUpdateDescriptorSetWithTemplate(device, descriptorSet, descriptorUpdateTemplate, pData);
}

void GetPhysicalDeviceExternalBufferProperties(
    const VkPhysicalDeviceExternalBufferInfo*   pExternalBufferInfo,
    VkExternalBufferProperties*                 pExternalBufferProperties) {
        return vkGetPhysicalDeviceExternalBufferProperties(physicalDevice, pExternalBufferInfo, pExternalBufferProperties);
}

void GetPhysicalDeviceExternalFenceProperties(
    const VkPhysicalDeviceExternalFenceInfo*    pExternalFenceInfo,
    VkExternalFenceProperties*                  pExternalFenceProperties) {
        return vkGetPhysicalDeviceExternalFenceProperties(physicalDevice, pExternalFenceInfo, pExternalFenceProperties);
}

void GetPhysicalDeviceExternalSemaphoreProperties(
    const VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo,
    VkExternalSemaphoreProperties*              pExternalSemaphoreProperties) {
        return vkGetPhysicalDeviceExternalSemaphoreProperties(physicalDevice, pExternalSemaphoreInfo, pExternalSemaphoreProperties);
}

void GetDescriptorSetLayoutSupport(
    const VkDescriptorSetLayoutCreateInfo*      pCreateInfo,
    VkDescriptorSetLayoutSupport*               pSupport) {
        return vkGetDescriptorSetLayoutSupport(device, pCreateInfo, pSupport);
}

void CmdDrawIndirectCount(
    VkCommandBuffer                             commandBuffer,
    VkBuffer                                    buffer,
    VkDeviceSize                                offset,
    VkBuffer                                    countBuffer,
    VkDeviceSize                                countBufferOffset,
    uint32_t                                    maxDrawCount,
    uint32_t                                    stride) {
        return vkCmdDrawIndirectCount(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}

void CmdDrawIndexedIndirectCount(
    VkCommandBuffer                             commandBuffer,
    VkBuffer                                    buffer,
    VkDeviceSize                                offset,
    VkBuffer                                    countBuffer,
    VkDeviceSize                                countBufferOffset,
    uint32_t                                    maxDrawCount,
    uint32_t                                    stride) {
        return vkCmdDrawIndexedIndirectCount(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}

VkResult CreateRenderPass2(
    const VkRenderPassCreateInfo2*              pCreateInfo,
    VkRenderPass*                               pRenderPass) {
        return vkCreateRenderPass2(device, pCreateInfo, 0, pRenderPass);
}

void CmdBeginRenderPass2(
    VkCommandBuffer                             commandBuffer,
    const VkRenderPassBeginInfo*                pRenderPassBegin,
    const VkSubpassBeginInfo*                   pSubpassBeginInfo) {
        return vkCmdBeginRenderPass2(commandBuffer, pRenderPassBegin, pSubpassBeginInfo);
}

void CmdNextSubpass2(
    VkCommandBuffer                             commandBuffer,
    const VkSubpassBeginInfo*                   pSubpassBeginInfo,
    const VkSubpassEndInfo*                     pSubpassEndInfo) {
        return vkCmdNextSubpass2(commandBuffer, pSubpassBeginInfo, pSubpassEndInfo);
}

void CmdEndRenderPass2(
    VkCommandBuffer                             commandBuffer,
    const VkSubpassEndInfo*                     pSubpassEndInfo) {
        return vkCmdEndRenderPass2(commandBuffer, pSubpassEndInfo);
}

void ResetQueryPool(
    VkQueryPool                                 queryPool,
    uint32_t                                    firstQuery,
    uint32_t                                    queryCount) {
        return vkResetQueryPool(device, queryPool, firstQuery, queryCount);
}

VkResult GetSemaphoreCounterValue(
    VkSemaphore                                 semaphore,
    uint64_t*                                   pValue) {
        return vkGetSemaphoreCounterValue(device, semaphore, pValue);
}

VkResult WaitSemaphores(
    const VkSemaphoreWaitInfo*                  pWaitInfo,
    uint64_t                                    timeout) {
        return vkWaitSemaphores(device, pWaitInfo, timeout);
}

VkResult SignalSemaphore(
    const VkSemaphoreSignalInfo*                pSignalInfo) {
        return vkSignalSemaphore(device, pSignalInfo);
}

VkDeviceAddress GetBufferDeviceAddress(
    const VkBufferDeviceAddressInfo*            pInfo) {
        return vkGetBufferDeviceAddress(device, pInfo);
}

uint64_t GetBufferOpaqueCaptureAddress(
    const VkBufferDeviceAddressInfo*            pInfo) {
        return vkGetBufferOpaqueCaptureAddress(device, pInfo);
}

uint64_t GetDeviceMemoryOpaqueCaptureAddress(
    const VkDeviceMemoryOpaqueCaptureAddressInfo* pInfo) {
        return vkGetDeviceMemoryOpaqueCaptureAddress(device, pInfo);
}

void DestroySurfaceKHR(
    VkSurfaceKHR                                surface) {
        return vkDestroySurfaceKHR(instance, surface, 0);
}

VkResult GetPhysicalDeviceSurfaceSupportKHR(
    uint32_t                                    queueFamilyIndex,
    VkSurfaceKHR                                surface,
    VkBool32*                                   pSupported) {
        return vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queueFamilyIndex, surface, pSupported);
}

VkResult GetPhysicalDeviceSurfaceCapabilitiesKHR(
    VkSurfaceKHR                                surface,
    VkSurfaceCapabilitiesKHR*                   pSurfaceCapabilities) {
        return vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, pSurfaceCapabilities);
}

VkResult GetPhysicalDeviceSurfaceFormatsKHR(
    VkSurfaceKHR                                surface,
    uint32_t*                                   pSurfaceFormatCount,
    VkSurfaceFormatKHR*                         pSurfaceFormats) {
        return vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, pSurfaceFormatCount, pSurfaceFormats);
}

VkResult GetPhysicalDeviceSurfacePresentModesKHR(
    VkSurfaceKHR                                surface,
    uint32_t*                                   pPresentModeCount,
    VkPresentModeKHR*                           pPresentModes) {
        return vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, pPresentModeCount, pPresentModes);
}

VkResult CreateSwapchainKHR(
    const VkSwapchainCreateInfoKHR*             pCreateInfo,
    VkSwapchainKHR*                             pSwapchain) {
        return vkCreateSwapchainKHR(device, pCreateInfo, 0, pSwapchain);
}

void DestroySwapchainKHR(
    VkSwapchainKHR                              swapchain) {
        return vkDestroySwapchainKHR(device, swapchain, 0);
}

VkResult GetSwapchainImagesKHR(
    VkSwapchainKHR                              swapchain,
    uint32_t*                                   pSwapchainImageCount,
    VkImage*                                    pSwapchainImages) {
        return vkGetSwapchainImagesKHR(device, swapchain, pSwapchainImageCount, pSwapchainImages);
}

VkResult AcquireNextImageKHR(
    VkSwapchainKHR                              swapchain,
    uint64_t                                    timeout,
    VkSemaphore                                 semaphore,
    VkFence                                     fence,
    uint32_t*                                   pImageIndex) {
        return vkAcquireNextImageKHR(device, swapchain, timeout, semaphore, fence, pImageIndex);
}

VkResult QueuePresentKHR(
    const VkPresentInfoKHR*                     pPresentInfo) {
        return vkQueuePresentKHR(queue, pPresentInfo);
}

VkResult GetDeviceGroupPresentCapabilitiesKHR(
    VkDeviceGroupPresentCapabilitiesKHR*        pDeviceGroupPresentCapabilities) {
        return vkGetDeviceGroupPresentCapabilitiesKHR(device, pDeviceGroupPresentCapabilities);
}

VkResult GetDeviceGroupSurfacePresentModesKHR(
    VkSurfaceKHR                                surface,
    VkDeviceGroupPresentModeFlagsKHR*           pModes) {
        return vkGetDeviceGroupSurfacePresentModesKHR(device, surface, pModes);
}

VkResult GetPhysicalDevicePresentRectanglesKHR(
    VkSurfaceKHR                                surface,
    uint32_t*                                   pRectCount,
    VkRect2D*                                   pRects) {
        return vkGetPhysicalDevicePresentRectanglesKHR(physicalDevice, surface, pRectCount, pRects);
}

VkResult AcquireNextImage2KHR(
    const VkAcquireNextImageInfoKHR*            pAcquireInfo,
    uint32_t*                                   pImageIndex) {
        return vkAcquireNextImage2KHR(device, pAcquireInfo, pImageIndex);
}

VkResult GetPhysicalDeviceDisplayPropertiesKHR(
    uint32_t*                                   pPropertyCount,
    VkDisplayPropertiesKHR*                     pProperties) {
        return vkGetPhysicalDeviceDisplayPropertiesKHR(physicalDevice, pPropertyCount, pProperties);
}

VkResult GetPhysicalDeviceDisplayPlanePropertiesKHR(
    uint32_t*                                   pPropertyCount,
    VkDisplayPlanePropertiesKHR*                pProperties) {
        return vkGetPhysicalDeviceDisplayPlanePropertiesKHR(physicalDevice, pPropertyCount, pProperties);
}

VkResult GetDisplayPlaneSupportedDisplaysKHR(
    uint32_t                                    planeIndex,
    uint32_t*                                   pDisplayCount,
    VkDisplayKHR*                               pDisplays) {
        return vkGetDisplayPlaneSupportedDisplaysKHR(physicalDevice, planeIndex, pDisplayCount, pDisplays);
}

VkResult GetDisplayModePropertiesKHR(
    VkDisplayKHR                                display,
    uint32_t*                                   pPropertyCount,
    VkDisplayModePropertiesKHR*                 pProperties) {
        return vkGetDisplayModePropertiesKHR(physicalDevice, display, pPropertyCount, pProperties);
}

VkResult CreateDisplayModeKHR(
    VkDisplayKHR                                display,
    const VkDisplayModeCreateInfoKHR*           pCreateInfo,
    VkDisplayModeKHR*                           pMode) {
        return vkCreateDisplayModeKHR(physicalDevice, display, pCreateInfo, 0, pMode);
}

VkResult GetDisplayPlaneCapabilitiesKHR(
    VkDisplayModeKHR                            mode,
    uint32_t                                    planeIndex,
    VkDisplayPlaneCapabilitiesKHR*              pCapabilities) {
        return vkGetDisplayPlaneCapabilitiesKHR(physicalDevice, mode, planeIndex, pCapabilities);
}

VkResult CreateDisplayPlaneSurfaceKHR(
    const VkDisplaySurfaceCreateInfoKHR*        pCreateInfo,
    VkSurfaceKHR*                               pSurface) {
        return vkCreateDisplayPlaneSurfaceKHR(instance, pCreateInfo, 0, pSurface);
}

VkResult CreateSharedSwapchainsKHR(
    uint32_t                                    swapchainCount,
    const VkSwapchainCreateInfoKHR*             pCreateInfos,
    VkSwapchainKHR*                             pSwapchains) {
        return vkCreateSharedSwapchainsKHR(device, swapchainCount, pCreateInfos, 0, pSwapchains);
}

void GetPhysicalDeviceFeatures2KHR(
    VkPhysicalDeviceFeatures2*                  pFeatures) {
        return vkGetPhysicalDeviceFeatures2KHR(physicalDevice, pFeatures);
}

void GetPhysicalDeviceProperties2KHR(
    VkPhysicalDeviceProperties2*                pProperties) {
        return vkGetPhysicalDeviceProperties2KHR(physicalDevice, pProperties);
}

void GetPhysicalDeviceFormatProperties2KHR(
    VkFormat                                    format,
    VkFormatProperties2*                        pFormatProperties) {
        return vkGetPhysicalDeviceFormatProperties2KHR(physicalDevice, format, pFormatProperties);
}

VkResult GetPhysicalDeviceImageFormatProperties2KHR(
    const VkPhysicalDeviceImageFormatInfo2*     pImageFormatInfo,
    VkImageFormatProperties2*                   pImageFormatProperties) {
        return vkGetPhysicalDeviceImageFormatProperties2KHR(physicalDevice, pImageFormatInfo, pImageFormatProperties);
}

void GetPhysicalDeviceQueueFamilyProperties2KHR(
    uint32_t*                                   pQueueFamilyPropertyCount,
    VkQueueFamilyProperties2*                   pQueueFamilyProperties) {
        return vkGetPhysicalDeviceQueueFamilyProperties2KHR(physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties);
}

void GetPhysicalDeviceMemoryProperties2KHR(
    VkPhysicalDeviceMemoryProperties2*          pMemoryProperties) {
        return vkGetPhysicalDeviceMemoryProperties2KHR(physicalDevice, pMemoryProperties);
}

void GetPhysicalDeviceSparseImageFormatProperties2KHR(
    const VkPhysicalDeviceSparseImageFormatInfo2* pFormatInfo,
    uint32_t*                                   pPropertyCount,
    VkSparseImageFormatProperties2*             pProperties) {
        return vkGetPhysicalDeviceSparseImageFormatProperties2KHR(physicalDevice, pFormatInfo, pPropertyCount, pProperties);
}

void GetDeviceGroupPeerMemoryFeaturesKHR(
    uint32_t                                    heapIndex,
    uint32_t                                    localDeviceIndex,
    uint32_t                                    remoteDeviceIndex,
    VkPeerMemoryFeatureFlags*                   pPeerMemoryFeatures) {
        return vkGetDeviceGroupPeerMemoryFeaturesKHR(device, heapIndex, localDeviceIndex, remoteDeviceIndex, pPeerMemoryFeatures);
}

void CmdSetDeviceMaskKHR(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    deviceMask) {
        return vkCmdSetDeviceMaskKHR(commandBuffer, deviceMask);
}

void CmdDispatchBaseKHR(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    baseGroupX,
    uint32_t                                    baseGroupY,
    uint32_t                                    baseGroupZ,
    uint32_t                                    groupCountX,
    uint32_t                                    groupCountY,
    uint32_t                                    groupCountZ) {
        return vkCmdDispatchBaseKHR(commandBuffer, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY, groupCountZ);
}

void TrimCommandPoolKHR(
    VkCommandPool                               commandPool,
    VkCommandPoolTrimFlags                      flags) {
        return vkTrimCommandPoolKHR(device, commandPool, flags);
}

VkResult EnumeratePhysicalDeviceGroupsKHR(
    uint32_t*                                   pPhysicalDeviceGroupCount,
    VkPhysicalDeviceGroupProperties*            pPhysicalDeviceGroupProperties) {
        return vkEnumeratePhysicalDeviceGroupsKHR(instance, pPhysicalDeviceGroupCount, pPhysicalDeviceGroupProperties);
}

void GetPhysicalDeviceExternalBufferPropertiesKHR(
    const VkPhysicalDeviceExternalBufferInfo*   pExternalBufferInfo,
    VkExternalBufferProperties*                 pExternalBufferProperties) {
        return vkGetPhysicalDeviceExternalBufferPropertiesKHR(physicalDevice, pExternalBufferInfo, pExternalBufferProperties);
}

VkResult GetMemoryFdKHR(
    const VkMemoryGetFdInfoKHR*                 pGetFdInfo,
    int*                                        pFd) {
        return vkGetMemoryFdKHR(device, pGetFdInfo, pFd);
}

VkResult GetMemoryFdPropertiesKHR(
    VkExternalMemoryHandleTypeFlagBits          handleType,
    int                                         fd,
    VkMemoryFdPropertiesKHR*                    pMemoryFdProperties) {
        return vkGetMemoryFdPropertiesKHR(device, handleType, fd, pMemoryFdProperties);
}

void GetPhysicalDeviceExternalSemaphorePropertiesKHR(
    const VkPhysicalDeviceExternalSemaphoreInfo* pExternalSemaphoreInfo,
    VkExternalSemaphoreProperties*              pExternalSemaphoreProperties) {
        return vkGetPhysicalDeviceExternalSemaphorePropertiesKHR(physicalDevice, pExternalSemaphoreInfo, pExternalSemaphoreProperties);
}

VkResult ImportSemaphoreFdKHR(
    const VkImportSemaphoreFdInfoKHR*           pImportSemaphoreFdInfo) {
        return vkImportSemaphoreFdKHR(device, pImportSemaphoreFdInfo);
}

VkResult GetSemaphoreFdKHR(
    const VkSemaphoreGetFdInfoKHR*              pGetFdInfo,
    int*                                        pFd) {
        return vkGetSemaphoreFdKHR(device, pGetFdInfo, pFd);
}

void CmdPushDescriptorSetKHR(
    VkCommandBuffer                             commandBuffer,
    VkPipelineBindPoint                         pipelineBindPoint,
    VkPipelineLayout                            layout,
    uint32_t                                    set,
    uint32_t                                    descriptorWriteCount,
    const VkWriteDescriptorSet*                 pDescriptorWrites) {
        return vkCmdPushDescriptorSetKHR(commandBuffer, pipelineBindPoint, layout, set, descriptorWriteCount, pDescriptorWrites);
}

void CmdPushDescriptorSetWithTemplateKHR(
    VkCommandBuffer                             commandBuffer,
    VkDescriptorUpdateTemplate                  descriptorUpdateTemplate,
    VkPipelineLayout                            layout,
    uint32_t                                    set,
    const void*                                 pData) {
        return vkCmdPushDescriptorSetWithTemplateKHR(commandBuffer, descriptorUpdateTemplate, layout, set, pData);
}

VkResult CreateDescriptorUpdateTemplateKHR(
    const VkDescriptorUpdateTemplateCreateInfo* pCreateInfo,
    VkDescriptorUpdateTemplate*                 pDescriptorUpdateTemplate) {
        return vkCreateDescriptorUpdateTemplateKHR(device, pCreateInfo, 0, pDescriptorUpdateTemplate);
}

void DestroyDescriptorUpdateTemplateKHR(
    VkDescriptorUpdateTemplate                  descriptorUpdateTemplate) {
        return vkDestroyDescriptorUpdateTemplateKHR(device, descriptorUpdateTemplate, 0);
}

void UpdateDescriptorSetWithTemplateKHR(
    VkDescriptorSet                             descriptorSet,
    VkDescriptorUpdateTemplate                  descriptorUpdateTemplate,
    const void*                                 pData) {
        return vkUpdateDescriptorSetWithTemplateKHR(device, descriptorSet, descriptorUpdateTemplate, pData);
}

VkResult CreateRenderPass2KHR(
    const VkRenderPassCreateInfo2*              pCreateInfo,
    VkRenderPass*                               pRenderPass) {
        return vkCreateRenderPass2KHR(device, pCreateInfo, 0, pRenderPass);
}

void CmdBeginRenderPass2KHR(
    VkCommandBuffer                             commandBuffer,
    const VkRenderPassBeginInfo*                pRenderPassBegin,
    const VkSubpassBeginInfo*                   pSubpassBeginInfo) {
        return vkCmdBeginRenderPass2KHR(commandBuffer, pRenderPassBegin, pSubpassBeginInfo);
}

void CmdNextSubpass2KHR(
    VkCommandBuffer                             commandBuffer,
    const VkSubpassBeginInfo*                   pSubpassBeginInfo,
    const VkSubpassEndInfo*                     pSubpassEndInfo) {
        return vkCmdNextSubpass2KHR(commandBuffer, pSubpassBeginInfo, pSubpassEndInfo);
}

void CmdEndRenderPass2KHR(
    VkCommandBuffer                             commandBuffer,
    const VkSubpassEndInfo*                     pSubpassEndInfo) {
        return vkCmdEndRenderPass2KHR(commandBuffer, pSubpassEndInfo);
}

VkResult GetSwapchainStatusKHR(
    VkSwapchainKHR                              swapchain) {
        return vkGetSwapchainStatusKHR(device, swapchain);
}

void GetPhysicalDeviceExternalFencePropertiesKHR(
    const VkPhysicalDeviceExternalFenceInfo*    pExternalFenceInfo,
    VkExternalFenceProperties*                  pExternalFenceProperties) {
        return vkGetPhysicalDeviceExternalFencePropertiesKHR(physicalDevice, pExternalFenceInfo, pExternalFenceProperties);
}

VkResult ImportFenceFdKHR(
    const VkImportFenceFdInfoKHR*               pImportFenceFdInfo) {
        return vkImportFenceFdKHR(device, pImportFenceFdInfo);
}

VkResult GetFenceFdKHR(
    const VkFenceGetFdInfoKHR*                  pGetFdInfo,
    int*                                        pFd) {
        return vkGetFenceFdKHR(device, pGetFdInfo, pFd);
}

VkResult EnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR(
    uint32_t                                    queueFamilyIndex,
    uint32_t*                                   pCounterCount,
    VkPerformanceCounterKHR*                    pCounters,
    VkPerformanceCounterDescriptionKHR*         pCounterDescriptions) {
        return vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR(physicalDevice, queueFamilyIndex, pCounterCount, pCounters, pCounterDescriptions);
}

void GetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR(
    const VkQueryPoolPerformanceCreateInfoKHR*  pPerformanceQueryCreateInfo,
    uint32_t*                                   pNumPasses) {
        return vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR(physicalDevice, pPerformanceQueryCreateInfo, pNumPasses);
}

VkResult AcquireProfilingLockKHR(
    const VkAcquireProfilingLockInfoKHR*        pInfo) {
        return vkAcquireProfilingLockKHR(device, pInfo);
}

void ReleaseProfilingLockKHR() {
        return vkReleaseProfilingLockKHR(device);
}

VkResult GetPhysicalDeviceSurfaceCapabilities2KHR(
    const VkPhysicalDeviceSurfaceInfo2KHR*      pSurfaceInfo,
    VkSurfaceCapabilities2KHR*                  pSurfaceCapabilities) {
        return vkGetPhysicalDeviceSurfaceCapabilities2KHR(physicalDevice, pSurfaceInfo, pSurfaceCapabilities);
}

VkResult GetPhysicalDeviceSurfaceFormats2KHR(
    const VkPhysicalDeviceSurfaceInfo2KHR*      pSurfaceInfo,
    uint32_t*                                   pSurfaceFormatCount,
    VkSurfaceFormat2KHR*                        pSurfaceFormats) {
        return vkGetPhysicalDeviceSurfaceFormats2KHR(physicalDevice, pSurfaceInfo, pSurfaceFormatCount, pSurfaceFormats);
}

VkResult GetPhysicalDeviceDisplayProperties2KHR(
    uint32_t*                                   pPropertyCount,
    VkDisplayProperties2KHR*                    pProperties) {
        return vkGetPhysicalDeviceDisplayProperties2KHR(physicalDevice, pPropertyCount, pProperties);
}

VkResult GetPhysicalDeviceDisplayPlaneProperties2KHR(
    uint32_t*                                   pPropertyCount,
    VkDisplayPlaneProperties2KHR*               pProperties) {
        return vkGetPhysicalDeviceDisplayPlaneProperties2KHR(physicalDevice, pPropertyCount, pProperties);
}

VkResult GetDisplayModeProperties2KHR(
    VkDisplayKHR                                display,
    uint32_t*                                   pPropertyCount,
    VkDisplayModeProperties2KHR*                pProperties) {
        return vkGetDisplayModeProperties2KHR(physicalDevice, display, pPropertyCount, pProperties);
}

VkResult GetDisplayPlaneCapabilities2KHR(
    const VkDisplayPlaneInfo2KHR*               pDisplayPlaneInfo,
    VkDisplayPlaneCapabilities2KHR*             pCapabilities) {
        return vkGetDisplayPlaneCapabilities2KHR(physicalDevice, pDisplayPlaneInfo, pCapabilities);
}

void GetImageMemoryRequirements2KHR(
    const VkImageMemoryRequirementsInfo2*       pInfo,
    VkMemoryRequirements2*                      pMemoryRequirements) {
        return vkGetImageMemoryRequirements2KHR(device, pInfo, pMemoryRequirements);
}

void GetBufferMemoryRequirements2KHR(
    const VkBufferMemoryRequirementsInfo2*      pInfo,
    VkMemoryRequirements2*                      pMemoryRequirements) {
        return vkGetBufferMemoryRequirements2KHR(device, pInfo, pMemoryRequirements);
}

void GetImageSparseMemoryRequirements2KHR(
    const VkImageSparseMemoryRequirementsInfo2* pInfo,
    uint32_t*                                   pSparseMemoryRequirementCount,
    VkSparseImageMemoryRequirements2*           pSparseMemoryRequirements) {
        return vkGetImageSparseMemoryRequirements2KHR(device, pInfo, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
}

VkResult CreateSamplerYcbcrConversionKHR(
    const VkSamplerYcbcrConversionCreateInfo*   pCreateInfo,
    VkSamplerYcbcrConversion*                   pYcbcrConversion) {
        return vkCreateSamplerYcbcrConversionKHR(device, pCreateInfo, 0, pYcbcrConversion);
}

void DestroySamplerYcbcrConversionKHR(
    VkSamplerYcbcrConversion                    ycbcrConversion) {
        return vkDestroySamplerYcbcrConversionKHR(device, ycbcrConversion, 0);
}

VkResult BindBufferMemory2KHR(
    uint32_t                                    bindInfoCount,
    const VkBindBufferMemoryInfo*               pBindInfos) {
        return vkBindBufferMemory2KHR(device, bindInfoCount, pBindInfos);
}

VkResult BindImageMemory2KHR(
    uint32_t                                    bindInfoCount,
    const VkBindImageMemoryInfo*                pBindInfos) {
        return vkBindImageMemory2KHR(device, bindInfoCount, pBindInfos);
}

void GetDescriptorSetLayoutSupportKHR(
    const VkDescriptorSetLayoutCreateInfo*      pCreateInfo,
    VkDescriptorSetLayoutSupport*               pSupport) {
        return vkGetDescriptorSetLayoutSupportKHR(device, pCreateInfo, pSupport);
}

void CmdDrawIndirectCountKHR(
    VkCommandBuffer                             commandBuffer,
    VkBuffer                                    buffer,
    VkDeviceSize                                offset,
    VkBuffer                                    countBuffer,
    VkDeviceSize                                countBufferOffset,
    uint32_t                                    maxDrawCount,
    uint32_t                                    stride) {
        return vkCmdDrawIndirectCountKHR(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}

void CmdDrawIndexedIndirectCountKHR(
    VkCommandBuffer                             commandBuffer,
    VkBuffer                                    buffer,
    VkDeviceSize                                offset,
    VkBuffer                                    countBuffer,
    VkDeviceSize                                countBufferOffset,
    uint32_t                                    maxDrawCount,
    uint32_t                                    stride) {
        return vkCmdDrawIndexedIndirectCountKHR(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}

VkResult GetSemaphoreCounterValueKHR(
    VkSemaphore                                 semaphore,
    uint64_t*                                   pValue) {
        return vkGetSemaphoreCounterValueKHR(device, semaphore, pValue);
}

VkResult WaitSemaphoresKHR(
    const VkSemaphoreWaitInfo*                  pWaitInfo,
    uint64_t                                    timeout) {
        return vkWaitSemaphoresKHR(device, pWaitInfo, timeout);
}

VkResult SignalSemaphoreKHR(
    const VkSemaphoreSignalInfo*                pSignalInfo) {
        return vkSignalSemaphoreKHR(device, pSignalInfo);
}

VkDeviceAddress GetBufferDeviceAddressKHR(
    const VkBufferDeviceAddressInfo*            pInfo) {
        return vkGetBufferDeviceAddressKHR(device, pInfo);
}

uint64_t GetBufferOpaqueCaptureAddressKHR(
    const VkBufferDeviceAddressInfo*            pInfo) {
        return vkGetBufferOpaqueCaptureAddressKHR(device, pInfo);
}

uint64_t GetDeviceMemoryOpaqueCaptureAddressKHR(
    const VkDeviceMemoryOpaqueCaptureAddressInfo* pInfo) {
        return vkGetDeviceMemoryOpaqueCaptureAddressKHR(device, pInfo);
}

VkResult GetPipelineExecutablePropertiesKHR(
    const VkPipelineInfoKHR*                    pPipelineInfo,
    uint32_t*                                   pExecutableCount,
    VkPipelineExecutablePropertiesKHR*          pProperties) {
        return vkGetPipelineExecutablePropertiesKHR(device, pPipelineInfo, pExecutableCount, pProperties);
}

VkResult GetPipelineExecutableStatisticsKHR(
    const VkPipelineExecutableInfoKHR*          pExecutableInfo,
    uint32_t*                                   pStatisticCount,
    VkPipelineExecutableStatisticKHR*           pStatistics) {
        return vkGetPipelineExecutableStatisticsKHR(device, pExecutableInfo, pStatisticCount, pStatistics);
}

VkResult GetPipelineExecutableInternalRepresentationsKHR(
    const VkPipelineExecutableInfoKHR*          pExecutableInfo,
    uint32_t*                                   pInternalRepresentationCount,
    VkPipelineExecutableInternalRepresentationKHR* pInternalRepresentations) {
        return vkGetPipelineExecutableInternalRepresentationsKHR(device, pExecutableInfo, pInternalRepresentationCount, pInternalRepresentations);
}

VkResult CreateDebugReportCallbackEXT(
    const VkDebugReportCallbackCreateInfoEXT*   pCreateInfo,
    VkDebugReportCallbackEXT*                   pCallback) {
        return vkCreateDebugReportCallbackEXT(instance, pCreateInfo, 0, pCallback);
}

void DestroyDebugReportCallbackEXT(
    VkDebugReportCallbackEXT                    callback) {
        return vkDestroyDebugReportCallbackEXT(instance, callback, 0);
}

void DebugReportMessageEXT(
    VkDebugReportFlagsEXT                       flags,
    VkDebugReportObjectTypeEXT                  objectType,
    uint64_t                                    object,
    size_t                                      location,
    int32_t                                     messageCode,
    const char*                                 pLayerPrefix,
    const char*                                 pMessage) {
        return vkDebugReportMessageEXT(instance, flags, objectType, object, location, messageCode, pLayerPrefix, pMessage);
}

VkResult DebugMarkerSetObjectTagEXT(
    const VkDebugMarkerObjectTagInfoEXT*        pTagInfo) {
        return vkDebugMarkerSetObjectTagEXT(device, pTagInfo);
}

VkResult DebugMarkerSetObjectNameEXT(
    const VkDebugMarkerObjectNameInfoEXT*       pNameInfo) {
        return vkDebugMarkerSetObjectNameEXT(device, pNameInfo);
}

void CmdDebugMarkerBeginEXT(
    VkCommandBuffer                             commandBuffer,
    const VkDebugMarkerMarkerInfoEXT*           pMarkerInfo) {
        return vkCmdDebugMarkerBeginEXT(commandBuffer, pMarkerInfo);
}

void CmdDebugMarkerEndEXT(
    VkCommandBuffer                             commandBuffer) {
        return vkCmdDebugMarkerEndEXT(commandBuffer);
}

void CmdDebugMarkerInsertEXT(
    VkCommandBuffer                             commandBuffer,
    const VkDebugMarkerMarkerInfoEXT*           pMarkerInfo) {
        return vkCmdDebugMarkerInsertEXT(commandBuffer, pMarkerInfo);
}

void CmdBindTransformFeedbackBuffersEXT(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    firstBinding,
    uint32_t                                    bindingCount,
    const VkBuffer*                             pBuffers,
    const VkDeviceSize*                         pOffsets,
    const VkDeviceSize*                         pSizes) {
        return vkCmdBindTransformFeedbackBuffersEXT(commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets, pSizes);
}

void CmdBeginTransformFeedbackEXT(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    firstCounterBuffer,
    uint32_t                                    counterBufferCount,
    const VkBuffer*                             pCounterBuffers,
    const VkDeviceSize*                         pCounterBufferOffsets) {
        return vkCmdBeginTransformFeedbackEXT(commandBuffer, firstCounterBuffer, counterBufferCount, pCounterBuffers, pCounterBufferOffsets);
}

void CmdEndTransformFeedbackEXT(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    firstCounterBuffer,
    uint32_t                                    counterBufferCount,
    const VkBuffer*                             pCounterBuffers,
    const VkDeviceSize*                         pCounterBufferOffsets) {
        return vkCmdEndTransformFeedbackEXT(commandBuffer, firstCounterBuffer, counterBufferCount, pCounterBuffers, pCounterBufferOffsets);
}

void CmdBeginQueryIndexedEXT(
    VkCommandBuffer                             commandBuffer,
    VkQueryPool                                 queryPool,
    uint32_t                                    query,
    VkQueryControlFlags                         flags,
    uint32_t                                    index) {
        return vkCmdBeginQueryIndexedEXT(commandBuffer, queryPool, query, flags, index);
}

void CmdEndQueryIndexedEXT(
    VkCommandBuffer                             commandBuffer,
    VkQueryPool                                 queryPool,
    uint32_t                                    query,
    uint32_t                                    index) {
        return vkCmdEndQueryIndexedEXT(commandBuffer, queryPool, query, index);
}

void CmdDrawIndirectByteCountEXT(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    instanceCount,
    uint32_t                                    firstInstance,
    VkBuffer                                    counterBuffer,
    VkDeviceSize                                counterBufferOffset,
    uint32_t                                    counterOffset,
    uint32_t                                    vertexStride) {
        return vkCmdDrawIndirectByteCountEXT(commandBuffer, instanceCount, firstInstance, counterBuffer, counterBufferOffset, counterOffset, vertexStride);
}

uint32_t GetImageViewHandleNVX(
    const VkImageViewHandleInfoNVX*             pInfo) {
        return vkGetImageViewHandleNVX(device, pInfo);
}

VkResult GetImageViewAddressNVX(
    VkImageView                                 imageView,
    VkImageViewAddressPropertiesNVX*            pProperties) {
        return vkGetImageViewAddressNVX(device, imageView, pProperties);
}

void CmdDrawIndirectCountAMD(
    VkCommandBuffer                             commandBuffer,
    VkBuffer                                    buffer,
    VkDeviceSize                                offset,
    VkBuffer                                    countBuffer,
    VkDeviceSize                                countBufferOffset,
    uint32_t                                    maxDrawCount,
    uint32_t                                    stride) {
        return vkCmdDrawIndirectCountAMD(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}

void CmdDrawIndexedIndirectCountAMD(
    VkCommandBuffer                             commandBuffer,
    VkBuffer                                    buffer,
    VkDeviceSize                                offset,
    VkBuffer                                    countBuffer,
    VkDeviceSize                                countBufferOffset,
    uint32_t                                    maxDrawCount,
    uint32_t                                    stride) {
        return vkCmdDrawIndexedIndirectCountAMD(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}

VkResult GetShaderInfoAMD(
    VkPipeline                                  pipeline,
    VkShaderStageFlagBits                       shaderStage,
    VkShaderInfoTypeAMD                         infoType,
    size_t*                                     pInfoSize,
    void*                                       pInfo) {
        return vkGetShaderInfoAMD(device, pipeline, shaderStage, infoType, pInfoSize, pInfo);
}

VkResult GetPhysicalDeviceExternalImageFormatPropertiesNV(
    VkFormat                                    format,
    VkImageType                                 type,
    VkImageTiling                               tiling,
    VkImageUsageFlags                           usage,
    VkImageCreateFlags                          flags,
    VkExternalMemoryHandleTypeFlagsNV           externalHandleType,
    VkExternalImageFormatPropertiesNV*          pExternalImageFormatProperties) {
        return vkGetPhysicalDeviceExternalImageFormatPropertiesNV(physicalDevice, format, type, tiling, usage, flags, externalHandleType, pExternalImageFormatProperties);
}

void CmdBeginConditionalRenderingEXT(
    VkCommandBuffer                             commandBuffer,
    const VkConditionalRenderingBeginInfoEXT*   pConditionalRenderingBegin) {
        return vkCmdBeginConditionalRenderingEXT(commandBuffer, pConditionalRenderingBegin);
}

void CmdEndConditionalRenderingEXT(
    VkCommandBuffer                             commandBuffer) {
        return vkCmdEndConditionalRenderingEXT(commandBuffer);
}

void CmdSetViewportWScalingNV(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    firstViewport,
    uint32_t                                    viewportCount,
    const VkViewportWScalingNV*                 pViewportWScalings) {
        return vkCmdSetViewportWScalingNV(commandBuffer, firstViewport, viewportCount, pViewportWScalings);
}

VkResult ReleaseDisplayEXT(
    VkDisplayKHR                                display) {
        return vkReleaseDisplayEXT(physicalDevice, display);
}

VkResult GetPhysicalDeviceSurfaceCapabilities2EXT(
    VkSurfaceKHR                                surface,
    VkSurfaceCapabilities2EXT*                  pSurfaceCapabilities) {
        return vkGetPhysicalDeviceSurfaceCapabilities2EXT(physicalDevice, surface, pSurfaceCapabilities);
}

VkResult DisplayPowerControlEXT(
    VkDisplayKHR                                display,
    const VkDisplayPowerInfoEXT*                pDisplayPowerInfo) {
        return vkDisplayPowerControlEXT(device, display, pDisplayPowerInfo);
}

VkResult RegisterDeviceEventEXT(
    const VkDeviceEventInfoEXT*                 pDeviceEventInfo,
    VkFence*                                    pFence) {
        return vkRegisterDeviceEventEXT(device, pDeviceEventInfo, 0, pFence);
}

VkResult RegisterDisplayEventEXT(
    VkDisplayKHR                                display,
    const VkDisplayEventInfoEXT*                pDisplayEventInfo,
    VkFence*                                    pFence) {
        return vkRegisterDisplayEventEXT(device, display, pDisplayEventInfo, 0, pFence);
}

VkResult GetSwapchainCounterEXT(
    VkSwapchainKHR                              swapchain,
    VkSurfaceCounterFlagBitsEXT                 counter,
    uint64_t*                                   pCounterValue) {
        return vkGetSwapchainCounterEXT(device, swapchain, counter, pCounterValue);
}

VkResult GetRefreshCycleDurationGOOGLE(
    VkSwapchainKHR                              swapchain,
    VkRefreshCycleDurationGOOGLE*               pDisplayTimingProperties) {
        return vkGetRefreshCycleDurationGOOGLE(device, swapchain, pDisplayTimingProperties);
}

VkResult GetPastPresentationTimingGOOGLE(
    VkSwapchainKHR                              swapchain,
    uint32_t*                                   pPresentationTimingCount,
    VkPastPresentationTimingGOOGLE*             pPresentationTimings) {
        return vkGetPastPresentationTimingGOOGLE(device, swapchain, pPresentationTimingCount, pPresentationTimings);
}

void CmdSetDiscardRectangleEXT(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    firstDiscardRectangle,
    uint32_t                                    discardRectangleCount,
    const VkRect2D*                             pDiscardRectangles) {
        return vkCmdSetDiscardRectangleEXT(commandBuffer, firstDiscardRectangle, discardRectangleCount, pDiscardRectangles);
}

void SetHdrMetadataEXT(
    uint32_t                                    swapchainCount,
    const VkSwapchainKHR*                       pSwapchains,
    const VkHdrMetadataEXT*                     pMetadata) {
        return vkSetHdrMetadataEXT(device, swapchainCount, pSwapchains, pMetadata);
}

VkResult SetDebugUtilsObjectNameEXT(
    const VkDebugUtilsObjectNameInfoEXT*        pNameInfo) {
        return vkSetDebugUtilsObjectNameEXT(device, pNameInfo);
}

VkResult SetDebugUtilsObjectTagEXT(
    const VkDebugUtilsObjectTagInfoEXT*         pTagInfo) {
        return vkSetDebugUtilsObjectTagEXT(device, pTagInfo);
}

void QueueBeginDebugUtilsLabelEXT(
    VkQueue                                     queue,
    const VkDebugUtilsLabelEXT*                 pLabelInfo) {
        return vkQueueBeginDebugUtilsLabelEXT(queue, pLabelInfo);
}

void QueueEndDebugUtilsLabelEXT(
    VkQueue                                     queue) {
        return vkQueueEndDebugUtilsLabelEXT(queue);
}

void QueueInsertDebugUtilsLabelEXT(
    VkQueue                                     queue,
    const VkDebugUtilsLabelEXT*                 pLabelInfo) {
        return vkQueueInsertDebugUtilsLabelEXT(queue, pLabelInfo);
}

void CmdBeginDebugUtilsLabelEXT(
    VkCommandBuffer                             commandBuffer,
    const VkDebugUtilsLabelEXT*                 pLabelInfo) {
        return vkCmdBeginDebugUtilsLabelEXT(commandBuffer, pLabelInfo);
}

void CmdEndDebugUtilsLabelEXT(
    VkCommandBuffer                             commandBuffer) {
        return vkCmdEndDebugUtilsLabelEXT(commandBuffer);
}

void CmdInsertDebugUtilsLabelEXT(
    VkCommandBuffer                             commandBuffer,
    const VkDebugUtilsLabelEXT*                 pLabelInfo) {
        return vkCmdInsertDebugUtilsLabelEXT(commandBuffer, pLabelInfo);
}

VkResult CreateDebugUtilsMessengerEXT(
    const VkDebugUtilsMessengerCreateInfoEXT*   pCreateInfo,
    VkDebugUtilsMessengerEXT*                   pMessenger) {
        return vkCreateDebugUtilsMessengerEXT(instance, pCreateInfo, 0, pMessenger);
}

void DestroyDebugUtilsMessengerEXT(
    VkDebugUtilsMessengerEXT                    messenger) {
        return vkDestroyDebugUtilsMessengerEXT(instance, messenger, 0);
}

void SubmitDebugUtilsMessageEXT(
    VkDebugUtilsMessageSeverityFlagBitsEXT      messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT             messageTypes,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData) {
        return vkSubmitDebugUtilsMessageEXT(instance, messageSeverity, messageTypes, pCallbackData);
}

void CmdSetSampleLocationsEXT(
    VkCommandBuffer                             commandBuffer,
    const VkSampleLocationsInfoEXT*             pSampleLocationsInfo) {
        return vkCmdSetSampleLocationsEXT(commandBuffer, pSampleLocationsInfo);
}

void GetPhysicalDeviceMultisamplePropertiesEXT(
    VkSampleCountFlagBits                       samples,
    VkMultisamplePropertiesEXT*                 pMultisampleProperties) {
        return vkGetPhysicalDeviceMultisamplePropertiesEXT(physicalDevice, samples, pMultisampleProperties);
}

VkResult GetImageDrmFormatModifierPropertiesEXT(
    VkImage                                     image,
    VkImageDrmFormatModifierPropertiesEXT*      pProperties) {
        return vkGetImageDrmFormatModifierPropertiesEXT(device, image, pProperties);
}

VkResult CreateValidationCacheEXT(
    const VkValidationCacheCreateInfoEXT*       pCreateInfo,
    VkValidationCacheEXT*                       pValidationCache) {
        return vkCreateValidationCacheEXT(device, pCreateInfo, 0, pValidationCache);
}

void DestroyValidationCacheEXT(
    VkValidationCacheEXT                        validationCache) {
        return vkDestroyValidationCacheEXT(device, validationCache, 0);
}

VkResult MergeValidationCachesEXT(
    VkValidationCacheEXT                        dstCache,
    uint32_t                                    srcCacheCount,
    const VkValidationCacheEXT*                 pSrcCaches) {
        return vkMergeValidationCachesEXT(device, dstCache, srcCacheCount, pSrcCaches);
}

VkResult GetValidationCacheDataEXT(
    VkValidationCacheEXT                        validationCache,
    size_t*                                     pDataSize,
    void*                                       pData) {
        return vkGetValidationCacheDataEXT(device, validationCache, pDataSize, pData);
}

void CmdBindShadingRateImageNV(
    VkCommandBuffer                             commandBuffer,
    VkImageView                                 imageView,
    VkImageLayout                               imageLayout) {
        return vkCmdBindShadingRateImageNV(commandBuffer, imageView, imageLayout);
}

void CmdSetViewportShadingRatePaletteNV(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    firstViewport,
    uint32_t                                    viewportCount,
    const VkShadingRatePaletteNV*               pShadingRatePalettes) {
        return vkCmdSetViewportShadingRatePaletteNV(commandBuffer, firstViewport, viewportCount, pShadingRatePalettes);
}

void CmdSetCoarseSampleOrderNV(
    VkCommandBuffer                             commandBuffer,
    VkCoarseSampleOrderTypeNV                   sampleOrderType,
    uint32_t                                    customSampleOrderCount,
    const VkCoarseSampleOrderCustomNV*          pCustomSampleOrders) {
        return vkCmdSetCoarseSampleOrderNV(commandBuffer, sampleOrderType, customSampleOrderCount, pCustomSampleOrders);
}

VkResult CreateAccelerationStructureNV(
    const VkAccelerationStructureCreateInfoNV*  pCreateInfo,
    VkAccelerationStructureNV*                  pAccelerationStructure) {
        return vkCreateAccelerationStructureNV(device, pCreateInfo, 0, pAccelerationStructure);
}

void DestroyAccelerationStructureKHR(
    VkAccelerationStructureKHR                  accelerationStructure) {
        return vkDestroyAccelerationStructureKHR(device, accelerationStructure, 0);
}

void DestroyAccelerationStructureNV(
    VkAccelerationStructureKHR                  accelerationStructure) {
        return vkDestroyAccelerationStructureNV(device, accelerationStructure, 0);
}

void GetAccelerationStructureMemoryRequirementsNV(
    const VkAccelerationStructureMemoryRequirementsInfoNV* pInfo,
    VkMemoryRequirements2KHR*                   pMemoryRequirements) {
        return vkGetAccelerationStructureMemoryRequirementsNV(device, pInfo, pMemoryRequirements);
}

VkResult BindAccelerationStructureMemoryKHR(
    uint32_t                                    bindInfoCount,
    const VkBindAccelerationStructureMemoryInfoKHR* pBindInfos) {
        return vkBindAccelerationStructureMemoryKHR(device, bindInfoCount, pBindInfos);
}

VkResult BindAccelerationStructureMemoryNV(
    uint32_t                                    bindInfoCount,
    const VkBindAccelerationStructureMemoryInfoKHR* pBindInfos) {
        return vkBindAccelerationStructureMemoryNV(device, bindInfoCount, pBindInfos);
}

void CmdBuildAccelerationStructureNV(
    VkCommandBuffer                             commandBuffer,
    const VkAccelerationStructureInfoNV*        pInfo,
    VkBuffer                                    instanceData,
    VkDeviceSize                                instanceOffset,
    VkBool32                                    update,
    VkAccelerationStructureKHR                  dst,
    VkAccelerationStructureKHR                  src,
    VkBuffer                                    scratch,
    VkDeviceSize                                scratchOffset) {
        return vkCmdBuildAccelerationStructureNV(commandBuffer, pInfo, instanceData, instanceOffset, update, dst, src, scratch, scratchOffset);
}

void CmdCopyAccelerationStructureNV(
    VkCommandBuffer                             commandBuffer,
    VkAccelerationStructureKHR                  dst,
    VkAccelerationStructureKHR                  src,
    VkCopyAccelerationStructureModeKHR          mode) {
        return vkCmdCopyAccelerationStructureNV(commandBuffer, dst, src, mode);
}

void CmdTraceRaysNV(
    VkCommandBuffer                             commandBuffer,
    VkBuffer                                    raygenShaderBindingTableBuffer,
    VkDeviceSize                                raygenShaderBindingOffset,
    VkBuffer                                    missShaderBindingTableBuffer,
    VkDeviceSize                                missShaderBindingOffset,
    VkDeviceSize                                missShaderBindingStride,
    VkBuffer                                    hitShaderBindingTableBuffer,
    VkDeviceSize                                hitShaderBindingOffset,
    VkDeviceSize                                hitShaderBindingStride,
    VkBuffer                                    callableShaderBindingTableBuffer,
    VkDeviceSize                                callableShaderBindingOffset,
    VkDeviceSize                                callableShaderBindingStride,
    uint32_t                                    width,
    uint32_t                                    height,
    uint32_t                                    depth) {
        return vkCmdTraceRaysNV(commandBuffer, raygenShaderBindingTableBuffer, raygenShaderBindingOffset, missShaderBindingTableBuffer, missShaderBindingOffset, missShaderBindingStride, hitShaderBindingTableBuffer, hitShaderBindingOffset, hitShaderBindingStride, callableShaderBindingTableBuffer, callableShaderBindingOffset, callableShaderBindingStride, width, height, depth);
}

VkResult CreateRayTracingPipelinesNV(
    VkPipelineCache                             pipelineCache,
    uint32_t                                    createInfoCount,
    const VkRayTracingPipelineCreateInfoNV*     pCreateInfos,
    VkPipeline*                                 pPipelines) {
        return vkCreateRayTracingPipelinesNV(device, pipelineCache, createInfoCount, pCreateInfos, 0, pPipelines);
}

VkResult GetRayTracingShaderGroupHandlesKHR(
    VkPipeline                                  pipeline,
    uint32_t                                    firstGroup,
    uint32_t                                    groupCount,
    size_t                                      dataSize,
    void*                                       pData) {
        return vkGetRayTracingShaderGroupHandlesKHR(device, pipeline, firstGroup, groupCount, dataSize, pData);
}

VkResult GetRayTracingShaderGroupHandlesNV(
    VkPipeline                                  pipeline,
    uint32_t                                    firstGroup,
    uint32_t                                    groupCount,
    size_t                                      dataSize,
    void*                                       pData) {
        return vkGetRayTracingShaderGroupHandlesNV(device, pipeline, firstGroup, groupCount, dataSize, pData);
}

VkResult GetAccelerationStructureHandleNV(
    VkAccelerationStructureKHR                  accelerationStructure,
    size_t                                      dataSize,
    void*                                       pData) {
        return vkGetAccelerationStructureHandleNV(device, accelerationStructure, dataSize, pData);
}

void CmdWriteAccelerationStructuresPropertiesKHR(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    accelerationStructureCount,
    const VkAccelerationStructureKHR*           pAccelerationStructures,
    VkQueryType                                 queryType,
    VkQueryPool                                 queryPool,
    uint32_t                                    firstQuery) {
        return vkCmdWriteAccelerationStructuresPropertiesKHR(commandBuffer, accelerationStructureCount, pAccelerationStructures, queryType, queryPool, firstQuery);
}

void CmdWriteAccelerationStructuresPropertiesNV(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    accelerationStructureCount,
    const VkAccelerationStructureKHR*           pAccelerationStructures,
    VkQueryType                                 queryType,
    VkQueryPool                                 queryPool,
    uint32_t                                    firstQuery) {
        return vkCmdWriteAccelerationStructuresPropertiesNV(commandBuffer, accelerationStructureCount, pAccelerationStructures, queryType, queryPool, firstQuery);
}

VkResult CompileDeferredNV(
    VkPipeline                                  pipeline,
    uint32_t                                    shader) {
        return vkCompileDeferredNV(device, pipeline, shader);
}

VkResult GetMemoryHostPointerPropertiesEXT(
    VkExternalMemoryHandleTypeFlagBits          handleType,
    const void*                                 pHostPointer,
    VkMemoryHostPointerPropertiesEXT*           pMemoryHostPointerProperties) {
        return vkGetMemoryHostPointerPropertiesEXT(device, handleType, pHostPointer, pMemoryHostPointerProperties);
}

void CmdWriteBufferMarkerAMD(
    VkCommandBuffer                             commandBuffer,
    VkPipelineStageFlagBits                     pipelineStage,
    VkBuffer                                    dstBuffer,
    VkDeviceSize                                dstOffset,
    uint32_t                                    marker) {
        return vkCmdWriteBufferMarkerAMD(commandBuffer, pipelineStage, dstBuffer, dstOffset, marker);
}

VkResult GetPhysicalDeviceCalibrateableTimeDomainsEXT(
    uint32_t*                                   pTimeDomainCount,
    VkTimeDomainEXT*                            pTimeDomains) {
        return vkGetPhysicalDeviceCalibrateableTimeDomainsEXT(physicalDevice, pTimeDomainCount, pTimeDomains);
}

VkResult GetCalibratedTimestampsEXT(
    uint32_t                                    timestampCount,
    const VkCalibratedTimestampInfoEXT*         pTimestampInfos,
    uint64_t*                                   pTimestamps,
    uint64_t*                                   pMaxDeviation) {
        return vkGetCalibratedTimestampsEXT(device, timestampCount, pTimestampInfos, pTimestamps, pMaxDeviation);
}

void CmdDrawMeshTasksNV(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    taskCount,
    uint32_t                                    firstTask) {
        return vkCmdDrawMeshTasksNV(commandBuffer, taskCount, firstTask);
}

void CmdDrawMeshTasksIndirectNV(
    VkCommandBuffer                             commandBuffer,
    VkBuffer                                    buffer,
    VkDeviceSize                                offset,
    uint32_t                                    drawCount,
    uint32_t                                    stride) {
        return vkCmdDrawMeshTasksIndirectNV(commandBuffer, buffer, offset, drawCount, stride);
}

void CmdDrawMeshTasksIndirectCountNV(
    VkCommandBuffer                             commandBuffer,
    VkBuffer                                    buffer,
    VkDeviceSize                                offset,
    VkBuffer                                    countBuffer,
    VkDeviceSize                                countBufferOffset,
    uint32_t                                    maxDrawCount,
    uint32_t                                    stride) {
        return vkCmdDrawMeshTasksIndirectCountNV(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount, stride);
}

void CmdSetExclusiveScissorNV(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    firstExclusiveScissor,
    uint32_t                                    exclusiveScissorCount,
    const VkRect2D*                             pExclusiveScissors) {
        return vkCmdSetExclusiveScissorNV(commandBuffer, firstExclusiveScissor, exclusiveScissorCount, pExclusiveScissors);
}

void CmdSetCheckpointNV(
    VkCommandBuffer                             commandBuffer,
    const void*                                 pCheckpointMarker) {
        return vkCmdSetCheckpointNV(commandBuffer, pCheckpointMarker);
}

void GetQueueCheckpointDataNV(
    VkQueue                                     queue,
    uint32_t*                                   pCheckpointDataCount,
    VkCheckpointDataNV*                         pCheckpointData) {
        return vkGetQueueCheckpointDataNV(queue, pCheckpointDataCount, pCheckpointData);
}

VkResult InitializePerformanceApiINTEL(
    const VkInitializePerformanceApiInfoINTEL*  pInitializeInfo) {
        return vkInitializePerformanceApiINTEL(device, pInitializeInfo);
}

void UninitializePerformanceApiINTEL() {
        return vkUninitializePerformanceApiINTEL(device);
}

VkResult CmdSetPerformanceMarkerINTEL(
    VkCommandBuffer                             commandBuffer,
    const VkPerformanceMarkerInfoINTEL*         pMarkerInfo) {
        return vkCmdSetPerformanceMarkerINTEL(commandBuffer, pMarkerInfo);
}

VkResult CmdSetPerformanceStreamMarkerINTEL(
    VkCommandBuffer                             commandBuffer,
    const VkPerformanceStreamMarkerInfoINTEL*   pMarkerInfo) {
        return vkCmdSetPerformanceStreamMarkerINTEL(commandBuffer, pMarkerInfo);
}

VkResult CmdSetPerformanceOverrideINTEL(
    VkCommandBuffer                             commandBuffer,
    const VkPerformanceOverrideInfoINTEL*       pOverrideInfo) {
        return vkCmdSetPerformanceOverrideINTEL(commandBuffer, pOverrideInfo);
}

VkResult AcquirePerformanceConfigurationINTEL(
    const VkPerformanceConfigurationAcquireInfoINTEL* pAcquireInfo,
    VkPerformanceConfigurationINTEL*            pConfiguration) {
        return vkAcquirePerformanceConfigurationINTEL(device, pAcquireInfo, pConfiguration);
}

VkResult ReleasePerformanceConfigurationINTEL(
    VkPerformanceConfigurationINTEL             configuration) {
        return vkReleasePerformanceConfigurationINTEL(device, configuration);
}

VkResult QueueSetPerformanceConfigurationINTEL(
    VkQueue                                     queue,
    VkPerformanceConfigurationINTEL             configuration) {
        return vkQueueSetPerformanceConfigurationINTEL(queue, configuration);
}

VkResult GetPerformanceParameterINTEL(
    VkPerformanceParameterTypeINTEL             parameter,
    VkPerformanceValueINTEL*                    pValue) {
        return vkGetPerformanceParameterINTEL(device, parameter, pValue);
}

void SetLocalDimmingAMD(
    VkSwapchainKHR                              swapChain,
    VkBool32                                    localDimmingEnable) {
        return vkSetLocalDimmingAMD(device, swapChain, localDimmingEnable);
}

VkDeviceAddress GetBufferDeviceAddressEXT(
    const VkBufferDeviceAddressInfo*            pInfo) {
        return vkGetBufferDeviceAddressEXT(device, pInfo);
}

VkResult GetPhysicalDeviceToolPropertiesEXT(
    uint32_t*                                   pToolCount,
    VkPhysicalDeviceToolPropertiesEXT*          pToolProperties) {
        return vkGetPhysicalDeviceToolPropertiesEXT(physicalDevice, pToolCount, pToolProperties);
}

VkResult GetPhysicalDeviceCooperativeMatrixPropertiesNV(
    uint32_t*                                   pPropertyCount,
    VkCooperativeMatrixPropertiesNV*            pProperties) {
        return vkGetPhysicalDeviceCooperativeMatrixPropertiesNV(physicalDevice, pPropertyCount, pProperties);
}

VkResult GetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV(
    uint32_t*                                   pCombinationCount,
    VkFramebufferMixedSamplesCombinationNV*     pCombinations) {
        return vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV(physicalDevice, pCombinationCount, pCombinations);
}

VkResult CreateHeadlessSurfaceEXT(
    const VkHeadlessSurfaceCreateInfoEXT*       pCreateInfo,
    VkSurfaceKHR*                               pSurface) {
        return vkCreateHeadlessSurfaceEXT(instance, pCreateInfo, 0, pSurface);
}

void CmdSetLineStippleEXT(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    lineStippleFactor,
    uint16_t                                    lineStipplePattern) {
        return vkCmdSetLineStippleEXT(commandBuffer, lineStippleFactor, lineStipplePattern);
}

void ResetQueryPoolEXT(
    VkQueryPool                                 queryPool,
    uint32_t                                    firstQuery,
    uint32_t                                    queryCount) {
        return vkResetQueryPoolEXT(device, queryPool, firstQuery, queryCount);
}

void GetGeneratedCommandsMemoryRequirementsNV(
    const VkGeneratedCommandsMemoryRequirementsInfoNV* pInfo,
    VkMemoryRequirements2*                      pMemoryRequirements) {
        return vkGetGeneratedCommandsMemoryRequirementsNV(device, pInfo, pMemoryRequirements);
}

void CmdPreprocessGeneratedCommandsNV(
    VkCommandBuffer                             commandBuffer,
    const VkGeneratedCommandsInfoNV*            pGeneratedCommandsInfo) {
        return vkCmdPreprocessGeneratedCommandsNV(commandBuffer, pGeneratedCommandsInfo);
}

void CmdExecuteGeneratedCommandsNV(
    VkCommandBuffer                             commandBuffer,
    VkBool32                                    isPreprocessed,
    const VkGeneratedCommandsInfoNV*            pGeneratedCommandsInfo) {
        return vkCmdExecuteGeneratedCommandsNV(commandBuffer, isPreprocessed, pGeneratedCommandsInfo);
}

void CmdBindPipelineShaderGroupNV(
    VkCommandBuffer                             commandBuffer,
    VkPipelineBindPoint                         pipelineBindPoint,
    VkPipeline                                  pipeline,
    uint32_t                                    groupIndex) {
        return vkCmdBindPipelineShaderGroupNV(commandBuffer, pipelineBindPoint, pipeline, groupIndex);
}

VkResult CreateIndirectCommandsLayoutNV(
    const VkIndirectCommandsLayoutCreateInfoNV* pCreateInfo,
    VkIndirectCommandsLayoutNV*                 pIndirectCommandsLayout) {
        return vkCreateIndirectCommandsLayoutNV(device, pCreateInfo, 0, pIndirectCommandsLayout);
}

void DestroyIndirectCommandsLayoutNV(
    VkIndirectCommandsLayoutNV                  indirectCommandsLayout) {
        return vkDestroyIndirectCommandsLayoutNV(device, indirectCommandsLayout, 0);
}

VkResult CreatePrivateDataSlotEXT(
    const VkPrivateDataSlotCreateInfoEXT*       pCreateInfo,
    VkPrivateDataSlotEXT*                       pPrivateDataSlot) {
        return vkCreatePrivateDataSlotEXT(device, pCreateInfo, 0, pPrivateDataSlot);
}

void DestroyPrivateDataSlotEXT(
    VkPrivateDataSlotEXT                        privateDataSlot) {
        return vkDestroyPrivateDataSlotEXT(device, privateDataSlot, 0);
}

VkResult SetPrivateDataEXT(
    VkObjectType                                objectType,
    uint64_t                                    objectHandle,
    VkPrivateDataSlotEXT                        privateDataSlot,
    uint64_t                                    data) {
        return vkSetPrivateDataEXT(device, objectType, objectHandle, privateDataSlot, data);
}

void GetPrivateDataEXT(
    VkObjectType                                objectType,
    uint64_t                                    objectHandle,
    VkPrivateDataSlotEXT                        privateDataSlot,
    uint64_t*                                   pData) {
        return vkGetPrivateDataEXT(device, objectType, objectHandle, privateDataSlot, pData);
}

#include "stdio.h"
void InitVulkanSimple(const char* surface_extension)
{
    printf("Surface extension is %s\n", surface_extension);
	load_fn();

    const char* extensions[] = { "VK_KHR_surface", surface_extension };
    const char* layers[] = { "VK_LAYER_KHRONOS_validation" };
    const char* device_ext[] = { "VK_KHR_swapchain" };

	VkApplicationInfo appinfo = { 0 };
    appinfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appinfo.engineVersion = VK_MAKE_VERSION(1, 2, 0);
	appinfo.applicationVersion = VK_MAKE_VERSION(1, 2, 0);
	appinfo.apiVersion = VK_MAKE_VERSION(1, 2, 0);

	VkInstanceCreateInfo info = { 0 };
    info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	info.pApplicationInfo = &appinfo;
	info.pNext = 0;
	info.enabledLayerCount = sizeof(layers) / sizeof(char*);
	info.ppEnabledLayerNames = layers;
	info.enabledExtensionCount = sizeof(extensions) / sizeof(char*);
	info.ppEnabledExtensionNames = extensions;

	VkResult res = vkCreateInstance(&info, 0, &instance);
	printf("Instance created %d\n", res);
	load_instance_fn(instance);

	VkPhysicalDevice devices[16];
	uint count;
	vkEnumeratePhysicalDevices(instance, &count, 0);
	vkEnumeratePhysicalDevices(instance, &count, devices);

	physicalDevice = devices[0];
	for (uint i = 0; i < count; ++i)
	{
		VkPhysicalDeviceProperties props;
		vkGetPhysicalDeviceProperties(devices[i], &props);
		if (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
		{
			physicalDevice = devices[i];
		}
	}

	float prio = 1;
	VkDeviceQueueCreateInfo qinfo = { 0 };
    qinfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	qinfo.queueCount = 1;
	qinfo.pQueuePriorities = &prio;

	VkPhysicalDeviceFeatures features = { 0 };
	features.fillModeNonSolid = 1;
	features.sampleRateShading = 1;
	features.vertexPipelineStoresAndAtomics = 1;
	features.samplerAnisotropy = 1;

	VkPhysicalDeviceDescriptorIndexingFeatures extFeatures = { 0 };
    extFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DESCRIPTOR_INDEXING_FEATURES;
	extFeatures.descriptorBindingPartiallyBound = 1;
	extFeatures.descriptorBindingVariableDescriptorCount = 1;

	VkDeviceCreateInfo deviceInfo = { 0 };
    deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceInfo.queueCreateInfoCount = 1;
	deviceInfo.pNext = &extFeatures;
	deviceInfo.pQueueCreateInfos = &qinfo;
	deviceInfo.pEnabledFeatures = &features;
	deviceInfo.enabledExtensionCount = sizeof(device_ext) / sizeof(char*);
	deviceInfo.ppEnabledExtensionNames = device_ext;
	deviceInfo.enabledLayerCount = sizeof(layers) / sizeof(char*);
	deviceInfo.ppEnabledLayerNames = layers;


	res = vkCreateDevice(physicalDevice, &deviceInfo, 0, &device);
    printf("Device created %d\n", res);
	load_device_fn(device);
	vkGetDeviceQueue(device, 0, 0, &queue);
	VkPhysicalDeviceMemoryProperties props;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &props);
	memory_idx[0] = 0xffffffff;
	memory_idx[1] = 0xffffffff;
    
	for (uint i = 0; i < props.memoryTypeCount; ++i)
	{
		VkMemoryPropertyFlags flag = props.memoryTypes[i].propertyFlags;
		if (memory_idx[0] == 0xffffffff && (flag & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)) memory_idx[0] = i;
		if (memory_idx[1] == 0xffffffff && (flag & (VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT))) memory_idx[1] = i;
	}

	VkCommandPoolCreateInfo poolInfo = { 0 };
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    vkCreateCommandPool(device, &poolInfo, 0, &transient_pool);
}