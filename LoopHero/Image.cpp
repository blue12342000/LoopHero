#include "Image.h"

HRESULT Image::Init(int width, int height, bool isTransparent, COLORREF transColor)
{
    HDC hdc = GetDC(g_hWnd);

    lpImageInfo = new ImageInfo();

    splitAngle = 1;
    lpImageInfo->vHMemDC.resize(splitAngle);
    lpImageInfo->vHBitmap.resize(splitAngle);

    lpImageInfo->resID = 0;
    lpImageInfo->vHMemDC[0] = CreateCompatibleDC(hdc);
    lpImageInfo->vHBitmap[0] = CreateCompatibleBitmap(hdc, width, height);
    lpImageInfo->width = width;
    lpImageInfo->height = height;
    lpImageInfo->loadType = IMAGE_LOAD_TYPE::EMPTY;

    lpImageInfo->maxFrameX = 1;
    lpImageInfo->maxFrameY = 1;
    lpImageInfo->totalFrame = 1;

    DeleteObject(SelectObject(lpImageInfo->vHMemDC[0], lpImageInfo->vHBitmap[0]));
    ReleaseDC(g_hWnd, hdc);

    if (lpImageInfo->vHBitmap[0] == NULL)
    {
        Release();
        return E_FAIL;
    }

    lpImageInfo->isTransparent = isTransparent;
    lpImageInfo->transColor = transColor;
    lpImageInfo->hBrush = CreateSolidBrush(transColor);

    lpBlendInfo = nullptr;

    HBRUSH hBrush, hOldBrush;
    PatBlt(lpImageInfo->vHMemDC[0], 0, 0, width, height, WHITENESS);
    return S_OK;
}

