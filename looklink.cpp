///////////////////////////////////////////////////////////////////////////////
///
/// Written 2009-2010, Oliver Schneider (assarbad.net) - PUBLIC DOMAIN
///
/// Original filename: looklink.cpp
/// Project          : looklink
/// Date of creation : 2009-02-03
/// Author(s)        : Oliver Schneider
///
/// Purpose          : Program to investigate all kinds of reparse points on
///                    Windows 2000 and later.
///
///////////////////////////////////////////////////////////////////////////////

// 
#include <cstdio>
#ifdef _DEBUG
#include <crtdbg.h>
#endif // _DEBUG
#include <tchar.h>
#include "looklink.h"

BOOL isCommandLineSwitch(_TCHAR *arg, const _TCHAR *switchName = NULL)
{
    if(arg && wcslen(arg) > 2 && arg[0] == L'-' && arg[1] == L'-')
    {
        if(!switchName)
        {
            return TRUE;
        }
        if(0 == wcscmp(&arg[2], switchName))
        {
            return TRUE;
        }
    }
    return FALSE;
}

int __cdecl _tmain(int argc, _TCHAR *argv[])
{
#ifdef _DEBUG
    _CrtSetDbgFlag(_CRTDBG_CHECK_ALWAYS_DF | _CRTDBG_ALLOC_MEM_DF);
#endif // _DEBUG
    if(2 <= argc)
    {
        bool show_logo = true, be_verbose = false;
        int fname_arg = 1;
        while(isCommandLineSwitch(argv[fname_arg]))
        {
            if(isCommandLineSwitch(argv[fname_arg], L"nologo"))
                show_logo = false;
            else
                if(isCommandLineSwitch(argv[fname_arg], L"verbose"))
                    be_verbose = true;
            fname_arg++;
        }

        if(show_logo)
        {
            _tprintf(_T("looklink %1d.%1d/%03d - written %hs, Oliver Schneider (assarbad.net)\n\n"), PRD_MAJVER, PRD_MINVER, FILE_BUILD, ANSISTRING(EXE_YEAR));
        }

        CReparsePoint rp(argv[fname_arg]);
        if(ERROR_SUCCESS != rp.LastError())
        {
            _ftprintf(stderr, _T("ERROR: Win32 error code %d. Probably file (2) or path (3) not found or access issue (5)?!\n"), rp.LastError());
            return 3;
        }
        if(rp.isReparsePoint())
        {
            _tprintf(_T("'%ws' is a %sMicrosoft %s (virt == %d)\n"),
                rp.Path(),
                ((rp.isMicrosoftTag()) ? _T("") : _T("non-")),
                ((rp.isSymbolicLink()) ? _T("symbolic link") : ((rp.isVolumeMountPoint()) ? _T("volume mount point") : _T("junction point"))),
                rp.isVirtual()
                );
            if(be_verbose)
            {
                if(-1 != rp.FileIndex())
                {
                    _tprintf(_T("\tFile Index: %I64u (%08X%08X)\n"), rp.FileIndex(), rp.FileIndexHigh(), rp.FileIndexLow());
                }
            }
            if(rp.isMicrosoftTag())
            {
                _tprintf(_T("\tPrint name: %ws\n"), rp.PrintName());
                _tprintf(_T("\tSubst name: %ws\n"), rp.SubstName());
                _tprintf(_T("\tSubst name: %ws (w32)\n"), rp.CanonicalSubstName());
            }
            if(!rp.isMicrosoftTag() || be_verbose)
            {
                if(be_verbose)
                {
                    _tprintf(_T("\tTag       : %08X\n"), rp.ReparseTag());
                }
                _tprintf(_T("\tGUID      : {%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x}\n"),
                    rp.ReparseGuid().Data1,
                    rp.ReparseGuid().Data2,
                    rp.ReparseGuid().Data3,
                    rp.ReparseGuid().Data4[0],
                    rp.ReparseGuid().Data4[1],
                    rp.ReparseGuid().Data4[2],
                    rp.ReparseGuid().Data4[3],
                    rp.ReparseGuid().Data4[4],
                    rp.ReparseGuid().Data4[5],
                    rp.ReparseGuid().Data4[6],
                    rp.ReparseGuid().Data4[7]
                );
            }
        }
        else
        {
            _ftprintf(stderr, _T("ERROR: '%ws' is not a reparse point\n"), rp.Path());
            return 2;
        }
        return 0;
    }
    else
    {
        _ftprintf(stderr, _T("Syntax: looklink [--nologo] [--verbose] <path>\n"));
    }
    return 1;
}
