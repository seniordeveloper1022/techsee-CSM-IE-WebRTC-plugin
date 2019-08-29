// VideoRenderer.cpp : Implementation of VideoRenderer
#include "stdafx.h"
#include "WebRTCProxy.h"
#include "VideoRenderer.h"
#undef FOURCC
#include "third_party/libyuv/include/libyuv.h"
#include "api/video/i420_buffer.h"


HRESULT VideoRenderer::FinalConstruct()
{
	SetThread(WebRTCProxy::GetEventThread());
	rotation = webrtc::kVideoRotation_0;

	//Done
	return S_OK;
}

void VideoRenderer::OnFrame(const webrtc::VideoFrame& frame)
{
	//Check if size has changed
	if (videoWidth != frame.width() || videoHeight != frame.height())
	{
		//Update
		videoWidth = frame.width();
		videoHeight = frame.height();

		//Fire event
		variant_t width = videoWidth;
		variant_t height = videoWidth;
		DispatchAsync(onresize, width, height);
	}
	
	if (frame.rotation() != rotation)
		RTC_LOG(LS_INFO) << "frame rotation changed to " << frame.rotation();
	rotation = frame.rotation();

	// Only iOS rotated frames needs to be adjusted.
	webrtc::VideoFrame* clone = nullptr;
	if (rotation == webrtc::kVideoRotation_90 ||
		rotation == webrtc::kVideoRotation_180)
	{
		// Apply rotation and clone video frame
		webrtc::VideoFrame rotated_frame(frame);
		rotated_frame.set_video_frame_buffer(
				webrtc::I420Buffer::Rotate(*frame.video_frame_buffer()->GetI420(),
				frame.rotation()));
		rotated_frame.set_rotation(webrtc::kVideoRotation_0);
		clone = new webrtc::VideoFrame(rotated_frame);
	}
	else
		clone = new webrtc::VideoFrame(frame);

	//Store clone as background frame
	mutex.lock();

	frames[background] = std::shared_ptr<webrtc::VideoFrame>(clone);

	//Move background buffer to foreground
	background = !background;

	mutex.unlock();

	//Redraw
	::InvalidateRect(hwndParent, NULL, 0);
}

void VideoRenderer::OnDiscardedFrame()
{
	RTC_LOG(LS_WARNING) << "A frame has been discarded.";
}

HRESULT VideoRenderer::OnDrawAdvanced(ATL_DRAWINFO& di)
{
	RECT* rc = (RECT*)di.prcBounds;
	HDC hdc = di.hdcDraw;

	// Create black brush
	HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));

	//Fill rect with black
	FillRect(hdc, rc, hBrush);

	// Delete black brush
	DeleteObject(hBrush);

	//Lock
	mutex.lock();

	//Get foreground buffer
	std::shared_ptr<webrtc::VideoFrame> frame(frames[!background]);

	//Unlock
	mutex.unlock();

	//Check if we have a frame already
	if (!frame)
		return S_OK;

	//Get width and height
	int width = frame->width();
	int height = frame->height();

	//Get I420 data
	auto yuv = frame->video_frame_buffer()->ToI420();

	//Create ARGB data
	uint8_t* rgb = (uint8_t*)malloc(width * height * 4);

	//Convert to rgb
	libyuv::I420ToARGB(
		yuv->DataY(),
		yuv->StrideY(),
		yuv->DataU(),
		yuv->StrideU(),
		yuv->DataV(),
		yuv->StrideV(),
		rgb,
		width * 4,
		width,
		height);

	//Set stretching mode
	int oldMode = SetStretchBltMode(hdc, HALFTONE);

	// Create bitmap
	BITMAPINFO info;
	info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	info.bmiHeader.biWidth = width;
	info.bmiHeader.biHeight = height;
	info.bmiHeader.biPlanes = 1;
	info.bmiHeader.biBitCount = 32;
	info.bmiHeader.biCompression = BI_RGB;
	info.bmiHeader.biSizeImage = 0;
	info.bmiHeader.biXPelsPerMeter = 0;
	info.bmiHeader.biYPelsPerMeter = 0;
	info.bmiHeader.biClrUsed = 0;
	info.bmiHeader.biClrImportant = 0;

	//Fill rect
	int xDest = rc->left;
	int yDest = rc->bottom;
	int wDest = rc->right - rc->left;
	int hDest = rc->top - rc->bottom;

	//Copy & stretch
	StretchDIBits(
		hdc,
		xDest,
		yDest,
		wDest,
		hDest,
		0,
		0,
		width,
		height,
		rgb,
		&info,
		DIB_RGB_COLORS,
		SRCCOPY
	);

	//////////////////////////////////////////////////////////////////////////
