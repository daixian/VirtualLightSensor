#include "MappingFile.h"

//#include "dlog/dlog.h"

//#include "Poco/SharedMemory.h"
#include "SharedMemory.hpp"

namespace dxlib {

class MappingFile::Impl
{
  public:
    Impl()
    {
    }
    ~Impl()
    {
    }

    SharedMemory sharedMemory;

    int mapSize = 4 * 1024; //使用4k的共享内存

    void open()
    {
        wchar_t* name = L"Global\\xxvirtuallightsensordata";

        LogI("MappingFile.open():尝试打开FileMapping");
        //上来先按名字打开
        int success = sharedMemory.Open(name, mapSize);
        //如果打开失败，那么就创建一个
        if (success != 0) {
            LogI("MappingFile.open():打开失败，之前未有人创建，新建一个...");
            success = sharedMemory.Creat(name, mapSize);
        }
        if (success == 0) {
            LogI("MappingFile.open():FileMapping构建成功!");
        }
        else {
            LogE("MappingFile.open():FileMapping打开失败,可能需要管理员权限,只能创建一个Local");

            name = L"Local\\xxvirtuallightsensordata"; //只能本地创建一个补救（防止内存错误）
            if (sharedMemory.Creat(name, mapSize) == 0) {
                LogI("MappingFile.open():Local对象创建成功!");
            }
        }
    }

  private:
};

MappingFile* MappingFile::m_pInstance = NULL;

MappingFile::MappingFile()
{
    _impl = new Impl();
}

MappingFile::~MappingFile()
{
    delete _impl;
}

MappingFile* MappingFile::GetInst()
{
    if (m_pInstance == NULL)
        m_pInstance = new MappingFile();
    return m_pInstance;
}

int MappingFile::init()
{
    _impl->open();

    auto& fileMapping = _impl->sharedMemory;
    if (fileMapping.pView != NULL) {
        LSData = (LightSensorData*)fileMapping.pView;
    }
    else {
        LSData = NULL;
    }
    if (LSData == nullptr) {
        LogE("MappingFile.init():LSData=null 初始化失败!!");
        return -1;
    }
    //初始化整个共享内存数据
    LogI("MappingFile.init():空间大小 size = %d 字节", _impl->mapSize);
    memset(LSData, 0x00, _impl->mapSize);

    //设置一下状态初值
    reset();
    return 0;
}

void MappingFile::reset()
{
    if (LSData == nullptr) {
        LogE("MappingFile.reset():LSData值为null,直接返回!");
        return;
    }
    LogI("MappingFile.reset():重设LSData初始值!");
    *LSData = {
        1,    // 驱动内部版本
        0,    // windows服务还没有运行
        1.0f, // Lux
        1.0f, // Kelvins
        0.5f, // Chromaticity X
        0.5f, // Chromaticity Y
        TRUE  // IsValid
    };
}

void MappingFile::writeLux(float lux)
{
    if (LSData == nullptr) {
        LogE("MappingFile.reset():LSData值为null,直接返回!");
        return;
    }

    LSData->Lux = lux;
}

float MappingFile::readLux()
{
    if (LSData == nullptr) {
        LogE("MappingFile.reset():LSData值为null,直接返回!");
        return 0;
    }

    return LSData->Lux;
}
} // namespace dxlib
