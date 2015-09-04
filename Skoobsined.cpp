#define WIN32_LEAN_AND_MEAN
#define STRICT

//----------------------------------------------------------------------------------------------

#include <ddraw.h>
#include <mmsystem.h>

#include "resource.h"
#include "misc.h"

//----------------------------------------------------------------------------------------------

#define PICS 8

#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

//----------------------------------------------------------------------------------------------

IDirectDraw        *g_pDD     = NULL;
IDirectDrawSurface *g_pddsPri = NULL;
IDirectDrawSurface *g_pddsBck = NULL;
IDirectDrawSurface *g_pddsBMP = NULL;
IDirectDrawSurface *g_pddsBlr = NULL;

char                g_szAppName[] = "Skoobsined";
bool                g_bActive     = TRUE;
DWORD               g_dwLastTick;
POINT               g_ptMouse;

//----------------------------------------------------------------------------------------------

char *g_pTxt =
 "зад божества // skoobsined // http://mumidol.ru/~skoobsined // [денис яцyтко / елена злобина / " \
 "геннадий котов / маpгаpита воpонова / дмитpий воpсин / андpей козлов / " \
 "андpей колотилин / лев пиpогов / денис жypлаков / глеб гоpбатов / lolita lenin / михаил самойленко] -- чеpновики | глоссолалия" \
 " | тело | отходы | концы | злые сны -- ";

char *g_pMrz =
 "За спиной своей бога-коpшyна\x00" \
 "Ощyтил. Запyскает лапы\x00" \
 "Он цаpапающей пpигоpшнею\x00" \
 "Мне под кожy и валит на пол.\x00" \
 "У меня жена светлолицая,\x00" \
 "У меня коня нет в помине,\x00" \
 "Hо во снах я дышy столицею\x00" \
 "И мечтаю о кокаине.\x00" \
 "Мне мyжчины звонят поpочные,\x00" \
 "Аpоматные злые жены\x00" \
 "Ранят дyшy мою непpочнyю,\x00" \
 "Ловят фаллос мой напpяженный,\x00" \
 "Только выбpал я пyть непpаведный,\x00" \
 "Светят мне yбежденья звезды,\x00" \
 "Богy-коpшyнy стало завидно,\x00" \
 "Я ногам не дозволю pоздыхy.\x00" \
 "Hо с женой своей ясноликою\x00" \
 "Двинyсь вдаль -- к гоpодам стеклянным,\x00" \
 "Пyсть часы, неyклонно тикая,\x00" \
 "Пpиближают нас к их полянам,\x00" \
 "Где, одеждой пpостой одетые,\x00" \
 "Безволосые ходят люди,\x00" \
 "Пpивечая yлыбкой светлою\x00" \
 "И дыша свободною гpyдью,\x00" \
 "Где забыты слова \"полиция\",\x00" \
 "\"Бог\", \"моpаль\", \"алкоголь\", \"паpламент\",\x00" \
 "Где щебечет весна синицами,\x00" \
 "Где зима меня не достанет.\x00" \
 "Hо обломки когтей божественных\x00" \
 "Загноятся под нежной кожей --\x00" \
 "Возжелаю я всех, кто женственен,\x00" \
 "Пьяным быть возжелаю тоже...\x00" \
 "И поэтомy нам нельзя тyда --\x00" \
 "Пyсть идyт тyда те, кто следом.\x00" \
 "Hо чтоб в миp весны не пpишла беда,\x00" \
 "Одеpжy я однy победy:\x00" \
 "Повеpнyсь лицом к богy-коpшyнy\x00" \
 "И всажy емy член под pебpа,\x00" \
 "Hапою кого, обнимy женy\x00" \
 "И подохнy, святой и добpый.\x00";

//----------------------------------------------------------------------------------------------

int  APIENTRY    WinMain( HINSTANCE hInst, HINSTANCE hPInst, LPSTR lpCmdLine, int nCmdShow );
long WINAPI      WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
INT_PTR CALLBACK CfgDlgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

HRESULT CrtWnd( HINSTANCE hInst, int nCmdShow, HWND *phWnd );
HRESULT InitDD( HWND hWnd );
void    FreeDD( HWND hWnd );
HRESULT CrtPal();
HRESULT InitBMP();
HRESULT RestoreSurfaces();
HRESULT ClrSurface( IDirectDrawSurface *pdds );
HRESULT DisplayFrame();

