#ifndef _DEVICE_H_
#define _DEVICE_H_

//#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx9math.h>

//#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

#define SAFE_RELEASE(x){ if(x != NULL)x->Release(); }

struct VERTEX {
	float x, y, z;
	D3DXCOLOR color;
};

extern ID3D11Device* d3dDevice;
extern ID3D11DeviceContext* context;
extern IDXGISwapChain* swapChain;
extern ID3D11RenderTargetView* renderTargetView;
extern ID3D11Buffer *pVBuffer;

HRESULT InitDirect3D(HWND &hwnd);
void InitGraphics();
void InitCompileShader();
void RenderGraphics();
void CleanD3D();

#endif