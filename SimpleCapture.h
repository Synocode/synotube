#pragma once
#include <SimpleMath.h>
//#include <DirectXPackedVector.h>
using namespace DirectX;

class SimpleCapture
{
public:
    SimpleCapture(
        winrt::Windows::Graphics::DirectX::Direct3D11::IDirect3DDevice const& device,
        winrt::Windows::Graphics::Capture::GraphicsCaptureItem const& item);
    ~SimpleCapture() { Close(); }

    void StartCapture();
    winrt::Windows::UI::Composition::ICompositionSurface CreateSurface(
        winrt::Windows::UI::Composition::Compositor const& compositor);

    void Close();
    
    /*struct Vertex
    {
        XMFLOAT3 position;
        XMFLOAT4 color;
    };*/

    struct Vertex
    {
        XMFLOAT3 position;
        XMFLOAT2 tex;
    };


private:
    void OnFrameArrived(
        winrt::Windows::Graphics::Capture::Direct3D11CaptureFramePool const& sender,
        winrt::Windows::Foundation::IInspectable const& args);

    void CheckClosed()
    {
        if (m_closed.load() == true)
        {
            throw winrt::hresult_error(RO_E_CLOSED);
        }
    }


private:
    winrt::Windows::Graphics::Capture::GraphicsCaptureItem m_item{ nullptr };
    winrt::Windows::Graphics::Capture::Direct3D11CaptureFramePool m_framePool{ nullptr };
    winrt::Windows::Graphics::Capture::GraphicsCaptureSession m_session{ nullptr };
    winrt::Windows::Graphics::SizeInt32 m_lastSize;

    winrt::Windows::Graphics::DirectX::Direct3D11::IDirect3DDevice m_device{ nullptr };
    winrt::com_ptr<IDXGISwapChain1> m_swapChain{ nullptr };
    winrt::com_ptr<ID3D11DeviceContext> m_d3dContext{ nullptr };

    std::atomic<bool> m_closed = false;
	winrt::Windows::Graphics::Capture::Direct3D11CaptureFramePool::FrameArrived_revoker m_frameArrived;

    ////ADDED THESE
    //ID3D11ShaderResourceView*  srv_1[1] = { nullptr };
    //ID3D11ShaderResourceView* srv_1 = nullptr;
    winrt::com_ptr<ID3D11InputLayout> pLayout{ nullptr };
    /*ID3D11RenderTargetView* backbuffer;*/  // the pointer to our back buffer
    winrt::com_ptr<ID3D11VertexShader> pVS{ nullptr };               // the pointer to the vertex shader
    ID3D11PixelShader* pPS;                // the pointer to the pixel shader
    ID3D11Buffer* pVBuffer;                // the pointer to the vertex buffer

    //winrt::com_ptr<ID3D11Texture2D> tex{ nullptr };
    D3D11_TEXTURE2D_DESC txtDesc = {};
    ID3D11SamplerState* m_SamplerLinear{ nullptr };
    D3D11_VIEWPORT viewport;

};

