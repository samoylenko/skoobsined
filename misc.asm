bits 32

;-----------------------------------------------------------------------------------------------

global _g_pScr
global _g_lPitch
global _g_dwPrnX
global _g_dwPrnY
global _g_ucPrnC
global _g_dwCutLf
global _g_dwCutRt

global _PrintL
global _CalcLen
global _Blur

;-----------------------------------------------------------------------------------------------

section .data

;-----------------------------------------------------------------------------------------------

_g_pScr     dd      0
_g_lPitch   dd      0
_g_dwPrnX   dd      0
_g_dwPrnY   dd	    0
_g_ucPrnC   db      0xff
_g_ucPrnM   db      0
_g_dwCutLf  dd      0
_g_dwCutRt  dd      639

mask        times 16 dw 0
font        incbin  "res\skoobwin.fnt"

;-----------------------------------------------------------------------------------------------

section .code

;-----------------------------------------------------------------------------------------------

_PrintL:                                            ; void PrintL( char )
            push    ebp
            mov	    ebp, esp
            push    esi
            push    edi
            push    ebx
            push    ecx
            push    edx

            mov     edi, mask
            mov     ecx, 8
            xor     eax, eax
            rep     stosd

            mov     al, [ebp + 8]
            shl     eax, 5
            add     eax, font
            push    eax
            mov     esi, eax
            add     esi, 2
            mov     edi, mask
            mov     ecx, 14
.genmask:
            lodsw
            xchg    al, ah
            or      [edi + 0], ax
            or      [edi + 2], ax
            or      [edi + 4], ax
            shl     ax, 1
            or      [edi + 0], ax
            or      [edi + 2], ax
            or      [edi + 4], ax
            shr     ax, 2
            or      [edi + 0], ax
            or      [edi + 2], ax
            or      [edi + 4], ax
            add     edi, 2
            loop    .genmask

            mov     esi, mask
            mov     eax, [_g_lPitch]
            mul     dword [_g_dwPrnY]
            add     eax,  [_g_dwPrnX]
            add     eax,  [_g_pScr]
            push    eax
            mov     edi, eax
            mov     dl, [_g_ucPrnM]
            mov     ecx, 16
.putmask:
            lodsw
            mov     ebx, [_g_dwPrnX]
            push    ecx
            mov     ecx, 16
.pm1:   
            rcl     ax, 1
            jnc     .pm2
            cmp     ebx, [_g_dwCutLf]
            jl      .pm2
            cmp     ebx, [_g_dwCutRt]
            jg      .pm2
            mov     [edi], dl
.pm2:
            inc     ebx
            inc     edi
            loop    .pm1
            add     edi, [_g_lPitch]
            sub     edi, 16
            pop     ecx
            loop    .putmask

            pop     edi
            pop     esi
            mov     dl, [_g_ucPrnC]
            mov     ecx, 16
.putletter:
            lodsw
            xchg    al, ah
            mov     ebx, [_g_dwPrnX]
            push    ecx
            mov     ecx, 16
.pl1:   
            rcl     ax, 1
            jnc     .pl2
            cmp     ebx, [_g_dwCutLf]
            jl      .pl2
            cmp     ebx, [_g_dwCutRt]
            jg      .pl2
            mov     [edi], dl
.pl2:
            inc     ebx
            inc     edi
            loop    .pl1
            add     edi, [_g_lPitch]
            sub     edi, 16
            pop     ecx
            loop    .putletter

            xor     eax, eax
            mov     al, [ebp + 8]
            mov     al, [font + 0x2000 + eax]
            add     [_g_dwPrnX], eax

            pop     edx
            pop     ecx
            pop     ebx
            pop     edi
            pop     esi
            pop     ebp
            ret

;-----------------------------------------------------------------------------------------------

_CalcLen:                                           ; DWORD CalcLen( char * );
            push    ebp
            mov     ebp, esp
            push    esi
            push    ebx
            push    ecx

            mov     esi, [ebp + 8]
            xor     ebx, ebx
            xor     eax, eax
            xor     ecx, ecx
.cl1:
            lodsb
            and     al, al
            jz      .cl2
            mov     cl, [font + 0x2000 + eax]
            add     ebx, ecx
            jmp     .cl1
.cl2:
            mov     eax, ebx

            pop     ecx
            pop     ebx
            pop     esi
            pop     ebp
            ret

;-----------------------------------------------------------------------------------------------

_Blur:                                              ; void Blur( void *, long )
            push    ebp
            mov	    ebp, esp
            push    esi
            push    edi
            push    ebx
            push    ecx
            push    edx

            mov     edi, [_g_pScr]
            mov     esi, [ebp + 0x08]

            push    edi           
            xor     eax, eax
            mov     ecx, 160
            rep     stosd
            pop     edi

            add     edi, [_g_lPitch]
            add     esi, [ebp + 0x0c]

            mov     ecx, 478
.blr1:
            push    ecx
            push    edi
            push    esi
            mov     byte [edi], 0
            inc     edi
            inc     esi
            mov     ecx, 638
.blr2:
            xor     eax, eax
            mov     bl, [esi - 641]
            add     eax, ebx
            mov     bl, [esi - 640]
            add     eax, ebx
            mov     bl, [esi - 639]
            add     eax, ebx
            mov     bl, [esi + 641]
            add     eax, ebx
            mov     bl, [esi + 640]
            add     eax, ebx
            mov     bl, [esi + 639]
            add     eax, ebx
            mov     bl, [esi - 1]
            add     eax, ebx
            mov     bl, [esi + 1]
            add     eax, ebx
            mov     bl, [esi]
            add     eax, ebx
            mov     bl, 9
            div     bl
            neg     ah
            and     al, ah
            stosb
            inc     esi
            loop    .blr2
            pop     esi
            mov     byte [edi], 0
            pop     edi
            add     esi, [ebp + 0x0c]
            add     edi, [_g_lPitch]
            pop     ecx
            loop    .blr1

            xor     eax, eax
            mov     ecx, 160
            rep     stosd
            
            pop     edx
            pop     ecx
            pop     ebx
            pop     edi
            pop     esi
            pop     ebp
            ret

;-----------------------------------------------------------------------------------------------
