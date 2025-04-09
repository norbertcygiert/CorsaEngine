#include "DXGIInfoManager.h"
#include "GraphicsMacros.h"
#include "Window.h"
#include "Graphics.h"
#include <dxgidebug.h>
#include <memory>

#pragma comment(lib, "dxguid.lib")


DXGIInfoManager::DXGIInfoManager()
{
	//Define the signature for DXGIGetDebugInterface
	typedef HRESULT(WINAPI* DXGIGetDebugInterface)(REFIID, void**);

	//Load the dll containing the definition for this signature
	const auto hModDXGIDebug = LoadLibraryEx("dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
	
	if (hModDXGIDebug == nullptr) 
		throw WND_LAST_EXCEPT();

	//Get the adress inside the loaded DLL
	const auto dbgInterfaceAdress = reinterpret_cast<DXGIGetDebugInterface>(
		reinterpret_cast<void*>(GetProcAddress(hModDXGIDebug, "DXGIGetDebugInterface"))
		);
	
	if (dbgInterfaceAdress == nullptr) 
		throw WND_LAST_EXCEPT();
	
	HRESULT cc;
	GRAPHICS_THROW_NOINFO(dbgInterfaceAdress(__uuidof(IDXGIInfoQueue),&pDXGIInfoQueue)); 

}

void DXGIInfoManager::set() noexcept {
	//Set the "next" field value so it only gets errors/messages generated after this call
	next = pDXGIInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
}

std::vector<std::string> DXGIInfoManager::getMessages() const
{
	std::vector<std::string> messages;
	const auto end = pDXGIInfoQueue->GetNumStoredMessages(DXGI_DEBUG_ALL);
	for (auto i = next; i < end; i++)
	{
		HRESULT cc;

		// First get the size of the message in bytes
		SIZE_T msgLen;
		GRAPHICS_THROW_NOINFO(pDXGIInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, nullptr, &msgLen));

		// Create a buffer (an array of bytes) for a message
		auto bytes = std::make_unique<byte[]>(msgLen);
		auto pMessage = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(bytes.get());

		// Get the message
		GRAPHICS_THROW_NOINFO(pDXGIInfoQueue->GetMessage(DXGI_DEBUG_ALL, i, pMessage, &msgLen));

		// We emplace because we dont wanna actually move the message but copy it to the vector instead
		messages.emplace_back(pMessage->pDescription);
	}

	return messages;
}