char *PrintS( char *pszTxt );
char *PrintCS( char *pszTxt );

//----------------------------------------------------------------------------------------------

HRESULT DisplayFrame()
{
    HRESULT       hr;
    DDSURFACEDESC ddsd;
    DWORD         dwCurrTick = timeGetTime();
    DWORD         dwTickDiff = dwCurrTick - g_dwLastTick;
    RECT          rc   = { 0, 0, 256, 256 };
    int           i;
    char         *pText;

    static int    nPic   = -1;
    static int    xLine  = 640;
    static DWORD  dwCntr = 0;
    static int    xSpr, ySpr;
    static int    xSprD, ySprD;
    static int    xLink = rand() % 475;
    static int    yLink = rand() % 460;

    if( dwTickDiff < 100 )
    {
        return S_OK;

    } // if( dwTickDiff < 100 )

    ZeroMemory( &ddsd, sizeof( ddsd ) );
    ddsd.dwSize = sizeof( ddsd );

    if( ( xLine -= 5 ) < -2245 )
    {
        xLine = 0;

    } // if( ( xLine -= 5 ) < -2245 )

    if( !( dwCntr % 5 ) )
    {
        if( ++nPic >= PICS )
        {
            nPic = 0;

        } // if( nPic >= PICS )

        xSpr = 2 + rand() % 380;
        ySpr = 2 + rand() % 220;

        xSprD = 5 - rand() % 11;
        ySprD = 5 - rand() % 11;

    } // if( !( dwCntr % 3 ) )

    rc.top    = nPic * 256;
    rc.bottom = ( nPic + 1 ) * 256;

    if( g_pddsBck->Lock( NULL, &ddsd, DDLOCK_WAIT, NULL ) != DDERR_SURFACELOST )
    {
        g_pScr   = ddsd.lpSurface;
        g_lPitch = ddsd.lPitch;

        if( g_pddsBlr->Lock( NULL, &ddsd, DDLOCK_WAIT, NULL ) != DDERR_SURFACELOST )
        {
            Blur( ddsd.lpSurface, ddsd.lPitch );

            if( ( g_pddsBlr->Unlock( NULL ) == DDERR_SURFACELOST )||
                ( g_pddsBck->Unlock( NULL ) == DDERR_SURFACELOST ) )
            {
                if( FAILED( hr = RestoreSurfaces() ) )
                {
                    return hr;

                } // if( FAILED( hr = RestoreSurfaces() ) )

            } // if( ( g_pddsBlr->Unlock( NULL ) == DDERR_SURFACELOST )||( g_pddsBck->Unlock( NULL ) == DDERR_SURFACELOST ) )

        } // if( g_pddsBlr->Lock( NULL, &ddsd, DDLOCK_WAIT, NULL ) != DDERR_SURFACELOST )
        else
        {
            g_pddsBck->Unlock( NULL );

            if( FAILED( hr = RestoreSurfaces() ) )
            {
                return hr;

            } // if( FAILED( hr = RestoreSurfaces() ) )

        } // if( g_pddsBlr->Lock( NULL, &ddsd, DDLOCK_WAIT, NULL ) == DDERR_SURFACELOST )

    } // if( g_pddsBck->Lock( NULL, &ddsd, DDLOCK_WAIT, NULL ) != DDERR_SURFACELOST )
    else
    {
        if( FAILED( hr = RestoreSurfaces() ) )
        {
            return hr;

        } // if( FAILED( hr = RestoreSurfaces() ) )

    } // if( g_pddsBck->Lock( NULL, &ddsd, DDLOCK_WAIT, NULL ) == DDERR_SURFACELOST )

    if( dwCntr > 300 )
    {
        if( g_pddsBck->BltFast( xSpr += xSprD, ySpr += ySprD, g_pddsBMP, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY ) == DDERR_SURFACELOST )
        {
            if( FAILED( hr = RestoreSurfaces() ) )
            {
                return hr;

            } // if( FAILED( hr = RestoreSurfaces() ) )

        } // if( g_pddsBck->BltFast( xSpr += xSprD, ySpr += ySprD, g_pddsBMP, &rc, DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY ) == DDERR_SURFACELOST )

    } // if( dwCntr > 300 )

    if( g_pddsBlr->BltFast( 0, 0, g_pddsBck, NULL, DDBLTFAST_WAIT ) == DDERR_SURFACELOST )
    {
        if( FAILED( hr = RestoreSurfaces() ) )
        {
            return hr;

        } // if( FAILED( hr = RestoreSurfaces() ) )

    } // if( g_pddsBlr->BltFast( 0, 0, g_pddsBck, NULL, DDBLTFAST_WAIT ) == DDERR_SURFACELOST )

    if( g_pddsBck->Lock( NULL, &ddsd, DDLOCK_WAIT, NULL ) != DDERR_SURFACELOST )
    {
        g_pScr   = ddsd.lpSurface;
        g_lPitch = ddsd.lPitch;

        g_dwCutLf = 2;
        g_dwCutRt = 638;
        g_dwPrnY  = 460;
        g_dwPrnX  = xLine;
        PrintS( g_pTxt );
        PrintS( g_pTxt );

        if( ( dwCntr >= 300 )&&( ( dwCntr % 100 ) >= 0 )&&( ( dwCntr % 100 ) <= 30 ) )
        {
            g_dwPrnX = xLink;
            g_dwPrnY = yLink;
            PrintS( "http://mumidol.ru/~skoobsined" );

        } // if( ( ( dwCntr % 100 ) >= 0 )&&( ( dwCntr % 100 ) <= 30 ) )
        else
        {
            xLink = 20 + rand() % 400;
            yLink = 20 + rand() % 400;

        } // if( !( ( ( dwCntr % 100 ) >= 0 )&&( ( dwCntr % 100 ) <= 30 ) ) )

        if( dwCntr <= 300 )
        {
            pText = g_pMrz;

            g_dwCutLf = 0;
            g_dwCutRt = 639;
            g_dwPrnY  = 0x18;
            PrintCS( "Денис Яцутко\n" );
            PrintCS( "Мороз\n" );
            PrintCS( "----------------------------------------------------------" );

            g_dwPrnY  = 0x58;
            for( i = 0; i < 20; i++ )
            {
                g_dwPrnX  = 36;
                pText     = PrintS( pText );
                g_dwPrnY += 0x10;

            } // for( i = 0; i < 20; i++ )

            g_dwPrnY  = 0x58;
            for( ; i < 40; i++ )
            {
                g_dwPrnX = 356;
                pText = PrintS( pText );
                g_dwPrnY += 0x10;

            } // for( ; i < 40; i++ )

        } // if( dwCntr <= 300 )

        if( g_pddsBck->Unlock( NULL ) == DDERR_SURFACELOST )
        {
            if( FAILED( hr = RestoreSurfaces() ) )
            {
                return hr;

            } // if( FAILED( hr = RestoreSurfaces() ) )

        } // if( g_pddsBck->Unlock( NULL ) == DDERR_SURFACELOST )

    } // if( g_pddsBck->Lock( NULL, &ddsd, DDLOCK_WAIT, NULL ) != DDERR_SURFACELOST )
    else
    {
        if( FAILED( hr = RestoreSurfaces() ) )
        {
            return hr;

        } // if( FAILED( hr = RestoreSurfaces() ) )

    } // if( g_pddsBck->Lock( NULL, &ddsd, DDLOCK_WAIT, NULL ) == DDERR_SURFACELOST)

    if( g_pddsPri->Flip( NULL, DDFLIP_WAIT ) == DDERR_SURFACELOST )
    {
        if( FAILED( hr = RestoreSurfaces() ) )
        {
            return hr;

        } // if( FAILED( hr = RestoreSurfaces() ) )

    } //  if( g_pddsPri->Flip( NULL, DDFLIP_WAIT ) == DDERR_SURFACELOST )

    g_dwLastTick = dwCurrTick;
    dwCntr++;

    return S_OK;

} // DisplayFrame

