extern "C" void  PrintL( char );
extern "C" DWORD CalcLen( char * );
extern "C" void  Blur( void *, long );

//----------------------------------------------------------------------------------------------

extern "C" void  *g_pScr;
extern "C" long   g_lPitch;
extern "C" DWORD  g_dwPrnX;
extern "C" DWORD  g_dwPrnY;
extern "C" BYTE   g_ucPrnInk;
extern "C" BYTE   g_ucPrnPap;
extern "C" DWORD  g_dwCutLf;
extern "C" DWORD  g_dwCutRt;
