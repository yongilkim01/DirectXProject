#pragma once

class Game
{
public:
	Game();
	~Game();

public:
	void Init(HWND hwnd);
	void Update();
	void Render();

private:
	void RenderBegin();
	void RenderEnd();

private:
	void CreateDeviceAndSwapChain();
	void CreateRenderTargetView(); 
	void SetViewport();

private:
	HWND m_HWnd;
	uint32 m_Width = 0;
	uint32 m_Height = 0;

	// DirectX variable
	Microsoft::WRL::ComPtr<ID3D11Device> m_Device = nullptr; // À¯´Ö »ý¼º
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_DeviceContext = nullptr; // À¯´Ö ¸í·É
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_SwapChain = nullptr;

	/// Render target view
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_RenderTargetView;

	// Misc
	D3D11_VIEWPORT m_Viewport = { 0 };
	float m_ClearColor[4] = { 0.f, 0.f, 0.f, 0.f };
};

