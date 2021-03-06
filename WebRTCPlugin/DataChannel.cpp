// DataChannel.cpp : Implementation of DataChannel
#include "stdafx.h"
#include "DataChannel.h"
#include "JSObject.h"

STDMETHODIMP DataChannel::send(VARIANT data)
{
	if (data.vt == !VT_BSTR)
		return E_INVALIDARG;

	//Get string
	std::string message = (char*)_bstr_t(data);

	//Send it
	if (!datachannel->Send(webrtc::DataBuffer(message)))
		//Error
		return E_FAIL;

	//Done
	return S_OK;
}


STDMETHODIMP DataChannel::close()
{
	datachannel->Close();
	return S_OK;
}


// The data channel state have changed.
void DataChannel::OnStateChange()
{
	//Check state
	switch (datachannel->state())
	{
	case webrtc::DataChannelInterface::kConnecting:
		return;
	case webrtc::DataChannelInterface::kOpen:
		DispatchAsync(onopen);
		return;
	case webrtc::DataChannelInterface::kClosing:
		return;
	case webrtc::DataChannelInterface::kClosed:
		DispatchAsync(onclose);
		return;
	}
}

void DataChannel::OnMessage(const webrtc::DataBuffer& buffer)
{
	//If we still don't have the handler
	if (onmessage.IsSet())
	{
		//Check buffer type
		if (!buffer.binary)
		{
			//Create string
			std::string message((const char*)buffer.data.data(), buffer.size());
			//Event
			DispatchAsync(onmessage, message);
		}
	}
	else {
		//Enqueue
		pending.push_back(buffer);
	}
}

void DataChannel::OnBufferedAmountChange(uint64_t previous_amount)
{
	//Check buffered ammount is below threshold
	if (datachannel->buffered_amount() < bufferedAmountLowThreshold)
		//Fire event
		DispatchAsync(onbufferedamountlow);
}


STDMETHODIMP DataChannel::get_label(VARIANT* val)
{
	variant_t variant = this->datachannel->label().c_str();
	*val = variant;
	return S_OK;
}
STDMETHODIMP DataChannel::get_ordered(VARIANT* val)
{
	variant_t variant = this->datachannel->ordered();
	*val = variant;
	return S_OK;
}
STDMETHODIMP DataChannel::get_maxPacketLifeTime(VARIANT* val)
{
	variant_t variant = this->datachannel->maxRetransmitTime();
	*val = variant;
	return S_OK;
}
STDMETHODIMP DataChannel::get_negotiated(VARIANT* val)
{
	variant_t variant = this->datachannel->negotiated();
	*val = variant;
	return S_OK;
}
STDMETHODIMP DataChannel::get_id(VARIANT* val)
{
	variant_t variant = this->datachannel->id();
	*val = variant;
	return S_OK;
}
STDMETHODIMP DataChannel::get_priority(VARIANT* val)
{
	variant_t variant = this->priority.c_str();
	*val = variant;
	return S_OK;
}

STDMETHODIMP DataChannel::get_readyState(VARIANT* val)
{
	variant_t variant = webrtc::DataChannelInterface::DataStateString(this->datachannel->state());
	*val = variant;
	return S_OK;
}
STDMETHODIMP DataChannel::get_bufferedAmount(VARIANT* val)
{
	variant_t variant = this->datachannel->buffered_amount();
	*val = variant;
	return S_OK;
}
STDMETHODIMP DataChannel::get_bufferedAmountLowThreshold(VARIANT* val)
{
	variant_t variant = this->bufferedAmountLowThreshold;
	*val = variant;
	return S_OK;
}
STDMETHODIMP DataChannel::put_bufferedAmountLowThreshold(VARIANT val)
{
	bufferedAmountLowThreshold = GetInt(&val, 0);
	return S_OK;
}

// RTCPeerConnection event handlers
STDMETHODIMP DataChannel::put_onopen(VARIANT handler) { return MarshalCallback(onopen, handler); }
STDMETHODIMP DataChannel::put_onbufferedamountlow(VARIANT handler) { return MarshalCallback(onbufferedamountlow, handler); }
STDMETHODIMP DataChannel::put_onerror(VARIANT handler) { return MarshalCallback(onerror, handler); }
STDMETHODIMP DataChannel::put_onclose(VARIANT handler) { return MarshalCallback(onclose, handler); }
STDMETHODIMP DataChannel::put_onmessage(VARIANT handler)
{
	HRESULT hr = MarshalCallback(onmessage, handler);
	//If ok
	if (!FAILED(hr))
	{
		//Deliver pending messages
		for (auto buffer : pending)
		{
			//Check buffer type
			if (!buffer.binary)
			{
				//Create string
				std::string message((const char*)buffer.data.data(), buffer.size());
				//Event
				DispatchAsync(onmessage, message);
			}
		}
		//Remove all pending messages
		pending.clear();
	}

	return hr;
}
