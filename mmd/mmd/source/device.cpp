#include "device.h"

ID3D11Device* d3dDevice;
ID3D11DeviceContext* context;
IDXGISwapChain* swapChain;

ID3D11RenderTargetView* renderTargetView;
D3D11_VIEWPORT viewPort;

ID3D11Texture2D* pDepthStencil;
ID3D11DepthStencilView* pDepthStencilView;
ID3D11Buffer *pVBuffer;
ID3D11Buffer *pIBuffer;

ID3D11VertexShader* pVS; // vertex shader
ID3D11PixelShader* pPS;  // pixel shader

D3D_FEATURE_LEVEL pFeatureLevels[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1,D3D_FEATURE_LEVEL_10_0 };
UINT featureLevels = 3;
D3D_FEATURE_LEVEL featureLevelsSupported;

ID3D11InputLayout* pLayout;
D3D11_VIEWPORT viewport;


HRESULT InitBackBuffer();

VERTEX squareVertex[] = {
	{ -0.5f,  0.5f, 0.5f, { 1.0f, 0.0f, 0.0f, 1.0f } },
	{  0.5f, -0.5f, 0.5f, { 0.0f, 1.0f, 0.0f, 1.0f } },
	{ -0.5f, -0.5f, 0.5f, { 0.0f, 0.0f, 1.0f, 1.0f } },
	{  0.5f,  0.5f, 0.5f, { 1.0f, 1.0f, 0.0f, 1.0f } }
};

DWORD squareList[] = {
	0,1,2,
	0,3,1,
};

//-------------------
// Direct3D�̏�����
//-------------------
HRESULT InitDirect3D(HWND &hwnd)
{
	RECT rc;
	GetClientRect(hwnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	// �f�o�C�X�ƃX���b�v�`�F�C���̍쐬
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));

	// �o�b�N�o�b�t�@�̐�
	sd.BufferCount = 1;

	// �o�b�N�o�b�t�@�̃T�C�Y
	sd.BufferDesc.Width = WINDOW_WIDTH;
	sd.BufferDesc.Height = WINDOW_HEIGHT;

	// �t�H�[�}�b�g
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// ���t���b�V�����[�g Denominator/Numerator
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;

	// �o�b�N�o�b�t�@�̎g�p�@
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// �֘A�t����E�B���h�E
	sd.OutputWindow = hwnd;

	// �}���`�T���v���̐�
	sd.SampleDesc.Count = 1;

	// �}���`�T���v���̃N�I���e�B
	sd.SampleDesc.Quality = 0;

	// �E�B���h�E���[�h
	sd.Windowed = true;

	// ���[�h�ؑ�
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	UINT createDeviceFlags = 0;

	// �n�[�h�E�F�A�f�o�C�X�̍쐬
	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags,
		pFeatureLevels, featureLevels,D3D11_SDK_VERSION, &sd,
		&swapChain, &d3dDevice, &featureLevelsSupported,
		&context);
	if (FAILED(hr)) {
		// warp�f�o�C�X���쐬
		hr = D3D11CreateDeviceAndSwapChain(
			NULL, D3D_DRIVER_TYPE_WARP, NULL, createDeviceFlags,
			pFeatureLevels, featureLevels, D3D11_SDK_VERSION, &sd,
			&swapChain, &d3dDevice, &featureLevelsSupported,
			&context
		);
		if (FAILED(hr)) {
			// ���t�@�����X�f�o�C�X���쐬
			hr = D3D11CreateDeviceAndSwapChain(
				NULL, D3D_DRIVER_TYPE_REFERENCE, NULL, createDeviceFlags,
				pFeatureLevels, featureLevels, D3D11_SDK_VERSION, &sd,
				&swapChain, &d3dDevice, &featureLevelsSupported,
				&context
			);

			if (FAILED(hr)) {
				return S_FALSE;
			}
		}

	}

	// �o�b�N�o�b�t�@�̏�����
	hr = InitBackBuffer();
	if (FAILED(hr)) {
		return S_FALSE;
	}

	// �r���[�|�[�g�̐ݒ�
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = WINDOW_WIDTH;
	viewport.Height = WINDOW_HEIGHT;

	return S_OK;
}

// �o�b�N�o�b�t�@������
HRESULT InitBackBuffer() {
	ID3D11Texture2D *pBackBuffer; //�o�b�N�o�b�t�@
	swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	
	d3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &renderTargetView);
	pBackBuffer->Release();

	context->OMSetRenderTargets(1, &renderTargetView, NULL);

	return S_OK;
}

