#pragma once
#include "LoopHero.h"

enum class IMAGE_ALIGN
{
	LEFT_TOP,
	LEFT_MIDDLE,
	CENTER,
	MIDDLE_TOP,
	MIDDLE_BOTTOM
};

enum class IMAGE_FRAME_TYPE
{
	NORMAL,
	LOOP
};

class Image
{
public:
	enum IMAGE_LOAD_TYPE
	{
		RESOURCE,
		FILE,
		EMPTY,
		END
	};

	struct ImageInfo
	{
		DWORD resID;
		HDC hMemDC;
		HBITMAP hBitmap;

		IMAGE_FRAME_TYPE frameType;

		int width;
		int height;
		BYTE loadType;

		int maxFrameX;
		int maxFrameY;
		int totalFrame;

		bool isTransparent;
		COLORREF transColor;
		HBRUSH hBrush;
	};

	struct ImageFrame
	{
		float axisX[4];
		float axisY[4];
	};

private:
	ImageInfo* lpImageInfo;
	ImageFrame* lpImageFrame;

public:
	HRESULT Init(int width, int height, bool isTransparent = false, COLORREF transColor = RGB(255, 0, 255));
	HRESULT Init(string fileName, int width, int height, bool isTransparent = false, COLORREF transColor = RGB(255, 0, 255));
	HRESULT Init(string fileName, int width, int height, int maxFrameX, int maxFrameY, int totalFrame, bool isTransparent = false, COLORREF transColor = RGB(255, 0, 255));
	HRESULT Init(string fileName, POINT size, POINT frame, POINTFLOAT ltFrame, POINTFLOAT rbFrame, bool isTransparent = false, COLORREF transColor = RGB(255, 0, 255));

	HRESULT Reverse(const Image& target);

	void Fill();
	void Fill(int x, int y, int width, int height, COLORREF color = RGB(0, 0, 0));
	void Fill(HDC copyDC, int x, int y, int width, int height);

	void Render(HDC hdc, int destX = 0, int destY = 0, int frame = 0, IMAGE_ALIGN align = IMAGE_ALIGN::LEFT_TOP);
	void Render(HDC hdc, int destX, int destY, POINT frame, IMAGE_ALIGN align = IMAGE_ALIGN::LEFT_TOP);
	void ResizeRender(HDC hdc, RECT inRect, int frame = 0);
	void LoopRender(HDC hdc, POINT pos, int width, int height, int frame, IMAGE_ALIGN align = IMAGE_ALIGN::LEFT_TOP);
	void PatternRender(HDC hdc, int destX, int destY, int width, int height, int frame = 0);
	
	void Release();

	inline HDC GetMemDC() { return (lpImageInfo) ? lpImageInfo->hMemDC : NULL; }
	inline int GetTotalFrame() { return (lpImageInfo) ? lpImageInfo->totalFrame : 1; }
	inline int GetMaxFrameX() { return (lpImageInfo) ? lpImageInfo->maxFrameX : 1; }
	inline int GetMaxFrameY() { return (lpImageInfo) ? lpImageInfo->maxFrameY : 1; }
	inline int GetWidth() { return lpImageInfo->width; }
	inline int GetHeight() { return lpImageInfo->height; }
	inline IMAGE_FRAME_TYPE GetImageFrameType() { return (lpImageInfo) ? lpImageInfo->frameType : IMAGE_FRAME_TYPE::NORMAL; }
	inline bool IsFrameLoop() { return lpImageInfo->frameType == IMAGE_FRAME_TYPE::LOOP; }
};
