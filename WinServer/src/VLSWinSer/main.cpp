#include "stdio.h"
#include "./web/HttpServer.h"
#include "dlog/dlog.h"

int main(int argc, char** argv)
{
    dlog_init("log", "VLSWinSer", dlog_init_relative::MODULE, false);
    dlog_set_console_thr(dlog_level::info);
    LogI("程序启动...");
    dxlib::AppHttpServer app;
    return app.run(argc, argv);
}