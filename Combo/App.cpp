// App.cpp

#include "stdafx.h"
#include "App.h"

FControl::FControl()
	: StepDirection{ 0, 0, 0 }
	, CameraPos{ 0, 0, -10, 1 }
	, ViewMode(EViewMode::Solid)
	, DoPost(!true)
	, DoMSAA(false)
{
}

FControl GRequestControl;
FControl GControl;

FVector4 GCameraPos = { 0, 0, -10, 1 };

bool DoInit(HINSTANCE hInstance, HWND hWnd, uint32& Width, uint32& Height)
{
	LPSTR CmdLine = ::GetCommandLineA();
	const char* Token = CmdLine;
	bool bD3D12 = false;
	bool bVulkan = false;
	while (Token = strchr(Token, ' '))
	{
		++Token;
		if (!_strcmpi(Token, "-debugger"))
		{
			while (!::IsDebuggerPresent())
			{
				Sleep(0);
			}
		}
		else if (!_strcmpi(Token, "-d3d12"))
		{
			bD3D12 = true;
		}
		else if (!_strcmpi(Token, "-vulkan"))
		{
			bVulkan = true;
		}
	}
	check(bVulkan ^ bD3D12);
	return true;
}

void DoRender()
{

}

void DoResize(uint32 Width, uint32 Height)
{

}

void DoDeinit()
{

}
