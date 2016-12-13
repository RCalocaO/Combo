// D3D12Device.h

#pragma once
#include <dxgi1_4.h>
#include <d3d12.h>
#include <wrl.h>
#include <d3dcompiler.h>

#include "../../Utils/Util.h"

#define checkD3D12(r) do { HRESULT hr = r; check(SUCCEEDED(hr)); hr = hr; } while (0)

struct FD3D12Instance
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

	void Destroy()
	{
		DestroyInstance();
	}

	Microsoft::WRL::ComPtr<ID3D12Debug> DebugInterface;
	Microsoft::WRL::ComPtr<IDXGIFactory4> DXGIFactory;
};

struct FD3D12Device
{
	Microsoft::WRL::ComPtr<ID3D12Device> Device;
	Microsoft::WRL::ComPtr<IDXGIAdapter1> Adapter;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> Queue;

	void Create()
	{
		checkD3D12(D3D12CreateDevice(Adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), &Device));

		D3D12_COMMAND_QUEUE_DESC QueueDesc;
		MemZero(QueueDesc);
		QueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		QueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
		QueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		checkD3D12(Device->CreateCommandQueue(&QueueDesc, IID_PPV_ARGS(&Queue)));
	}

	void Destroy()
	{
		Queue = nullptr;
		Device = nullptr;
		Adapter = nullptr;
	}

	operator ID3D12Device* ()
	{
		return Device.Get();
	}
};
