#pragma once


#define GRAPHICS_THROW_NOINFO(call) if( FAILED( cc = (call) ) ) throw Graphics::HRESException( __LINE__,__FILE__,cc, {})

//We need an HRESULT codecatcher in scope for these macros two work.
#define GFX_EXCEPT_NOINFO(cc) Graphics::HRESException( __LINE__,__FILE__,(cc) )
#define GFX_THROW_NOINFO(cc) if( FAILED( cc = (call) ) ) throw Graphics::HRESException( __LINE__,__FILE__,cc )

#ifndef NDEBUG

#define INFO_INSERT(g) HRESULT cc; DXGIInfoManager& infoManager = getInfoManager((g))
#define GFX_EXCEPT(cc) Graphics::HRESException( __LINE__,__FILE__,(cc),infoManager.getMessages() )
#define GFX_THROW_INFO(call) infoManager.set(); if( FAILED( cc = (call) ) ) throw GFX_EXCEPT(cc)
#define GFX_DEVICE_REMOVED_EXCEPT(cc) Graphics::DeviceRemovedException( __LINE__,__FILE__,(cc),infoManager.getMessages() )
#define GFX_THROW_INFO_ONLY(call) infoManager.set(); (call); {auto v = infoManager.getMessages(); if(!v.empty()) {throw Graphics::InfoException( __LINE__,__FILE__,v);}}

#else

#define INFO_INSERT(g) HRESULT cc
#define GFX_EXCEPT(cc) Graphics::HRESException( __LINE__,__FILE__,(cc) )
#define GFX_THROW_INFO(call) GFX_THROW_NOINFO(call)
#define GFX_DEVICE_REMOVED_EXCEPT(cc) Graphics::DeviceRemovedException( __LINE__,__FILE__,(cc) )
#define GFX_THROW_INFO_ONLY(call) (call)

#endif