#include "main.h"
#include "device.h"

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void RenderFrame();

HRESULT InitApp(HINSTANCE hInst)
{
	return 0;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hwnd;
	
	WNDCLASSEX wc;

	// left, top, right, bottom
	RECT rc = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };

	// �E�B���h�E�̒���
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = "air";

	RegisterClassEx(&wc);

	hwnd = CreateWindowEx(NULL,
						"air",
						MY_TITLE,
						WS_OVERLAPPEDWINDOW,
						300,
						300,
						rc.right - rc.left,
						rc.bottom - rc.top,
						NULL,
						NULL,
						hInstance,
						NULL);

	ShowWindow(hwnd, nCmdShow);
	
	InitDirect3D(hwnd);
	InitGraphics();
	InitCompileShader();
	// main loop

	MSG msg;

	while (true) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);

			DispatchMessage(&msg);

			if (msg.message == WM_QUIT) {
				break;
			}

		} else {
			// main
			RenderFrame();


		}
	}

	// �I������
	CleanD3D();

	return msg.wParam;
}

// �`��
void RenderFrame() {
	RenderGraphics();
}


LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}