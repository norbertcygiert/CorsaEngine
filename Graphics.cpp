#include "Graphics.h"
#include <sstream>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")  //We dont have to change the linker settings this way.
#pragma comment(lib, "D3DCompiler.lib")

namespace wrl = Microsoft::WRL;

//We need an HRESULT codecatcher in scope for these macros two work.
#define GFX_EXCEPT_NOINFO(cc) Graphics::HRESException( __LINE__,__FILE__,(cc) )
#define GFX_THROW_NOINFO(cc) if( FAILED( cc = (call) ) ) throw Graphics::HRESException( __LINE__,__FILE__,cc )

#ifndef NDEBUG
#define GFX_EXCEPT(cc) Graphics::HRESException( __LINE__,__FILE__,(cc),infoManager.getMessages() )
#define GFX_THROW_INFO(call) infoManager.set(); if( FAILED( cc = (call) ) ) throw GFX_EXCEPT(cc)
#define GFX_DEVICE_REMOVED_EXCEPT(cc) Graphics::DeviceRemovedException( __LINE__,__FILE__,(cc),infoManager.getMessages() )
#define GFX_THROW_INFO_ONLY(call) infoManager.set(); (call); {auto v = infoManager.getMessages(); if(!v.empty()) {throw Graphics::InfoException( __LINE__,__FILE__,v);}}
#else
#define GFX_EXCEPT(cc) Graphics::HRESException( __LINE__,__FILE__,(cc) )
#define GFX_THROW_INFO(call) GFX_THROW_NOINFO(call)
#define GFX_DEVICE_REMOVED_EXCEPT(cc) Graphics::DeviceRemovedException( __LINE__,__FILE__,(cc) )
#define GFX_THROW_INFO_ONLY(call) (call)
#endif

Graphics::Graphics(HWND hWnd) {
    //https://learn.microsoft.com/en-us/windows/win32/api/dxgi/ns-dxgi-dxgi_swap_chain_desc
    DXGI_SWAP_CHAIN_DESC sd = {};
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 0;
    sd.BufferDesc.RefreshRate.Denominator = 0;
    sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.BufferCount = 1;
    sd.OutputWindow = hWnd;
    sd.Windowed = true;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; //Setting this flag makes the driver select the most efficient presentation technique
    sd.Flags = 0;
    UINT swapCreateFlags = 0u;
#ifndef NDEBUG
    swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
    HRESULT cc;

    GFX_THROW_INFO(D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        swapCreateFlags,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &sd,
        &pSwap,
        &pDevice,
        nullptr,
        &pContext
    ));

    // Gain access to texture subresource in the swap-chain
    wrl::ComPtr<ID3D11Resource> pBackBuf;
    //https://learn.microsoft.com/en-us/windows/win32/api/dxgi/nf-dxgi-idxgiswapchain-getbuffer
    GFX_THROW_INFO(pSwap->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuf));
    GFX_THROW_INFO(pDevice->CreateRenderTargetView(pBackBuf.Get(), nullptr, &pTarget));
}


void Graphics::endFrame() {
    HRESULT cc;
#ifndef NDEBUG
    infoManager.set();
#endif // !NDEBUG

    if (FAILED(cc = pSwap->Present(1u, 0u))) //Syncinterval is basically the value which our device framerate will be divided by, giving us the target FR.
    {
        if (cc == DXGI_ERROR_DEVICE_REMOVED) {
            throw GFX_DEVICE_REMOVED_EXCEPT(pDevice->GetDeviceRemovedReason());
        }
        else throw GFX_EXCEPT(cc);
    }
}

void Graphics::clearBuffer(float r, float g, float b) noexcept{
    const float color[] = { r, g, b, 1.0f };
    pContext->ClearRenderTargetView(pTarget.Get(), color);
}

