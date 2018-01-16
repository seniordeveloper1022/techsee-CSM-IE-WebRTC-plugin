

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


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


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __WebRTCPlugin_i_h__
#define __WebRTCPlugin_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IWebRTCProxy_FWD_DEFINED__
#define __IWebRTCProxy_FWD_DEFINED__
typedef interface IWebRTCProxy IWebRTCProxy;

#endif 	/* __IWebRTCProxy_FWD_DEFINED__ */


#ifndef __IRTCPeerConnection_FWD_DEFINED__
#define __IRTCPeerConnection_FWD_DEFINED__
typedef interface IRTCPeerConnection IRTCPeerConnection;

#endif 	/* __IRTCPeerConnection_FWD_DEFINED__ */


#ifndef __IMediaStreamTrack_FWD_DEFINED__
#define __IMediaStreamTrack_FWD_DEFINED__
typedef interface IMediaStreamTrack IMediaStreamTrack;

#endif 	/* __IMediaStreamTrack_FWD_DEFINED__ */


#ifndef __IRTPSender_FWD_DEFINED__
#define __IRTPSender_FWD_DEFINED__
typedef interface IRTPSender IRTPSender;

#endif 	/* __IRTPSender_FWD_DEFINED__ */


#ifndef __IVideoRenderer_FWD_DEFINED__
#define __IVideoRenderer_FWD_DEFINED__
typedef interface IVideoRenderer IVideoRenderer;

#endif 	/* __IVideoRenderer_FWD_DEFINED__ */


#ifndef __WebRTCProxy_FWD_DEFINED__
#define __WebRTCProxy_FWD_DEFINED__

#ifdef __cplusplus
typedef class WebRTCProxy WebRTCProxy;
#else
typedef struct WebRTCProxy WebRTCProxy;
#endif /* __cplusplus */

#endif 	/* __WebRTCProxy_FWD_DEFINED__ */


#ifndef __RTCPeerConnection_FWD_DEFINED__
#define __RTCPeerConnection_FWD_DEFINED__

#ifdef __cplusplus
typedef class RTCPeerConnection RTCPeerConnection;
#else
typedef struct RTCPeerConnection RTCPeerConnection;
#endif /* __cplusplus */

#endif 	/* __RTCPeerConnection_FWD_DEFINED__ */


#ifndef __MediaStreamTrack_FWD_DEFINED__
#define __MediaStreamTrack_FWD_DEFINED__

#ifdef __cplusplus
typedef class MediaStreamTrack MediaStreamTrack;
#else
typedef struct MediaStreamTrack MediaStreamTrack;
#endif /* __cplusplus */

#endif 	/* __MediaStreamTrack_FWD_DEFINED__ */


#ifndef __RTPSender_FWD_DEFINED__
#define __RTPSender_FWD_DEFINED__

#ifdef __cplusplus
typedef class RTPSender RTPSender;
#else
typedef struct RTPSender RTPSender;
#endif /* __cplusplus */

#endif 	/* __RTPSender_FWD_DEFINED__ */


#ifndef __VideoRenderer_FWD_DEFINED__
#define __VideoRenderer_FWD_DEFINED__

#ifdef __cplusplus
typedef class VideoRenderer VideoRenderer;
#else
typedef struct VideoRenderer VideoRenderer;
#endif /* __cplusplus */

#endif 	/* __VideoRenderer_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IWebRTCProxy_INTERFACE_DEFINED__
#define __IWebRTCProxy_INTERFACE_DEFINED__

