// D3D12Engine.h

#pragma once

#include "../Engine.h"
#include "D3D12Device.h"

struct FD3D12Engine : public FEngine
{
	virtual ~FD3D12Engine();

	virtual void Deinit() override;

	virtual bool CreateInstance(HINSTANCE hInstance, HWND hWnd) override;
	virtual bool CreateDevice() override;


	FInstance Instance;
};