void Graphics::drawTriangle(float angle) {
    HRESULT cc;
    struct Vertex {
        struct {
            float x, y;
        } pos;
        struct {
            float r, g, b;
        } color;
    };
    const Vertex v[] = {
        { .0f, .5f, 1.0f, 0.0f, 0.0f },
        { .5f, -.5f, 0.0f, 1.0f, 0.0f },
        { -0.5f, -.5f, 0.0f, 0.0f, 1.0f },
        { -.3f, .3f, 1.0f, 0.0f, 0.0f },
        { .3f, .3f, 1.0f, 0.0f, 0.0f },
        { .0f, -.8f, 1.0f, 0.0f, 0.0f },
    };
    wrl::ComPtr<ID3D11Buffer> pVertexBuf;
    D3D11_BUFFER_DESC bd = {};
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.CPUAccessFlags = 0;
    bd.MiscFlags = 0;
    bd.ByteWidth = sizeof(v);
    bd.StructureByteStride = sizeof(Vertex);
    D3D11_SUBRESOURCE_DATA sd = {};
    sd.pSysMem = v;
    GFX_THROW_INFO(pDevice->CreateBuffer(&bd, &sd, &pVertexBuf));


    //Binding our vertex buffer to the rendering pipeline
    const UINT stride = sizeof(Vertex);
    const UINT offset = 0u;
    pContext->IASetVertexBuffers(0u, 1u, pVertexBuf.GetAddressOf(), &stride, &offset);

    //Index buffer for indexed drawing
    const unsigned short indices[] = {
        0,1,2,
        0,2,3,
        0,4,1,
        2,1,5,
    };
    wrl::ComPtr<ID3D11Buffer> indexBuf;
    D3D11_BUFFER_DESC ibd;
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.Usage = D3D11_USAGE_DEFAULT;
    ibd.CPUAccessFlags = 0;
    ibd.MiscFlags = 0;
    ibd.ByteWidth = sizeof(indices);
    ibd.StructureByteStride = sizeof(unsigned short);
    D3D11_SUBRESOURCE_DATA isd = {};
    isd.pSysMem = indices;
    GFX_THROW_INFO(pDevice->CreateBuffer(&ibd, &isd, &indexBuf));
    pContext->IASetIndexBuffer(indexBuf.Get(), DXGI_FORMAT_R16_UINT, 0);


    struct ConstBuffer {
        struct {
            float elem[4][4];
        } transformation_matrix;
    };
    //https://en.wikipedia.org/wiki/Rotation_matrix#In_three_dimensions
    const ConstBuffer cb = {
        {
            (0.75f)*std::cos(angle), std::sin(angle), .0f, .0f,
            (0.75f)*-std::sin(angle), std::cos(angle), .0f, .0f,
            .0f,            .0f,               1.0f, .0f,
            .0f,            .0f,               .0f, 1.0f,
        }
    };
    wrl::ComPtr<ID3D11Buffer> pConstantBuf;
    D3D11_BUFFER_DESC cbd;
    cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbd.Usage = D3D11_USAGE_DYNAMIC;
    cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    cbd.MiscFlags = 0;
    cbd.ByteWidth = sizeof(cb);
    cbd.StructureByteStride = 0;
    D3D11_SUBRESOURCE_DATA cbsd = {};
    cbsd.pSysMem = &cb;
    GFX_THROW_INFO(pDevice->CreateBuffer(&cbd, &cbsd, &pConstantBuf));
    pContext->VSSetConstantBuffers(0, 1, pConstantBuf.GetAddressOf());


    //Pixel Shader
    wrl::ComPtr<ID3D11PixelShader> pPixelShader;
    wrl::ComPtr<ID3DBlob> pBlob;
    D3DReadFileToBlob(L"PixelShader.cso", &pBlob);
    GFX_THROW_INFO(pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader));
    pContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);


    //Vertex shader
    wrl::ComPtr<ID3D11VertexShader> pVertexShader;
    //We reuse the pointer for blob as it is freed.
    D3DReadFileToBlob(L"VertexShader.cso", &pBlob);
    GFX_THROW_INFO(pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader));
    pContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);


    //Input vertex layout https://learn.microsoft.com/en-us/windows/win32/api/d3d11/ns-d3d11-d3d11_input_element_desc
    wrl::ComPtr<ID3D11InputLayout> pInputLayout;
    const D3D11_INPUT_ELEMENT_DESC ied[] = {
        //Disclaimer: this R32G32 doesn't have anything to do with color
        {"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };
    GFX_THROW_INFO(pDevice->CreateInputLayout(ied, std::size(ied), pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &pInputLayout));

    pContext->IASetInputLayout(pInputLayout.Get());
    pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), nullptr); //Calling GetAdressOf makes sure the underlying pointer is not freed

    //Setting primitive topology to triangle list: https://learn.microsoft.com/en-us/windows/win32/direct3d11/d3d10-graphics-programming-guide-primitive-topologies
    pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    //Viewport configuration;
    D3D11_VIEWPORT vp;
    vp.Width = 1280;
    vp.Height = 960;
    vp.MinDepth = 0;
    vp.MaxDepth = 1;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    pContext->RSSetViewports(1u, &vp);

    GFX_THROW_INFO_ONLY(pContext->DrawIndexed(std::size(indices), 0u, 0u));
}

