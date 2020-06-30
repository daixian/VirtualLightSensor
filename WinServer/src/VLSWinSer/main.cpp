#include "stdio.h"
#include "dlog/dlog.h"
#include "./Shared/MappingFile.h"
#include "./web/HttpServer.hpp"
#include "./Proc/VLSModule.h"

using namespace dxlib;

int main(int argc, char** argv)
{
    dlog_init("log", "VLSWinSer", dlog_init_relative::MODULE, false);
    dlog_set_console_thr(dlog_level::info);
    LogI("程序启动...");

    //程序启动之后第三步载入数据库配置

    MappingFile::GetInst()->init_c();
    LightSensorData* LSData = MappingFile::GetInst()->LSData;
    if (LSData != nullptr) {
        if (LSData->DriverVer > 0 && LSData->runCount > 0) {
            LogI("驱动程序已经启动了!");
        }
        LogI("当前DriverVer = %d", LSData->DriverVer);
    }
    //VLSModule vls;

    dxlib::AppHttpServer app;
    return app.run(argc, argv);
}