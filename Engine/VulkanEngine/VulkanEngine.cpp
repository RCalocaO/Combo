// VulkanEngine.cpp

#include "stdafx.h"
#include "VulkanEngine.h"

#pragma comment(lib, "vulkan-1.lib")

FEngine* CreateVulkanEngine()
{
	return new FVulkanEngine;
}

FVulkanEngine::~FVulkanEngine()
{

}

void FVulkanEngine::Deinit()
{
}

bool FVulkanEngine::CreateInstance(HINSTANCE hInstance, HWND hWnd)
{
	Instance.Create(hInstance, hWnd);

	return true;
}

bool FVulkanEngine::CreateDevice()
{
	FVulkanDevice* Device = nullptr;

	uint32 NumDevices;
	checkVk(vkEnumeratePhysicalDevices(Instance.Instance, &NumDevices, nullptr));
	std::vector<VkPhysicalDevice> PhysicalDevices;
	PhysicalDevices.resize(NumDevices);
	checkVk(vkEnumeratePhysicalDevices(Instance.Instance, &NumDevices, &PhysicalDevices[0]));

	for (uint32 Index = 0; Index < NumDevices; ++Index)
	{
		VkPhysicalDeviceProperties DeviceProperties;
		vkGetPhysicalDeviceProperties(PhysicalDevices[Index], &DeviceProperties);

		uint32 NumQueueFamilies;
		vkGetPhysicalDeviceQueueFamilyProperties(PhysicalDevices[Index], &NumQueueFamilies, nullptr);
		std::vector<VkQueueFamilyProperties> QueueFamilies;
		QueueFamilies.resize(NumQueueFamilies);
		vkGetPhysicalDeviceQueueFamilyProperties(PhysicalDevices[Index], &NumQueueFamilies, &QueueFamilies[0]);

		for (uint32 QueueIndex = 0; QueueIndex < NumQueueFamilies; ++QueueIndex)
		{
			VkBool32 bSupportsPresent;
			checkVk(vkGetPhysicalDeviceSurfaceSupportKHR(PhysicalDevices[Index], QueueIndex, Instance.Surface, &bSupportsPresent));
			if (bSupportsPresent && QueueFamilies[QueueIndex].queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				Device = new FVulkanDevice;
				Device->PhysicalDevice = PhysicalDevices[Index];
				Device->DeviceProperties = DeviceProperties;
				Device->PresentQueueFamilyIndex = QueueIndex;
				goto Found;
			}
		}
	}

	// Not found!
	check(0);
	return false;

Found:
	Device->Create(Instance.Layers);
	Devices.push_back(Device);
	return true;
}
