// VulkanDevice.h

#pragma once

#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>

#include "../../Utils/Util.h"

#define checkVk(r) check((r) == VK_SUCCESS)

struct FVulkanInstance
{
	VkSurfaceKHR Surface = VK_NULL_HANDLE;
	VkInstance Instance = VK_NULL_HANDLE;
	VkDebugReportCallbackEXT DebugReportCB = VK_NULL_HANDLE;

	std::vector<const char*> Layers;

	void GetInstanceLayersAndExtensions(std::vector<const char*>& OutLayers, std::vector<const char*>& OutExtensions);

	void CreateInstance()
	{
		VkApplicationInfo AppInfo;
		MemZero(AppInfo);
		AppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		AppInfo.pApplicationName = "Test0";
		AppInfo.pEngineName = "VkTest";
		AppInfo.engineVersion = 1;
		AppInfo.apiVersion = VK_API_VERSION_1_0;

		VkInstanceCreateInfo InstanceInfo;
		MemZero(InstanceInfo);
		InstanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		InstanceInfo.pApplicationInfo = &AppInfo;

		std::vector<const char*> InstanceLayers;
		std::vector<const char*> InstanceExtensions;
		GetInstanceLayersAndExtensions(InstanceLayers, InstanceExtensions);

		InstanceInfo.enabledLayerCount = (uint32)InstanceLayers.size();
		InstanceInfo.ppEnabledLayerNames = InstanceLayers.size() > 0 ? &InstanceLayers[0] : nullptr;
		InstanceInfo.enabledExtensionCount = (uint32)InstanceExtensions.size();
		InstanceInfo.ppEnabledExtensionNames = InstanceExtensions.size() > 0 ? &InstanceExtensions[0] : nullptr;

		checkVk(vkCreateInstance(&InstanceInfo, nullptr, &Instance));

		Layers = InstanceLayers;
	}

	void DestroyInstance()
	{
		vkDestroyInstance(Instance, nullptr);
		Instance = VK_NULL_HANDLE;
	}

	void CreateSurface(HINSTANCE hInstance, HWND hWnd)
	{
		VkWin32SurfaceCreateInfoKHR SurfaceInfo;
		MemZero(SurfaceInfo);
		SurfaceInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		SurfaceInfo.hinstance = hInstance;
		SurfaceInfo.hwnd = hWnd;
		checkVk(vkCreateWin32SurfaceKHR(Instance, &SurfaceInfo, nullptr, &Surface));
	}

	void DestroySurface()
	{
		vkDestroySurfaceKHR(Instance, Surface, nullptr);
		Surface = VK_NULL_HANDLE;
	}

	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugReportCallback(
		VkDebugReportFlagsEXT Flags,
		VkDebugReportObjectTypeEXT ObjectType,
		uint64_t Object,
		size_t Location,
		int32_t MessageCode,
		const char* LayerPrefix,
		const char* Message,
		void* UserData);

	void CreateDebugCallback()
	{
		auto* CreateCB = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(Instance, "vkCreateDebugReportCallbackEXT");
		if (CreateCB)
		{
			VkDebugReportCallbackCreateInfoEXT CreateInfo;
			MemZero(CreateInfo);
			CreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
			CreateInfo.pfnCallback = &DebugReportCallback;
			CreateInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT | VK_DEBUG_REPORT_INFORMATION_BIT_EXT/* | VK_DEBUG_REPORT_DEBUG_BIT_EXT*/;
			checkVk((*CreateCB)(Instance, &CreateInfo, nullptr, &DebugReportCB));
		}
	}

	void DestroyDebugCallback()
	{
		auto* DestroyCB = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(Instance, "vkDestroyDebugReportCallbackEXT");
		if (DestroyCB)
		{
			(*DestroyCB)(Instance, DebugReportCB, nullptr);
		}
	}

	void Create(HINSTANCE hInstance, HWND hWnd)
	{
		CreateInstance();
		CreateDebugCallback();
		CreateSurface(hInstance, hWnd);
	}

	void Destroy()
	{
		DestroySurface();
		DestroyDebugCallback();
		DestroyInstance();
	}
};

struct FVulkanDevice
{
	VkPhysicalDevice PhysicalDevice = VK_NULL_HANDLE;
	VkDevice Device = VK_NULL_HANDLE;
	VkPhysicalDeviceProperties DeviceProperties;
	uint32 PresentQueueFamilyIndex = UINT32_MAX;
	VkQueue PresentQueue = VK_NULL_HANDLE;

	void Create(std::vector<const char*>& Layers)
	{
		VkPhysicalDeviceFeatures DeviceFeatures;
		vkGetPhysicalDeviceFeatures(PhysicalDevice, &DeviceFeatures);

		VkDeviceQueueCreateInfo QueueInfo;
		MemZero(QueueInfo);
		QueueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		QueueInfo.queueFamilyIndex = PresentQueueFamilyIndex;
		QueueInfo.queueCount = 1;
		float Priorities[1] = { 1.0f };
		QueueInfo.pQueuePriorities = Priorities;

		const char* DeviceExtensions[] = { "VK_KHR_swapchain" };

		VkDeviceCreateInfo DeviceInfo;
		MemZero(DeviceInfo);
		DeviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		DeviceInfo.queueCreateInfoCount = 1;
		DeviceInfo.pQueueCreateInfos = &QueueInfo;
		DeviceInfo.enabledLayerCount = (uint32)Layers.size();
		DeviceInfo.ppEnabledLayerNames = Layers.size() > 0 ? &Layers[0] : nullptr;
		DeviceInfo.enabledExtensionCount = 1;
		DeviceInfo.ppEnabledExtensionNames = DeviceExtensions;
		DeviceInfo.pEnabledFeatures = &DeviceFeatures;
		checkVk(vkCreateDevice(PhysicalDevice, &DeviceInfo, nullptr, &Device));

		vkGetDeviceQueue(Device, PresentQueueFamilyIndex, 0, &PresentQueue);
	}

	void Destroy()
	{
		vkDestroyDevice(Device, nullptr);
		Device = VK_NULL_HANDLE;
	}
};