//----------------------------------------------------------------------------------------------

char *PrintCS( char *pszTxt )
{
    g_dwPrnX = g_dwCutLf + ( ( g_dwCutRt - g_dwCutLf  - CalcLen( pszTxt ) ) / 2 );
    return PrintS( pszTxt );

} // PrintCS

//----------------------------------------------------------------------------------------------

char *PrintS( char *pszTxt )
{
    char a;

    while( TRUE )
    {
        switch( a = *pszTxt++ )
        {
        case 0x00:

            return pszTxt;

        case 0x0d:

            g_dwPrnX = g_dwCutLf;
            break;

        case 0x0a:

            g_dwPrnY += 16;
            break;

        default:

            PrintL( a );

        } // switch( a = *pszTxt )

    } // while( TRUE )

} // PrintS

//----------------------------------------------------------------------------------------------

HRESULT ClrSurface( IDirectDrawSurface *pdds )
{
    DDBLTFX ddbltfx;

    ZeroMemory( &ddbltfx, sizeof( ddbltfx ) );

    ddbltfx.dwSize      = sizeof( ddbltfx );
    ddbltfx.dwFillColor = 0;

    return pdds->Blt( NULL, NULL, NULL, DDBLT_COLORFILL, &ddbltfx );

} // ClrSurface