HRESULT Image::Init(string fileName, int width, int height, bool isTransparent, COLORREF transColor)
{
    HDC hdc = GetDC(g_hWnd);

    lpImageInfo = new ImageInfo();
    lpImageInfo->resID = 0;

    splitAngle = 1;
    lpImageInfo->vHMemDC.resize(splitAngle);
    lpImageInfo->vHBitmap.resize(splitAngle);

    lpImageInfo->vHMemDC[0] = CreateCompatibleDC(hdc);
    lpImageInfo->vHBitmap[0] = (HBITMAP)LoadImage(g_hInstance, fileName.c_str(), IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
    lpImageInfo->width = width;
    lpImageInfo->height = height;
    lpImageInfo->loadType = IMAGE_LOAD_TYPE::FILE;

    lpImageInfo->maxFrameX = 1;
    lpImageInfo->maxFrameY = 1;
    lpImageInfo->totalFrame = 1;

    DeleteObject(SelectObject(lpImageInfo->vHMemDC[0], lpImageInfo->vHBitmap[0]));
    ReleaseDC(g_hWnd, hdc);

    if (lpImageInfo->vHBitmap[0] == NULL)
    {
        Release();
        return E_FAIL;
    }

    lpImageInfo->isTransparent = isTransparent;
    lpImageInfo->transColor = transColor;
    lpImageInfo->hBrush = CreateSolidBrush(transColor);

    lpBlendInfo = new BlendInfo();
    lpBlendInfo->blendFunc.AlphaFormat = 0;
    lpBlendInfo->blendFunc.BlendFlags = 0;
    lpBlendInfo->blendFunc.BlendOp = AC_SRC_OVER;
    lpBlendInfo->blendFunc.SourceConstantAlpha = 255;

    return S_OK;
}

HRESULT Image::Init(string fileName, int width, int height, int maxFrameX, int maxFrameY, int totalFrame, bool isTransparent, COLORREF transColor)
{
    HDC hdc = GetDC(g_hWnd);

    lpImageInfo = new ImageInfo();
    lpImageInfo->resID = 0;

    splitAngle = 1;
    lpImageInfo->vHMemDC.resize(splitAngle);
    lpImageInfo->vHBitmap.resize(splitAngle);

    lpImageInfo->vHMemDC[0] = CreateCompatibleDC(hdc);
    lpImageInfo->vHBitmap[0] = (HBITMAP)LoadImage(g_hInstance, fileName.c_str(), IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
    lpImageInfo->width = width / maxFrameX;
    lpImageInfo->height = height / maxFrameY;
    lpImageInfo->loadType = IMAGE_LOAD_TYPE::FILE;

    lpImageInfo->maxFrameX = maxFrameX;
    lpImageInfo->maxFrameY = maxFrameY;
    lpImageInfo->totalFrame = totalFrame;

    DeleteObject(SelectObject(lpImageInfo->vHMemDC[0], lpImageInfo->vHBitmap[0]));
    ReleaseDC(g_hWnd, hdc);

    if (lpImageInfo->vHBitmap[0] == NULL)
    {
        Release();
        return E_FAIL;
    }

    lpImageInfo->isTransparent = isTransparent;
    lpImageInfo->transColor = transColor;
    lpImageInfo->hBrush = CreateSolidBrush(transColor);

    lpBlendInfo = new BlendInfo();
    lpBlendInfo->blendFunc.AlphaFormat = 0;
    lpBlendInfo->blendFunc.BlendFlags = 0;
    lpBlendInfo->blendFunc.BlendOp = AC_SRC_OVER;
    lpBlendInfo->blendFunc.SourceConstantAlpha = 255;

    return S_OK;
}

HRESULT Image::RotateInit(string fileName, int width, int height, int maxFrameX, int maxFrameY, int totalFrame, int splitAngle)
{
    HDC hdc = GetDC(g_hWnd);

    int reSize = (int)ceil(sqrt(width / maxFrameX * width / maxFrameX + height / maxFrameY * height / maxFrameY));

    lpImageInfo = new ImageInfo();
    lpImageInfo->resID = 0;
    lpImageInfo->width = width / maxFrameX;
    lpImageInfo->height = height / maxFrameY;
    lpImageInfo->loadType = IMAGE_LOAD_TYPE::FILE;

    lpImageInfo->maxFrameX = maxFrameX;
    lpImageInfo->maxFrameY = maxFrameY;
    lpImageInfo->totalFrame = totalFrame;

    this->splitAngle = splitAngle;
    lpImageInfo->vHMemDC.resize(splitAngle);
    lpImageInfo->vHBitmap.resize(splitAngle);

    HDC hTempDC = CreateCompatibleDC(hdc);
    HBITMAP hTempBitmap = (HBITMAP)LoadImage(g_hInstance, fileName.c_str(), IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
    HBRUSH hTempBrush = CreateSolidBrush(RGB(255, 0, 255));
    DeleteObject(SelectObject(hTempDC, hTempBitmap));
    XFORM xForm, xOldForm;
    float angle = 0;

    for (int i = 0; i < splitAngle; ++i)
    {
        lpImageInfo->vHMemDC[i] = CreateCompatibleDC(hTempDC);
        lpImageInfo->vHBitmap[i] = CreateCompatibleBitmap(hTempDC, reSize * maxFrameX, reSize * maxFrameY);
        DeleteObject(SelectObject(lpImageInfo->vHMemDC[i], lpImageInfo->vHBitmap[i]));


        HDC hTempFrameDC1 = CreateCompatibleDC(hdc);
        HBITMAP hTempFrameBitmap1 = CreateCompatibleBitmap(hdc, lpImageInfo->width, lpImageInfo->height);
        DeleteObject(SelectObject(hTempFrameDC1, hTempFrameBitmap1));

        HDC hTempFrameDC2 = CreateCompatibleDC(hdc);
        HBITMAP hTempFrameBitmap2 = CreateCompatibleBitmap(hdc, reSize, reSize);
        DeleteObject(SelectObject(hTempFrameDC2, hTempFrameBitmap2));
        DeleteObject(SelectObject(hTempFrameDC2, hTempBrush));
        GetWorldTransform(hTempFrameDC2, &xOldForm);
        PatBlt(hTempFrameDC2, 0, 0, reSize, reSize, PATCOPY);

        angle = PI * 2 / splitAngle * i;
        xForm.eM11 = cosf(angle); xForm.eM12 = sinf(angle);
        xForm.eM21 = -sinf(angle); xForm.eM22 = cosf(angle);
        xForm.eDx = (reSize - cos(angle) * lpImageInfo->width + sin(angle) * lpImageInfo->height) / 2;
        xForm.eDy = (reSize - cos(angle) * lpImageInfo->height - sin(angle) * lpImageInfo->width) / 2;

        SetGraphicsMode(hTempFrameDC2, GM_ADVANCED);
        for (int y = 0; y < maxFrameY; ++y)
        {
            for (int x = 0; x < maxFrameX; ++x)
            {
                BitBlt(hTempFrameDC1, 0, 0, lpImageInfo->width, lpImageInfo->height, hTempDC, lpImageInfo->width * x, lpImageInfo->width * y, SRCCOPY);

                SetWorldTransform(hTempFrameDC2, &xForm);
                StretchBlt(
                    hTempFrameDC2,
                    0, 0,
                    lpImageInfo->width,
                    lpImageInfo->height,
                    hTempFrameDC1,
                    0, 0,
                    lpImageInfo->width,
                    lpImageInfo->height,
                    SRCCOPY
                );
                SetWorldTransform(hTempFrameDC2, &xOldForm);

                BitBlt(lpImageInfo->vHMemDC[i], reSize * x, reSize * y, reSize, reSize, hTempFrameDC2, 0, 0, SRCCOPY);
            }
        }

        SetGraphicsMode(hTempFrameDC2, GM_COMPATIBLE);
        DeleteObject(hTempFrameBitmap1);
        DeleteDC(hTempFrameDC1);
        DeleteObject(hTempFrameBitmap2);
        DeleteDC(hTempFrameDC2);
    }

    DeleteObject(hTempBrush);
    DeleteObject(hTempBitmap);
    DeleteDC(hTempDC);

    lpImageInfo->width = reSize;
    lpImageInfo->height = reSize;


    ReleaseDC(g_hWnd, hdc);

    if (lpImageInfo->vHBitmap[0] == NULL)
    {
        Release();
        return E_FAIL;
    }

    lpImageInfo->isTransparent = true;
    lpImageInfo->transColor = RGB(255, 0, 255);

    lpBlendInfo = new BlendInfo();
    lpBlendInfo->blendFunc.AlphaFormat = 0;
    lpBlendInfo->blendFunc.BlendFlags = 0;
    lpBlendInfo->blendFunc.BlendOp = AC_SRC_OVER;
    lpBlendInfo->blendFunc.SourceConstantAlpha = 255;

    return S_OK;
}

HRESULT Image::Reverse(const Image& target)
{
    lpImageInfo = new ImageInfo();
    lpImageInfo->resID = target.lpImageInfo->resID;

    splitAngle = target.splitAngle;
    lpImageInfo->vHMemDC.resize(splitAngle);
    lpImageInfo->vHBitmap.resize(splitAngle);

    lpImageInfo->vHMemDC[0] = CreateCompatibleDC(target.lpImageInfo->vHMemDC[0]);
    lpImageInfo->vHBitmap[0] = CreateCompatibleBitmap(target.lpImageInfo->vHMemDC[0], target.lpImageInfo->width * target.lpImageInfo->maxFrameX, target.lpImageInfo->height * target.lpImageInfo->maxFrameY);
    lpImageInfo->width = target.lpImageInfo->width;
    lpImageInfo->height = target.lpImageInfo->height;
    lpImageInfo->loadType = target.lpImageInfo->loadType;

    lpImageInfo->maxFrameX = target.lpImageInfo->maxFrameX;
    lpImageInfo->maxFrameY = target.lpImageInfo->maxFrameY;
    lpImageInfo->totalFrame = target.lpImageInfo->totalFrame;

    DeleteObject(SelectObject(lpImageInfo->vHMemDC[0], lpImageInfo->vHBitmap[0]));

    if (lpImageInfo->vHBitmap[0] == NULL)
    {
        Release();
        return E_FAIL;
    }

    lpImageInfo->isTransparent = target.lpImageInfo->isTransparent;
    lpImageInfo->transColor = target.lpImageInfo->transColor;

    if (target.lpBlendInfo)
    {
        lpBlendInfo = new BlendInfo();
        lpBlendInfo->blendFunc.AlphaFormat = target.lpBlendInfo->blendFunc.AlphaFormat;
        lpBlendInfo->blendFunc.BlendFlags = target.lpBlendInfo->blendFunc.BlendFlags;
        lpBlendInfo->blendFunc.BlendOp = target.lpBlendInfo->blendFunc.BlendOp;
        lpBlendInfo->blendFunc.SourceConstantAlpha = target.lpBlendInfo->blendFunc.SourceConstantAlpha;
    }
    else
    {
        lpBlendInfo = nullptr;
    }

    for (int y = 0; y < lpImageInfo->maxFrameY; ++y)
    {
        for (int x = 0; x < lpImageInfo->maxFrameX; ++x)
        {
            StretchBlt(lpImageInfo->vHMemDC[0], (x + 1) * lpImageInfo->width - 1, y * lpImageInfo->height, -lpImageInfo->width, lpImageInfo->height,
                target.lpImageInfo->vHMemDC[0], x * lpImageInfo->width, y * lpImageInfo->height, lpImageInfo->width, lpImageInfo->height, SRCCOPY);
        }
    }

    return S_OK;
}

void Image::Fill()
{
    for (int i = 0; i < splitAngle; ++i)
    {
        HBRUSH hOldBrush = (HBRUSH)SelectObject(lpImageInfo->vHMemDC[i], lpImageInfo->hBrush);
        PatBlt(lpImageInfo->vHMemDC[i], 0, 0, lpImageInfo->width, lpImageInfo->height, PATCOPY);
        SelectObject(lpImageInfo->vHMemDC[i], hOldBrush);
    }
}

void Image::Render(HDC hdc, int destX, int destY, int frame, IMAGE_ALIGN align)
{
    switch (align)
    {
    case IMAGE_ALIGN::CENTER:
        destX -= lpImageInfo->width / 2;
        destY -= lpImageInfo->height / 2;
    }

    //switch (uFlag)
    //{
    //case U_IA_CENTER:
    //    destX -= lpImageInfo->width / 2;
    //    destY -= lpImageInfo->height / 2;
    //    break;
    //}

    if (lpImageInfo->isTransparent)
    {
        GdiTransparentBlt(hdc, destX, destY,lpImageInfo->width, lpImageInfo->height,
            lpImageInfo->vHMemDC[0], lpImageInfo->width * (frame % lpImageInfo->maxFrameX), lpImageInfo->height * (frame / lpImageInfo->maxFrameX), lpImageInfo->width, lpImageInfo->height, lpImageInfo->transColor);
    }
    else
    {
        BitBlt(hdc, destX, destY, lpImageInfo->width, lpImageInfo->height,
            lpImageInfo->vHMemDC[0], lpImageInfo->width * (frame % lpImageInfo->maxFrameX), lpImageInfo->height * (frame / lpImageInfo->maxFrameX), SRCCOPY);
    }
}

void Image::Render(HDC hdc, int destX, int destY, POINT frame)
{
    if (lpImageInfo->isTransparent)
    {
        GdiTransparentBlt(hdc, destX, destY, lpImageInfo->width, lpImageInfo->height,
            lpImageInfo->vHMemDC[0], lpImageInfo->width * frame.x, lpImageInfo->height * frame.y, lpImageInfo->width, lpImageInfo->height, lpImageInfo->transColor);
    }
    else
    {
        BitBlt(hdc, destX, destY, lpImageInfo->width, lpImageInfo->height,
            lpImageInfo->vHMemDC[0], lpImageInfo->width * frame.x, lpImageInfo->height * frame.y, SRCCOPY);
    }
}

void Image::AlphaRender(HDC hdc, int destX, int destY)
{
    BitBlt(lpBlendInfo->hBlendDC, 0, 0, lpImageInfo->width, lpImageInfo->height, hdc, destX, destY, SRCCOPY);

    GdiTransparentBlt(lpBlendInfo->hBlendDC, 0, 0, lpImageInfo->width, lpImageInfo->height,
        lpImageInfo->vHMemDC[0], 0, 0, lpImageInfo->width, lpImageInfo->height, lpImageInfo->transColor);

    AlphaBlend(hdc, destX, destY, lpImageInfo->width, lpImageInfo->height,
        lpBlendInfo->hBlendDC, 0, 0, lpImageInfo->width, lpImageInfo->height, lpBlendInfo->blendFunc);
}

void Image::RotateRender(HDC hdc, int destX, int destY, float angle, int frame)
{
    destX -= lpImageInfo->width / 2;
    destY -= lpImageInfo->height / 2;
    frame %= lpImageInfo->totalFrame;

    int degree = angle * 180 / PI;
    while (degree < 0) degree += 360;
    degree += 180 / splitAngle;
    degree %= 360;
    int index = (degree / (360 / splitAngle));
    if (lpImageInfo->isTransparent)
    {
        GdiTransparentBlt(hdc, destX, destY, lpImageInfo->width, lpImageInfo->height,
            lpImageInfo->vHMemDC[index], lpImageInfo->width * (frame % lpImageInfo->maxFrameX), lpImageInfo->height * (frame / lpImageInfo->maxFrameX), lpImageInfo->width, lpImageInfo->height, lpImageInfo->transColor);
    }
    else
    {
        BitBlt(hdc, destX, destY, lpImageInfo->width, lpImageInfo->height,
            lpImageInfo->vHMemDC[index], lpImageInfo->width * (frame % lpImageInfo->maxFrameX), lpImageInfo->height * (frame / lpImageInfo->maxFrameX), SRCCOPY);
    }
}

void Image::SplitRender(HDC hdc, POINT dest, int splitX, int splitY, int splitIndex, int frame, UINT uFlag)
{
    int width = lpImageInfo->width / splitX;
    int height = lpImageInfo->height / splitY;

    //switch (uFlag)
    //{
    //case U_IA_CENTER:
    //    dest.x -= width / 2;
    //    dest.y -= height / 2;
    //    break;
    //}

    if (lpImageInfo->isTransparent)
    {
        GdiTransparentBlt(hdc, dest.x, dest.y, width, height,
            lpImageInfo->vHMemDC[0], lpImageInfo->width * (frame % lpImageInfo->maxFrameX) + width * (splitIndex % splitX), lpImageInfo->height * (frame / lpImageInfo->maxFrameX) + height * (splitIndex / splitX), width, height, lpImageInfo->transColor);
     }
    else
    {
        StretchBlt(hdc, dest.x, dest.y, width, height,
            lpImageInfo->vHMemDC[0], lpImageInfo->width * (frame % lpImageInfo->maxFrameX) + width * (splitIndex % splitX), lpImageInfo->height * (frame / lpImageInfo->maxFrameX) + height * (splitIndex / splitX), width, height, SRCCOPY);
    }
}

void Image::PatternRender(HDC hdc, int destX, int destY, int width, int height, int frame)
{
    int limitX = destX + width;
    int limitY = destY - height;
    int drawX, drawY;

    if (lpImageInfo->isTransparent)
    {
        for (int y = destY; y > limitY; y -= lpImageInfo->height)
        {
            for (int x = destX; x < limitX; x += lpImageInfo->width)
            {
                if (x + lpImageInfo->width <= limitX) { drawX = lpImageInfo->width; }
                else { drawX = width % lpImageInfo->width; }
                if (y - lpImageInfo->height >= limitY) { drawY = lpImageInfo->height; }
                else { drawY = height % lpImageInfo->height; }

                GdiTransparentBlt(hdc, x, y, drawX, -drawY,
                    lpImageInfo->vHMemDC[0], lpImageInfo->width * (frame % lpImageInfo->maxFrameX), lpImageInfo->height * (frame / lpImageInfo->maxFrameX), drawX, drawY, lpImageInfo->transColor);
            }
        }
    }
    else
    {
        for (int y = destY; y > limitY; y -= lpImageInfo->height)
        {
            for (int x = destX; x < limitX; x += lpImageInfo->width)
            {
                if (x + lpImageInfo->width <= limitX) { drawX = lpImageInfo->width; }
                else { drawX = width % lpImageInfo->width; }
                if (y - lpImageInfo->height >= limitY) { drawY = lpImageInfo->height; }
                else { drawY = height % lpImageInfo->height; }

                StretchBlt(hdc, x, y, drawX, -drawY,
                    lpImageInfo->vHMemDC[0], lpImageInfo->width * (frame % lpImageInfo->maxFrameX), lpImageInfo->height * (frame / lpImageInfo->maxFrameX), drawX, drawY, SRCCOPY);
            }
        }
    }
}

void Image::Release()
{
    if (lpImageInfo)
    {
        for (int i = 0; i < splitAngle; ++i)
        {
            DeleteObject(lpImageInfo->vHBitmap[i]);
            DeleteDC(lpImageInfo->vHMemDC[i]);
        }
        delete lpImageInfo;
        lpImageInfo = nullptr;

        if (lpBlendInfo)
        {
            DeleteObject(lpBlendInfo->hBlendBit);
            DeleteDC(lpBlendInfo->hBlendDC);
            delete lpBlendInfo;
            lpBlendInfo = nullptr;
        }
    }
}
