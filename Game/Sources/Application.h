#pragma once

class Application
{
public:
	Application();
	~Application();

public:
	void Init(HWND hwnd);
	void Update();
	void Render();

private:
	void RenderBegin();
	void RenderEnd();

private:
	// Render initialize method
	void CreateDeviceAndSwapChain();
	void CreateRenderTargetView();
	void SetViewport();

	// Vectex create method
	void CreateGeometry();
	void CreateInputLayout();

	// Shader create method
	void CreateVertexShader();
	void CreatePiexlShader();
	void CreateSRV();
	void CreateConstantBuffer();
	void LoadShaderFromFile(const std::wstring& path, const std::string& name, const std::string& version, Microsoft::WRL::ComPtr<ID3DBlob>& blob);

private:
	HWND m_HWnd;
	uint32 m_Width = 0;
	uint32 m_Height = 0;

	// DirectX variable
	Microsoft::WRL::ComPtr<ID3D11Device> m_Device = nullptr; // 유닛 생성
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_DeviceContext = nullptr; // 유닛 명령
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_SwapChain = nullptr;

	/// Render target view
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_RenderTargetView;

	// Misc
	D3D11_VIEWPORT m_Viewport = { 0 };
	float m_ClearColor[4] = { 0.f, 0.f, 0.f, 0.f };

	// Vertex variable
	std::vector<Vertex> m_Verties;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_VertexBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_InputLayout = nullptr; // Vertex의 최종 구조 정보를 담는 객체
	std::vector<uint32> m_Indices;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_IndexBuffer = nullptr;

	// Vertex shader
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VertexShader = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> m_VsBlob = nullptr;

	// Pixel shader
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PixelShader = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> m_PsBlob = nullptr;

	// Shader resource view - using image file cpu -> gpu memory
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_ShaderResourceView = nullptr;

	TransformData m_TransformData;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_ConstantBuffer = nullptr;

};