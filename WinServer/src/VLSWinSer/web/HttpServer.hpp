#pragma once

#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Timestamp.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"
#include "Poco/Exception.h"
#include "Poco/ThreadPool.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"
#include "Poco/RegularExpression.h"
#include <iostream>

#include "CVSystem/web/handler/MultiCameraRequestHandler.h"
#include "CVSystem/web/handler/CameraRequestHandler.h"
#include "CVSystem/web/handler/FileHandle.h"

#include "./handler/DebugRequestHandler.h"
#include "./handler/MathRequestHandler.h"

#include "dlog/dlog.h"
#include "CVSystem/Common/FileHelper.h"

using Poco::DateTimeFormat;
using Poco::DateTimeFormatter;
using Poco::ThreadPool;
using Poco::Timestamp;
using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Net::HTTPServer;
using Poco::Net::HTTPServerParams;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Net::ServerSocket;
using Poco::Util::Application;
using Poco::Util::HelpFormatter;
using Poco::Util::Option;
using Poco::Util::OptionCallback;
using Poco::Util::OptionSet;
using Poco::Util::ServerApplication;
//using Poco::RegularExpression;

namespace dxlib {

class AppRequestHandlerFactory : public HTTPRequestHandlerFactory
{
  public:
    AppRequestHandlerFactory() {}

    virtual HTTPRequestHandler* createRequestHandler(const HTTPServerRequest& request)
    {
        std::string decodeText;
        Poco::URI::decode(request.getURI(), decodeText);
        Poco::URI uri(decodeText);
        std::string path(uri.getPath());

        auto handler = CameraRequestHandler::creat(uri, path);
        if (handler != nullptr) {
            return handler;
        }

        //handler = CmdRequestHandler::creat(uri, path);
        //if (handler != nullptr) {
        //    return handler;
        //}

        handler = MultiCameraRequestHandler::creat(uri, path);
        if (handler != nullptr) {
            return handler;
        }

        handler = MathRequestHandler::creat(uri, path);
        if (handler != nullptr) {
            return handler;
        }

        handler = DebugRequestHandler::creat(uri, path);
        if (handler != nullptr) {
            return handler;
        }

        //这个Poco::Path::current()路径在windows下面最后是带/的
        return new FileHandle(cvsystem::FileHelper::getModuleDir() + cvsystem::FileHelper ::sep() + "static");
    }

  private:
};

class AppHttpServer : public Poco::Util::ServerApplication
{
  public:
    AppHttpServer() : _helpRequested(false)
    {
    }

    ~AppHttpServer()
    {
    }

  protected:
    void initialize(Application& self) override
    {
        //目前没有配置文件
        loadConfiguration();
        ServerApplication::initialize(self);
    }

    void uninitialize() override
    {
        ServerApplication::uninitialize();
    }

    void defineOptions(OptionSet& options) override
    {
        //最先会进这个函数
        ServerApplication::defineOptions(options);

        options.addOption(
            Option("help", "h", "display argument help information")
                .required(false)
                .repeatable(false)
                .callback(OptionCallback<AppHttpServer>(this, &AppHttpServer::handleHelp)));
    }

    void handleHelp(const std::string& name,
                    const std::string& value)
    {
        HelpFormatter helpFormatter(options());
        helpFormatter.setCommand(commandName());
        helpFormatter.setUsage("OPTIONS");
        helpFormatter.setHeader(
            "A web server that serves the current date and time.");
        helpFormatter.format(std::cout);
        stopOptionsProcessing();
        _helpRequested = true;
    }

    int main(const std::vector<std::string>& args) override
    {
        //在上面的内容加载完毕之后才会进入这个函数
        if (!_helpRequested) {

            if (config().getBool("application.runAsService", false)) {
                // do service specific things
                LogI("AppHttpServer.initialize():检测到作为服务运行的设置..");
            }
            else {
                LogI("AppHttpServer.initialize():未检测到作为服务运行..");
            }

            unsigned short port = (unsigned short)config().getInt("HttpServer.port", 42016);

            ServerSocket svs(port);
            HTTPServer srv(new AppRequestHandlerFactory(), svs, new HTTPServerParams());
            LogI("服务器启动 port:%d...", port);
            srv.start();
            waitForTerminationRequest();
            srv.stop();
        }
        return Application::EXIT_OK;
    }

  private:
    bool _helpRequested;
};

} // namespace dxlib
