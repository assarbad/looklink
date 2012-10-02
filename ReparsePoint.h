///////////////////////////////////////////////////////////////////////////////
///
/// Written 2009-2010, Oliver Schneider (assarbad.net) - PUBLIC DOMAIN
///
/// Original filename: ReparsePoint.h
/// Project          : looklink
/// Date of creation : 2009-02-03
/// Author(s)        : Oliver Schneider
///
/// Purpose          : Class to investigate reparse points on Windows 2000 and
///                    later.
///
///////////////////////////////////////////////////////////////////////////////

#ifndef __REPARSEPOINT_H_VER__
#define __REPARSEPOINT_H_VER__ 2010012623
// $Id: ReparsePoint.h 47 2010-01-31 16:47:26Z oliver $
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif // Check for "#pragma once" support

#include <tchar.h>
#include <Windows.h>
#include <WinIoCtl.h>

#ifndef IO_REPARSE_TAG_DFS
#define IO_REPARSE_TAG_DFS                      (0x8000000AL)       
#endif // IO_REPARSE_TAG_DFS

#ifndef IO_REPARSE_TAG_SYMLINK
#define IO_REPARSE_TAG_SYMLINK                  (0xA000000CL)       
#endif // IO_REPARSE_TAG_SYMLINK

#ifndef IO_REPARSE_TAG_DFSR
#define IO_REPARSE_TAG_DFSR                     (0x80000012L)       
#endif // IO_REPARSE_TAG_DFSR

#ifndef FILE_ATTRIBUTE_VIRTUAL
#define FILE_ATTRIBUTE_VIRTUAL                  0x00010000  
#endif // FILE_ATTRIBUTE_VIRTUAL

#define WIN32_UNICODE_PREFIX                    L"\\\\?\\"

class CReparsePoint
{
    template <typename T> class CVerySimpleBuf
    {
    public:
        CVerySimpleBuf(size_t count = 0)
            : m_buf(0)
            , m_count(0)
        {
            reAlloc(count);
        }

        CVerySimpleBuf(const CVerySimpleBuf& rval)
            : m_buf(0)
            , m_count(0)
        {
            operator=(rval);
        }

        CVerySimpleBuf(const T* buf)
            : m_buf(0)
            , m_count(0)
        {
            if(buf)
            {
                operator=(buf);
            }
        }

        ~CVerySimpleBuf()
        {
            delete[] m_buf;
        }

        CVerySimpleBuf& operator=(const CVerySimpleBuf& rval)
        {
            if(&rval != this)
            {
                reAlloc(0);
                if(rval.getBuf() && reAlloc(rval.getCount()))
                {
                    memcpy(getBuf(), rval.getBuf(), getMin_(getByteCount(), rval.getByteCount()));
                }
            }
            return *this;
        }

        CVerySimpleBuf& operator=(const T* buf)
        {
            reAlloc(0);
            if(buf)
            {
                const size_t len = getBufLenZ_<const T*>(buf);
                if(!len)
                {
                    reAlloc(1);
                }
                else if(reAlloc(len))
                {
                    memcpy(getBuf(), buf, getMin_(sizeof(T) * len, getByteCount()));
                }
            }
            return *this;
        }

        CVerySimpleBuf& operator+=(const CVerySimpleBuf& rval)
        {
            if(rval.getCountZ() && reAlloc(getCountZ() + rval.getCountZ()))
            {
                memcpy(getBuf() + getCountZ(), rval.getBuf(), sizeof(T) * rval.getCountZ());
            }
            return *this;
        }

        CVerySimpleBuf& operator+=(const T* buf)
        {
            const size_t len = getBufLenZ_<const T*>(buf);
            if(len && reAlloc(getCountZ() + len))
            {
                memcpy(getBuf() + getCountZ(), buf, sizeof(T) * len);
            }
            return *this;
        }

        inline T* getBuf() const
        {
            return m_buf;
        };

        inline operator bool() const
        {
            return (0 != m_buf);
        }

        inline bool operator!() const
        {
            return (0 != m_buf);
        }

        bool reAlloc(size_t count)
        {
            T* tempBuf = 0;
            size_t count_ = 0;
            if(count)
            {
                count_ = getCeil_(count+1);
                if(count_ <= m_count)
                {
                    memset(m_buf + count, 0, sizeof(T) * (m_count - count));
                    return true;
                }
                if(0 != (tempBuf = new T[count_]))
                {
                    memset(tempBuf, 0, sizeof(T) * count_);
                }
                if(tempBuf && m_buf)
                {
                    memcpy(tempBuf, m_buf, sizeof(T) * getMin_(count, m_count));
                }
            }
            delete[] m_buf;
            m_buf = tempBuf;
            m_count = count_;
            return (0 != m_buf);
        }

        inline size_t getCount() const
        {
            return m_count;
        }

        inline size_t getCountZ() const
        {
            return getBufLenZ_<const T*>(m_buf);
        }

        inline size_t getByteCount() const
        {
            return m_count * sizeof(T);
        }
    protected:
        T* m_buf;
        size_t m_count;

        inline size_t getCeil_(size_t count) const
        {
            const size_t align = sizeof(void*);
            return (((sizeof(T) * count) + (align - 1)) & (~(align - 1))) / sizeof(T);
        }

        inline size_t getMin_(size_t a, size_t b) const
        {
            return ((a < b) ? a : b);
        }

        template <typename T> static size_t getBufLenZ_(const char* val)
        {
            return (val) ? strlen(val) : 0;
        }

        template <typename T> static size_t getBufLenZ_(const wchar_t* val)
        {
            return (val) ? wcslen(val) : 0;
        }
    };

#   include <PshPack1.h>
    // The Microsoft version of the reparse data, as opposed to REPARSE_GUID_DATA_BUFFER
    typedef struct _REPARSE_DATA_BUFFER
    {
        ULONG  ReparseTag;
        USHORT  ReparseDataLength;
        USHORT  Reserved;
        union
        {
            struct
            {
                USHORT  SubstituteNameOffset;
                USHORT  SubstituteNameLength;
                USHORT  PrintNameOffset;
                USHORT  PrintNameLength;
                ULONG   Flags;
                WCHAR   PathBuffer[1];
            } SymbolicLinkReparseBuffer;
            struct
            {
                USHORT  SubstituteNameOffset;
                USHORT  SubstituteNameLength;
                USHORT  PrintNameOffset;
                USHORT  PrintNameLength;
                WCHAR   PathBuffer[1];
            } MountPointReparseBuffer;
            struct
            {
                UCHAR  DataBuffer[1];
            } GenericReparseBuffer;
        };
    } REPARSE_DATA_BUFFER, *PREPARSE_DATA_BUFFER;
#   include <PopPack.h>
public:
    CReparsePoint(WCHAR const* Path)
        : m_Path(normalizePath_(Path))
        , m_Attr(::GetFileAttributesW(m_Path.getBuf()))
        , m_OpenFlags(FILE_FLAG_OPEN_REPARSE_POINT | ((m_Attr & FILE_ATTRIBUTE_DIRECTORY) ? FILE_FLAG_BACKUP_SEMANTICS : 0))
        , m_ReparseTag(0)
    {
        cacheValues_();
    }

