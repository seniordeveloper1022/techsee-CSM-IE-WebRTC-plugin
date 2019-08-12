// WebRTCProxy.cpp : Implementation of WebRTCProxy
#include "stdafx.h"
#include <atlsafe.h>

#include "JSObject.h"
#include "WebRTCProxy.h"
#include "RTCPeerConnection.h"
#include "MediaStreamTrack.h"

#include "rtc_base/ssl_adapter.h"
#include "api/create_peerconnection_factory.h"
#include "api/peer_connection_interface.h"
#include "api/audio_codecs/builtin_audio_decoder_factory.h"
#include "api/audio_codecs/builtin_audio_encoder_factory.h"

// Normal Device Capture
#include "modules/video_capture/video_capture.h"
#include "modules/video_capture/video_capture_factory.h"
#include "pc/video_track_source.h"
#include "VcmCapturer.hpp"

bool WebRTCProxy::inited = false;
std::shared_ptr<rtc::Thread> WebRTCProxy::signalingThread;
std::shared_ptr<rtc::Thread> WebRTCProxy::eventThread;
std::shared_ptr<rtc::Thread> WebRTCProxy::workThread;
std::shared_ptr<rtc::Thread> WebRTCProxy::networkThread;

// WebRTCProxy
HRESULT WebRTCProxy::FinalConstruct()
{
	if (!inited)
	{
		//rtc::LogMessage::ConfigureLogging("sensitive debug");
		rtc::LogMessage::ConfigureLogging("error");
		rtc::InitializeSSL();
		rtc::InitRandom(rtc::Time());

		signalingThread = std::shared_ptr<rtc::Thread>(rtc::Thread::Create().release());
		eventThread = std::shared_ptr<rtc::Thread>(rtc::Thread::Create().release());
		workThread = std::shared_ptr<rtc::Thread>(rtc::Thread::Create().release());
		networkThread = std::shared_ptr<rtc::Thread>(rtc::Thread::CreateWithSocketServer().release());

		signalingThread->SetName("signaling_thread", NULL);
		eventThread->SetName("event_thread", NULL);
		workThread->SetName("work_thread", NULL);
		networkThread->SetName("network_thread", NULL);

		if (!signalingThread->Start() || !eventThread->Start() || !workThread->Start() || !networkThread->Start())
			return false;

		//Initialize things on event thread
		eventThread->Invoke<void>(RTC_FROM_HERE, []() {
			CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
		});

		inited = true;
	}

	//Create peer connection factory
	peer_connection_factory_ =
		webrtc::CreatePeerConnectionFactory(
			networkThread.get(),
			workThread.get(),
			signalingThread.get(),
			NULL,
			webrtc::CreateBuiltinAudioEncoderFactory(),
			webrtc::CreateBuiltinAudioDecoderFactory(),
			NULL,
			NULL,
			NULL,
			NULL
		).release();

	//Check
	if (!peer_connection_factory_)
		return S_FALSE;

	return S_OK;
}

void WebRTCProxy::FinalRelease()
{
	//Remove factory
	peer_connection_factory_ = nullptr;
}

