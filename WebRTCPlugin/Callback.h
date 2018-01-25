#ifndef _CALLBACK_H_
#define _CALLBACK_H_
#include <Objbase.h>
#include <atlctl.h>
#include <comutil.h>
#include <string>
#include <vector>
using namespace ATL;

class Callback 
{
public:
  Callback()
  {

  }

  Callback(const Callback& callback)
  {
	  //Get Idispatch and stream interface
    stream = callback.GetIStream();
	  disp   = callback.GetIDispatch();

	  if (disp)
		  //Inc reference
		  disp->AddRef();
	  if (stream)
		  //Inc reference
		  stream->AddRef();
  }

  Callback(VARIANT &handler)
  {
    Set(handler);
  }

  Callback(IStream* stream)
  {
	if (stream)
	  stream->AddRef();
	this->stream = stream;
	UnMarshal();
  }

  ~Callback()
  {
    Reset();
  }

  HRESULT Marshal(VARIANT &handler)
  {
    //Check input type
    if (handler.vt != VT_DISPATCH)
      return E_INVALIDARG;
	  //Create stream on global mem
	  CreateStreamOnHGlobal(NULL, true, &stream);
	  //Add ref
	  stream->AddRef();
	  //Marshal variant into stream that can be unmarshalled multiple times
    return CoMarshalInterface(stream,IID_IDispatch, V_DISPATCH(&handler), MSHCTX_INPROC, NULL, MSHLFLAGS_TABLESTRONG);
  }

  HRESULT UnMarshal()
  {
    if (!stream)
      return E_INVALIDARG;

    if (disp)
      disp->Release();

	//Rewind stream
	stream->Seek({ 0 }, STREAM_SEEK_SET, NULL);
	//Unmarshall
    HRESULT hr = CoUnmarshalInterface(stream, IID_IDispatch, reinterpret_cast<LPVOID*>(&disp));

	//release ref
	stream->Release();

	stream = nullptr;

    if (FAILED(hr))
      return hr;

    if (!disp)
      return E_INVALIDARG;

    //Inc reference
    disp->AddRef();

    return S_OK;
  }
  bool IsSet()
  {
    return disp || stream;
  }
  HRESULT Set(VARIANT &handler)
  {
    if (handler.vt == VT_NULL)
    {
      //Reset previous pointer
      Reset();
      return S_OK;
    }
    //Check input type
    if (handler.vt != VT_DISPATCH)
      return E_INVALIDARG;

    //Reset previous pointer
    Reset();

    //Get new dispatk interface
    disp = V_DISPATCH(&handler);

    if (!disp)
      return E_INVALIDARG;

    //Inc reference
    disp->AddRef();
    return S_OK;
  }

  IDispatch* GetIDispatch() const
  {
	  return disp;
  }

  IStream* GetIStream() const
  {
	  return stream;
  }

  void Reset()
  {
    if (disp)
    {
      disp->Release();
      disp = nullptr;
    }
	if (stream)
	{
		stream->Release();
		stream = nullptr;
	}
  }

  HRESULT Invoke()
  {
    return Invoke(nullptr, 0);
  }

  HRESULT Invoke(const std::string &msg)
  {
    _variant_t variant = msg.c_str();
    return Invoke(variant);
  }

  HRESULT Invoke(const char* msg)
  {
    _variant_t variant = msg;
    return Invoke(variant);
  }

  HRESULT Invoke(const _variant_t &variant)
  {
    VARIANT args[1] = { variant };
    return Invoke(args, 1);
  }

  HRESULT Invoke(const std::vector<variant_t> &variants)
  {
	  size_t size = variants.size();
	  VARIANT *args = new VARIANT[size];
	  //Put in reverse order
	  for (size_t i = 0; i<variants.size(); i++)
		  args[size-i-1] = variants [i];
	  HRESULT hr = Invoke(args, size);
	  delete (args);
	  return hr;
  }
  
  HRESULT Invoke(const VARIANT* args, size_t size)
  {
    if (!disp && stream)
	  UnMarshal();

    if (!disp)
      return E_NOT_SET;

    DISPPARAMS dispparams = { 0, 0, 0, 0 };
    dispparams.cArgs = size;
    dispparams.rgvarg = (VARIANTARG*)args;
    dispparams.cNamedArgs = 0;
    VARIANT Var = { 0 };
	//Get IDispatch
	IDispatch* iDispacth = disp;

	//Ensure it is still valid
	if (!iDispacth)
		return E_INVALIDARG;

    return disp->Invoke((DISPID)0, IID_NULL, 0, DISPATCH_METHOD, &dispparams, &Var, NULL, NULL);
  }

  HRESULT Invoke(const _variant_t &variant1, const _variant_t &variant2)
  {
    //in reverse order
    VARIANT args[2] = { variant2, variant1 };
    return Invoke(args, 2);
  }

  HRESULT Invoke(const std::string &msg1, const std::string &msg2)
  {
    //in reverse order
    _variant_t variant1 = msg1.c_str();
    _variant_t variant2 = msg2.c_str();
    return Invoke(variant1,variant2);
  }

private:
  IDispatch *disp = nullptr;
  IStream *stream = nullptr;
};

#endif