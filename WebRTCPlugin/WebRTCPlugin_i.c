

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 04:14:07 2038
 */
/* Compiler settings for WebRTCPlugin.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.01.0622 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        EXTERN_C __declspec(selectany) const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif // !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, IID_IWebRTCProxy,0x4FE9050D,0x54FF,0x4C8A,0xB7,0x81,0xDF,0x77,0xFD,0x95,0xF2,0x4B);


MIDL_DEFINE_GUID(IID, IID_IRTCPeerConnection,0xEA4F5879,0x986E,0x4D86,0xAF,0xFA,0x71,0x0C,0xAD,0xB2,0x9A,0x45);


MIDL_DEFINE_GUID(IID, IID_IMediaStreamTrack,0x7C0AB5CB,0xFDF9,0x4C2E,0x93,0xBD,0xE9,0xAF,0x00,0x74,0x53,0x9C);


MIDL_DEFINE_GUID(IID, IID_IRTPSender,0x291C777C,0x0E58,0x4B9C,0x96,0xCE,0xAA,0xDC,0xD0,0x94,0x58,0x26);


MIDL_DEFINE_GUID(IID, IID_IVideoRenderer,0x65C1AD7A,0x7A7F,0x4139,0xBC,0xC4,0x76,0x8D,0xF3,0x3B,0xF2,0xD1);


MIDL_DEFINE_GUID(IID, IID_IDataChannel,0xa5082e08,0xd3f5,0x41e8,0x80,0xa5,0xe7,0xfd,0x2a,0x83,0x34,0xe7);


MIDL_DEFINE_GUID(IID, LIBID_WebRTCPluginLib,0xD4447E9C,0x3398,0x4C2A,0xAD,0xB9,0x54,0xC5,0x74,0x41,0xF4,0x77);


MIDL_DEFINE_GUID(CLSID, CLSID_WebRTCProxy,0x78190E67,0x29CE,0x46BC,0xBC,0xCA,0x96,0xE1,0x08,0xE5,0xD5,0x6E);


MIDL_DEFINE_GUID(CLSID, CLSID_RTCPeerConnection,0x93AA26E7,0x51F0,0x4BEC,0x8F,0xEE,0xE5,0x9C,0x60,0x5D,0x50,0x3B);


MIDL_DEFINE_GUID(CLSID, CLSID_MediaStreamTrack,0x93C5AC2B,0x5F36,0x4FAA,0xB8,0x30,0xCE,0x64,0xAF,0xE9,0x21,0xE5);


MIDL_DEFINE_GUID(CLSID, CLSID_RTPSender,0xC98B209A,0x7914,0x466A,0xBD,0x02,0x0A,0x4A,0x13,0xE1,0x5D,0x83);


MIDL_DEFINE_GUID(CLSID, CLSID_VideoRenderer,0xB8E874D9,0x72BA,0x4E54,0xB5,0x9B,0x35,0x13,0x08,0x1D,0xF5,0x16);


MIDL_DEFINE_GUID(CLSID, CLSID_DataChannel,0x093a1398,0xbd66,0x4547,0xaa,0x73,0x79,0xd6,0xa0,0xc7,0xe5,0x78);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



