// RTCPeerConnection.cpp : Implementation of RTCPeerConnection
#include "stdafx.h"
#include <atlsafe.h>

#include "api\jsep.h"
#include "JSObject.h"
#include "RTCPeerConnection.h"
#include "MediaStreamTrack.h"
#include "RTPSender.h"
#include "DataChannel.h"

class SetSessionDescriptionCallback :
	public rtc::RefCountedObject<CallbackDispatcher<webrtc::SetSessionDescriptionObserver>>
{
public:
	SetSessionDescriptionCallback(webrtc::PeerConnectionInterface* pci, std::shared_ptr<rtc::Thread> &thread, VARIANT successCallback, VARIANT failureCallback) :
		pc(pci)
	{
		//Set dispatcher thread
		SetThread(thread);
		//Marshal callbacks
		MarshalCallback(success, successCallback);
		MarshalCallback(failure, failureCallback);
	}

	virtual ~SetSessionDescriptionCallback() = default;

	void OnSuccess() override
	{
		//Call sucess without args
		DispatchAsync(success);
	}

	void OnFailure(const std::string& error) override
	{
		//Call error callback
		DispatchAsync(failure, error);
	}

private:
	rtc::scoped_refptr<webrtc::PeerConnectionInterface> pc;
	Callback success;
	Callback failure;
};


class CreateSessionDescriptionCallback :
	public rtc::RefCountedObject<CallbackDispatcher<webrtc::CreateSessionDescriptionObserver>>
{
public:
	CreateSessionDescriptionCallback(webrtc::PeerConnectionInterface* pci, std::shared_ptr<rtc::Thread> &thread, VARIANT successCallback, VARIANT failureCallback) :
		pc(pci)
	{
		//Set dispatcher thread
		SetThread(thread);
		//Marshal callbacks
		MarshalCallback(success, successCallback);
		MarshalCallback(failure, failureCallback);
	}

	virtual ~CreateSessionDescriptionCallback() = default;

	void OnSuccess(webrtc::SessionDescriptionInterface* desc) override
	{
		// up to JS
		std::string str;
		desc->ToString(&str);
		//Set properties
		_variant_t type = desc->type().c_str();
		_variant_t sdp = str.c_str();

		DispatchAsync(success, type, sdp);
	}

	void OnFailure(const std::string& error) override
	{
		DispatchAsync(failure, error);
	}
private:
	rtc::scoped_refptr<webrtc::PeerConnectionInterface> pc;
	Callback success;
	Callback failure;
};

class FakeMediaStream : public rtc::RefCountedObject<webrtc::MediaStreamInterface>
{
public:
	FakeMediaStream(const std::string &id) : id_(id)
	{
	}
	virtual std::string id() const { return id_; }
	virtual webrtc::AudioTrackVector GetAudioTracks() { return webrtc::AudioTrackVector(); }
	virtual webrtc::VideoTrackVector GetVideoTracks() { return webrtc::VideoTrackVector(); }
	virtual rtc::scoped_refptr<webrtc::AudioTrackInterface> FindAudioTrack(const std::string& track_id) { return nullptr; };
	virtual rtc::scoped_refptr<webrtc::VideoTrackInterface> FindVideoTrack(const std::string& track_id) { return nullptr; };
	virtual bool AddTrack(webrtc::AudioTrackInterface* track) { return true; };
	virtual bool AddTrack(webrtc::VideoTrackInterface* track) { return true; };
	virtual bool RemoveTrack(webrtc::AudioTrackInterface* track) { return true; };
	virtual bool RemoveTrack(webrtc::VideoTrackInterface* track) { return true; };

	virtual void RegisterObserver(webrtc::ObserverInterface* observer) {};
	virtual void UnregisterObserver(webrtc::ObserverInterface* observer) {};
private:
	std::string id_;
};


