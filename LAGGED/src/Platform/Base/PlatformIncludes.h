#pragma once

//If we're using windows, include the correct Win headers
#ifdef PLATFORM_WINDOWS

	//Target windows 8 and/or later: https://docs.microsoft.com/en-us/windows/win32/winprog/using-the-windows-headers?redirectedfrom=MSDN
	#define _WIN32_WINNT 0x0602
	
	//Taken from PlanetChili hw3d project.
	//Disables a bunch of unused windows stuff. 
	//If strange errors related to windows api / win32 show up, try removing some of the defines below.
	//Doing this might fix issues, but will also increase build time. So be careful!
	#define WIN32_LEAN_AND_MEAN
	#define NOGDICAPMASKS
	//#define NOSYSMETRICS
	#define NOMENUS
	#define NOICONS
	#define NOSYSCOMMANDS
	#define NORASTEROPS
	#define OEMRESOURCE
	#define NOATOM
	#define NOCLIPBOARD
	#define NOCOLOR
	#define NOCTLMGR
	#define NODRAWTEXT
	#define NOKERNEL
	//#define NONLS //Commented out, because it disables WideCharToMultiByte(), which is used in tinyGLTF
	#define NOMEMMGR
	#define NOMETAFILE
	#define NOOPENFILE
	#define NOSCROLL
	#define NOSERVICE
	#define NOSOUND
	#define NOTEXTMETRIC
	#define NOWH
	#define NOCOMM
	#define NOKANJI
	#define NOHELP
	#define NOPROFILER
	#define NODEFERWINDOWPOS
	#define NOMCX
	#define NORPC
	#define NOPROXYSTUB
	#define NOIMAGE
	#define NOTAPE
	
	#define NOMINMAX
	
	#define STRICT
	
	#include <Windows.h>
	#include <shellapi.h>

#endif // PLATFORM_WINDOWS

//If we're using DirectX, include all the DirectX headers
#ifdef PLATFORM_DIRECTX
	
//Include Windows Runtime Library and use a namespace to avoid typing Microsoft::WRL everytime
	#include <wrl.h>
	using namespace Microsoft::WRL;
	
	#include <d3d12.h>
	#include <dxgi1_6.h>
	#include <d3dcompiler.h>
	#include <DirectXMath.h>
	
	//DirectX12 extensions: 
	#include "Platform/DX12/Renderer/d3dx12.h"
	#include "SimpleMath.h"

#endif

#ifdef PLATFORM_OPENGL

#endif