// 	BITMAPINFO bmpInfo = info;
// 
// 	//HDC hdcDst = hdc;
// 	//HDC hdcDst = CreateDC(L"DISPLAY", NULL, NULL, NULL);
// 	//HBITMAP hBitmap = CreateBitmap(width, height, 1, 32, rgb);
// 	
// 	HDC  hdcDst = CreateCompatibleDC(NULL);
// 	
// 	HBITMAP hBitmap = CreateCompatibleBitmap(hdcDst, width, height);
// 	SelectObject(hdcDst, hBitmap);
// 	GetDIBits(hdcDst, hBitmap, 0, 0, NULL, &bmpInfo, DIB_RGB_COLORS);
// 
// 	void* pBuf = malloc(bmpInfo.bmiHeader.biSizeImage);
// 	memset(pBuf, 0x0, bmpInfo.bmiHeader.biSizeImage);
// 	memcpy(pBuf, rgb, bmpInfo.bmiHeader.biSizeImage);
// 
// 	// Swap X
// 	//////////////////////////////////////////////////////////////////////////
// 
// 	unsigned long bytesperrow = width * 4; // assuming 4 bytes/pixel
// 	uint8_t *destination = (uint8_t*)malloc(height*bytesperrow);
// 	memset(destination, 0x0, height*bytesperrow);
// 
// 	// Point to the start of bottom line
// 	void *source = pBuf; //(uint8_t*)pBuf + bytesperrow * (height - 1);
// 	
// 	int dstPos = 0;
// 	int srcPos = (uint8_t)pBuf + bytesperrow * (height - 1);
// 	for (int row = 0; row < height; row++)
// 	{
// 		memcpy((uint8_t*)destination + dstPos, (uint8_t*)source + srcPos, bytesperrow);
// 		dstPos += bytesperrow;
// 		srcPos -= bytesperrow;
// 	}
// 
// 	// flipped image is now ready for further processing
// 	//////////////////////////////////////////////////////////////////////////
// 
// 	BITMAPFILEHEADER bmpFileHeader = { 0 };
// 	bmpFileHeader.bfReserved1 = 0;
// 	bmpFileHeader.bfReserved2 = 0;
// 	bmpFileHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) +
// 							bmpInfo.bmiHeader.biSizeImage;
// 	bmpFileHeader.bfType = 'MB';
// 	bmpFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
// 
// 	FILE* fp = fopen("C:\\test.bmp", "wb");
// 	if (fp != NULL)
// 	{
// 		fwrite(&bmpFileHeader, sizeof(BITMAPFILEHEADER), 1, fp);
// 		fwrite(&bmpInfo.bmiHeader, sizeof(BITMAPINFOHEADER), 1, fp);
// 		fwrite(destination, bmpInfo.bmiHeader.biSizeImage, 1, fp);
// 		fclose(fp);
// 	}
// 
// 	DeleteObject(hBitmap);
// 
// 	free(destination);
// 	free(pBuf);
	//////////////////////////////////////////////////////////////////////////

	// Clean rgb data
	free(rgb);
	
	// Restore stretching mode
	SetStretchBltMode(hdc, oldMode);

	return S_OK;
}

STDMETHODIMP VideoRenderer::setTrack(VARIANT track)
{
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

	//Convert to video
	webrtc::VideoTrackInterface* videoTrack = reinterpret_cast<webrtc::VideoTrackInterface*>(proxy->GetTrack().get());
	if (!videoTrack)
		return E_INVALIDARG;

	//Add us as video 
	rtc::VideoSinkWants wanted;
	wanted.rotation_applied = true;
	videoTrack->AddOrUpdateSink(this, wanted);

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////

static const std::string base64_chars =
	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len)
{
	std::string ret;
	int i = 0;
	int j = 0;
	unsigned char char_array_3[3];
	unsigned char char_array_4[4];

	while (in_len--)
	{
		char_array_3[i++] = *(bytes_to_encode++);
		if (i == 3)
		{
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;

			for (i = 0; (i < 4); i++)
				ret += base64_chars[char_array_4[i]];
			i = 0;
		}
	}

	if (i)
	{
		for (j = i; j < 3; j++)
			char_array_3[j] = '\0';

		char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
		char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
		char_array_4[3] = char_array_3[2] & 0x3f;

		for (j = 0; (j < i + 1); j++)
			ret += base64_chars[char_array_4[j]];

		while ((i++ < 3))
			ret += '=';
	}

	return ret;
}

//////////////////////////////////////////////////////////////////////////

