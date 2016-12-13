// D3D12Engine.cpp

#include "stdafx.h"
#include "D3D12Engine.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

FEngine* CreateD3D12Engine()
{
	return new FD3D12Engine;
}

FD3D12Engine::~FD3D12Engine()
{

}

void FD3D12Engine::Deinit()
{
	for (auto* Device : Devices)
	{
		Device->Destroy();
		delete Device;
	}
	Devices.clear();
}

bool FD3D12Engine::CreateInstance(HINSTANCE hInstance, HWND hWnd)
{
	Instance.Create(hInstance, hWnd);

	return true;
}

bool FD3D12Engine::CreateDevice()
{
	auto* Device = new FD3D12Device;
	Device->Create();
	Devices.push_back(Device);
	return true;
}
