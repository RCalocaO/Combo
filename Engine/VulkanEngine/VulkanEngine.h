// VulkanEngine.h

#pragma once

#include "../Engine.h"
#include "VulkanDevice.h"

struct FVulkanEngine : public FEngine
{
	virtual ~FVulkanEngine();

	virtual void Deinit() override;

	virtual bool CreateInstance(HINSTANCE hInstance, HWND hWnd) override;
	virtual bool CreateDevice() override;


	FInstance Instance;
};