STDMETHODIMP VideoRenderer::getFrame(VARIANT* val)
{
	HRESULT hr = S_OK;

	// Get foreground buffer
	mutex.lock();
	std::shared_ptr<webrtc::VideoFrame> frame(frames[false]);
	mutex.unlock();

	// Check if we have a frame already
	if (!frame)
		return S_OK;

	// Get width and height
	int width = frame->width();
	int height = frame->height();
	int imageSize = width * height * 4;

	// Get I420 data
	auto yuv = frame->video_frame_buffer()->ToI420();

	uint8_t* rgba = (uint8_t*)malloc(imageSize);
	memset(rgba, 0x0, imageSize);

	// Convert to rgba
	libyuv::I420ToARGB(
		yuv->DataY(),
		yuv->StrideY(),
		yuv->DataU(),
		yuv->StrideU(),
		yuv->DataV(),
		yuv->StrideV(),
		rgba,
		width * 4,
		width,
		height);

	//////////////////////////////////////////////////////////////////////////

	// Create bitmap
	BITMAPINFO bmpInfo;
	bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfo.bmiHeader.biWidth = width;
	bmpInfo.bmiHeader.biHeight = height;
	bmpInfo.bmiHeader.biPlanes = 1;
	bmpInfo.bmiHeader.biBitCount = 32;
	bmpInfo.bmiHeader.biCompression = BI_RGB;
	bmpInfo.bmiHeader.biSizeImage = 0;
	bmpInfo.bmiHeader.biXPelsPerMeter = 0;
	bmpInfo.bmiHeader.biYPelsPerMeter = 0;
	bmpInfo.bmiHeader.biClrUsed = 0;
	bmpInfo.bmiHeader.biClrImportant = 0;

	// Create DC
	HDC hdcDst = CreateCompatibleDC(nullptr);
	if (!hdcDst)
		return S_OK;

	// Create bitmap
	HBITMAP hBitmap = CreateCompatibleBitmap(hdcDst, width, height);
	if (!hBitmap)
		return S_OK;

	// Get bitmap size
	SelectObject(hdcDst, hBitmap);
	GetDIBits(hdcDst, hBitmap, 0, 0, NULL, &bmpInfo, DIB_RGB_COLORS);

	//////////////////////////////////////////////////////////////////////////
	
	// Flip and mirror

	// Assuming 4 bytes per pixel
	unsigned long bytesperrow = width * 4;
	uint8_t *destination = (uint8_t*)malloc(imageSize);
	memset(destination, 0x0, imageSize);

	int dstPos = 0;

	// Point to the start of bottom line
	uint8_t *source = rgba;
	int srcPos = (uint8_t)source[0] + bytesperrow * (height - 1);
	for (int row = 0; row < height; row++)
	{
		memcpy((uint8_t*)destination + dstPos, (uint8_t*)source + srcPos, bytesperrow);
		dstPos += bytesperrow;
		srcPos -= bytesperrow;
	}

	if (dstPos != bmpInfo.bmiHeader.biSizeImage)
		return S_OK;

	//////////////////////////////////////////////////////////////////////////
	int ntotalSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) +
		bmpInfo.bmiHeader.biSizeImage;

	BITMAPFILEHEADER bmpFileHeader = { 0 };
	bmpFileHeader.bfReserved1 = 0;
	bmpFileHeader.bfReserved2 = 0;
	bmpFileHeader.bfSize = ntotalSize;
	bmpFileHeader.bfType = 'MB';
	bmpFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	//////////////////////////////////////////////////////////////////////////

	unsigned char * bmpBuff = new unsigned char[ntotalSize];
	memset(bmpBuff, 0x0, ntotalSize);

	int i = 0;
	memcpy((void *)(&(bmpBuff[i])), (void *)&bmpFileHeader, sizeof(BITMAPFILEHEADER));
	i += sizeof(BITMAPFILEHEADER);
	memcpy((void *)(&(bmpBuff[i])), (void *)&bmpInfo.bmiHeader, sizeof(BITMAPINFOHEADER));
	i += sizeof(BITMAPINFOHEADER);
	memcpy((void *)(&(bmpBuff[i])), (void *)destination, bmpInfo.bmiHeader.biSizeImage);

#ifdef DEBUG
	FILE* fp = fopen("C:\\test.bmp", "wb");
	if (fp != NULL)
	{
		fwrite(&bmpFileHeader, sizeof(BITMAPFILEHEADER), 1, fp);
		fwrite(&bmpInfo.bmiHeader, sizeof(BITMAPINFOHEADER), 1, fp);
		fwrite(destination, bmpInfo.bmiHeader.biSizeImage, 1, fp);
		fclose(fp);
	}
#endif

	std::string encodedImage = base64_encode(bmpBuff, ntotalSize);
	if (val != nullptr)
	{
		variant_t out = encodedImage.c_str();
		(*val).vt = VT_BSTR;
		(*val).bstrVal = SysAllocString(out.bstrVal);
	}

	if (hBitmap)
		DeleteObject(hBitmap);

	if (hdcDst)
		DeleteDC(hdcDst);

	if (destination)
		free(destination);

	// Clean rgba data
	if (rgba)
		free(rgba);

	return hr;
}
