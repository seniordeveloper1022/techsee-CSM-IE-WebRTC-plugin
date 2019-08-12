// VideoRenderer.h : Declaration of the CVideoRenderer
#pragma once
#include "resource.h"       // main symbols
#include <atlctl.h>
#include "WebRTCPlugin_i.h"
#include "MediaStreamTrack.h"
#include "CallbackDispatcher.h"
#include  <mutex>

#include "api/video/video_frame.h"
#include "api/video/video_sink_interface.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

using namespace ATL;



struct VideoBuffer
{
	size_t width	= 0;
	size_t height	= 0;
	uint8_t* image	= nullptr;

	~VideoBuffer()
	{
		if (image) free(image);
	}
};

// CVideoRenderer
class ATL_NO_VTABLE CVideoRenderer :
	public CComObjectRootEx<CComSingleThreadModel>,
	public IDispatchImpl<ICVideoRenderer, &IID_ICVideoRenderer, &LIBID_WebRTCPluginLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IOleControlImpl<CVideoRenderer>,
	public IOleObjectImpl<CVideoRenderer>,
	public IOleInPlaceActiveObjectImpl<CVideoRenderer>,
	public IViewObjectExImpl<CVideoRenderer>,
	public IOleInPlaceObjectWindowlessImpl<CVideoRenderer>,
	public IObjectSafetyImpl<CVideoRenderer, INTERFACESAFE_FOR_UNTRUSTED_CALLER>,
	public CComCoClass<CVideoRenderer, &CLSID_CVideoRenderer>,
	public CComControl<CVideoRenderer>,
	public rtc::VideoSinkInterface<webrtc::VideoFrame>,
	public CallbackDispatcher<IUnknown>
{
public:


	CVideoRenderer() : shadowWindow(_T("ShadowVideoRenderer"), this, 1)
	{
		
	}

	DECLARE_OLEMISC_STATUS(
	    OLEMISC_RECOMPOSEONRESIZE |
		OLEMISC_CANTLINKINSIDE |
		OLEMISC_INSIDEOUT |
		OLEMISC_ACTIVATEWHENVISIBLE |
		OLEMISC_SETCLIENTSITEFIRST
		)

		DECLARE_REGISTRY_RESOURCEID(IDR_VIDEORENDERER)
		

	DECLARE_NOT_AGGREGATABLE(CVideoRenderer)

	BEGIN_COM_MAP(CVideoRenderer)
		COM_INTERFACE_ENTRY(ICVideoRenderer)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(IViewObjectEx)
		COM_INTERFACE_ENTRY(IViewObject2)
		COM_INTERFACE_ENTRY(IViewObject)
		COM_INTERFACE_ENTRY(IOleInPlaceObjectWindowless)
		COM_INTERFACE_ENTRY(IOleInPlaceObject)
		COM_INTERFACE_ENTRY2(IOleWindow, IOleInPlaceObjectWindowless)
		COM_INTERFACE_ENTRY(IOleInPlaceActiveObject)
		COM_INTERFACE_ENTRY(IOleControl)
		COM_INTERFACE_ENTRY(IOleObject)
		COM_INTERFACE_ENTRY_IID(IID_IObjectSafety, IObjectSafety)
	END_COM_MAP()

	BEGIN_PROP_MAP(CVideoRenderer)
		PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
		PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
		// Example entries
		// PROP_ENTRY_TYPE("Property Name", dispid, clsid, vtType)
		// PROP_PAGE(CLSID_StockColorPage)
	END_PROP_MAP()


	BEGIN_MSG_MAP(CVideoRenderer)
		CHAIN_MSG_MAP(CComControl<CVideoRenderer>)
		DEFAULT_REFLECTION_HANDLER()
	END_MSG_MAP()
	// Handler prototypes:
	//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
	//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	// IViewObjectEx
	DECLARE_VIEW_STATUS(0)

	// ICVideoRenderer
public:

	virtual void OnFrame(const webrtc::VideoFrame& frame) override;
	HRESULT OnDrawAdvanced(ATL_DRAWINFO& di);


	
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct();

	HRESULT OnPostVerbInPlaceActivate()
	{
		// Get parent window
		return  m_spInPlaceSite->GetWindow(&hwndParent);
	}

	void FinalRelease()
	{
	}

	STDMETHOD(setTrack) (VARIANT track);
	STDMETHOD(get_videoWidth)(SHORT* pVal)
	{
		*pVal = (SHORT)videoWidth;
		return S_OK;
	}
	STDMETHOD(get_videoHeight)(SHORT* pVal)
	{
		*pVal = (SHORT)videoHeight;
		return S_OK;
	}
	STDMETHODIMP put_onresize(VARIANT handler) { return MarshalCallback(onresize, handler); }

private:
	
	std::shared_ptr<webrtc::VideoFrame> frames[2];
	std::mutex mutex;
	bool background = 0;
	size_t videoWidth;
	size_t videoHeight;

	HWND hwndParent;
	Callback onresize;
	CContainedWindow shadowWindow;
};

OBJECT_ENTRY_AUTO(__uuidof(CVideoRenderer), CVideoRenderer)
