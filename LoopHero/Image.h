#pragma once
#include "LoopHero.h"

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

		vector<HDC> vHMemDC;
		vector<HBITMAP> vHBitmap;

		int width;
		int height;
		BYTE loadType;

		int maxFrameX;
		int maxFrameY;
		int totalFrame;

		bool isTransparent;
		COLORREF transColor;
		HBRUSH hBrush;

		ImageInfo()
		{
			resID = 0;
			width = 0;
			height = 0;
			loadType = IMAGE_LOAD_TYPE::EMPTY;

			maxFrameX = 0;
			maxFrameY = 0;
			totalFrame = 0;

			isTransparent = false;
			transColor = 0;
		};
	};

	struct BlendInfo
	{
		HDC hBlendDC;
		HBITMAP hBlendBit;
		BLENDFUNCTION blendFunc;
	};

private:
	int splitAngle;
	ImageInfo* lpImageInfo;
	BlendInfo* lpBlendInfo;

public:
	HRESULT Init(int width, int height, bool isTransparent = false, COLORREF transColor = RGB(255, 0, 255));
	HRESULT Init(string fileName, int width, int height, bool isTransparent = false, COLORREF transColor = RGB(255, 0, 255));
	HRESULT Init(string fileName, int width, int height, int maxFrameX, int maxFrameY, int totalFrame, bool isTransparent = false, COLORREF transColor = RGB(255, 0, 255));
	HRESULT RotateInit(string fileName, int width, int height, int maxFrameX, int maxFrameY, int totalFrame, int splitAngle);

	HRESULT Reverse(const Image& target);

	void Fill();

	void Render(HDC hdc, int destX = 0, int destY = 0, int frame = 0, UINT uFlag = 0);
	void AlphaRender(HDC hdc, int destX, int destY);
	void RotateRender(HDC hdc, int destX, int destY, float angle, int frame);
	void SplitRender(HDC hdc, POINT dest, int splitX, int splitY, int splitIndex, int frame = 0, UINT uFlag = 0);
	void PatternRender(HDC hdc, int destX, int destY, int width, int height, int frame = 0);
	
	void Release();

	inline HDC GetMemDC() { return (lpImageInfo) ? lpImageInfo->vHMemDC[0] : NULL; }
	inline int GetTotalFrame() { return (lpImageInfo) ? lpImageInfo->totalFrame : 1; }
	inline int GetWidth() { return lpImageInfo->width; }
	inline int GetHeight() { return lpImageInfo->height; }
};