// �V�F�[�_�[�������Ɠǂݍ���
void InitCompileShader() {
	ID3D10Blob *VS, *PS;	// ���[�h���vertexShader,pixelShader

	// �V�F�[�_�[�̓ǂݍ���
	//D3DX11CompileFromFile("shader/vshader.vsh", 0, 0, "VShader", "vs_4_0", 0, 0, 0, &VS, 0, 0);
	//D3DX11CompileFromFile("shader/pshader.psh", 0, 0, "PShader", "ps_4_0", 0, 0, 0, &PS, 0, 0);
	
	D3DX11CompileFromFile("shader/shader.sh", 0, 0, "VShader", "vs_4_0", 0, 0, 0, &VS, 0, 0);
	D3DX11CompileFromFile("shader/shader.sh", 0, 0, "PShader", "ps_4_0", 0, 0, 0, &PS, 0, 0);

	// �V�F�[�_�[�I�u�W�F�N�g�̍쐬
	d3dDevice->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &pVS);
	d3dDevice->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &pPS);

	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	d3dDevice->CreateInputLayout(ied, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &pLayout);
	context->IASetInputLayout(pLayout);
}

D3D11_BUFFER_DESC InitTriangle(D3D11_BUFFER_DESC &bd) {
	// �O�p�`
	VERTEX TriangleVertex[] = {
		{0.0f, 0.5f, 0.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f,1.0f)} ,
		{0.45f, -0.5f, 0.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f,1.0f)} ,
		{-0.45f, -0.5f, 0.0f, D3DXCOLOR(0.0f, 0.0f, 1.0f,1.0f)}
	};

	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));

	bd.ByteWidth = sizeof(VERTEX) * 3;

	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	// �o�b�t�@�쐬
	d3dDevice->CreateBuffer(&bd, NULL, &pVBuffer);

	D3D11_MAPPED_SUBRESOURCE ms;
	context->Map(pVBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	memcpy(ms.pData, TriangleVertex, sizeof(TriangleVertex));
	context->Unmap(pVBuffer, NULL);

	return bd;
}

void InitSquareVertex(D3D11_BUFFER_DESC &vd) {
	ZeroMemory(&vd, sizeof(D3D11_BUFFER_DESC));

	vd.ByteWidth = sizeof(VERTEX) * 4;
	vd.Usage = D3D11_USAGE_DEFAULT;
	vd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vd.CPUAccessFlags = 0;
	vd.MiscFlags = 0;
	vd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA subdata;
	subdata.pSysMem = squareVertex;
	subdata.SysMemPitch = 0;
	subdata.SysMemSlicePitch = 0;

	d3dDevice->CreateBuffer(&vd, &subdata, &pVBuffer);
}

void InitSquareIndex(D3D11_BUFFER_DESC &id) {
	ZeroMemory(&id, sizeof(D3D11_BUFFER_DESC));

	id.ByteWidth = sizeof(DWORD) * 6;

	id.Usage = D3D11_USAGE_DEFAULT;
	id.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	id.CPUAccessFlags = 0;
	id.MiscFlags = 0;
	id.StructureByteStride = 0;
	//bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA subdata;

	subdata.pSysMem = squareVertex;
	subdata.SysMemPitch = 0;
	subdata.SysMemSlicePitch = 0;

	// �o�b�t�@�쐬
	d3dDevice->CreateBuffer(&id, &subdata, &pIBuffer);
}


void InitSquare() {
	D3D11_BUFFER_DESC vb,ib;

	InitSquareVertex(vb);

	InitSquareIndex(ib);



}

void InitGraphics() {
	//D3D11_BUFFER_DESC buffer;

	//InitTriangle(buffer);

	//InitSquare(buffer);
}

void RenderGraphics() {
	context->ClearRenderTargetView(renderTargetView, D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f));

	// render
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	//context->IASetInputLayout()
	context->IASetVertexBuffers(0, 1, &pVBuffer, &stride, &offset);
	context->IASetIndexBuffer(pIBuffer, DXGI_FORMAT_R16_UINT, 0);
	//context->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// �V�F�[�_�[�I�u�W�F�N�g�̃Z�b�g
	context->VSSetShader(pVS, 0, 0);
	context->RSSetViewports(1, &viewport);
	context->PSSetShader(pPS, 0, 0);
	context->OMSetRenderTargets(1, &renderTargetView, pDepthStencilView);


	//context->Draw(3, 0);

	context->ClearRenderTargetView(renderTargetView, D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f));
	context->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	context->DrawIndexed(6, 0, 0);

	swapChain->Present(0, 0);
}

// �������
void CleanD3D() {
	// �E�B���h�E���[�h�����ɖ߂�
	swapChain->SetFullscreenState(false, NULL);

	// ���
	SAFE_RELEASE(pPS);
	SAFE_RELEASE(pVS);
	SAFE_RELEASE(swapChain);
	SAFE_RELEASE(renderTargetView);
	SAFE_RELEASE(d3dDevice);
	SAFE_RELEASE(context);
}