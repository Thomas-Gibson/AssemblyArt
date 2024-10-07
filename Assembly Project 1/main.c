#define WIN32_LEAN_AND_MEAN
#include <Windows.h>


typedef struct 
{
	unsigned int width, height;
	size_t pitch;
	size_t size;
	void* memory;
} BitMap;

BitMap bit_map;
BITMAPINFO info;

extern void update();
extern void render(BitMap bit_map);

RECT cr;

static LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

	switch (msg)
	{
		case WM_CREATE:
		{
			GetClientRect(hWnd, &cr);
			bit_map.width = cr.right - cr.left;
			bit_map.height = cr.bottom - cr.top;
			bit_map.pitch = bit_map.width * sizeof(unsigned int);
			bit_map.size = bit_map.pitch * bit_map.height;

			bit_map.memory = VirtualAlloc(NULL, bit_map.size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

			info.bmiHeader.biSize = sizeof(info.bmiHeader);
			info.bmiHeader.biWidth = bit_map.width;
			info.bmiHeader.biHeight = bit_map.height;
			info.bmiHeader.biPlanes = 1;
			info.bmiHeader.biBitCount = 32;
			info.bmiHeader.biCompression = BI_RGB;

			ShowWindow(hWnd, SW_SHOW);
		} break;

		case WM_SIZE:
		{
			GetClientRect(hWnd, &cr);
		} break;

		case WM_DESTROY:
		{
			VirtualFree(bit_map.memory, 0, MEM_RELEASE);
			PostQuitMessage(0);
		} break;

		case WM_CLOSE:
		{
			DestroyWindow(hWnd);
		} break;

		default:
			result = DefWindowProcA(hWnd, msg, wParam, lParam);
	}

	return result;
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	WNDCLASSEXA wc = { 0 };
	wc.cbSize = sizeof(WNDCLASSEXA);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIconA(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursorA(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "Win32Window";
	wc.hIconSm = LoadIconA(NULL, IDI_APPLICATION);

	RegisterClassExA(&wc);

	RECT wr = { 0, 0, 800, 600 };

	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	unsigned int width = wr.right - wr.left;
	unsigned int height = wr.bottom - wr.top;

	unsigned int x = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
	unsigned int y = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;

	HWND hWnd =
		CreateWindowExA(
			0,
			wc.lpszClassName,
			"My Window",
			WS_OVERLAPPEDWINDOW,
			x, y, width, height,
			NULL,
			NULL,
			hInstance,
			NULL);

	MSG msg;
	HDC hdc = GetDC(hWnd);
	while (TRUE)
	{
		while (PeekMessage(&msg, NULL, 0u, 0u, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}

		if (msg.message == WM_QUIT) break;

		update();
		render(bit_map);


		StretchDIBits(hdc, 0, 0, cr.right - cr.left, cr.bottom - cr.top, 0, 0, bit_map.width, bit_map.height, bit_map.memory, &info, DIB_RGB_COLORS, SRCCOPY);
	}

	return (int)msg.wParam;
}