/* interface IWebRTCProxy */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IWebRTCProxy;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("4FE9050D-54FF-4C8A-B781-DF77FD95F24B")
    IWebRTCProxy : public IDispatch
    {
    public:
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE createPeerConnection( 
            /* [in] */ VARIANT configuration,
            /* [retval][out] */ IUnknown **pc) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE createLocalAudioTrack( 
            /* [in] */ VARIANT constraints,
            /* [retval][out] */ IUnknown **track) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE createLocalVideoTrack( 
            /* [in] */ VARIANT constraints,
            /* [retval][out] */ IUnknown **track) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE parseIceCandidate( 
            /* [in] */ VARIANT candidate,
            /* [retval][out] */ VARIANT *parsed) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IWebRTCProxyVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IWebRTCProxy * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IWebRTCProxy * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IWebRTCProxy * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IWebRTCProxy * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IWebRTCProxy * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IWebRTCProxy * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IWebRTCProxy * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *createPeerConnection )( 
            IWebRTCProxy * This,
            /* [in] */ VARIANT configuration,
            /* [retval][out] */ IUnknown **pc);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *createLocalAudioTrack )( 
            IWebRTCProxy * This,
            /* [in] */ VARIANT constraints,
            /* [retval][out] */ IUnknown **track);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *createLocalVideoTrack )( 
            IWebRTCProxy * This,
            /* [in] */ VARIANT constraints,
            /* [retval][out] */ IUnknown **track);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *parseIceCandidate )( 
            IWebRTCProxy * This,
            /* [in] */ VARIANT candidate,
            /* [retval][out] */ VARIANT *parsed);
        
        END_INTERFACE
    } IWebRTCProxyVtbl;

    interface IWebRTCProxy
    {
        CONST_VTBL struct IWebRTCProxyVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IWebRTCProxy_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IWebRTCProxy_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IWebRTCProxy_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IWebRTCProxy_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IWebRTCProxy_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IWebRTCProxy_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IWebRTCProxy_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IWebRTCProxy_createPeerConnection(This,configuration,pc)	\
    ( (This)->lpVtbl -> createPeerConnection(This,configuration,pc) ) 

#define IWebRTCProxy_createLocalAudioTrack(This,constraints,track)	\
    ( (This)->lpVtbl -> createLocalAudioTrack(This,constraints,track) ) 

#define IWebRTCProxy_createLocalVideoTrack(This,constraints,track)	\
    ( (This)->lpVtbl -> createLocalVideoTrack(This,constraints,track) ) 

#define IWebRTCProxy_parseIceCandidate(This,candidate,parsed)	\
    ( (This)->lpVtbl -> parseIceCandidate(This,candidate,parsed) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IWebRTCProxy_INTERFACE_DEFINED__ */


#ifndef __IRTCPeerConnection_INTERFACE_DEFINED__
#define __IRTCPeerConnection_INTERFACE_DEFINED__

/* interface IRTCPeerConnection */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IRTCPeerConnection;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("EA4F5879-986E-4D86-AFFA-710CADB29A45")
    IRTCPeerConnection : public IDispatch
    {
    public:
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE createOffer( 
            /* [in] */ VARIANT successCallback,
            /* [in] */ VARIANT failureCallback,
            /* [in] */ VARIANT options) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE setLocalDescription( 
            /* [in] */ VARIANT successCallback,
            /* [in] */ VARIANT failureCallback,
            /* [in] */ VARIANT description) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE createAnswer( 
            /* [in] */ VARIANT successCallback,
            /* [in] */ VARIANT failureCallback,
            /* [in] */ VARIANT options) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE setRemoteDescription( 
            /* [in] */ VARIANT successCallback,
            /* [in] */ VARIANT failureCallback,
            /* [in] */ VARIANT description) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE addIceCandidate( 
            /* [in] */ VARIANT successCallback,
            /* [in] */ VARIANT failureCallback,
            /* [in] */ VARIANT candidate) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE addTrack( 
            /* [in] */ VARIANT track,
            VARIANT stream,
            /* [retval][out] */ IUnknown **sender) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE removeTrack( 
            /* [in] */ VARIANT sender) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE getRemoteStreamTracks( 
            /* [in] */ VARIANT stream,
            VARIANT successCallback) = 0;
        
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE close( void) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_onnegotiationneeded( 
            /* [in] */ VARIANT handler) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_onicecandidate( 
            /* [in] */ VARIANT handler) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_onicecandidateerror( 
            /* [in] */ VARIANT handler) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_onsignalingstatechange( 
            /* [in] */ VARIANT handler) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_oniceconnectionstatechange( 
            /* [in] */ VARIANT handler) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_onicegatheringstatechange( 
            /* [in] */ VARIANT handler) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_onconnectionstatechange( 
            /* [in] */ VARIANT handler) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_onaddstream( 
            /* [in] */ VARIANT handler) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_onremovestream( 
            /* [in] */ VARIANT handler) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IRTCPeerConnectionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IRTCPeerConnection * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IRTCPeerConnection * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IRTCPeerConnection * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IRTCPeerConnection * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IRTCPeerConnection * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IRTCPeerConnection * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IRTCPeerConnection * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *createOffer )( 
            IRTCPeerConnection * This,
            /* [in] */ VARIANT successCallback,
            /* [in] */ VARIANT failureCallback,
            /* [in] */ VARIANT options);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *setLocalDescription )( 
            IRTCPeerConnection * This,
            /* [in] */ VARIANT successCallback,
            /* [in] */ VARIANT failureCallback,
            /* [in] */ VARIANT description);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *createAnswer )( 
            IRTCPeerConnection * This,
            /* [in] */ VARIANT successCallback,
            /* [in] */ VARIANT failureCallback,
            /* [in] */ VARIANT options);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *setRemoteDescription )( 
            IRTCPeerConnection * This,
            /* [in] */ VARIANT successCallback,
            /* [in] */ VARIANT failureCallback,
            /* [in] */ VARIANT description);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *addIceCandidate )( 
            IRTCPeerConnection * This,
            /* [in] */ VARIANT successCallback,
            /* [in] */ VARIANT failureCallback,
            /* [in] */ VARIANT candidate);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *addTrack )( 
            IRTCPeerConnection * This,
            /* [in] */ VARIANT track,
            VARIANT stream,
            /* [retval][out] */ IUnknown **sender);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *removeTrack )( 
            IRTCPeerConnection * This,
            /* [in] */ VARIANT sender);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *getRemoteStreamTracks )( 
            IRTCPeerConnection * This,
            /* [in] */ VARIANT stream,
            VARIANT successCallback);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *close )( 
            IRTCPeerConnection * This);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_onnegotiationneeded )( 
            IRTCPeerConnection * This,
            /* [in] */ VARIANT handler);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_onicecandidate )( 
            IRTCPeerConnection * This,
            /* [in] */ VARIANT handler);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_onicecandidateerror )( 
            IRTCPeerConnection * This,
            /* [in] */ VARIANT handler);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_onsignalingstatechange )( 
            IRTCPeerConnection * This,
            /* [in] */ VARIANT handler);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_oniceconnectionstatechange )( 
            IRTCPeerConnection * This,
            /* [in] */ VARIANT handler);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_onicegatheringstatechange )( 
            IRTCPeerConnection * This,
            /* [in] */ VARIANT handler);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_onconnectionstatechange )( 
            IRTCPeerConnection * This,
            /* [in] */ VARIANT handler);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_onaddstream )( 
            IRTCPeerConnection * This,
            /* [in] */ VARIANT handler);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_onremovestream )( 
            IRTCPeerConnection * This,
            /* [in] */ VARIANT handler);
        
        END_INTERFACE
    } IRTCPeerConnectionVtbl;

    interface IRTCPeerConnection
    {
        CONST_VTBL struct IRTCPeerConnectionVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IRTCPeerConnection_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IRTCPeerConnection_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IRTCPeerConnection_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IRTCPeerConnection_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IRTCPeerConnection_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IRTCPeerConnection_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IRTCPeerConnection_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IRTCPeerConnection_createOffer(This,successCallback,failureCallback,options)	\
    ( (This)->lpVtbl -> createOffer(This,successCallback,failureCallback,options) ) 

#define IRTCPeerConnection_setLocalDescription(This,successCallback,failureCallback,description)	\
    ( (This)->lpVtbl -> setLocalDescription(This,successCallback,failureCallback,description) ) 

#define IRTCPeerConnection_createAnswer(This,successCallback,failureCallback,options)	\
    ( (This)->lpVtbl -> createAnswer(This,successCallback,failureCallback,options) ) 

#define IRTCPeerConnection_setRemoteDescription(This,successCallback,failureCallback,description)	\
    ( (This)->lpVtbl -> setRemoteDescription(This,successCallback,failureCallback,description) ) 

#define IRTCPeerConnection_addIceCandidate(This,successCallback,failureCallback,candidate)	\
    ( (This)->lpVtbl -> addIceCandidate(This,successCallback,failureCallback,candidate) ) 

#define IRTCPeerConnection_addTrack(This,track,stream,sender)	\
    ( (This)->lpVtbl -> addTrack(This,track,stream,sender) ) 

#define IRTCPeerConnection_removeTrack(This,sender)	\
    ( (This)->lpVtbl -> removeTrack(This,sender) ) 

#define IRTCPeerConnection_getRemoteStreamTracks(This,stream,successCallback)	\
    ( (This)->lpVtbl -> getRemoteStreamTracks(This,stream,successCallback) ) 

#define IRTCPeerConnection_close(This)	\
    ( (This)->lpVtbl -> close(This) ) 

#define IRTCPeerConnection_put_onnegotiationneeded(This,handler)	\
    ( (This)->lpVtbl -> put_onnegotiationneeded(This,handler) ) 

#define IRTCPeerConnection_put_onicecandidate(This,handler)	\
    ( (This)->lpVtbl -> put_onicecandidate(This,handler) ) 

#define IRTCPeerConnection_put_onicecandidateerror(This,handler)	\
    ( (This)->lpVtbl -> put_onicecandidateerror(This,handler) ) 

#define IRTCPeerConnection_put_onsignalingstatechange(This,handler)	\
    ( (This)->lpVtbl -> put_onsignalingstatechange(This,handler) ) 

#define IRTCPeerConnection_put_oniceconnectionstatechange(This,handler)	\
    ( (This)->lpVtbl -> put_oniceconnectionstatechange(This,handler) ) 

#define IRTCPeerConnection_put_onicegatheringstatechange(This,handler)	\
    ( (This)->lpVtbl -> put_onicegatheringstatechange(This,handler) ) 

#define IRTCPeerConnection_put_onconnectionstatechange(This,handler)	\
    ( (This)->lpVtbl -> put_onconnectionstatechange(This,handler) ) 

#define IRTCPeerConnection_put_onaddstream(This,handler)	\
    ( (This)->lpVtbl -> put_onaddstream(This,handler) ) 

#define IRTCPeerConnection_put_onremovestream(This,handler)	\
    ( (This)->lpVtbl -> put_onremovestream(This,handler) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IRTCPeerConnection_INTERFACE_DEFINED__ */


#ifndef __IMediaStreamTrack_INTERFACE_DEFINED__
#define __IMediaStreamTrack_INTERFACE_DEFINED__

/* interface IMediaStreamTrack */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IMediaStreamTrack;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("7C0AB5CB-FDF9-4C2E-93BD-E9AF0074539C")
    IMediaStreamTrack : public IDispatch
    {
    public:
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_id( 
            /* [retval][out] */ VARIANT *val) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_kind( 
            /* [retval][out] */ VARIANT *val) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_label( 
            /* [retval][out] */ VARIANT *val) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_state( 
            /* [retval][out] */ VARIANT *val) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_enabled( 
            /* [retval][out] */ VARIANT *val) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_enabled( 
            /* [in] */ VARIANT val) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IMediaStreamTrackVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMediaStreamTrack * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMediaStreamTrack * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMediaStreamTrack * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IMediaStreamTrack * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IMediaStreamTrack * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IMediaStreamTrack * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IMediaStreamTrack * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_id )( 
            IMediaStreamTrack * This,
            /* [retval][out] */ VARIANT *val);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_kind )( 
            IMediaStreamTrack * This,
            /* [retval][out] */ VARIANT *val);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_label )( 
            IMediaStreamTrack * This,
            /* [retval][out] */ VARIANT *val);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_state )( 
            IMediaStreamTrack * This,
            /* [retval][out] */ VARIANT *val);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_enabled )( 
            IMediaStreamTrack * This,
            /* [retval][out] */ VARIANT *val);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_enabled )( 
            IMediaStreamTrack * This,
            /* [in] */ VARIANT val);
        
        END_INTERFACE
    } IMediaStreamTrackVtbl;

    interface IMediaStreamTrack
    {
        CONST_VTBL struct IMediaStreamTrackVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMediaStreamTrack_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMediaStreamTrack_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMediaStreamTrack_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMediaStreamTrack_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IMediaStreamTrack_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IMediaStreamTrack_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IMediaStreamTrack_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IMediaStreamTrack_get_id(This,val)	\
    ( (This)->lpVtbl -> get_id(This,val) ) 

#define IMediaStreamTrack_get_kind(This,val)	\
    ( (This)->lpVtbl -> get_kind(This,val) ) 

#define IMediaStreamTrack_get_label(This,val)	\
    ( (This)->lpVtbl -> get_label(This,val) ) 

#define IMediaStreamTrack_get_state(This,val)	\
    ( (This)->lpVtbl -> get_state(This,val) ) 

#define IMediaStreamTrack_get_enabled(This,val)	\
    ( (This)->lpVtbl -> get_enabled(This,val) ) 

#define IMediaStreamTrack_put_enabled(This,val)	\
    ( (This)->lpVtbl -> put_enabled(This,val) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMediaStreamTrack_INTERFACE_DEFINED__ */


#ifndef __IRTPSender_INTERFACE_DEFINED__
#define __IRTPSender_INTERFACE_DEFINED__

/* interface IRTPSender */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IRTPSender;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("291C777C-0E58-4B9C-96CE-AADCD0945826")
    IRTPSender : public IDispatch
    {
    public:
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_id( 
            /* [retval][out] */ VARIANT *val) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IRTPSenderVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IRTPSender * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IRTPSender * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IRTPSender * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IRTPSender * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IRTPSender * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IRTPSender * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IRTPSender * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_id )( 
            IRTPSender * This,
            /* [retval][out] */ VARIANT *val);
        
        END_INTERFACE
    } IRTPSenderVtbl;

    interface IRTPSender
    {
        CONST_VTBL struct IRTPSenderVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IRTPSender_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IRTPSender_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IRTPSender_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IRTPSender_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IRTPSender_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IRTPSender_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IRTPSender_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IRTPSender_get_id(This,val)	\
    ( (This)->lpVtbl -> get_id(This,val) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IRTPSender_INTERFACE_DEFINED__ */


#ifndef __IVideoRenderer_INTERFACE_DEFINED__
#define __IVideoRenderer_INTERFACE_DEFINED__

/* interface IVideoRenderer */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IVideoRenderer;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("65C1AD7A-7A7F-4139-BCC4-768DF33BF2D1")
    IVideoRenderer : public IDispatch
    {
    public:
        virtual /* [local][id] */ HRESULT STDMETHODCALLTYPE setTrack( 
            /* [in] */ VARIANT track) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_videoWidth( 
            /* [retval][out] */ SHORT *pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_videoHeight( 
            /* [retval][out] */ SHORT *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_onresize( 
            /* [in] */ VARIANT handler) = 0;
        
    };
    
    
#else 	/* C style interface */

    typedef struct IVideoRendererVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IVideoRenderer * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IVideoRenderer * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IVideoRenderer * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IVideoRenderer * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IVideoRenderer * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IVideoRenderer * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IVideoRenderer * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        /* [local][id] */ HRESULT ( STDMETHODCALLTYPE *setTrack )( 
            IVideoRenderer * This,
            /* [in] */ VARIANT track);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_videoWidth )( 
            IVideoRenderer * This,
            /* [retval][out] */ SHORT *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_videoHeight )( 
            IVideoRenderer * This,
            /* [retval][out] */ SHORT *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_onresize )( 
            IVideoRenderer * This,
            /* [in] */ VARIANT handler);
        
        END_INTERFACE
    } IVideoRendererVtbl;

    interface IVideoRenderer
    {
        CONST_VTBL struct IVideoRendererVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IVideoRenderer_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IVideoRenderer_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IVideoRenderer_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IVideoRenderer_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IVideoRenderer_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IVideoRenderer_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IVideoRenderer_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IVideoRenderer_setTrack(This,track)	\
    ( (This)->lpVtbl -> setTrack(This,track) ) 

#define IVideoRenderer_get_videoWidth(This,pVal)	\
    ( (This)->lpVtbl -> get_videoWidth(This,pVal) ) 

#define IVideoRenderer_get_videoHeight(This,pVal)	\
    ( (This)->lpVtbl -> get_videoHeight(This,pVal) ) 

#define IVideoRenderer_put_onresize(This,handler)	\
    ( (This)->lpVtbl -> put_onresize(This,handler) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IVideoRenderer_INTERFACE_DEFINED__ */



#ifndef __WebRTCPluginLib_LIBRARY_DEFINED__
#define __WebRTCPluginLib_LIBRARY_DEFINED__

/* library WebRTCPluginLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_WebRTCPluginLib;

EXTERN_C const CLSID CLSID_WebRTCProxy;

#ifdef __cplusplus

class DECLSPEC_UUID("78190E67-29CE-46BC-BCCA-96E108E5D56E")
WebRTCProxy;
#endif

EXTERN_C const CLSID CLSID_RTCPeerConnection;

#ifdef __cplusplus

class DECLSPEC_UUID("93AA26E7-51F0-4BEC-8FEE-E59C605D503B")
RTCPeerConnection;
#endif

EXTERN_C const CLSID CLSID_MediaStreamTrack;

#ifdef __cplusplus

class DECLSPEC_UUID("93C5AC2B-5F36-4FAA-B830-CE64AFE921E5")
MediaStreamTrack;
#endif

EXTERN_C const CLSID CLSID_RTPSender;

#ifdef __cplusplus

class DECLSPEC_UUID("C98B209A-7914-466A-BD02-0A4A13E15D83")
RTPSender;
#endif

EXTERN_C const CLSID CLSID_VideoRenderer;

#ifdef __cplusplus

class DECLSPEC_UUID("B8E874D9-72BA-4E54-B59B-3513081DF516")
VideoRenderer;
#endif
#endif /* __WebRTCPluginLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long *, unsigned long            , VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserMarshal(  unsigned long *, unsigned char *, VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserUnmarshal(unsigned long *, unsigned char *, VARIANT * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long *, VARIANT * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


