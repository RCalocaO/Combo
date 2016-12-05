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
}

bool FD3D12Engine::CreateInstance(HINSTANCE hInstance, HWND hWnd)
{
	Instance.Create(hInstance, hWnd);

	return true;
}

bool FD3D12Engine::CreateDevice()
{
	return true;
}
