#ifndef _DIRECTX12HEADERS_H_
#define _DIRECTX12HEADERS_H_

//
//	Predefs
//
#include "DLLIE.h"

//
//	Libs
//
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

//
//	Includes
//
#include <d3d12.h>
#include <dxgi1_4.h>
#include <DirectXMath.h>


#define GD_DEVICE_TYPE			ID3D11Device
#define GD_DEVICE_CONTEXT		ID3D11DeviceContext
#define GD_SHADER_RESOURCE		ID3D11ShaderResourceView
#define GD_BLOB					ID3D10Blob
#define GD_BUFFER				ID3D11Buffer

#define GD_SHADER_VERTEX		ID3D11VertexShader
#define GD_SHADER_PIXEL			ID3D11PixelShader
#define GD_SHADER_INPUTLAYOUT	ID3D11InputLayout


#endif