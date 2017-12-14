#ifndef _CALLBACK_DISPATCHER_H
#define _CALLBACK_DISPATCHER_H

#include "Callback.h"
#include "webrtc/rtc_base/refcount.h"
#include "webrtc/rtc_base/thread.h"


template<class RefCount, class FunctorT>
class EventMessageHandler :
  public rtc::MessageHandler
{
public:
  explicit EventMessageHandler(RefCount* obj, FunctorT& functor) :
    functor_(functor)
  {
    obj->AddRef();
    this->obj = obj;
  }

  virtual void OnMessage(rtc::Message* msg) {
    functor_();
    obj->Release();
    delete(this);
  }

private:
  RefCount* obj;
  FunctorT functor_;
};


template<class RefCount>
class CallbackDispatcher : public RefCount
{
public:
  void SetThread(std::shared_ptr<rtc::Thread> &thread)
  {
    this->thread = thread;
  }

  std::shared_ptr<rtc::Thread> &GetThread()
  {
    return thread;
  }

  HRESULT MarshalCallback(Callback& callback, VARIANT &handler)
  {
    IStream *stream;
    HRESULT hr = Callback::Marshal(handler, &stream);
    if (FAILED(hr))
      return hr;
    //Unmarshal it on siganling thread async
    DispatchAsync([stream,&callback]() {
      callback.UnMarshal(stream);
    });
    return S_OK;
  }

  template<typename FunctorT>
  void DispatchAsync(FunctorT functor)
  {
    thread->Post(RTC_FROM_HERE, new EventMessageHandler<RefCount,FunctorT>(this, functor));
  }

  virtual ~CallbackDispatcher() = default;

private:
  std::shared_ptr<rtc::Thread> thread;
};
#endif