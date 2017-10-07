////////////////////////////////////////////////////////////////////////////////////////////////////////
// Part of Injectable Generic Camera System
// Copyright(c) 2017, Frans Bouma
// All rights reserved.
// https://github.com/FransBouma/InjectableGenericCameraSystem
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met :
//
//  * Redistributions of source code must retain the above copyright notice, this
//	  list of conditions and the following disclaimer.
//
//  * Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and / or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED.IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "ShaderToggleManager.h"
#include "OverlayConsole.h"
#include <d3d11.h> 
#include <D3Dcompiler.h> 

#pragma comment(lib,"D3dcompiler.lib")

namespace IGCS
{
	ShaderToggleManager::ShaderToggleManager() : _compiledDiscardPixelShaderBlob(nullptr), _discardingPixelShader(nullptr)
	{
	}


	void ShaderToggleManager::addShader(const void *buffer, size_t bufferLength, __int64 shaderInstanceAddress)
	{
		__int64 hash = calculateFNVHash(buffer, bufferLength);
		_shaderHashLock.lock();
			_shaderHashPerShaderObjectAddress[shaderInstanceAddress] = hash;
		_shaderHashLock.unlock();
	}


	void ShaderToggleManager::reset()
	{
		if (nullptr != _discardingPixelShader)
		{
			_discardingPixelShader->Release();
			_discardingPixelShader = nullptr;
		}
		clearShaderHashMap();
	}


	void ShaderToggleManager::init(ID3D11Device *device)
	{
		reset();

		// compile our discard shader
		char* discardingPixelShader =
			"float4 discardingPixelShader() : SV_TARGET0"
			"{"
			"    if(true)"
			"    {"
			"        discard;"
			"    }"
			"    return float4(1, 1, 1, 0);"
			"}";
		ID3DBlob* errorBlob;
		HRESULT hr = D3DCompile(discardingPixelShader, strnlen(discardingPixelShader, 1024), "discardingPixelShader", NULL, NULL, "discardingPixelShader", "ps_4_0", 
								0, 0, &_compiledDiscardPixelShaderBlob, &errorBlob);
		if (FAILED(hr))
		{
			OverlayConsole::instance().logError("Compiling discardingPixelShader FAILED! Error code: %d.", hr);
			OverlayConsole::instance().logError("Compile error: %s", (const char*)errorBlob->GetBufferPointer());
			_compiledDiscardPixelShaderBlob = nullptr;
		}
		else
		{
			// upload to D3D
			hr = device->CreatePixelShader(_compiledDiscardPixelShaderBlob->GetBufferPointer(), _compiledDiscardPixelShaderBlob->GetBufferSize(), NULL, &_discardingPixelShader);
			if (FAILED(hr))
			{
				OverlayConsole::instance().logError("Creating discardingPixelShader FAILED! Error code: %d", hr);
			}
			_compiledDiscardPixelShaderBlob->Release();
			_compiledDiscardPixelShaderBlob = nullptr;
		}
	}


	void ShaderToggleManager::clearShaderHashMap()
	{
		_shaderHashLock.lock();
			_shaderHashPerShaderObjectAddress.clear();
		_shaderHashLock.unlock();
	}


	// From 3DMigoto, util.h. 
	__int64 ShaderToggleManager::calculateFNVHash(const void *buffer, size_t bufferLength)
	{
		__int64 hval = 0;
		unsigned const char *bp = (unsigned const char *)buffer;	/* start of buffer */
		unsigned const char *be = bp + bufferLength;				/* beyond end of buffer */

		// FNV-1 hash each octet of the buffer
		while (bp < be)
		{
			// multiply by the 64 bit FNV magic prime mod 2^64
			// 64 bit magic FNV-0 and FNV-1 prime
			hval *= ((__int64)0x100000001b3ULL);
			// xor the bottom with the current octet
			hval ^= (__int64)*bp++;
		}
		return hval;
	}
}
