// VulkanDevice.cpp

#include "stdafx.h"
#include "VulkanDevice.h"

static bool GSkipValidation = false;

void FVulkanInstance::GetInstanceLayersAndExtensions(std::vector<const char*>& OutLayers, std::vector<const char*>& OutExtensions)
{
	if (!GSkipValidation)
	{
		uint32 NumLayers;
		checkVk(vkEnumerateInstanceLayerProperties(&NumLayers, nullptr));
		if (NumLayers > 0)
		{
			std::vector<VkLayerProperties> InstanceProperties;
			InstanceProperties.resize(NumLayers);

			checkVk(vkEnumerateInstanceLayerProperties(&NumLayers, &InstanceProperties[0]));

			for (auto& Property : InstanceProperties)
			{
				std::string s = "Found Layer: ";
				s += Property.layerName;
				s += "\n";
				::OutputDebugStringA(s.c_str());
			}

			const char* UseValidationLayers[] =
			{
				//"VK_LAYER_LUNARG_api_dump",
				"VK_LAYER_LUNARG_standard_validation",
				"VK_LAYER_LUNARG_image",
				"VK_LAYER_LUNARG_object_tracker",
				"VK_LAYER_LUNARG_parameter_validation",
				"VK_LAYER_LUNARG_screenshot",
				"VK_LAYER_LUNARG_swapchain",
				"VK_LAYER_GOOGLE_threading",
				"VK_LAYER_GOOGLE_unique_objects",
				"VK_LAYER_RENDERDOC_Capture",
			};

			for (auto* DesiredLayer : UseValidationLayers)
			{
				for (auto& Prop : InstanceProperties)
				{
					if (!strcmp(Prop.layerName, DesiredLayer))
					{
						OutLayers.push_back(DesiredLayer);
						// Should probably remove it from InstanceProperties array...
						break;
					}
				}
			}
		}
	}

	{
		uint32 NumExtensions;
		checkVk(vkEnumerateInstanceExtensionProperties(nullptr, &NumExtensions, nullptr));

		std::vector<VkExtensionProperties> ExtensionsProperties;
		ExtensionsProperties.resize(NumExtensions);

		checkVk(vkEnumerateInstanceExtensionProperties(nullptr, &NumExtensions, &ExtensionsProperties[0]));

		const char* UseExtensions[] =
		{
			"VK_KHR_surface",
			"VK_KHR_win32_surface",
			"VK_EXT_debug_report",
		};
		for (auto* DesiredExtension : UseExtensions)
		{
			for (auto& Extension : ExtensionsProperties)
			{
				if (!strcmp(Extension.extensionName, DesiredExtension))
				{
					OutExtensions.push_back(DesiredExtension);
					// Should probably remove it from ExtensionsProperties array...
					break;
				}
			}
		}
	}
}

VkBool32 FVulkanInstance::DebugReportCallback(VkDebugReportFlagsEXT Flags, VkDebugReportObjectTypeEXT ObjectType, uint64_t Object, size_t Location, int32_t MessageCode, const char* LayerPrefix, const char* Message, void* UserData)
{
	int n = 0;
	if (Flags & VK_DEBUG_REPORT_ERROR_BIT_EXT)
	{
		char s[2048];
		sprintf_s(s, "<VK>Error[%s:%d] %s\n", LayerPrefix, MessageCode, Message);
		::OutputDebugStringA(s);
		check(0);
		++n;
	}
	else if (Flags & VK_DEBUG_REPORT_WARNING_BIT_EXT)
	{
		char s[2048];
		sprintf_s(s, "<VK>Warn: %s\n", Message);
		::OutputDebugStringA(s);
		++n;
	}
	else if (Flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT)
	{
		char s[2048];
		sprintf_s(s, "<VK>Perf: %s\n", Message);
		::OutputDebugStringA(s);
		++n;
	}
	else if (1)
	{
		static const char* SkipPrefixes[] =
		{
			"ObjectTracker",
			"OBJTRACK",
			"loader",
			"MEM",
			"DS",
		};
		for (uint32 Index = 0; Index < ARRAYSIZE(SkipPrefixes); ++Index)
		{
			if (!strcmp(LayerPrefix, SkipPrefixes[Index]))
			{
				return false;
			}
		}

		uint32 Size = (uint32)strlen(Message) + 100;
		auto* s = new char[Size];
		snprintf(s, Size - 1, "<VK>: %s\n", Message);
		::OutputDebugStringA(s);
		delete[] s;
	}

	return false;
}