Graphics::HRESException::HRESException(int line, const char* file, HRESULT hr, std::vector<std::string> infoVec) noexcept : Exception(line, file), hr(hr) {
    for (const auto& m : infoVec) {
        info += m;
        info.push_back('\n');
    }
    if (!info.empty()) {
        info.pop_back();
    }
}

const char* Graphics::HRESException::what() const noexcept {
    std::ostringstream oss;
    oss << getType() << std::endl
        << "[Error Code] 0x" << std::hex << getErrorCode()  << std::dec << "(" << getErrorCode() << ")" << std::endl
        << "[Description] " << getErrorDescription() << std::endl
        << getOriginString();
    buf = oss.str();
    return buf.c_str();
}

const char* Graphics::HRESException::getType() const noexcept {
    return "Astra Engine Graphics Exception";
}

HRESULT Graphics::HRESException::getErrorCode() const noexcept { return hr; }

std::string Graphics::HRESException::getErrorDescription() const noexcept
{
    //Here normally we would use DXGetErrorString(), but Microsoft decided to get rid of the dxerr.h library so we have to make do 
    //with the Windows alternative, so just FormatMessageA (it has to be the ANSI (narrow char) version).
    //Now we also don't need the getErrorString function featured in dxerr.h
    char* msgBuf = nullptr;
    DWORD size = FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        hr,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPSTR)&msgBuf,
        0,
        nullptr
    );

    std::string msg = (size && msgBuf) ? std::string(msgBuf) : "Unidentified error code.";
    if (msgBuf) {
        LocalFree(msgBuf);
    }
    return msg;
}

std::string Graphics::HRESException::getErrorInfo() const noexcept
{
    return info;
}

const char* Graphics::DeviceRemovedException::getType() const noexcept {
    return "Astra Engine Exception: Device Removed (DXGI_ERROR_DEVICE_REMOVED)";
}

Graphics::InfoException::InfoException(int line, const char* file, std::vector<std::string> infoVec) noexcept : Exception(line, file) {
    for (const auto& m : infoVec) {
        info += m;
        info.push_back('\n');
    }
    if (!info.empty()) {
        info.pop_back();
    }
}

const char* Graphics::InfoException::what() const noexcept
{
    std::ostringstream oss;
    oss << getType() << std::endl
        << "[Additional Info] " << getErrorInfo() << std::endl
        << "[Description] " << getErrorDescription() << std::endl
        << getOriginString();
    buf = oss.str();
    return buf.c_str();
}

const char* Graphics::InfoException::getType() const noexcept
{
    return "Astra Engine - Info Exception";
}

std::string Graphics::InfoException::getErrorDescription() const noexcept
{
    char* msgBuf = nullptr;
    DWORD size = FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        0,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPSTR)&msgBuf,
        0,
        nullptr
    );

    std::string msg = (size && msgBuf) ? std::string(msgBuf) : "Unidentified error code.";
    if (msgBuf) {
        LocalFree(msgBuf);
    }
    return msg;
}

std::string Graphics::InfoException::getErrorInfo() const noexcept
{
    return info;
}