STDMETHODIMP WebRTCProxy::createPeerConnection(VARIANT variant, IUnknown** peerConnection)
{
	webrtc::PeerConnectionInterface::RTCConfiguration configuration;
	JSObject obj(variant);

	if (!obj.isNull())
	{
		/*
		dictionary RTCIceServer {
		  required (DOMString or sequence<DOMString>) urls;
		  DOMString                          username;
		  (DOMString or RTCOAuthCredential)  credential;
		  RTCIceCredentialType               credentialType = "password";
		};

		dictionary RTCConfiguration {
		  sequence<RTCIceServer>   iceServers;
		  RTCIceTransportPolicy    iceTransportPolicy = "all";
		  RTCBundlePolicy          bundlePolicy = "balanced";
		  RTCRtcpMuxPolicy         rtcpMuxPolicy = "require";
		  DOMString                peerIdentity;
		  sequence<RTCCertificate> certificates;
		  [EnforceRange]
		  octet                    iceCandidatePoolSize = 0;
		  */
		  //Get ice servers array
		JSObject iceServers = obj.GetProperty(L"iceServers");
		//TODO: support the following ones
		_bstr_t bundlePolicy = obj.GetStringProperty(L"bundlePolicy");
		_bstr_t rtcpMuxPolicy = obj.GetStringProperty(L"rtcpMuxPolicy");
		//_bstr_t peerIdentity      = obj.GetStringProperty(L"peerIdentity");
		int iceCandidatePoolSize = obj.GetIntegerProperty(L"iceServers");

		//If we have them
		if (!iceServers.isNull())
		{
			//For each property
			for (auto name : iceServers.GetPropertyNames())
			{
				//Get ice server
				JSObject server = iceServers.GetProperty(name);
				//If we have it
				if (!server.isNull())
				{
					webrtc::PeerConnectionInterface::IceServer iceServer;

					//Get the values
					auto urls = server.GetProperty(L"urls");
					_bstr_t username = server.GetStringProperty(L"username");
					_bstr_t credential = server.GetStringProperty(L"credential");
					//TODO: Support credential type
					_bstr_t credentialType = server.GetStringProperty(L"credentialType"); //Not supported yet
					//if url is an string
					if (urls.vt == VT_BSTR)
					{
						//Get url
						_bstr_t url(urls.bstrVal);
						//Append
						iceServer.urls.push_back((char*)url);
					}
					else {
						//Convert to object
						JSObject aux(urls);
						//Ensure we have it
						if (!aux.isNull())
						{
							//Get all urls
							for (auto idx : aux.GetPropertyNames())
							{
								//Get url
								_bstr_t url = aux.GetStringProperty(idx);
								//Append
								iceServer.urls.push_back((char*)url);
							}
						}
					}
					//Set username and credential, OATH not supported yet
					if ((char*)username)
						iceServer.username = (char*)username;
					if ((char*)credential)
						iceServer.password = (char*)credential;
					//Push
					configuration.servers.push_back(iceServer);
				}
			}
		}
	};

	//Create activeX object which is a
	CComObject<RTCPeerConnection>* pc;
	HRESULT hresult = CComObject<RTCPeerConnection>::CreateInstance(&pc);

	if (FAILED(hresult))
		return hresult;

	//Create peerconnection object, it will call the AddRef inside as it gets a ref to the observer
	rtc::scoped_refptr<webrtc::PeerConnectionInterface> pci = peer_connection_factory_->CreatePeerConnection(
		configuration,
		NULL,
		NULL,
		pc
	);

	//Check it was created correctly
	if (!pci)
		//Error
		return E_INVALIDARG;

	//Set event thread
	pc->SetThread(eventThread);

	//Attach to PC
	pc->Attach(pci);

	//Get Reference to pass it to JS
	*peerConnection = pc->GetUnknown();

	//Add JS reference
	(*peerConnection)->AddRef();

	//OK
	return hresult;
}


STDMETHODIMP WebRTCProxy::createLocalAudioTrack(VARIANT constraints, IUnknown** track)
{
	const cricket::AudioOptions options;
	//Create audio source
	auto audioSource = peer_connection_factory_->CreateAudioSource(options);

	//Ensure it is created
	if (!audioSource)
		return E_UNEXPECTED;

	//Create track
	rtc::scoped_refptr<webrtc::MediaStreamTrackInterface> audioTrack = peer_connection_factory_->CreateAudioTrack("audio", audioSource);

	//Ensure it is created
	if (!audioTrack)
		return E_UNEXPECTED;

	//Create activeX object which is a
	CComObject<MediaStreamTrack>* mediaStreamTrack;
	HRESULT hresult = CComObject<MediaStreamTrack>::CreateInstance(&mediaStreamTrack);

	if (FAILED(hresult))
		return hresult;

	//Attach to native track
	mediaStreamTrack->Attach(audioTrack);

	//Set dummy audio  label
	mediaStreamTrack->SetLabel("Default Audio Device");

	//Get Reference to pass it to JS
	*track = mediaStreamTrack->GetUnknown();

	//Add JS reference
	(*track)->AddRef();

	//OK
	return hresult;
}

