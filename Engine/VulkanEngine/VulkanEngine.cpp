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
	return true;
}
