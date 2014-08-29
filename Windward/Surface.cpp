// Surface.cpp : class for manipulation and blitting of surfaces

#include "stdafx.h"

CSurface::CSurface(void)
{
	// CSurface construction
	m_surface = NULL;
	m_ColorKey = -1;
}

CSurface::~CSurface(void)
{
	// CSurface destruction
	Destroy();
}

BOOL CSurface::Create(LPDIRECTDRAW7 hDD, int width, int height, COLORREF dwColorKey)
{
	// create new surface
	DDSURFACEDESC2		ddsd;
	HRESULT				hRet;
	DDCOLORKEY          ddck;

	ZeroMemory( &ddsd, sizeof( ddsd ) );

	// set direct draw flags
    ddsd.dwSize = sizeof( ddsd );
	ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY;

	surfwidth = ddsd.dwWidth  = width;
    surfheight = ddsd.dwHeight = height;

	// create the surface
    hRet = hDD->CreateSurface(&ddsd, &m_surface, NULL );
    if( hRet != DD_OK )
	{
		// if user does not have enough video memory attempt to create the surface in memory instead
		if(hRet == DDERR_OUTOFVIDEOMEMORY)
		{
		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
		
		hRet = hDD->CreateSurface(&ddsd, &m_surface, NULL );
		}

		if( hRet != DD_OK )
		{
			return FALSE;
		}
	}

	// set color key
	if((int)dwColorKey != -1)
	{
		ddck.dwColorSpaceLowValue = dwColorKey;
		ddck.dwColorSpaceHighValue = 0;
		m_surface->SetColorKey(DDCKEY_SRCBLT, &ddck);
	}

	m_ColorKey = dwColorKey;

	return TRUE;
}

BOOL CSurface::LoadBitmap(HINSTANCE hInst, UINT nRes, int x, int y, int width, int height)
{
	// load bitmap to current surface

    HDC                     hdcImage;
    HDC                     hdc;
    BITMAP                  bm;
	HBITMAP					hbm;
    DDSURFACEDESC2          ddsd;
    HRESULT                 hr;

	hbm = (HBITMAP) LoadImage(hInst, MAKEINTRESOURCE(nRes), IMAGE_BITMAP, width, height, LR_CREATEDIBSECTION);
    if (hbm == NULL || m_surface == NULL)
        return FALSE;

    // make sure this surface is restored.
    m_surface->Restore();

    // select bitmap into a memoryDC so we can use it.
    hdcImage = CreateCompatibleDC(NULL);
    if (!hdcImage)
        return FALSE;

    SelectObject(hdcImage, hbm);

    // get size of the bitmap
    GetObject(hbm, sizeof(bm), &bm);

	if(width == 0)
		width = bm.bmWidth;
	
	if(height == 0)
		height = bm.bmHeight;
    
    // get size of surface.
    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_HEIGHT | DDSD_WIDTH;
    m_surface->GetSurfaceDesc(&ddsd);

    if ((hr = m_surface->GetDC(&hdc)) == DD_OK)
    {
        StretchBlt(hdc, 0, 0, ddsd.dwWidth, ddsd.dwHeight, hdcImage, x, y, width, height, SRCCOPY);
        m_surface->ReleaseDC(hdc);
    }
    DeleteDC(hdcImage);
	
	return TRUE;
}

BOOL CSurface::Draw(LPDIRECTDRAWSURFACE7 lpDest, int destx, int desty, int srcx, int srcy, int width, int height, bool mirror)
{
	// performs bit block transfer of surface to buffer

	RECT	src_rect, dest_rect;
	HRESULT	hRet;

	// source rectangle
	SetRect(&src_rect, srcx, srcy, width + srcx, height + srcy);

	// destination rectangle
	SetRect(&dest_rect, destx, desty, destx + width, desty + height);

	// blt fx struct for mirroring bitmap
	DDBLTFX ddbltfx;
	ZeroMemory(&ddbltfx,sizeof(ddbltfx));
	ddbltfx.dwSize = sizeof(ddbltfx);
	ddbltfx.dwDDFX = DDBLTFX_MIRRORLEFTRIGHT;

	while(1)
	{
		if(mirror)
		{
			// blit bitmap using mirror effect
			if((int)m_ColorKey < 0)
				hRet = lpDest->Blt(&dest_rect, m_surface, &src_rect, DDBLT_DDFX, &ddbltfx);
			else
				hRet = lpDest->Blt(&dest_rect, m_surface, &src_rect, DDBLT_DDFX | DDBLT_KEYSRC, &ddbltfx);
		}
		else
		{
			// blit bitmap fast with no mirror effect
			if((int)m_ColorKey < 0)
				hRet = lpDest->BltFast(destx, desty, m_surface, &src_rect, DDBLTFAST_WAIT | DDBLTFAST_NOCOLORKEY);
			else
				hRet = lpDest->BltFast(destx, desty, m_surface, &src_rect, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);
		}

		if(hRet == DD_OK)
			break;

		if(hRet == DDERR_SURFACELOST)
		{
			Restore();
		}
		else
		{
			if(hRet != DDERR_WASSTILLDRAWING)
				return FALSE;
		}
	}

	return TRUE;
}

void CSurface::Destroy()
{
	// manual surface release
	if(m_surface != NULL)
	{
		m_surface->Release();
		m_surface = NULL;
	}
}

LPDIRECTDRAWSURFACE7 CSurface::GetSurface()
{
	return m_surface;
}

void CSurface::Restore()
{
	m_surface->Restore();
}