#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "Winmm.lib")

#include <windows.h>
#include <dinput.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9mesh.h>
#include <math.h>
#include "class_Sphere.h"
#include "class_Camera.h"
#include <ctime>
#include <MMSystem.h>
#include <ddraw.h>

struct Vertex
{ 
	float x, y, z;
	DWORD color;
};
	
const float pi = D3DX_PI;

LRESULT __stdcall WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// Инициализация окна windows:
	MSG msg;
	HRESULT hr;

	WNDCLASS wc;
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)(6);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = hInstance;
	wc.lpszClassName = L"class";
	wc.lpszMenuName = NULL;
	RegisterClass(&wc);
	HWND hWnd = CreateWindow(L"class",L"Terra v1.0", WS_OVERLAPPEDWINDOW,100,100, 700,700,NULL,NULL,hInstance, NULL);

	ShowWindow(hWnd,nCmdShow);
	UpdateWindow(hWnd);
	//-------------------------------------

	// Инициализация Direct3D
	D3DDISPLAYMODE Display;
	D3DPRESENT_PARAMETERS pp;
	IDirect3DVertexBuffer9 *vBuffer;
	IDirect3DIndexBuffer9 *iBuffer;
	D3DMATERIAL9 Material;
	D3DLIGHT9 Light;
	void *vbData, *ibData;
	LPDIRECT3D9 D3D;
	LPDIRECT3DDEVICE9 Videocard;
	IDirectInput8* DInput;
	IDirectInputDevice8* keyboard;	
	IDirectInputDevice8* Mouse;	
	DIMOUSESTATE MouseKeys;
	char bKey[256];
	
	D3D = Direct3DCreate9(D3D_SDK_VERSION);
	D3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &Display);
	ZeroMemory(&pp, sizeof(pp));
	pp.Windowed = true;
	pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	pp.BackBufferFormat = D3DFMT_A8R8G8B8;
	pp.EnableAutoDepthStencil = true;
	pp.AutoDepthStencilFormat = D3DFMT_D16;

	D3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING,  &pp, &Videocard);
	DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&DInput, NULL);
	DInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	DInput->CreateDevice(GUID_SysMouse, &Mouse, NULL);
	Videocard->CreateVertexBuffer(1*sizeof(Vertex), D3DUSAGE_WRITEONLY, D3DFVF_XYZ|D3DFVF_NORMAL, D3DPOOL_DEFAULT, &vBuffer, 0);
	Videocard->CreateIndexBuffer(1*sizeof(short), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &iBuffer, 0);
	Videocard->SetFVF(D3DFVF_XYZ);
	Videocard->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	Videocard->SetRenderState(D3DRS_LIGHTING, true);
	Videocard->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	Videocard->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);

	keyboard->SetDataFormat(&c_dfDIKeyboard);
	Mouse->SetDataFormat(&c_dfDIMouse);
	keyboard->SetCooperativeLevel(hWnd,DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	Mouse->SetCooperativeLevel(hWnd,DISCL_EXCLUSIVE);
	keyboard->Acquire();
	Mouse->Acquire();

	D3DXMATRIX matCam, matProj;

	Camera Cam(0, 100, -80, -0.7 ,0);
	D3DXMatrixPerspectiveFovLH(&matProj, pi/4, 1, 1, 1000000);
	Videocard->SetTransform(D3DTS_PROJECTION, &matProj);

	while (1) 
	{
		hr = keyboard->GetDeviceState(sizeof(bKey),bKey);
		if (hr != DI_OK) keyboard->Acquire(); 
		hr = Mouse->GetDeviceState(sizeof(DIMOUSESTATE), &MouseKeys);
		if (hr != DI_OK) Mouse->Acquire();

		if (bKey[DIK_W]) Cam.Walk(0.2f);
		if (bKey[DIK_S]) Cam.Walk(-0.2f);
		if (bKey[DIK_D]) Cam.Strafe(0.2f);
		if (bKey[DIK_A]) Cam.Strafe(-0.2f);
		if (bKey[DIK_SPACE]) Cam.Fly(0.2f);
		if (bKey[DIK_LCONTROL]) Cam.Fly(-0.2f);
		if (bKey[DIK_NUMPAD8]) Cam.Heading(0.01f);
		if (bKey[DIK_NUMPAD2]) Cam.Heading(-0.01f);
		if (bKey[DIK_NUMPAD6]) Cam.Bank(0.01f);
		if (bKey[DIK_NUMPAD4]) Cam.Bank(-0.01f);
		/*if (MouseKeys.lY < 0) Cam.Heading(-0.001f*MouseKeys.lY);
		if (MouseKeys.lY > 0) Cam.Heading(-0.001f*MouseKeys.lY);
		if (MouseKeys.lX > 0) Cam.Bank(0.001f*MouseKeys.lX); Управление мышью
		if (MouseKeys.lX < 0) Cam.Bank(0.001f*MouseKeys.lX);*/
		if (bKey[DIK_ESCAPE]) exit(0);

		
		Videocard->SetTransform(D3DTS_VIEW, Cam.GetMatrix()); 
		Videocard->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(255,255,255), 1.0f, 0);
		Videocard->BeginScene();

		Videocard->EndScene();
		Videocard->Present(0,0,0,0);


		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		   {if (msg.message == WM_QUIT)
		{
			ReleaseDC(hWnd, hdc);
			iBuffer->Release();
			vBuffer->Release();
			keyboard->Release();
			Mouse->Release();
			Videocard->Release();
			D3D->Release();
			break;			
		}
			TranslateMessage(&msg);
			DispatchMessage(&msg); }
	}
};

LRESULT __stdcall WndProc (HWND hWnd, UINT msg,
                           WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		exit(0);
		return 0;
	}
	return DefWindowProc(hWnd,msg,wParam,lParam);
}