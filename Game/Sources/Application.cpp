#include "pch.h"
#include "Application.h"

Application::Application()
{

}

Application::~Application()
{

}

void Application::Init(HWND hwnd)
{
	m_HWnd = hwnd;

	m_Width = g_WinSizeX;
	m_Height = g_WinSizeY;

	CreateDeviceAndSwapChain();
	CreateRenderTargetView();
	SetViewport();

	CreateGeometry();
	CreateVertexShader();
	CreateInputLayout();
	CreatePiexlShader();
}

void Application::Update()
{

}

void Application::Render()
{
	RenderBegin();

	// TODO
	{
		uint32 stride = sizeof(Vertex);
		uint32 offset = 0;

		// IA
		m_DeviceContext->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);
		m_DeviceContext->IASetInputLayout(m_InputLayout.Get());
		m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// VS
		m_DeviceContext->VSSetShader(m_VertexShader.Get(), nullptr, 0);

		// RS

		// PS
		m_DeviceContext->PSSetShader(m_PixelShader.Get(), nullptr, 0);

		// OM

		m_DeviceContext->Draw(m_Verties.size(), 0);
	}

	RenderEnd();
}

void Application::RenderBegin()
{
	m_DeviceContext->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(), nullptr);
	m_DeviceContext->ClearRenderTargetView(m_RenderTargetView.Get(), m_ClearColor);
	m_DeviceContext->RSSetViewports(1, &m_Viewport);
}

void Application::RenderEnd()
{
	HRESULT hr = m_SwapChain->Present(1, 0);
	CHECK(hr);
}

void Application::CreateDeviceAndSwapChain()
{
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	{
		desc.BufferDesc.Width = m_Width;
		desc.BufferDesc.Height = m_Height;
		desc.BufferDesc.RefreshRate.Numerator = 60;
		desc.BufferDesc.RefreshRate.Denominator = 1;
		desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // Using paint
		desc.BufferCount = 1;
		desc.OutputWindow = m_HWnd;
		desc.Windowed = true;
		desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	}

	HRESULT hr = ::D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&desc,
		m_SwapChain.GetAddressOf(),
		m_Device.GetAddressOf(),
		nullptr,
		m_DeviceContext.GetAddressOf()
	);

	CHECK(hr);
}

void Application::CreateRenderTargetView()
{
	HRESULT hr;

	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer = nullptr;
	hr = m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());
	CHECK(hr);

	m_Device->CreateRenderTargetView(backBuffer.Get(), nullptr, m_RenderTargetView.GetAddressOf());
	CHECK(hr);
}

void Application::SetViewport()
{
	m_Viewport.TopLeftX = 0.f;
	m_Viewport.TopLeftY = 0.f;
	m_Viewport.Width = static_cast<float>(m_Width);
	m_Viewport.Height = static_cast<float>(m_Height);
	m_Viewport.MinDepth = 0.f;
	m_Viewport.MaxDepth = 1.f;
}

void Application::CreateGeometry()
{
	// Vertex data initialize <=== RAM
	{
		m_Verties.resize(3);

		m_Verties[0].position = Vec3(-0.5f, -0.5f, 0.f);
		m_Verties[0].color = Color(1.f, 0.f, 0.f, 1.f);
		m_Verties[1].position = Vec3(0.f, 0.5f, 0.f);
		m_Verties[1].color = Color(0.f, 1.f, 0.f, 1.f);
		m_Verties[2].position = Vec3(0.5f, -0.5f, 0.f);
		m_Verties[2].color = Color(0.f, 0.f, 1.f, 1.f);
	}

	// VertexBuffer
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Usage = D3D11_USAGE_IMMUTABLE; // GPU read only
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER; // Bind vertex data point
		desc.ByteWidth = (uint32)(sizeof(Vertex) * m_Verties.size());

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = m_Verties.data();

		m_Device->CreateBuffer(&desc, &data, m_VertexBuffer.GetAddressOf());
	}
}

// GPU에게 Vertex 자료형의 구조 생김새를 알려준다.
void Application::CreateInputLayout()
{
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	const int32 count = sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC);
	m_Device->CreateInputLayout(layout, count, m_VsBlob->GetBufferPointer(), m_VsBlob->GetBufferSize(), m_InputLayout.GetAddressOf());
}

void Application::CreateVertexShader()
{
	//std::wstring testStr = 
	LoadShaderFromFile(L"./../Shaders/Default.hlsl", "VS", "vs_5_0", m_VsBlob);
	HRESULT hr = m_Device->CreateVertexShader(m_VsBlob->GetBufferPointer(), m_VsBlob->GetBufferSize(), nullptr, m_VertexShader.GetAddressOf());
	CHECK(hr);
}

void Application::CreatePiexlShader()
{
	LoadShaderFromFile(L"./../Shaders/Default.hlsl", "PS", "ps_5_0", m_PsBlob);
	HRESULT hr = m_Device->CreatePixelShader(m_PsBlob->GetBufferPointer(), m_PsBlob->GetBufferSize(), nullptr, m_PixelShader.GetAddressOf());
	CHECK(hr);
}

void Application::LoadShaderFromFile(const std::wstring& path, const std::string& name, const std::string& version, Microsoft::WRL::ComPtr<ID3DBlob>& blob)
{
	const uint32 compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION; // 디버그용 | 최적화는 건너뜀

	HRESULT hr = D3DCompileFromFile(
		path.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		name.c_str(),
		version.c_str(),
		compileFlag,
		0,
		blob.GetAddressOf(),
		nullptr);

	CHECK(hr);
}