//----------------------------------------------------------------------------------------------

HRESULT RestoreSurfaces()
{
    HRESULT hr;

    if( FAILED( hr = g_pddsPri->Restore() ) )
    {
        return hr;

    } // if( FAILED( hr = g_pddsPri->Restore() ) )

    if( FAILED( hr = g_pddsBck->Restore() ) )
    {
        return hr;

    } // if( FAILED( hr = g_pddsBck->Restore() ) )

    if( FAILED( hr = g_pddsBMP->Restore() ) )
    {
        return hr;

    } // if( FAILED( hr = g_pddsBMP->Restore() ) )

    if( FAILED( hr = ClrSurface( g_pddsPri ) ) )
    {
        return hr;

    } // if( FAILED( hr = ClrSurface( g_pddsPri ) ) )

    if( FAILED( hr = ClrSurface( g_pddsBck ) ) )
    {
        return hr;

    } // if( FAILED( hr = ClrSurface( g_pddsBck ) ) )

    if( FAILED( hr = ClrSurface( g_pddsBlr ) ) )
    {
        return hr;

    } // if( FAILED( hr = ClrSurface( g_pddsBlr ) ) )

    return InitBMP();

} // RestoreSurfaces

//----------------------------------------------------------------------------------------------

HRESULT InitBMP()
{
    HBITMAP            hBMP;
    HDC                hdcImage;
    HDC                hDC;
    HRESULT            hr;
    int                i, nPic;

    hBMP = ( HBITMAP )LoadImage(

        GetModuleHandle( NULL ),
        MAKEINTRESOURCE( IDB_SPRITES ),
        IMAGE_BITMAP,
        0,
        0,
        LR_CREATEDIBSECTION

    ); // LoadImage

    if( !( hdcImage = CreateCompatibleDC( NULL ) ) )
    {
        return E_FAIL;

    } // if( !( hdcImage = CreateCompatibleDC( NULL ) ) )

    SelectObject( hdcImage, hBMP );

    if( FAILED( hr = g_pddsBMP->GetDC( &hDC ) ) )
    {
        return hr;

    } // if( FAILED( hr = m_pddsBMP->GetDC( &hDC ) ) )

    nPic = rand() % 24;

    for( i = 0; i < PICS; i++ )
    {
        if( !StretchBlt(

            hDC,
            0,
            256 * i,
            256,
            256,
            hdcImage,
            0,
            128 * ( nPic ),
            128,
            128,
            SRCCOPY

        ) ) // StretchBlt
        {
            DeleteDC( hdcImage );
            return E_FAIL;

        } // if( !StretchBlt() )

        if( ++nPic >= 24 )
        {
            nPic = 0;

        } // if( ++nPic >= 24 )

    }// for( i = 0; i < PICS; i++ )

    DeleteDC( hdcImage );

    return g_pddsBMP->ReleaseDC( hDC );

} // DrawBMP

//----------------------------------------------------------------------------------------------