class CapturerTrackSource : public webrtc::VideoTrackSource
{
public:
	static rtc::scoped_refptr<CapturerTrackSource> Create()
	{
		const size_t kWidth = 640;
		const size_t kHeight = 480;
		const size_t kFps = 30;
		const size_t kDeviceIndex = 0;
		std::unique_ptr<VcmCapturer> capturer =
			absl::WrapUnique(VcmCapturer::Create(kWidth, kHeight, kFps, kDeviceIndex));
		if (!capturer)
		{
			return nullptr;
		}
		return new rtc::RefCountedObject<CapturerTrackSource>(std::move(capturer));
	}

protected:
	explicit CapturerTrackSource(std::unique_ptr<VcmCapturer> capturer)
		: VideoTrackSource(/*remote=*/false), capturer(std::move(capturer))
	{
	}

private:
	rtc::VideoSourceInterface<webrtc::VideoFrame>* source() override
	{
		return capturer.get();
	}

public:
	std::unique_ptr<VcmCapturer> capturer;
};

STDMETHODIMP WebRTCProxy::createLocalVideoTrack(VARIANT constraints, IUnknown** track)
{
	//Create the video source from capture, note that the video source keeps the std::unique_ptr of the videoCapturer
	auto videoSource = CapturerTrackSource::Create();
	
	//Ensure it is created
	if (!videoSource)
		return E_UNEXPECTED;

	//Now create the track
	rtc::scoped_refptr<webrtc::MediaStreamTrackInterface> videoTrack = peer_connection_factory_->CreateVideoTrack("video", videoSource);

	//Ensure it is created
	if (!videoTrack)
		return E_UNEXPECTED;

	//Create activeX object for media stream track
	CComObject<MediaStreamTrack>* mediaStreamTrack;
	HRESULT hresult = CComObject<MediaStreamTrack>::CreateInstance(&mediaStreamTrack);

	if (FAILED(hresult))
		return hresult;

	//Attach to native track
	mediaStreamTrack->Attach(videoTrack);

	//Set device name as label
	mediaStreamTrack->SetLabel(videoSource->capturer->label);

	//Get Reference to pass it to JS
	*track = mediaStreamTrack->GetUnknown();

	//Add JS reference
	(*track)->AddRef();

	//OK
	return hresult;
}


STDMETHODIMP WebRTCProxy::parseIceCandidate(VARIANT candidate, VARIANT* parsed)
{
	//Check input is a string
	if (candidate.vt != VT_BSTR)
		return E_INVALIDARG;

	//Get candidate as string
	std::string str = (char*)_bstr_t(candidate);

	//Try to parse input
	webrtc::SdpParseError parseError;
	// Creates a IceCandidateInterface based on SDP string.
	std::unique_ptr<webrtc::IceCandidateInterface> iceCandidate(webrtc::CreateIceCandidate("audio", 0, str, &parseError));

	if (!iceCandidate)
		//Parsing error
		return E_INVALIDARG;

	//Fill data
	_variant_t foundation = iceCandidate->candidate().foundation().c_str();
	_variant_t component = iceCandidate->candidate().component();
	_variant_t priority = iceCandidate->candidate().priority();
	_variant_t ip = iceCandidate->candidate().address().hostname().c_str();
	_variant_t protocol = iceCandidate->candidate().protocol().c_str();
	_variant_t port = iceCandidate->candidate().address().port();
	_variant_t type = iceCandidate->candidate().type().c_str();
	_variant_t tcpType = iceCandidate->candidate().tcptype().c_str();
	_variant_t relatedAddress = iceCandidate->candidate().related_address().hostname().c_str();
	_variant_t relatedPort = iceCandidate->candidate().related_address().port();
	_variant_t usernameFragment = iceCandidate->candidate().username().c_str();

	CComSafeArray<VARIANT> args(11);
	args.SetAt(0, foundation);
	args.SetAt(1, component);
	args.SetAt(2, priority);
	args.SetAt(3, ip);
	args.SetAt(4, protocol);
	args.SetAt(5, port);
	args.SetAt(6, type);
	args.SetAt(7, tcpType);
	args.SetAt(8, relatedAddress);
	args.SetAt(9, relatedPort);
	args.SetAt(10, usernameFragment);

	// Initialize the variant
	VariantInit(parsed);
	parsed->vt = VT_ARRAY | VT_VARIANT;
	parsed->parray = args.Detach();

	//Parsed ok
	return S_OK;
}
