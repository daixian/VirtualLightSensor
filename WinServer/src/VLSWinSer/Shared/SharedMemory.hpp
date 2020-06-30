#pragma once

#include <stdio.h>

#if defined(_WIN32) || defined(_WIN64)
#    include <windows.h>
#    include <Sddl.h>
#endif

#include "dlog/dlog.h"

#define VIEW_OFFSET 0

namespace dxlib {

/**
 * 共享内存，目前这是windows下的实现.
 *
 * @author daixian
 * @date 2017/2/8
 */
class SharedMemory
{
  public:
    SharedMemory()
    {
    }
    virtual ~SharedMemory()
    {
    }

#if defined(_WIN32) || defined(_WIN64)

    // FileMapping句柄.
    HANDLE hMapFile = NULL;

    // 这个地址即为内存共享地址.
    PVOID pView = NULL;

    // 记录的共享内存大小.
    int size = 0;

    // GA表示给全部权限.
    const wchar_t* sharessdl = L"D:P(A;OICI;GA;;;SY)(A;OICI;GA;;;BA)(A;OICI;GA;;;IU)";

    /**
     * 作为服务端创建一个共享内存.
     *
     * @author daixian
     * @date 2017/2/8
     *
     * @param [in] name    名字，形如"Local\\FileMappingForGLMap".
     * @param      mapSize map的大小.
     *
     * @returns 成功返回0,错误返回负数.
     */
    int Creat(wchar_t* name, int mapSize)
    {
        if (hMapFile != NULL) {
            Cleanup();
        }

        SECURITY_ATTRIBUTES security;
        ZeroMemory(&security, sizeof(security));
        security.nLength = sizeof(security);
        ConvertStringSecurityDescriptorToSecurityDescriptorW(
            sharessdl,
            SDDL_REVISION_1,
            &security.lpSecurityDescriptor,
            NULL);

        hMapFile = CreateFileMappingW(
            INVALID_HANDLE_VALUE, // Use paging file - shared memory
            &security,            // Default security attributes
            PAGE_READWRITE,       // Allow read and write access
            0,                    // High-order DWORD of file mapping max size
            mapSize,              // Low-order DWORD of file mapping max size
            name                  // Name of the file mapping object
        );

        if (hMapFile == NULL) {
            LogE("SharedMemory.Creat():CreateFileMapping failed w/err 0x%08lx", GetLastError());
            Cleanup();
            return -1;
        }

        // Map a view of the file mapping into the address space of the current process.
        pView = MapViewOfFile(
            hMapFile,            // Handle of the map object
            FILE_MAP_ALL_ACCESS, // Read and write access
            0,                   // High-order DWORD of the file offset
            VIEW_OFFSET,         // Low-order DWORD of the file offset
            mapSize              // The number of bytes to map to view
        );
        if (pView == NULL) {
            LogE("SharedMemory.Creat():MapViewOfFile failed w/err 0x%08lx", GetLastError());

            Cleanup();
            return -2;
        }
        LogI("SharedMemory.Creat():The file view is mapped");
        this->size = mapSize;

        LocalFree(security.lpSecurityDescriptor);
        return 0;
    }

    /**
     * 作为客户端打开一个共享内存.
     *
     * @author daixian
     * @date 2017/2/9
     *
     * @param [in] name    名字，形如"Local\\FileMappingForGLMap".
     * @param      mapSize Size of the map.
     *
     * @returns 成功返回0,错误返回负数.
     */
    int Open(wchar_t* name, int mapSize)
    {
        if (hMapFile != NULL) {
            Cleanup();
        }
        // Try to open the named file mapping identified by the map name.
        hMapFile = OpenFileMappingW(
            FILE_MAP_ALL_ACCESS, // Read access  原来老外代码使用的是FILE_MAP_READ
            TRUE,                // Do not inherit the name
            name                 // File mapping name
        );
        if (hMapFile == NULL) {
            LogW("SharedMemory.Open():OpenFileMapping failed w/err 0x%08lx", GetLastError());
            Cleanup();
            return -1;
        }
        //wprintf(L"The file mapping is opened\n");

        // Map a view of the file mapping into the address space of the current process.
        pView = MapViewOfFile(
            hMapFile,            // Handle of the map object
            FILE_MAP_ALL_ACCESS, // Read access    原来老外代码使用的是FILE_MAP_READ
            0,                   // High-order DWORD of the file offset
            VIEW_OFFSET,         // Low-order DWORD of the file offset
            mapSize              // The number of bytes to map to view
        );
        if (pView == NULL) {
            LogE("SharedMemory.Open():MapViewOfFile failed w/err 0x%08lx", GetLastError());
            Cleanup();
            return -2;
        }
        LogI("SharedMemory.Open():The file view is mapped");

        this->size = mapSize;
        return 0;
    }

    /**
     * 释放对象.
     *
     * @author daixian
     * @date 2017/2/8
     */
    void Cleanup()
    {
        if (hMapFile) {
            if (pView) {
                // Unmap the file view.
                UnmapViewOfFile(pView);
                pView = NULL;
            }
            // Close the file mapping object.
            CloseHandle(hMapFile);
            hMapFile = NULL;

            this->size = 0;
        }
    }

#endif
};
} // namespace dxlib