HRESULT CrtPal()
{
    IDirectDrawPalette *pddpal = NULL;
    BITMAPINFOHEADER   *pbi    = NULL;
    RGBQUAD            *pRGB   = NULL;
    PALETTEENTRY        aPalette[256];
    DWORD               dwColors;
    DWORD               iColor;
    HRESULT             hr;

    if( !( pbi = ( LPBITMAPINFOHEADER )LockResource( LoadResource( NULL, FindResource( NULL, MAKEINTRESOURCE( IDB_SPRITES ), RT_BITMAP ) ) ) ) )
    {
        return E_FAIL;

    } // if( !( pbi = ( LPBITMAPINFOHEADER )LockResource( LoadResource( NULL, FindResource( NULL, MAKEINTRESOURCE( IDB_SPRITES ), RT_BITMAP ) ) ) ) )

    pRGB = ( RGBQUAD * ) ( ( BYTE * )pbi + pbi->biSize );

    if( pbi->biClrUsed == 0 )
    {
        dwColors = 1 << pbi->biBitCount;

    } // if( pbi->biClrUsed == 0 )
    else
    {
        dwColors = pbi->biClrUsed;

    } // if( pbi->biClrUsed != 0 )

    for( iColor = 0; iColor < dwColors; iColor++ )
    {
        aPalette[iColor].peRed   = pRGB[iColor].rgbRed;
        aPalette[iColor].peGreen = pRGB[iColor].rgbGreen;
        aPalette[iColor].peBlue  = pRGB[iColor].rgbBlue;
        aPalette[iColor].peFlags = 0;

    } // for( iColor = 0; iColor < dwColors; iColor++ )

    if( FAILED( hr = g_pDD->CreatePalette( DDPCAPS_8BIT, aPalette, &pddpal, NULL ) ) )
    {
        return hr;

    } // if( FAILED( hr = g_pDD->CreatePalette( DDPCAPS_8BIT, aPalette, &pddpal, NULL ) ) )

    hr = g_pddsPri->SetPalette( pddpal );

    pddpal->Release();

    return hr;

} // CrtPal

//----------------------------------------------------------------------------------------------

INT_PTR CALLBACK CfgDlgProc( HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch( msg )
    {
    case WM_INITDIALOG:

        return TRUE;

    case WM_CLOSE:

        EndDialog( hDlg, IDCANCEL );
        break;

    case WM_COMMAND:

        switch( HIWORD(wParam) )
        {
            case BN_CLICKED:

                switch( LOWORD(wParam) )
                {
                    case IDOK:

                        EndDialog( hDlg, IDOK );
                        break;

                } // switch( LOWORD(wParam) )

                break;

        } // switch( HIWORD(wParam) )

        break;

    } // switch( msg )

    return FALSE;

} // long WINAPI CfgDlgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )

//----------------------------------------------------------------------------------------------

long WINAPI WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    POINT ptCurr;

    switch( msg )
    {
    case WM_SIZE:

        if( ( wParam == SIZE_MAXHIDE )||( wParam == SIZE_MINIMIZED ) )
        {
            g_bActive = FALSE;

        } // if( (SIZE_MAXHIDE == wParam )||( SIZE_MINIMIZED == wParam ) )
        else
        {
            g_bActive = TRUE;

        } // if( !( (SIZE_MAXHIDE == wParam )||( SIZE_MINIMIZED == wParam ) ) )

        break;

    case WM_MOUSEMOVE:

        GetCursorPos( &ptCurr );
        if( ( ptCurr.x == g_ptMouse.x )||( ptCurr.x == g_ptMouse.x ) )
        {
            break;

        } // if( ( ptCurr.x == g_ptMouse.x )||( ptCurr.x == g_ptMouse.x ) )

    case WM_LBUTTONDOWN:
    case WM_MBUTTONDOWN:
    case WM_RBUTTONDOWN:
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:

        PostMessage( hWnd, WM_CLOSE, 0, 0 );
        return 0;

    case WM_SETCURSOR:

        SetCursor( NULL );
        return TRUE;

    case WM_DESTROY:

        FreeDD( hWnd );
        PostQuitMessage( 0 );
        return 0;

    } // switch( msg )

    return DefWindowProc( hWnd, msg, wParam, lParam );

} // WndProc

//----------------------------------------------------------------------------------------------

