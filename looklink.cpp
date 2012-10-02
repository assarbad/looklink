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

// $Id: looklink.cpp 47 2010-01-31 16:47:26Z oliver $

#include <cstdio>
#ifdef _DEBUG
#include <crtdbg.h>
#endif // _DEBUG
#include <tchar.h>
#include "looklink.h"

int __cdecl _tmain(int argc, _TCHAR *argv[])
{
#ifdef _DEBUG
    _CrtSetDbgFlag(_CRTDBG_CHECK_ALWAYS_DF | _CRTDBG_ALLOC_MEM_DF);
#endif // _DEBUG
    if(2 == argc)
    {
        _tprintf(TEXT("looklink %1d.%1d/%03d - written 2009-2010, Oliver Schneider (assarbad.net)\n\n"), PRD_MAJVER, PRD_MINVER, FILE_BUILD);

        CReparsePoint rp(argv[1]);
        if(rp.isReparsePoint())
        {
            _tprintf(_T("'%ws' is a %sMicrosoft %s (virt == %d)\n"),
                rp.Path(),
                ((rp.isMicrosoftTag()) ? _T("") : _T("non-")),
                ((rp.isSymbolicLink()) ? _T("symbolic link") : ((rp.isVolumeMountPoint()) ? _T("volume mount point") : _T("junction point"))),
                rp.isVirtual()
                );
            if(rp.isMicrosoftTag())
            {
                _tprintf(_T("\tPrint name: %ws\n"), rp.PrintName());
                _tprintf(_T("\tSubst name: %ws\n"), rp.SubstName());
                _tprintf(_T("\tSubst name: %ws (w32)\n"), rp.CanonicalSubstName());
            }
            else
            {
                _tprintf(_T("Reparse GUID is {%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x}\n"),
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
            _tprintf(_T("'%ws' is not a reparse point\n"), rp.Path());
            return 2;
        }
        return 0;
    }
    else
    {
        _tprintf(_T("Syntax: looklink <path>\n"));
    }
    return 1;
}
