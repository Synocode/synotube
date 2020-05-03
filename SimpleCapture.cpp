//*********************************************************
//
// Copyright (c) Microsoft. All rights reserved.
// This code is licensed under the MIT License (MIT).
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, 
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH 
// THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//*********************************************************

#include "pch.h"
#include "SimpleCapture.h"
#include "d3d11.h"
//#include <Windows.Foundation.h>
//#include <wrl\wrappers\corewrappers.h>
//#include <wrl\client.h>

//using namespace ABI::Windows::Foundation;
//using namespace Microsoft::WRL;
//using namespace Microsoft::WRL::Wrappers;

using namespace winrt;
using namespace Windows;
using namespace Windows::Foundation;
using namespace Windows::System;
using namespace Windows::Graphics;
using namespace Windows::Graphics::Capture;
using namespace Windows::Graphics::DirectX;
using namespace Windows::Graphics::DirectX::Direct3D11;
using namespace Windows::Foundation::Numerics;
using namespace Windows::UI;
using namespace Windows::UI::Composition;


SimpleCapture::SimpleCapture(
    IDirect3DDevice const& device,
    GraphicsCaptureItem const& item)
{
    m_item = item;
    m_device = device;

	// Set up 
    auto d3dDevice = GetDXGIInterfaceFromObject<ID3D11Device>(m_device);
    d3dDevice->GetImmediateContext(m_d3dContext.put());
	auto size = m_item.Size();

    m_swapChain = CreateDXGISwapChain(
        d3dDevice,
		static_cast<uint32_t>(size.Width),
		static_cast<uint32_t>(size.Height),
        static_cast<DXGI_FORMAT>(DirectXPixelFormat::B8G8R8A8UIntNormalized),
        2);

    // ADDED THIS
    float x = -1.0f;
    float y = -1.0f;
    float width = 2.0f;
    float height = 2.0f;
    Vertex OurVertices[] =
    {
        { XMFLOAT3(x, y, 0), XMFLOAT2(0.0f, 1.0f) },
        { XMFLOAT3(x, y+height, 0), XMFLOAT2(0.0f, 0.0f)},
        { XMFLOAT3(x + width, y, 0), XMFLOAT2(1.0f, 1.0f)}, 
        { XMFLOAT3(x + width, y + height, 0), XMFLOAT2(1.0f, 0.0f)}
    };
    /*Vertex OurVertices[] =
    {

        {XMFLOAT3(-.5f, -.5f, 0.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.00f)},
        {XMFLOAT3(1.0f, -1.0f, 0.0f), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f)},
        {XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
        {XMFLOAT3(-1.0f, 1.0f, 0.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)}
    };*/


    // create the vertex buffer
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
    bd.ByteWidth = sizeof(Vertex) * 4;             // size is the VERTEX struct * 3
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

    d3dDevice->CreateBuffer(&bd, NULL, &pVBuffer);       // create the buffer


    // copy the vertices into the buffer
    D3D11_MAPPED_SUBRESOURCE ms;
    m_d3dContext->Map(pVBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
    memcpy(ms.pData, OurVertices, sizeof(OurVertices));                 // copy the data
    m_d3dContext->Unmap(pVBuffer, NULL);                                      // unmap the buffer

    // INIT PIPELINE
    ID3D10Blob* VS, * PS;
   /* HRESULT hr_0 = D3DReadFileToBlob(L"crt-royale-first-pass-vs_4_0.fxc", &VS);
    HRESULT hr_1 = D3DReadFileToBlob(L"crt-royale-first-pass-ps_4_0.fxc", &PS);*/
    //D3DCompileFromFile(L"shaders.shader", 0, 0, "VShader", "vs_4_0", 0, 0, &VS, 0);
    //D3DCompileFromFile(L"shaders.shader", 0, 0, "PShader", "ps_4_0", 0, 0, &PS, 0);
    D3DCompileFromFile(L"shaders2.shader", 0, 0, "VS", "vs_5_0", 0, 0, &VS, 0);
    D3DCompileFromFile(L"shaders2.shader", 0, 0, "PS", "ps_5_0", 0, 0, &PS, 0);
    HRESULT hr_2 = d3dDevice->CreateVertexShader(
        VS->GetBufferPointer(),
        VS->GetBufferSize(),
        nullptr,
        pVS.put()
    );
    HRESULT hr_3 = d3dDevice->CreatePixelShader(
        PS->GetBufferPointer(),
        PS->GetBufferSize(),
        nullptr,
        &pPS
    );
    m_d3dContext->VSSetShader(
        pVS.get(),
        nullptr,
        0
    );
    m_d3dContext->PSSetShader(
        pPS,
        nullptr,
        0
    );


    D3D11_INPUT_ELEMENT_DESC ied[] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        //{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };

    d3dDevice->CreateInputLayout(ied, 2, VS->GetBufferPointer(), VS->GetBufferSize(), pLayout.put());
    m_d3dContext->IASetInputLayout(pLayout.get());

    // CREATE SAMPLER
   //winrt::com_ptr<ID3D11SamplerState> m_SamplerLinear{ nullptr };
    //ID3D11SamplerState* m_SamplerLinear{ nullptr };
    D3D11_SAMPLER_DESC SampDesc;
    RtlZeroMemory(&SampDesc, sizeof(SampDesc));
    SampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    SampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    SampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    SampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    SampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    SampDesc.MinLOD = 0;
    SampDesc.MaxLOD = D3D11_FLOAT32_MAX;
    HRESULT hr = d3dDevice->CreateSamplerState(&SampDesc, &m_SamplerLinear);
    m_d3dContext->PSSetSamplers(0, 1, &m_SamplerLinear);


    //D3D11_TEXTURE2D_DESC txtDesc = {};
    txtDesc.MipLevels = txtDesc.ArraySize = 1;
    txtDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    txtDesc.SampleDesc.Count = 1;
    txtDesc.Usage = D3D11_USAGE_DEFAULT;
    txtDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    txtDesc.Width = 1366; // 1382;// size.Width;
    txtDesc.Height = 754; // 754; // size.Height;


	// Create framepool, define pixel format (DXGI_FORMAT_B8G8R8A8_UNORM), and frame size. 
    m_framePool = Direct3D11CaptureFramePool::Create(
        m_device,
        DirectXPixelFormat::B8G8R8A8UIntNormalized,
        2,
		size);
    m_session = m_framePool.CreateCaptureSession(m_item);
    m_lastSize = size;
	m_frameArrived = m_framePool.FrameArrived(auto_revoke, { this, &SimpleCapture::OnFrameArrived });

    // Set the viewport
    //D3D11_VIEWPORT viewport;
    ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width =  m_lastSize.Width; // SCREEN_WIDTH;
    viewport.Height =  m_lastSize.Height; // SCREEN_HEIGHT;

    m_d3dContext->RSSetViewports(1, &viewport);

    // select which vertex buffer to display
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    m_d3dContext->IASetVertexBuffers(0, 1, &pVBuffer, &stride, &offset);

    // select which primtive type we are using
    m_d3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}

// Start sending capture frames
void SimpleCapture::StartCapture()
{
    CheckClosed();
    m_session.StartCapture();
}

ICompositionSurface SimpleCapture::CreateSurface(
    Compositor const& compositor)
{
    CheckClosed();
    return CreateCompositionSurfaceForSwapChain(compositor, m_swapChain.get());
}

// Process captured frames
void SimpleCapture::Close()
{
    auto expected = false;
    if (m_closed.compare_exchange_strong(expected, true))
    {
		m_frameArrived.revoke();
		m_framePool.Close();
        m_session.Close();

        m_swapChain = nullptr;
        m_framePool = nullptr;
        m_session = nullptr;
        m_item = nullptr;
    }
}


void SimpleCapture::OnFrameArrived(
    Direct3D11CaptureFramePool const& sender,
    winrt::Windows::Foundation::IInspectable const&)
{
    auto newSize = false;

    {
        auto frame = sender.TryGetNextFrame();
		auto frameContentSize = frame.ContentSize();

        if (frameContentSize.Width != m_lastSize.Width ||
			frameContentSize.Height != m_lastSize.Height)
        {
            // The thing we have been capturing has changed size.
            // We need to resize our swap chain first, then blit the pixels.
            // After we do that, retire the frame and then recreate our frame pool.
            newSize = true;
            m_lastSize = frameContentSize;
            m_swapChain->ResizeBuffers(
                2, 
				static_cast<uint32_t>(m_lastSize.Width),
				static_cast<uint32_t>(m_lastSize.Height),
                static_cast<DXGI_FORMAT>(DirectXPixelFormat::B8G8R8A8UIntNormalized), 
                0);

            // ADDED THIS
            viewport.Width = m_lastSize.Width;
            viewport.Height = m_lastSize.Height;
            m_d3dContext->RSSetViewports(1, &viewport);
        }

        {
            auto frameSurface = GetDXGIInterfaceFromObject<ID3D11Texture2D>(frame.Surface());
            /*com_ptr<ID3D11Texture2D> backBuffer;
            check_hresult(m_swapChain->GetBuffer(0, guid_of<ID3D11Texture2D>(), backBuffer.put_void()));*/


            //// ADDED THIS
            //// use the back buffer address to create the render target
            com_ptr<ID3D11Texture2D> pBackBuffer;
            ID3D11RenderTargetView* backbuffer;
            check_hresult(m_swapChain->GetBuffer(0, guid_of<ID3D11Texture2D>(), pBackBuffer.put_void()));
            auto d3dDevice = GetDXGIInterfaceFromObject<ID3D11Device>(m_device);
            d3dDevice->CreateRenderTargetView(pBackBuffer.get(), NULL, &backbuffer);

            // set the render target as the back buffer
            m_d3dContext->OMSetRenderTargets(1, &backbuffer, NULL);
            ////float color[4] = { 0.0f, 0.2f, 0.4f, 1.0f };
            ////m_d3dContext->ClearRenderTargetView(backbuffer, color);

            com_ptr<ID3D11Texture2D> tex{ nullptr };
            check_hresult(d3dDevice->CreateTexture2D(&txtDesc, NULL,
                tex.put()));
            tex.copy_from(frameSurface.get());
            /*com_ptr<ID3D11ShaderResourceView> srv_1{ nullptr };*/
            ID3D11ShaderResourceView* srv_1 = nullptr;
            d3dDevice->CreateShaderResourceView(
                tex.get(),
                nullptr,
                &srv_1
            );

            //auto texture = srv_1;
            m_d3dContext->PSSetShaderResources(0, 1, &srv_1);// texture);

            //m_d3dContext->CopyResource(pBackBuffer.get(), frameSurface.get());
            // draw the vertex buffer to the back buffer
            m_d3dContext->Draw(4, 0);

            backbuffer->Release();
            srv_1->Release();
        }
    }

    // switch the back buffer and the front buffer
    DXGI_PRESENT_PARAMETERS presentParameters = { 0 };
    m_swapChain->Present1(1, 0, &presentParameters);

    if (newSize)
    {
        m_framePool.Recreate(
            m_device,
            DirectXPixelFormat::B8G8R8A8UIntNormalized,
            2,
            m_lastSize);
    }
}
