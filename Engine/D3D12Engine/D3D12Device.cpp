// D3D12Device.cpp

#include "stdafx.h"
#include "D3D12Device.h"

static bool GSkipValidation = false;

void FD3D12Instance::SetupDebugLayer()
{
	if (!GSkipValidation)
	{
		checkD3D12(D3D12GetDebugInterface(IID_PPV_ARGS(&DebugInterface)));
		if (DebugInterface)
		{
			DebugInterface->EnableDebugLayer();
		}
	}
}
