#ifndef _CALLBACK_H_
#define _CALLBACK_H_
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
  Callback(VARIANT &handler)
  {
    Set(handler);
  }

  Callback(IStream* stream)
  {
    UnMarshal(stream);
  }

  ~Callback()
  {
    Reset();
  }

  static HRESULT Marshal(VARIANT &handler, IStream** stream)
  {
    //Check input type
    if (handler.vt != VT_DISPATCH)
      return E_INVALIDARG;

    HRESULT hr = CoMarshalInterThreadInterfaceInStream(IID_IDispatch, V_DISPATCH(&handler), stream);

    return hr;
  }

  HRESULT UnMarshal(IStream* stream)
  {
    
    Reset();

    HRESULT hr = CoGetInterfaceAndReleaseStream(stream, IID_IDispatch, reinterpret_cast<LPVOID*>(&disp));

    if (FAILED(hr))
      return hr;

    if (!disp)
      return E_INVALIDARG;

    //Inc reference
    disp->AddRef();

    return S_OK;
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

  void Reset()
  {
    if (disp)
    {
      disp->Release();
      disp = nullptr;
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
    if (!disp)
      return E_NOT_SET;

    DISPPARAMS dispparams = { 0, 0, 0, 0 };
    dispparams.cArgs = size;
    dispparams.rgvarg = (VARIANTARG*)args;
    dispparams.cNamedArgs = 0;
    VARIANT Var = { 0 };

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
};

#endif