    inline bool isReparsePoint() const
    {
        return (0 != (m_Attr & FILE_ATTRIBUTE_REPARSE_POINT));
    }

    inline bool isDirectory() const
    {
        return (0 != (m_Attr & FILE_ATTRIBUTE_DIRECTORY));
    }

    inline bool isVirtual() const
    {
        return (0 != (m_Attr & FILE_ATTRIBUTE_VIRTUAL));
    }

    inline bool isSymbolicLink() const // Vista++ symlink
    {
        return (IO_REPARSE_TAG_SYMLINK == m_ReparseTag);
    }

    inline bool isJunctionPoint() const
    {
        return (IO_REPARSE_TAG_MOUNT_POINT == m_ReparseTag);
    }

    inline bool isVolumeMountPoint() const // Real *volume* mount point
    {
        WCHAR const VolMtPointPrefix[] = L"\\\\?\\Volume{";
        CVerySimpleBuf<WCHAR> buf(CanonicalSubstName());
        if(buf && (buf.getCountZ() > wcslen(VolMtPointPrefix)))
        {
            buf.getBuf()[wcslen(VolMtPointPrefix)] = 0;
            return isJunctionPoint() && buf && (0 == wcsncmp(buf.getBuf(), VolMtPointPrefix, wcslen(VolMtPointPrefix)));
        }
        return false;
    }

    inline bool isMicrosoftTag() const
    {
        return 0 != IsReparseTagMicrosoft(m_ReparseTag);
    }

    WCHAR const* Path() const
    {
        return m_Path.getBuf();
    }

    WCHAR const* CanonicalSubstName() const
    {
        return m_SubstNameCanonical.getBuf();
    }

    WCHAR const* PrintName() const
    {
        return m_PrintName.getBuf();
    }

    WCHAR const* SubstName() const
    {
        return m_SubstName.getBuf();
    }

    inline DWORD ReparseTag() const
    {
        return m_ReparseTag;
    }

    inline GUID const& ReparseGuid() const
    {
        return m_ReparseGuid;
    }

private:
    CVerySimpleBuf<WCHAR> normalizePath_(WCHAR const* Path)
    {
        if(Path)
        {
            WCHAR const Win32Prefix[] = WIN32_UNICODE_PREFIX;
            CVerySimpleBuf<WCHAR> sPath((0 != wcsncmp(Win32Prefix, Path, wcslen(Win32Prefix))) ? Win32Prefix : L"");
            if(sPath.getCountZ()) // Only do anything if the path doesn't have the prefix
            {
                LPWSTR filePart = 0;
                DWORD dwNeeded = ::GetFullPathNameW(Path, 0, sPath.getBuf(), &filePart);
                if(sPath.reAlloc(1 + dwNeeded + wcslen(Path)))
                {
                    if(0 < ::GetFullPathNameW(Path, static_cast<DWORD>(sPath.getCount() - sPath.getCountZ()), sPath.getBuf() + sPath.getCountZ(), &filePart))
                    {
                        return sPath;
                    }
                }
            }
        }
        return Path;
    }

