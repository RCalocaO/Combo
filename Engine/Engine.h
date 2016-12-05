// Engine.h

#pragma once


struct FEngine
{
	virtual ~FEngine() {}

	virtual void Deinit() = 0;

	virtual bool CreateInstance(HINSTANCE hInstance, HWND hWnd) = 0;
	virtual bool CreateDevice() = 0;
};

extern FEngine* CreateD3D12Engine();
extern FEngine* CreateVulkanEngine();
