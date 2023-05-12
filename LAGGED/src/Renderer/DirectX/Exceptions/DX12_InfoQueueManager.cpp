#include "Precomp.h"
#include "DX12_InfoQueueManager.h"
#include "DX12_ExceptionMacros.h"

//Including this lib solves some of the linker issues related to the extern DXGI_DEBUG_ALL
#pragma comment(lib, "dxguid.lib")


namespace LAG
{
	namespace InfoQueueManager
	{
		struct ExceptionInfo
		{
			Microsoft::WRL::ComPtr<IDXGIInfoQueue> infoQueuePtr;
			UINT64 exceptionFrameStartMessageCount = 0;
		};
		static ExceptionInfo* exceptionInfo = new ExceptionInfo();

		void Initialize()
		{
			//Setting up the exception info queue for the m_InfoQueuePtr pointer.
			//Kind of a tough situation, because we need to get the info queue before the device is created.
			//Some code taken from the following page: https://stackoverflow.com/questions/63090885/directx-get-infoqueue-before-device

			//First, create a typedef for the DXGIDebugInterface function to make the code a bit more readable. 
			typedef HRESULT(WINAPI* DXGIDebugInterface)(REFIID, void**);

			//We need to find the DXGIGetDebugInterface function. In order to do that, we first need to load the DLL that contains this function.
			HMODULE mDebugDLL = LoadLibraryEx("dxgidebug.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
			if (mDebugDLL == nullptr)
			{
				LAG_GRAPHICS_EXCEPTION_PREV();
			}

			//Now that the DLL is open, get the address of the DXGIGetDebugInterface function.
			DXGIDebugInterface dxgiDebugInterface = reinterpret_cast<DXGIDebugInterface>(reinterpret_cast<void*>(GetProcAddress(mDebugDLL, "DXGIGetDebugInterface")));
			if (dxgiDebugInterface == nullptr)
			{
				LAG_GRAPHICS_EXCEPTION_PREV();
			}

			LAG_GRAPHICS_EXCEPTION(dxgiDebugInterface(__uuidof(IDXGIInfoQueue), &exceptionInfo->infoQueuePtr));
		}

		void Shutdown()
		{
			if (exceptionInfo != nullptr)
				delete exceptionInfo;
		}

		void FrameStart()
		{
			//Set the m_TotalInfoMsgCount to the NumStoredMessages for all debug types.
			exceptionInfo->exceptionFrameStartMessageCount = exceptionInfo->infoQueuePtr->GetNumStoredMessages(DXGI_DEBUG_ALL);
		}

		std::vector<std::string> GetExceptionInfoMessages()
		{
			std::vector<std::string> queueMessages;
			UINT64 queueSize = exceptionInfo->infoQueuePtr->GetNumStoredMessages(DXGI_DEBUG_ALL);

			for (UINT64 i = exceptionInfo->exceptionFrameStartMessageCount; i < queueSize; i++)
			{
				//Get the size of the message
				SIZE_T messageLength = 0;
				LAG_GRAPHICS_EXCEPTION(exceptionInfo->infoQueuePtr->GetMessage(DXGI_DEBUG_ALL, i, nullptr, &messageLength));

				//Create a message buffer
				byte* byteroos = static_cast<byte*>(malloc(messageLength));
				DXGI_INFO_QUEUE_MESSAGE* pMessage = reinterpret_cast<DXGI_INFO_QUEUE_MESSAGE*>(byteroos);

				//Get the message. 
				LAG_GRAPHICS_EXCEPTION(exceptionInfo->infoQueuePtr->GetMessage(DXGI_DEBUG_ALL, i, pMessage, &messageLength));

				if (pMessage)
				{
					//Push the message into the vec. Do all of this until all messages are in the vec.
					queueMessages.emplace_back(pMessage->pDescription);
				}
				else queueMessages.emplace_back("pMessage unavailable in ExceptionInfoQueue.");


				//Delete the allocated bytes (for the message) and check for any previous errors. 
				if (byteroos != nullptr) free(byteroos);
			}

			return queueMessages;
		}
	}
}

