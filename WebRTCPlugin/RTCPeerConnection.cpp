// RTCPeerConnection.cpp : Implementation of RTCPeerConnection
#include "stdafx.h"
#include "JSObject.h"
#include "RTCPeerConnection.h"
#include "MediaStreamTrack.h"

class SetSessionDescriptionCallback: 
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
    DispatchAsync([=](){
      //Call sucess without args
      success.Invoke();
    });
  }

  void OnFailure(const std::string& error) override
  {
    DispatchAsync([=]() {
      //Call error callback
      failure.Invoke(error);
    });
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

    DispatchAsync([=]() {
      success.Invoke(type, sdp);
    });
  }

  void OnFailure(const std::string& error) override
  {
    DispatchAsync([=]() {
      failure.Invoke(error);
    });
  }
private:
  rtc::scoped_refptr<webrtc::PeerConnectionInterface> pc;
  Callback success;
  Callback failure;
};

class FakeMediaStream : public rtc::RefCountedObject<webrtc::MediaStreamInterface>
{
public:
  FakeMediaStream(const std::string &label) :
    label_(label)
  {
  }
  virtual std::string label() const { return label_; }
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
  std::string label_;
};

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
    rtcOfferAnswerOptions.voice_activity_detection  = obj.GetBooleanProperty(L"voiceActivityDetection", true);
    rtcOfferAnswerOptions.ice_restart               = obj.GetBooleanProperty(L"iceRestart", false);
  }

  //Create observer
  rtc::scoped_refptr<CreateSessionDescriptionCallback> observer = new CreateSessionDescriptionCallback(pc, GetThread(), successCallback, failureCallback);

  //Create new observer
  pc->CreateOffer(observer,rtcOfferAnswerOptions);

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
  std::string sdp  = obj.GetStringProperty(L"sdp");

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
  std::string sdp              = obj.GetStringProperty(L"candidate");
  std::string sdpMid           = obj.GetStringProperty(L"sdpMid");
  int64_t sdpMLineIndex        = obj.GetIntegerProperty(L"sdpMLineIndex",-1);
  std::string usernameFragment = obj.GetStringProperty(L"usernameFragment");

  //If not found
  if (sdpMid.empty() || sdpMLineIndex==-1)
    //Call error callback with message
    return failure.Invoke("Wrong input parameter, sdpMid or sdpMLineIndex missing");

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
  pc->AddIceCandidate(iceCandidate.get());

  //Get updated sdp
  Callback success(successCallback);

  // up to JS
  std::string current;
  std::string pending;
  if (pc->current_remote_description())
    pc->current_remote_description()->ToString(&current);
  if (pc->pending_remote_description())
    pc->pending_remote_description()->ToString(&pending);

  //Update sdps
  return  success.Invoke(current, pending);

  //Done
  return S_OK;
}



STDMETHODIMP RTCPeerConnection::addTrack(VARIANT track, VARIANT stream)
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

  //libwebrtc only supports adding track to one stream and it only requires the stream label, 
  //so we use a fake object instead of having to wrap the whole media stream 
  std::vector<webrtc::MediaStreamInterface*> streams;
  rtc::scoped_refptr<webrtc::MediaStreamInterface> mediaStream;
  std::string label = (char*)_bstr_t(stream);
  //If attaching to a stream
  if (!label.empty())
  {
    //Create new fake stream
    mediaStream = new FakeMediaStream(label);
    //Add it to list
    streams.push_back(mediaStream.get());
  }
  
  //Add track
  auto sender = pc->AddTrack(proxy->GetTrack(),streams);

  //Create sender object

  //Attach

  //return it

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

STDMETHODIMP RTCPeerConnection::close()
{
  if (!pc)
    return E_UNEXPECTED;

  pc->Close();

  return S_OK;
}



// RTCPeerConnection event handlers
STDMETHODIMP RTCPeerConnection::put_onnegotiationneeded(VARIANT handler)        { return MarshalCallback(onnegotiationneeded        , handler); }
STDMETHODIMP RTCPeerConnection::put_onicecandidate(VARIANT handler)             { return MarshalCallback(onicecandidate             , handler); }
STDMETHODIMP RTCPeerConnection::put_onicecandidateerror(VARIANT handler)        { return MarshalCallback(onicecandidateerror        , handler); }
STDMETHODIMP RTCPeerConnection::put_onsignalingstatechange(VARIANT handler)     { return MarshalCallback(onsignalingstatechange     , handler); }
STDMETHODIMP RTCPeerConnection::put_oniceconnectionstatechange(VARIANT handler) { return MarshalCallback(oniceconnectionstatechange , handler); }
STDMETHODIMP RTCPeerConnection::put_onicegatheringstatechange(VARIANT handler)  { return MarshalCallback(onicegatheringstatechange  , handler); }
STDMETHODIMP RTCPeerConnection::put_onconnectionstatechange(VARIANT handler)    { return MarshalCallback(onconnectionstatechange    , handler); }
STDMETHODIMP RTCPeerConnection::put_onfinalrelease(VARIANT handler)             { return MarshalCallback(onfinalrelease             , handler); }

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

  DispatchAsync([=]() {
    this->onsignalingstatechange.Invoke(state);
  });
  
}
void RTCPeerConnection::OnAddStream(rtc::scoped_refptr<webrtc::MediaStreamInterface> stream)
{
}

void RTCPeerConnection::OnRemoveStream(rtc::scoped_refptr<webrtc::MediaStreamInterface> stream)
{
}

void RTCPeerConnection::OnDataChannel(rtc::scoped_refptr<webrtc::DataChannelInterface> channel)
{
}

void RTCPeerConnection::OnRenegotiationNeeded()
{
  DispatchAsync([=]() {
    this->onnegotiationneeded.Invoke();
  });
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
  
  DispatchAsync([=]() {
    this->oniceconnectionstatechange.Invoke(state);
  });
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
 
  DispatchAsync([=]() {
    this->onicegatheringstatechange.Invoke(state);
  });
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
    candidate         = str.c_str();
    sdpMid            = iceCandidate->sdp_mid().c_str();
    sdpMLineIndex     = iceCandidate->sdp_mline_index();
    foundation        = iceCandidate->candidate().foundation().c_str();
    component         = iceCandidate->candidate().component();
    priority          = iceCandidate->candidate().priority();
    ip                = iceCandidate->candidate().address().hostname().c_str();
    protocol          = iceCandidate->candidate().protocol().c_str();
    port              = iceCandidate->candidate().address().port();
    type              = iceCandidate->candidate().type().c_str();
    tcpType           = iceCandidate->candidate().tcptype().c_str();
    relatedAddress    = iceCandidate->candidate().related_address().hostname().c_str();
    relatedPort       = iceCandidate->candidate().related_address().port();
    usernameFragment  = iceCandidate->candidate().username().c_str();
    url               = iceCandidate->server_url().c_str();
  }

  DispatchAsync([=]() {
    VARIANT args[15] = { 
      url,
      usernameFragment,
      relatedPort,
      relatedAddress,
      tcpType,
      type,
      port,
      protocol,
      ip,
      priority,
      component,
      foundation,
      sdpMLineIndex,
      sdpMid,
      candidate
    };
    this->onicecandidate.Invoke(args, 15);
  });
}

void RTCPeerConnection::OnIceConnectionReceivingChange(bool receiving)
{
}
