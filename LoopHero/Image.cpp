#include "Image.h"

HRESULT Image::Init(int width, int height, bool isTransparent, COLORREF transColor)
{
    HDC hdc = GetDC(g_hWnd);

    lpImageInfo = new ImageInfo();

    lpImageInfo->resID = 0;
    lpImageInfo->hMemDC = CreateCompatibleDC(hdc);
    lpImageInfo->hBitmap = CreateCompatibleBitmap(hdc, width, height);
    lpImageInfo->width = width;
    lpImageInfo->height = height;
    lpImageInfo->loadType = IMAGE_LOAD_TYPE::EMPTY;

    lpImageInfo->maxFrameX = 1;
    lpImageInfo->maxFrameY = 1;
    lpImageInfo->totalFrame = 1;

    DeleteObject(SelectObject(lpImageInfo->hMemDC, lpImageInfo->hBitmap));
    ReleaseDC(g_hWnd, hdc);

    if (lpImageInfo->hBitmap == NULL)
    {
        Release();
        return E_FAIL;
    }

    lpImageInfo->isTransparent = isTransparent;
    lpImageInfo->transColor = transColor;
    lpImageInfo->hBrush = CreateSolidBrush(transColor);

    lpImageFrame = nullptr;

    return S_OK;
}

HRESULT Image::Init(string fileName, int width, int height, bool isTransparent, COLORREF transColor)
{
    HDC hdc = GetDC(g_hWnd);

    lpImageInfo = new ImageInfo();
    lpImageInfo->resID = 0;

    lpImageInfo->hMemDC = CreateCompatibleDC(hdc);
    lpImageInfo->hBitmap = (HBITMAP)LoadImage(g_hInstance, fileName.c_str(), IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
    lpImageInfo->width = width;
    lpImageInfo->height = height;
    lpImageInfo->loadType = IMAGE_LOAD_TYPE::FILE;
    lpImageInfo->frameType = IMAGE_FRAME_TYPE::NORMAL;

    lpImageInfo->maxFrameX = 1;
    lpImageInfo->maxFrameY = 1;
    lpImageInfo->totalFrame = 1;

    DeleteObject(SelectObject(lpImageInfo->hMemDC, lpImageInfo->hBitmap));
    ReleaseDC(g_hWnd, hdc);

    if (lpImageInfo->hBitmap == NULL)
    {
        Release();
        return E_FAIL;
    }

    lpImageInfo->isTransparent = isTransparent;
    lpImageInfo->transColor = transColor;

    lpImageFrame = nullptr;

    return S_OK;
}

HRESULT Image::Init(string fileName, int width, int height, int maxFrameX, int maxFrameY, int totalFrame, bool isTransparent, COLORREF transColor)
{
    HDC hdc = GetDC(g_hWnd);

    lpImageInfo = new ImageInfo();
    lpImageInfo->resID = 0;

    lpImageInfo->hMemDC = CreateCompatibleDC(hdc);
    lpImageInfo->hBitmap = (HBITMAP)LoadImage(g_hInstance, fileName.c_str(), IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
    lpImageInfo->width = width / maxFrameX;
    lpImageInfo->height = height / maxFrameY;
    lpImageInfo->loadType = IMAGE_LOAD_TYPE::FILE;
    lpImageInfo->frameType = IMAGE_FRAME_TYPE::NORMAL;

    lpImageInfo->maxFrameX = maxFrameX;
    lpImageInfo->maxFrameY = maxFrameY;
    lpImageInfo->totalFrame = totalFrame;

    DeleteObject(SelectObject(lpImageInfo->hMemDC, lpImageInfo->hBitmap));
    ReleaseDC(g_hWnd, hdc);

    if (lpImageInfo->hBitmap == NULL)
    {
        Release();
        return E_FAIL;
    }

    lpImageInfo->isTransparent = isTransparent;
    lpImageInfo->transColor = transColor;

    lpImageFrame = nullptr;

    return S_OK;
}

HRESULT Image::Init(string fileName, POINT size, POINT frame, POINTFLOAT ltFrame, POINTFLOAT rbFrame, bool isTransparent, COLORREF transColor)
{
    HDC hdc = GetDC(g_hWnd);

    lpImageInfo = new ImageInfo();
    lpImageInfo->resID = 0;

    lpImageInfo->hMemDC = CreateCompatibleDC(hdc);
    lpImageInfo->hBitmap = (HBITMAP)LoadImage(g_hInstance, fileName.c_str(), IMAGE_BITMAP, size.x, size.y, LR_LOADFROMFILE);
    lpImageInfo->width = size.x / frame.x;
    lpImageInfo->height = size.y / frame.y;
    lpImageInfo->loadType = IMAGE_LOAD_TYPE::FILE;
    lpImageInfo->frameType = IMAGE_FRAME_TYPE::LOOP;

    lpImageInfo->maxFrameX = frame.x;
    lpImageInfo->maxFrameY = frame.y;
    lpImageInfo->totalFrame = frame.x * frame.y;

    DeleteObject(SelectObject(lpImageInfo->hMemDC, lpImageInfo->hBitmap));
    ReleaseDC(g_hWnd, hdc);

    if (lpImageInfo->hBitmap == NULL)
    {
        Release();
        return E_FAIL;
    }

    lpImageInfo->isTransparent = isTransparent;
    lpImageInfo->transColor = transColor;

    lpImageFrame = new ImageFrame();
    lpImageFrame->axisX[0] = 0.0f;
    lpImageFrame->axisX[1] = ltFrame.x; 
    lpImageFrame->axisX[2] = rbFrame.x;
    lpImageFrame->axisX[3] = 1.0f;

    lpImageFrame->axisY[0] = 0.0f;
    lpImageFrame->axisY[1] = ltFrame.y;
    lpImageFrame->axisY[2] = rbFrame.y;
    lpImageFrame->axisY[3] = 1.0f;
    return S_OK;
}

HRESULT Image::Reverse(const Image& target)
{
    lpImageInfo = new ImageInfo();
    lpImageInfo->resID = target.lpImageInfo->resID;

    lpImageInfo->hMemDC = CreateCompatibleDC(target.lpImageInfo->hMemDC);
    lpImageInfo->hBitmap = CreateCompatibleBitmap(target.lpImageInfo->hMemDC, target.lpImageInfo->width * target.lpImageInfo->maxFrameX, target.lpImageInfo->height * target.lpImageInfo->maxFrameY);
    lpImageInfo->width = target.lpImageInfo->width;
    lpImageInfo->height = target.lpImageInfo->height;
    lpImageInfo->loadType = target.lpImageInfo->loadType;

    lpImageInfo->maxFrameX = target.lpImageInfo->maxFrameX;
    lpImageInfo->maxFrameY = target.lpImageInfo->maxFrameY;
    lpImageInfo->totalFrame = target.lpImageInfo->totalFrame;

    DeleteObject(SelectObject(lpImageInfo->hMemDC, lpImageInfo->hBitmap));

    if (lpImageInfo->hBitmap == NULL)
    {
        Release();
        return E_FAIL;
    }

    lpImageInfo->isTransparent = target.lpImageInfo->isTransparent;
    lpImageInfo->transColor = target.lpImageInfo->transColor;

    for (int y = 0; y < lpImageInfo->maxFrameY; ++y)
    {
        for (int x = 0; x < lpImageInfo->maxFrameX; ++x)
        {
            StretchBlt(lpImageInfo->hMemDC, (x + 1) * lpImageInfo->width - 1, y * lpImageInfo->height, -lpImageInfo->width, lpImageInfo->height,
                target.lpImageInfo->hMemDC, x * lpImageInfo->width, y * lpImageInfo->height, lpImageInfo->width, lpImageInfo->height, SRCCOPY);
        }
    }

    return S_OK;
}

void Image::Fill()
{
    HBRUSH hOldBrush = (HBRUSH)SelectObject(lpImageInfo->hMemDC, lpImageInfo->hBrush);
    PatBlt(lpImageInfo->hMemDC, 0, 0, lpImageInfo->width, lpImageInfo->height, PATCOPY);
    SelectObject(lpImageInfo->hMemDC, hOldBrush);
}

void Image::Render(HDC hdc, int destX, int destY, int frame, IMAGE_ALIGN align)
{
    frame %= lpImageInfo->totalFrame;

    switch (align)
    {
    case IMAGE_ALIGN::CENTER:
        destX -= lpImageInfo->width / 2;
        destY -= lpImageInfo->height / 2;
        break;
    case IMAGE_ALIGN::MIDDLE_BOTTOM:
        destX -= lpImageInfo->width / 2;
        destY -= lpImageInfo->height;
        break;
    case IMAGE_ALIGN::MIDDLE_TOP:
        destX -= lpImageInfo->width / 2;
        break;
    }

    if (lpImageInfo->isTransparent)
    {
        GdiTransparentBlt(hdc, destX, destY,lpImageInfo->width, lpImageInfo->height,
            lpImageInfo->hMemDC, lpImageInfo->width * (frame % lpImageInfo->maxFrameX), lpImageInfo->height * (frame / lpImageInfo->maxFrameX), lpImageInfo->width, lpImageInfo->height, lpImageInfo->transColor);
    }
    else
    {
        BitBlt(hdc, destX, destY, lpImageInfo->width, lpImageInfo->height,
            lpImageInfo->hMemDC, lpImageInfo->width * (frame % lpImageInfo->maxFrameX), lpImageInfo->height * (frame / lpImageInfo->maxFrameX), SRCCOPY);
    }
}

void Image::Render(HDC hdc, int destX, int destY, POINT frame, IMAGE_ALIGN align)
{
    frame.x %= lpImageInfo->maxFrameX;
    frame.y %= lpImageInfo->maxFrameY;

    switch (align)
    {
    case IMAGE_ALIGN::CENTER:
        destX -= lpImageInfo->width / 2;
        destY -= lpImageInfo->height / 2;
        break;
    case IMAGE_ALIGN::MIDDLE_BOTTOM:
        destX -= lpImageInfo->width / 2;
        destY -= lpImageInfo->height;
        break;
    case IMAGE_ALIGN::MIDDLE_TOP:
        destX -= lpImageInfo->width / 2;
        break;
    }

    if (lpImageInfo->isTransparent)
    {
        GdiTransparentBlt(hdc, destX, destY, lpImageInfo->width, lpImageInfo->height,
            lpImageInfo->hMemDC, lpImageInfo->width * frame.x, lpImageInfo->height * frame.y, lpImageInfo->width, lpImageInfo->height, lpImageInfo->transColor);
    }
    else
    {
        BitBlt(hdc, destX, destY, lpImageInfo->width, lpImageInfo->height,
            lpImageInfo->hMemDC, lpImageInfo->width * frame.x, lpImageInfo->height * frame.y, SRCCOPY);
    }
}

void Image::LoopRender(HDC hdc, POINT pos, int width, int height, int frame, IMAGE_ALIGN align)
{
    int frameX = frame % lpImageInfo->maxFrameX;
    int frameY = frame / lpImageInfo->maxFrameX;

    switch (align)
    {
    case IMAGE_ALIGN::CENTER:
        pos.x -= width / 2;
        pos.y -= height / 2;
        break;
    case IMAGE_ALIGN::MIDDLE_BOTTOM:
        pos.x -= width / 2;
        pos.y -= height;
        break;
    case IMAGE_ALIGN::MIDDLE_TOP:
        pos.x -= height / 2;
        break;
    }

    POINT size;
    int drawX[4] = { 0, (int)(lpImageFrame->axisX[1] * lpImageInfo->width), width - (int)((1.0f - lpImageFrame->axisX[2]) * lpImageInfo->width), width };
    int drawY[4] = { 0, (int)(lpImageFrame->axisY[1] * lpImageInfo->height), height - (int)((1.0f - lpImageFrame->axisY[2]) * lpImageInfo->height), height };
    if (lpImageInfo->isTransparent)
    {
        for (int y = 0; y < 3; ++y)
        {
            for (int x = 0; x < 3; ++x)
            {
                size.x = max(drawX[x + 1] - drawX[x], 0);
                size.y = max(drawY[y + 1] - drawY[y], 0);

                if (size.x == 0 || size.y == 0) continue;

                GdiTransparentBlt(hdc, pos.x + drawX[x], pos.y + drawY[y], size.x, size.y,
                    lpImageInfo->hMemDC, (float)(frameX + lpImageFrame->axisX[x]) * lpImageInfo->width, (float)(frameY + lpImageFrame->axisY[y]) * lpImageInfo->height,
                    (float)(lpImageFrame->axisX[x + 1] - lpImageFrame->axisX[x]) * lpImageInfo->width, (float)(lpImageFrame->axisY[y + 1] - lpImageFrame->axisY[y]) * lpImageInfo->height, lpImageInfo->transColor);
            }
        }
    }
    else
    {
        for (int y = 0; y < 3; ++y)
        {
            for (int x = 0; x < 3; ++x)
            {
                size.x = max(drawX[x + 1] - drawX[x], 0);
                size.y = max(drawY[y + 1] - drawY[y], 0);

                if (size.x == 0 || size.y == 0) continue;

                StretchBlt(hdc, pos.x + drawX[x], pos.y + drawY[y], size.x, size.y,
                    lpImageInfo->hMemDC, (float)(frameX + lpImageFrame->axisX[x]) * lpImageInfo->width, (float)(frameY + lpImageFrame->axisY[y]) * lpImageInfo->height,
                    (float)(lpImageFrame->axisX[x + 1] - lpImageFrame->axisX[x]) * lpImageInfo->width, (float)(lpImageFrame->axisY[y + 1] - lpImageFrame->axisY[y]) * lpImageInfo->height, SRCCOPY);
            }
        }
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
                    lpImageInfo->hMemDC, lpImageInfo->width * (frame % lpImageInfo->maxFrameX), lpImageInfo->height * (frame / lpImageInfo->maxFrameX), drawX, drawY, lpImageInfo->transColor);
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
                    lpImageInfo->hMemDC, lpImageInfo->width * (frame % lpImageInfo->maxFrameX), lpImageInfo->height * (frame / lpImageInfo->maxFrameX), drawX, drawY, SRCCOPY);
            }
        }
    }
}

void Image::Release()
{
    if (lpImageInfo)
    {
        DeleteObject(lpImageInfo->hBitmap);
        DeleteDC(lpImageInfo->hMemDC);
        delete lpImageInfo;
        lpImageInfo = nullptr;
    }

    if (lpImageFrame)
    {
        delete lpImageFrame;
        lpImageFrame = nullptr;
    }
}
