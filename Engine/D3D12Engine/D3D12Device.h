// VulkanDevice.h

#pragma once
#include <dxgi1_4.h>
#include <d3d12.h>
#include <wrl.h>
#include <d3dcompiler.h>

#include "../../Utils/Util.h"

#define checkD3D12(r) do { HRESULT hr = r; check(SUCCEEDED(hr)); hr = hr; } while (0)

struct FInstance
{
	void CreateInstance()
	{
		SetupDebugLayer();
		//DXGI_CREATE_FACTORY_DEBUG
		checkD3D12(CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&DXGIFactory)));
	}

	void SetupDebugLayer();

	void DestroyInstance()
	{
		DebugInterface = nullptr;
		DXGIFactory = nullptr;
	}

	void Create(HINSTANCE hInstance, HWND hWnd)
	{
		CreateInstance();
	}

	void CreateDevice(struct FDevice& OutDevice);

	void Destroy()
	{
		DestroyInstance();
	}

	Microsoft::WRL::ComPtr<ID3D12Debug> DebugInterface;
	Microsoft::WRL::ComPtr<IDXGIFactory4> DXGIFactory;
};
