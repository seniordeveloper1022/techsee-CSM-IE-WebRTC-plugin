// dllmain.h : Declaration of module class.

class CWebRTCPluginModule : public ATL::CAtlDllModuleT< CWebRTCPluginModule >
{
public :
	DECLARE_LIBID(LIBID_WebRTCPluginLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_WEBRTCPLUGIN, "{30404F38-CA39-462E-83CC-F79276959029}")
};

extern class CWebRTCPluginModule _AtlModule;