void FreeDD( HWND hWnd )
{
    if( g_pDD )
    {
        SAFE_RELEASE( g_pddsBck );
        SAFE_RELEASE( g_pddsPri );
        SAFE_RELEASE( g_pddsBMP );
        SAFE_RELEASE( g_pddsBlr );

        g_pDD->RestoreDisplayMode();
        g_pDD->SetCooperativeLevel( hWnd, DDSCL_NORMAL );
        g_pDD->Release();

        g_pDD = NULL;

    } // if( g_pDD )

} // FreeDD

//----------------------------------------------------------------------------------------------

HRESULT InitDD( HWND hWnd )
{
    HRESULT       hr;
    DDSURFACEDESC ddsd;
    DDSCAPS       ddscaps;
    DDCOLORKEY    ddck;

    if( FAILED( hr = DirectDrawCreate( NULL, &g_pDD, NULL ) ) )
    {
        return hr;

    } // if( FAILED( hr = DirectDrawCreate( NULL, &g_pDD, NULL ) ) )

    if( FAILED( hr = g_pDD->SetCooperativeLevel( hWnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN ) ) )
    {
        return hr;

    } // if( FAILED( hr = g_pDD->SetCooperativeLevel( hWnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN ) ) )

    if( FAILED( hr = g_pDD->SetDisplayMode( 640, 480, 8 ) ) )
    {
        return hr;

    } // if( FAILED( hr = g_pDD->SetDisplayMode( 640, 480, 8 ) ) )

    ZeroMemory( &ddsd, sizeof( ddsd ) );

    ddsd.dwSize            = sizeof( ddsd );
    ddsd.dwFlags           = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
    ddsd.ddsCaps.dwCaps    = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
    ddsd.dwBackBufferCount = 1;

    if( FAILED( hr = g_pDD->CreateSurface( &ddsd, &g_pddsPri, NULL ) ) )
    {
        return hr;

    } // if( FAILED( hr = g_pDD->CreateSurface( &ddsd, &g_pddsPri, NULL ) ) )

    ZeroMemory( &ddscaps, sizeof( ddscaps ) );
    ddscaps.dwCaps = DDSCAPS_BACKBUFFER;

    if( FAILED( hr = g_pddsPri->GetAttachedSurface( &ddscaps, &g_pddsBck ) ) )
    {
        return hr;

    } // if( FAILED( hr = g_pddsPri->GetAttachedSurface( &ddscaps, &g_pddsBck ) ) )

    ZeroMemory( &ddsd, sizeof( ddsd ) );

    ddsd.dwSize            = sizeof( ddsd );
    ddsd.dwFlags           = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
    ddsd.ddsCaps.dwCaps    = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
    ddsd.dwWidth           = 256;
    ddsd.dwHeight          = 256 * PICS;

    if( FAILED( hr = g_pDD->CreateSurface( &ddsd, &g_pddsBMP, NULL ) ) )
    {
        return hr;

    } // if( FAILED( hr = g_pDD->CreateSurface( &ddsd, &g_pddsBMP, NULL ) )

    ddsd.dwWidth           = 640;
    ddsd.dwHeight          = 480;

    if( FAILED( hr = g_pDD->CreateSurface( &ddsd, &g_pddsBlr, NULL ) ) )
    {
        return hr;

    } // if( FAILED( hr = g_pDD->CreateSurface( &ddsd, &g_pddsBlr, NULL ) )

    if( FAILED( hr = CrtPal() ) )
    {
        return hr;

    } // if( FAILED( hr = CrtPal() ) )

    if( FAILED( hr = ClrSurface( g_pddsPri ) ) )
    {
        return hr;

    } // if( FAILED( hr = ClrSurface( g_pddsPri ) ) )

    if( FAILED( hr = ClrSurface( g_pddsBck ) ) )
    {
        return hr;

    } // if( FAILED( hr = ClrSurface( g_pddsBck ) ) )

    if( FAILED( hr = ClrSurface( g_pddsBlr ) ) )
    {
        return hr;

    } // if( FAILED( hr = ClrSurface( g_pddsBlr ) ) )

    if( FAILED( hr = InitBMP() ) )
    {
        return hr;

    } // if( FAILED( hr = InitBMP() ) )

    ddck.dwColorSpaceLowValue  = 0;
    ddck.dwColorSpaceHighValue = 0;

    return g_pddsBMP->SetColorKey( DDCKEY_SRCBLT, &ddck );

} // InitDD

//----------------------------------------------------------------------------------------------