STDMETHODIMP RTCPeerConnection::setConfiguration(VARIANT variant)
{
	if (!pc)
		return E_UNEXPECTED;

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
		//TODO: support the followin ones
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
					if (urls.bstrVal)
					{
						//Get url
						_bstr_t url(urls.bstrVal);
						//Append
						iceServer.urls.push_back((char*)url);
					}
					else {
						//Conver to object
						JSObject aux(urls);
						//Ensure we hage it
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

	//Apply new one
	if (!pc->SetConfiguration(configuration))
		return E_INVALIDARG;

	//OK
	return S_OK;
}

STDMETHODIMP RTCPeerConnection::createOffer(VARIANT successCallback, VARIANT failureCallback, VARIANT options)
{
	if (!pc)
		return E_UNEXPECTED;

	webrtc::PeerConnectionInterface::RTCOfferAnswerOptions rtcOfferAnswerOptions;
	JSObject obj(options);

	if (!obj.isNull())
	{
		/*
		dictionary RTCOfferAnswerOptions {
		  boolean voiceActivityDetection = true;
		};

		dictionary RTCOfferOptions : RTCOfferAnswerOptions {
		  boolean iceRestart = false;
		};
		//LEGACY
		partial dictionary RTCOfferOptions {
		  boolean offerToReceiveAudio;
		  boolean offerToReceiveVideo;
		};
	  */
		if (obj.GetBooleanProperty(L"offerToReceiveAudio", false))
			rtcOfferAnswerOptions.offer_to_receive_audio = webrtc::PeerConnectionInterface::RTCOfferAnswerOptions::kOfferToReceiveMediaTrue;
		if (obj.GetBooleanProperty(L"offerToReceiveVideo", false))
			rtcOfferAnswerOptions.offer_to_receive_video = webrtc::PeerConnectionInterface::RTCOfferAnswerOptions::kOfferToReceiveMediaTrue;
		rtcOfferAnswerOptions.voice_activity_detection = obj.GetBooleanProperty(L"voiceActivityDetection", true);
		rtcOfferAnswerOptions.ice_restart = obj.GetBooleanProperty(L"iceRestart", false);
	}

	//Create observer
	rtc::scoped_refptr<CreateSessionDescriptionCallback> observer = new CreateSessionDescriptionCallback(pc, GetThread(), successCallback, failureCallback);

	//Create new observer
	pc->CreateOffer(observer, rtcOfferAnswerOptions);

	return S_OK;
}

STDMETHODIMP RTCPeerConnection::setLocalDescription(VARIANT successCallback, VARIANT failureCallback, VARIANT description)
{
	if (!pc)
		return E_UNEXPECTED;

	Callback failure(failureCallback);
	JSObject obj(description);

	if (obj.isNull())
		return E_INVALIDARG;

	//Get type and sdp from object
	std::string type = obj.GetStringProperty(L"type");
	std::string sdp = obj.GetStringProperty(L"sdp");

	//If not found
	if (type.empty() || sdp.empty())
		//Call error callback with message
		return failure.Invoke("Wrong input parameter, type or sdp missing");

	//Try to parse input
	webrtc::SdpParseError parseError;
	webrtc::SessionDescriptionInterface* sessionDescription(webrtc::CreateSessionDescription(type, sdp, &parseError));

	if (!sessionDescription)
	{
		//Call errror msg
		std::string msg = "Can't parse received session description message. SdpParseError was: " + parseError.description;
		//Call error callback with message
		return failure.Invoke(msg);
	}

	//Create observer
	rtc::scoped_refptr<SetSessionDescriptionCallback> observer = new SetSessionDescriptionCallback(pc, GetThread(), successCallback, failureCallback);

	//Set it
	pc->SetLocalDescription(observer, sessionDescription);

	return S_OK;
}

STDMETHODIMP RTCPeerConnection::createAnswer(VARIANT successCallback, VARIANT failureCallback, VARIANT options)
{
	if (!pc)
		return E_UNEXPECTED;

	webrtc::PeerConnectionInterface::RTCOfferAnswerOptions rtcOfferAnswerOptions;
	JSObject obj(options);

	if (!obj.isNull())
	{
		/*
		dictionary RTCOfferAnswerOptions {
		boolean voiceActivityDetection = true;
		};

		dictionary RTCAnswerOptions : RTCOfferAnswerOptions {
		};

		*/
		rtcOfferAnswerOptions.voice_activity_detection = obj.GetBooleanProperty(L"voiceActivityDetection", true);
	}

	//Create observer
	rtc::scoped_refptr<CreateSessionDescriptionCallback> observer = new CreateSessionDescriptionCallback(pc, GetThread(), successCallback, failureCallback);

	//Create new observer
	pc->CreateAnswer(observer, rtcOfferAnswerOptions);

	return S_OK;
}
STDMETHODIMP RTCPeerConnection::setRemoteDescription(VARIANT successCallback, VARIANT failureCallback, VARIANT description)
{
	if (!pc)
		return E_UNEXPECTED;

	Callback failure(failureCallback);
	JSObject obj(description);

	if (obj.isNull())
		return E_INVALIDARG;

	//Get type and sdp from object
	std::string type = obj.GetStringProperty(L"type");
	std::string sdp = obj.GetStringProperty(L"sdp");

	//If not found
	if (type.empty() || sdp.empty())
		//Call error callback with message
		return failure.Invoke("Wrong input parameter, type or sdp missing");

	//Try to parse input
	webrtc::SdpParseError parseError;
	webrtc::SessionDescriptionInterface* sessionDescription(webrtc::CreateSessionDescription(type, sdp, &parseError));

	if (!sessionDescription)
	{
		//Call errror msg
		std::string msg = "Can't parse received session description message. SdpParseError was: " + parseError.description;
		//Call error callback with message
		return failure.Invoke(msg);
	}

	//Create observer
	rtc::scoped_refptr<SetSessionDescriptionCallback> observer = new SetSessionDescriptionCallback(pc, GetThread(), successCallback, failureCallback);

	//Set it
	pc->SetRemoteDescription(observer, sessionDescription);

	return S_OK;
}
STDMETHODIMP RTCPeerConnection::addIceCandidate(VARIANT successCallback, VARIANT failureCallback, VARIANT candidate)
{
	if (!pc)
		return E_UNEXPECTED;

	Callback failure(failureCallback);
	JSObject obj(candidate);

	if (obj.isNull())
		return E_INVALIDARG;

	//Get type and sdp from object
	std::string sdp = obj.GetStringProperty(L"candidate");
	std::string sdpMid = obj.GetStringProperty(L"sdpMid");
	int64_t sdpMLineIndex = obj.GetIntegerProperty(L"sdpMLineIndex", -1);

	//If not found
	if (sdpMid.empty() && sdpMLineIndex == -1)
		//Call error callback with message
		return failure.Invoke("Wrong input parameters, sdpMid and sdpMLineIndex missing");

	//Try to parse input
	webrtc::SdpParseError parseError;
	// Creates a IceCandidateInterface based on SDP string.
	std::unique_ptr<webrtc::IceCandidateInterface> iceCandidate(webrtc::CreateIceCandidate(sdpMid, sdpMLineIndex, sdp, &parseError));

	if (!iceCandidate)
	{
		//Call errror msg
		std::string msg = "Can't parse received candidate. SdpParseError was: " + parseError.description;
		//Call error callback with message
		return failure.Invoke(msg);
	}

	//Set it
	if (!pc->AddIceCandidate(iceCandidate.get()))
		//Call error callback with message
		return failure.Invoke("AddIceCandidate failed");

	//Get updated sdp
	Callback success(successCallback);

	//OK
	return success.Invoke();
}

STDMETHODIMP RTCPeerConnection::addTrack(VARIANT track, VARIANT stream, IUnknown** rtpSender)
{
	if (!pc)
		return E_UNEXPECTED;

	//Get dispatch interface
	if (track.vt != VT_DISPATCH)
		return E_INVALIDARG;

	IDispatch* disp = V_DISPATCH(&track);

	if (!disp)
		return E_INVALIDARG;

	//Get atl com object from track.
	CComPtr<ITrackAccess> proxy;
	HRESULT hr = disp->QueryInterface(IID_PPV_ARGS(&proxy));
	if (FAILED(hr))
		return hr;

	// libwebrtc only supports adding track to one stream and it only requires the stream label, 
	// so we use a fake object instead of having to wrap the whole media stream 
	//std::vector<webrtc::MediaStreamInterface*> streams;
	std::vector<std::string> streams;
	rtc::scoped_refptr<webrtc::MediaStreamInterface> mediaStream;
	std::string id = (char*)_bstr_t(stream);
	//If attaching to a stream
	if (!id.empty())
	{
		auto it = localStreams.find(id);
		//Check if we already have it
		if (it == localStreams.end())
		{
			//Create new fake stream
			mediaStream = new FakeMediaStream(id);
			//Add to list of local streams
			localStreams[id] = mediaStream;
		}
		else {
			//Get it
			mediaStream = it->second;
		}
		//Add it to list
		streams.push_back(mediaStream.get()->id());
	}

	//Add track
	rtc::scoped_refptr<webrtc::RtpSenderInterface > senderInterface = pc->AddTrack(proxy->GetTrack(), streams).value();

	//Create activeX object for media stream track
	CComObject<RTPSender>* sender;
	HRESULT hresult = CComObject<RTPSender>::CreateInstance(&sender);
	if (FAILED(hresult))
		return hresult;

	//Attach to native object
	sender->Attach(senderInterface);

	//Get Reference to pass it to JS
	*rtpSender = sender->GetUnknown();

	//Add JS reference
	(*rtpSender)->AddRef();

	//Done
	return S_OK;
}

STDMETHODIMP RTCPeerConnection::removeTrack(VARIANT sender)
{
	if (!pc)
		return E_UNEXPECTED;

	//Done
	return S_OK;
}

STDMETHODIMP RTCPeerConnection::getRemoteStreamTracks(VARIANT stream, VARIANT successCallback)
{
	Callback success(successCallback);

	std::vector<variant_t> args;
	//Get stream label
	std::string label = (char*)_bstr_t(stream);
	//Get remote stream
	auto remote = remoteStreams.find(label);

	//If not found
	if (remote == remoteStreams.end())
		return E_NOT_SET;

	//Get all tracks from stream
	auto audioTracks = remote->second->GetAudioTracks();
	auto videoTracks = remote->second->GetVideoTracks();

	for (rtc::scoped_refptr<webrtc::MediaStreamTrackInterface> audioTrack : audioTracks)
	{
		//Create activeX object which is a
		CComObject<MediaStreamTrack>* mediaStreamTrack;
		HRESULT hresult = CComObject<MediaStreamTrack>::CreateInstance(&mediaStreamTrack);

		if (FAILED(hresult))
			break;

		//Attach to native track
		mediaStreamTrack->Attach(audioTrack);

		//We need to marshall pointer so it is valid on event thread
		IUnknown *iUnknown = mediaStreamTrack->GetUnknown();

		//Add ref
		iUnknown->AddRef();

		//Add arg
		args.push_back(variant_t(iUnknown));
	}

	for (rtc::scoped_refptr<webrtc::MediaStreamTrackInterface> videoTrack : videoTracks)
	{
		//Create activeX object which is a
		CComObject<MediaStreamTrack>* mediaStreamTrack;
		HRESULT hresult = CComObject<MediaStreamTrack>::CreateInstance(&mediaStreamTrack);

		if (FAILED(hresult))
			break;

		//Attach to native track
		mediaStreamTrack->Attach(videoTrack);

		//We need to marshall pointer so it is valid on event thread
		IUnknown *iUnknown = mediaStreamTrack->GetUnknown();

		//Add ref
		iUnknown->AddRef();

		//Add arg
		args.push_back(variant_t(iUnknown));
	}

	//Call it now
	return success.Invoke(args);

}

STDMETHODIMP RTCPeerConnection::close()
{
	if (!pc)
		return E_UNEXPECTED;

	pc->Close();

	return S_OK;
}

// RTCPeerConnection event handlers
STDMETHODIMP RTCPeerConnection::put_onnegotiationneeded(VARIANT handler) { return MarshalCallback(onnegotiationneeded, handler); }
STDMETHODIMP RTCPeerConnection::put_onicecandidate(VARIANT handler) { return MarshalCallback(onicecandidate, handler); }
STDMETHODIMP RTCPeerConnection::put_onicecandidateerror(VARIANT handler) { return MarshalCallback(onicecandidateerror, handler); }
STDMETHODIMP RTCPeerConnection::put_onsignalingstatechange(VARIANT handler) { return MarshalCallback(onsignalingstatechange, handler); }
STDMETHODIMP RTCPeerConnection::put_oniceconnectionstatechange(VARIANT handler) { return MarshalCallback(oniceconnectionstatechange, handler); }
STDMETHODIMP RTCPeerConnection::put_onicegatheringstatechange(VARIANT handler) { return MarshalCallback(onicegatheringstatechange, handler); }
STDMETHODIMP RTCPeerConnection::put_onconnectionstatechange(VARIANT handler) { return MarshalCallback(onconnectionstatechange, handler); }
STDMETHODIMP RTCPeerConnection::put_onaddstream(VARIANT handler) { return MarshalCallback(onaddstream, handler); }
STDMETHODIMP RTCPeerConnection::put_onremovestream(VARIANT handler) { return MarshalCallback(onremovestream, handler); }
STDMETHODIMP RTCPeerConnection::put_ondatachannel(VARIANT handler) { return MarshalCallback(ondatachannel, handler); }

// RTCPeerConnection Observer interface

void RTCPeerConnection::OnSignalingChange(webrtc::PeerConnectionInterface::SignalingState signalingState)
{
	_variant_t state;

	switch (signalingState)
	{
	case webrtc::PeerConnectionInterface::SignalingState::kStable:
		state = "stable";
		break;
	case webrtc::PeerConnectionInterface::SignalingState::kHaveLocalOffer:
		state = "have-local-offer";
		break;
	case webrtc::PeerConnectionInterface::SignalingState::kHaveLocalPrAnswer:
		state = "have-remote-offe";
		break;
	case webrtc::PeerConnectionInterface::SignalingState::kHaveRemoteOffer:
		state = "have-local-pranswer";
		break;
	case webrtc::PeerConnectionInterface::SignalingState::kHaveRemotePrAnswer:
		state = "have-remote-pranswer";
		break;
	case webrtc::PeerConnectionInterface::SignalingState::kClosed:
		state = "closed";
		break;
	}

	DispatchAsync(onsignalingstatechange, state);

}
void RTCPeerConnection::OnAddStream(rtc::scoped_refptr<webrtc::MediaStreamInterface> stream)
{
	//Get stream label
	variant_t id = stream->id().c_str();

	//Add to stream map
	remoteStreams[stream->id()] = stream;

	DispatchAsync(onaddstream, id);
}

void RTCPeerConnection::OnRemoveStream(rtc::scoped_refptr<webrtc::MediaStreamInterface> stream)
{
	//Get stream label
	variant_t id = stream->id().c_str();

	//Remove from stream map
	remoteStreams.erase(stream->id());

	DispatchAsync(onremovestream, id);
}

void RTCPeerConnection::OnDataChannel(rtc::scoped_refptr<webrtc::DataChannelInterface> dataChannelInterface)
{

	//Create activeX object for media stream track
	CComObject<DataChannel>* dataChannelObj;
	HRESULT hresult = CComObject<DataChannel>::CreateInstance(&dataChannelObj);

	if (FAILED(hresult))
		return;

	//Attach to native object
	dataChannelObj->Attach(dataChannelInterface);

	//Get Reference to pass it to JS
	auto dataChannel = dataChannelObj->GetUnknown();

	DispatchAsync(ondatachannel, dataChannel);
}

void RTCPeerConnection::OnRenegotiationNeeded()
{
	DispatchAsync(onnegotiationneeded);
}

void RTCPeerConnection::OnIceConnectionChange(webrtc::PeerConnectionInterface::IceConnectionState iceConnectionState)
{
	_variant_t state;

	switch (iceConnectionState)
	{
	case webrtc::PeerConnectionInterface::IceConnectionState::kIceConnectionNew:
		state = "new";
		break;
	case webrtc::PeerConnectionInterface::IceConnectionState::kIceConnectionChecking:
		state = "checking";
		break;
	case webrtc::PeerConnectionInterface::IceConnectionState::kIceConnectionConnected:
		state = "connected";
		break;
	case webrtc::PeerConnectionInterface::IceConnectionState::kIceConnectionCompleted:
		state = "completed";
		break;
	case webrtc::PeerConnectionInterface::IceConnectionState::kIceConnectionFailed:
		state = "failed";
		break;
	case webrtc::PeerConnectionInterface::IceConnectionState::kIceConnectionDisconnected:
		state = "disconnected";
		break;
	case webrtc::PeerConnectionInterface::IceConnectionState::kIceConnectionClosed:
		state = "closed";
		break;
	case webrtc::PeerConnectionInterface::IceConnectionState::kIceConnectionMax:
		state = "max";
		break;
	}

	//Execute callback async
	DispatchAsync(oniceconnectionstatechange, state);

}


void RTCPeerConnection::OnIceGatheringChange(webrtc::PeerConnectionInterface::IceGatheringState iceGatheringState)
{
	_variant_t state;

	switch (iceGatheringState)
	{
	case webrtc::PeerConnectionInterface::IceGatheringState::kIceGatheringNew:
		state = "new";
		break;
	case webrtc::PeerConnectionInterface::IceGatheringState::kIceGatheringGathering:
		state = "gathering";
		break;
	case webrtc::PeerConnectionInterface::IceGatheringState::kIceGatheringComplete:
		state = "complete";
		break;
	};

	DispatchAsync(onicegatheringstatechange, state);
}

// Handle locally generated ICE Candidates (a.k.a. ICE Gathering)
void RTCPeerConnection::OnIceCandidate(const webrtc::IceCandidateInterface* iceCandidate)
{

	_variant_t candidate;
	_variant_t sdpMid;
	_variant_t sdpMLineIndex;
	_variant_t foundation;
	_variant_t component;
	_variant_t priority;
	_variant_t ip;
	_variant_t protocol;
	_variant_t port;
	_variant_t type;
	_variant_t tcpType;
	_variant_t relatedAddress;
	_variant_t relatedPort;
	_variant_t usernameFragment;
	_variant_t url;

	if (iceCandidate)
	{
		std::string str;
		iceCandidate->ToString(&str);
		//Fill data
		candidate = str.c_str();
		sdpMid = iceCandidate->sdp_mid().c_str();
		sdpMLineIndex = iceCandidate->sdp_mline_index();
		foundation = iceCandidate->candidate().foundation().c_str();
		component = iceCandidate->candidate().component();
		priority = iceCandidate->candidate().priority();
		ip = iceCandidate->candidate().address().hostname().c_str();
		protocol = iceCandidate->candidate().protocol().c_str();
		port = iceCandidate->candidate().address().port();
		type = iceCandidate->candidate().type().c_str();
		tcpType = iceCandidate->candidate().tcptype().c_str();
		relatedAddress = iceCandidate->candidate().related_address().hostname().c_str();
		relatedPort = iceCandidate->candidate().related_address().port();
		usernameFragment = iceCandidate->candidate().username().c_str();
		url = iceCandidate->server_url().c_str();
	}

	std::vector<variant_t> args = {
		candidate,
		sdpMid,
		sdpMLineIndex,
		foundation,
		component,
		priority,
		ip,
		protocol,
		port ,
		type ,
		tcpType ,
		relatedAddress,
		relatedPort,
		usernameFragment,
		url
	};

	DispatchAsync(onicecandidate, args);
}

void RTCPeerConnection::OnIceConnectionReceivingChange(bool receiving)
{
}


STDMETHODIMP RTCPeerConnection::createDataChannel(VARIANT label, VARIANT dataChannelDict, IUnknown** dataChannel)
{
	if (!pc)
		return E_UNEXPECTED;

	//Get label
	std::string str = (char*)_bstr_t(label);

	//Process dictionary and create config
	std::string priority = "low";
	webrtc::DataChannelInit config;

	JSObject obj(dataChannelDict);

	if (!obj.isNull())
	{
		/*
		dictionary RTCDataChannelInit {
		boolean         ordered = true;
		unsigned short  maxPacketLifeTime;
		unsigned short  maxRetransmits;
		USVString       protocol = "";
		boolean         negotiated = false;
		[EnforceRange]
		unsigned short  id;
		RTCPriorityType priority = "low";
		};
		*/
		bool ordered = obj.GetBooleanProperty(L"ordered", true);
		uint16_t maxPacketLifeTime = obj.GetIntegerProperty(L"maxPacketLifeTime", 0);
		uint16_t maxRetransmits = obj.GetIntegerProperty(L"maxRetransmits", 0);
		std::string protocol = obj.GetStringProperty(L"protocol", "");
		uint16_t id = obj.GetIntegerProperty(L"id", 0);
		priority = obj.GetStringProperty(L"protocol", "low");
		//Set them
		config.id = id;
		config.maxRetransmits = maxRetransmits;
		config.maxRetransmitTime = maxPacketLifeTime;
		config.ordered = ordered;
		config.protocol = protocol;
	}

	//Create datachannel
	auto dataChannelInterface = pc->CreateDataChannel(str, &config);

	//Check 
	if (!dataChannelInterface)
		return E_FAIL;

	//Create activeX object for media stream track
	CComObject<DataChannel>* dataChannelObj;
	HRESULT hresult = CComObject<DataChannel>::CreateInstance(&dataChannelObj);

	if (FAILED(hresult))
		return hresult;

	//Attach to native object
	dataChannelObj->Attach(dataChannelInterface);

	//Set placebo priority
	dataChannelObj->SetPriority(priority);

	//Get Reference to pass it to JS
	*dataChannel = dataChannelObj->GetUnknown();

	//Add JS reference
	(*dataChannel)->AddRef();

	//Done
	return S_OK;
}

STDMETHODIMP RTCPeerConnection::get_localDescription(VARIANT* val)
{
	if (!pc)
		return E_UNEXPECTED;

	//Get description
	auto sdp = pc->local_description();

	//If it is empty
	if (!sdp)
	{
		// Initialize the variant
		VariantInit(val);
		//Set array
		val->vt = VT_NULL;
		//Ok
		return S_OK;
	}

	//Get type and sdp as string
	std::string str;
	sdp->ToString(&str);
	std::string type = sdp->type();

	CComSafeArray<VARIANT> args(2);
	args.SetAt(0, _variant_t(type.c_str()));
	args.SetAt(1, _variant_t(str.c_str()));

	// Initialize the variant
	VariantInit(val);
	//Set array
	val->vt = VT_ARRAY | VT_VARIANT;
	val->parray = args.Detach();

	return S_OK;
}

STDMETHODIMP RTCPeerConnection::get_currentLocalDescription(VARIANT* val)
{
	if (!pc)
		return E_UNEXPECTED;

	//Get description
	auto sdp = pc->current_local_description();

	//If it is empty
	if (!sdp)
	{
		// Initialize the variant
		VariantInit(val);
		//Set array
		val->vt = VT_NULL;
		//Ok
		return S_OK;
	}

	//Get type and sdp as string
	std::string str;
	sdp->ToString(&str);
	std::string type = sdp->type();

	CComSafeArray<VARIANT> args(2);
	args.SetAt(0, _variant_t(type.c_str()));
	args.SetAt(1, _variant_t(str.c_str()));

	// Initialize the variant
	VariantInit(val);
	//Set array
	val->vt = VT_ARRAY | VT_VARIANT;
	val->parray = args.Detach();

	return S_OK;
}

STDMETHODIMP RTCPeerConnection::get_pendingLocalDescription(VARIANT* val)
{
	if (!pc)
		return E_UNEXPECTED;

	//Get description
	auto sdp = pc->pending_local_description();

	//If it is empty
	if (!sdp)
	{
		// Initialize the variant
		VariantInit(val);
		//Set array
		val->vt = VT_NULL;
		//Ok
		return S_OK;
	}

	//Get type and sdp as string
	std::string str;
	sdp->ToString(&str);
	std::string type = sdp->type();

	CComSafeArray<VARIANT> args(2);
	args.SetAt(0, _variant_t(type.c_str()));
	args.SetAt(1, _variant_t(str.c_str()));

	// Initialize the variant
	VariantInit(val);
	//Set array
	val->vt = VT_ARRAY | VT_VARIANT;
	val->parray = args.Detach();

	return S_OK;
}

STDMETHODIMP RTCPeerConnection::get_remoteDescription(VARIANT* val)
{
	if (!pc)
		return E_UNEXPECTED;

	//Get description
	auto sdp = pc->remote_description();

	//If it is empty
	if (!sdp)
	{
		// Initialize the variant
		VariantInit(val);
		//Set array
		val->vt = VT_NULL;
		//Ok
		return S_OK;
	}

	//Get type and sdp as string
	std::string str;
	sdp->ToString(&str);
	std::string type = sdp->type();

	CComSafeArray<VARIANT> args(2);
	args.SetAt(0, _variant_t(type.c_str()));
	args.SetAt(1, _variant_t(str.c_str()));

	// Initialize the variant
	VariantInit(val);
	//Set array
	val->vt = VT_ARRAY | VT_VARIANT;
	val->parray = args.Detach();

	return S_OK;
}

STDMETHODIMP RTCPeerConnection::get_currentRemoteDescription(VARIANT* val)
{
	if (!pc)
		return E_UNEXPECTED;

	//Get description
	auto sdp = pc->current_remote_description();

	//If it is empty
	if (!sdp)
	{
		// Initialize the variant
		VariantInit(val);
		//Set array
		val->vt = VT_NULL;
		//Ok
		return S_OK;
	}

	//Get type and sdp as string
	std::string str;
	sdp->ToString(&str);
	std::string type = sdp->type();

	CComSafeArray<VARIANT> args(2);
	args.SetAt(0, _variant_t(type.c_str()));
	args.SetAt(1, _variant_t(str.c_str()));

	// Initialize the variant
	VariantInit(val);
	//Set array
	val->vt = VT_ARRAY | VT_VARIANT;
	val->parray = args.Detach();

	return S_OK;
}

STDMETHODIMP RTCPeerConnection::get_pendingRemoteDescription(VARIANT* val)
{
	if (!pc)
		return E_UNEXPECTED;

	//Get description
	auto sdp = pc->pending_remote_description();

	//If it is empty
	if (!sdp)
	{
		// Initialize the variant
		VariantInit(val);
		//Set array
		val->vt = VT_NULL;
		//Ok
		return S_OK;
	}

	//Get type and sdp as string
	std::string str;
	sdp->ToString(&str);
	std::string type = sdp->type();

	CComSafeArray<VARIANT> args(2);
	args.SetAt(0, _variant_t(type.c_str()));
	args.SetAt(1, _variant_t(str.c_str()));

	// Initialize the variant
	VariantInit(val);
	//Set array
	val->vt = VT_ARRAY | VT_VARIANT;
	val->parray = args.Detach();

	return S_OK;
}