    void cacheValues_()
    {
        // WCHAR const Win32Prefix[] = WIN32_UNICODE_PREFIX;
        WCHAR const* NativePrefix = L"\\??\\";
        memset(&m_ReparseGuid, 0, sizeof(m_ReparseGuid));
        if(isReparsePoint())
        {
            HANDLE hFile = ::CreateFile(m_Path.getBuf(), FILE_READ_EA, FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, m_OpenFlags, 0);
            if(INVALID_HANDLE_VALUE != hFile)
            {
                CVerySimpleBuf<unsigned char> buf(MAXIMUM_REPARSE_DATA_BUFFER_SIZE + REPARSE_GUID_DATA_BUFFER_HEADER_SIZE);
                ::SetLastError(ERROR_SUCCESS);
                DWORD dwReturned = 0;
                if(::DeviceIoControl(
                    hFile,
                    FSCTL_GET_REPARSE_POINT,
                    NULL,
                    0,
                    buf.getBuf(),
                    static_cast<DWORD>(buf.getByteCount()),
                    &dwReturned,
                    0))
                {
                    PREPARSE_GUID_DATA_BUFFER const repbuf = reinterpret_cast<PREPARSE_GUID_DATA_BUFFER>(buf.getBuf());
                    m_ReparseTag = repbuf->ReparseTag;
                    memcpy(&m_ReparseGuid, &repbuf->ReparseGuid, sizeof(GUID));
                    if(0 != IsReparseTagMicrosoft(m_ReparseTag))
                    {
                        PREPARSE_DATA_BUFFER const msrepbuf = reinterpret_cast<PREPARSE_DATA_BUFFER>(repbuf);
                        if((IO_REPARSE_TAG_SYMLINK == m_ReparseTag)
                            && (m_PrintName.reAlloc(msrepbuf->SymbolicLinkReparseBuffer.PrintNameLength/sizeof(WCHAR)+1))
                            && (m_SubstName.reAlloc(msrepbuf->SymbolicLinkReparseBuffer.SubstituteNameLength/sizeof(WCHAR)+1))
                            )
                        {
                            memcpy(m_PrintName.getBuf(), &msrepbuf->SymbolicLinkReparseBuffer.PathBuffer[msrepbuf->SymbolicLinkReparseBuffer.PrintNameOffset/sizeof(WCHAR)], msrepbuf->SymbolicLinkReparseBuffer.PrintNameLength);
                            memcpy(m_SubstName.getBuf(), &msrepbuf->SymbolicLinkReparseBuffer.PathBuffer[msrepbuf->SymbolicLinkReparseBuffer.SubstituteNameOffset/sizeof(WCHAR)], msrepbuf->SymbolicLinkReparseBuffer.SubstituteNameLength);
                        }
                        else if((IO_REPARSE_TAG_MOUNT_POINT == m_ReparseTag)
                            && (m_PrintName.reAlloc(msrepbuf->MountPointReparseBuffer.PrintNameLength/sizeof(WCHAR)+1))
                            && (m_SubstName.reAlloc(msrepbuf->MountPointReparseBuffer.SubstituteNameLength/sizeof(WCHAR)+1))
                            )
                        {
                            memcpy(m_PrintName.getBuf(), &msrepbuf->MountPointReparseBuffer.PathBuffer[msrepbuf->MountPointReparseBuffer.PrintNameOffset/sizeof(WCHAR)], msrepbuf->MountPointReparseBuffer.PrintNameLength);
                            memcpy(m_SubstName.getBuf(), &msrepbuf->MountPointReparseBuffer.PathBuffer[msrepbuf->MountPointReparseBuffer.SubstituteNameOffset/sizeof(WCHAR)], msrepbuf->MountPointReparseBuffer.SubstituteNameLength);
                        }
                        m_SubstNameCanonical = m_SubstName;
                        // If the prefix is of the native NT form, change it to the Win32 form instead
                        if(m_SubstNameCanonical.getCountZ() && (0 == wcsncmp(NativePrefix, m_SubstNameCanonical.getBuf(), sizeof(NativePrefix)/sizeof(NativePrefix[0])-1)))
                        {
                            m_SubstNameCanonical.getBuf()[1] = L'\\';
                        }
                    }
                }
                ::CloseHandle(hFile);
            }
        }
    }

    CVerySimpleBuf<WCHAR>   m_Path;
    CVerySimpleBuf<WCHAR>   m_SubstName;
    CVerySimpleBuf<WCHAR>   m_PrintName;
    CVerySimpleBuf<WCHAR>   m_SubstNameCanonical;
    GUID                    m_ReparseGuid;
    DWORD const             m_Attr;
    DWORD const             m_OpenFlags;
    DWORD                   m_ReparseTag;
};

#endif // __REPARSEPOINT_H_VER__