HRESULT CrtWnd( HINSTANCE hInst, int nCmdShow, HWND *phWnd )
{
    HWND     hWnd;
    WNDCLASS wc;

    wc.lpszClassName = g_szAppName;
    wc.lpfnWndProc   = WndProc;
    wc.style         = CS_VREDRAW | CS_HREDRAW;
    wc.hInstance     = hInst;
    wc.hIcon         = LoadIcon( NULL, IDI_APPLICATION );
    wc.hCursor       = LoadCursor( NULL, IDC_ARROW );
    wc.hbrBackground = ( HBRUSH )GetStockObject( BLACK_BRUSH );
    wc.lpszMenuName  = NULL;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;

    if( !RegisterClass( &wc ) )
    {
        return E_FAIL;

    } // if( !RegisterClass( &wc ) )

    hWnd = CreateWindowEx(

        0,
        g_szAppName,
        g_szAppName,
        WS_POPUP,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL,
        NULL,
        hInst,
        NULL

    ); // CreateWindowEx

    if( !hWnd )
    {
        return E_FAIL;

    } // if( !hWnd )

    ShowWindow( hWnd, nCmdShow );
    UpdateWindow( hWnd );

    *phWnd = hWnd;

    return S_OK;

} // CrtWnd

//----------------------------------------------------------------------------------------------

int APIENTRY WinMain( HINSTANCE hInst, HINSTANCE hPInst, LPSTR lpCmdLine, int nCmdShow )
{
    MSG     msg;
    HWND    hWnd;
    HRESULT hr;

    char   *pSwitch = strtok( lpCmdLine, "/" );

    if( FindWindow( g_szAppName, g_szAppName ) )
    {
        return FALSE;

    } // if( FindWindow( g_szAppName. g_szAppName ) )

    if( ( pSwitch == NULL )||( *pSwitch == 'c' )||( *pSwitch == 'C' ) )
    {
        DialogBox( hInst, MAKEINTRESOURCE( IDD_CFGDLG ), NULL, ( DLGPROC )CfgDlgProc );
        return FALSE;

    } // if( ( pSwitch == NULL )||( *pSwitch == 'c' )||( *pSwitch == 'C' ) )
    else
    {
        switch( *pSwitch )
        {
            case 'p':
            case 'P':

                return TRUE;

        } // switch( *pSwitch )

    } // if( !( ( pSwitch == NULL )||( *pSwitch == 'c' )||( *pSwitch == 'C' ) ) )

    srand( g_dwLastTick = timeGetTime() );

    if( FAILED( CrtWnd( hInst, nCmdShow, &hWnd ) ) )
    {
        return FALSE;

    } // if( FAILED( CrtWnd( hInst, nCmdShow, &hWnd ) ) )

    if( FAILED( hr = InitDD( hWnd ) ) )
    {
        FreeDD( hWnd );

        if( hr != DDERR_INVALIDMODE )
        {
            MessageBox(

                hWnd,
                "DirectDraw init failed.",
                g_szAppName,
                MB_ICONERROR | MB_OK

            ); // MessageBox

        } // if( hr != DDERR_INVALIDMODE )

        return FALSE;

    } // if( FAILED( InitDD( hWnd ) ) )

    GetCursorPos( &g_ptMouse );

    while( TRUE )
    {
        if( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) )
        {
            if( !GetMessage(&msg, NULL, 0, 0 ) )
            {
                return msg.wParam;

            } // if( !GetMessage(&msg, NULL, 0, 0 ) )

            TranslateMessage( &msg );
            DispatchMessage( &msg );

        } // if( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) )
        else
        {
            if( g_bActive )
            {
                if( FAILED( DisplayFrame() ) )
                {
                    FreeDD( hWnd );

                    MessageBox(

                        NULL,
                        "Displaying the next frame failed.",
                        g_szAppName,
                        MB_ICONERROR | MB_OK

                    ); // MessageBox

                    return FALSE;

                } // if( FAILED( DisplayFrame() ) )

            } // if( g_bActive )
            else
            {
                WaitMessage();

                g_dwLastTick = timeGetTime();

            } // if( !g_bActive )

        } // if( !PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) )

    } // while( TRUE )

} // WinMain

//----------------------------------------------------------------------------------------------
