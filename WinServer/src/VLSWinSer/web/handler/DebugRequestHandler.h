#pragma once

#include "Poco/Net/HTTPServer.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Net/HTMLForm.h"
#include "Poco/Net/PartHandler.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/URI.h"
#include "Poco/File.h"

#define XUEXUE_JSON_SUPPORT_OPENCV
#define XUEXUE_JSON_SUPPORT_EIGEN
#include "xuexuejson/Serialize.hpp"

using Poco::URI;
using Poco::Net::HTTPRequestHandler;
using Poco::Net::HTTPRequestHandlerFactory;
using Poco::Net::HTTPServer;
using Poco::Net::HTTPServerParams;
using Poco::Net::HTTPServerRequest;
using Poco::Net::HTTPServerResponse;
using Poco::Net::ServerSocket;

namespace dxlib {

/**
 * ����http��Ϣ:����.
 *
 * @author daixian
 * @date 2020/6/11
 */
class DebugRequestHandler : public HTTPRequestHandler
{
  public:
    enum class HandlerType
    {
        // û��֧�ֵĴ���.
        none,

        // ���õ�ǰ�Ĺ����ڴ���.
        MFResultSet,
    };

    /**
     * �ж��Ƿ���֧�ֵ�Path����.
     *
     * @author daixian
     * @date 2020/3/18
     *
     * @param  path ����֮���path.
     *
     * @returns ���������Ĵ�������.
     */
    static HandlerType ParsePath(const std::string& path)
    {
        if (path == "/debug/mf/result/set") {
            return HandlerType::MFResultSet;
        }
        return HandlerType::none;
    }

    /**
     * ��������,����ܹ�����֧�ֵ�path�ͳɹ���������,���򷵻�null.
     *
     * @author daixian
     * @date 2020/3/18
     *
     * @param  uri  ����uri.
     * @param  path ����֮���path.
     *
     * @returns ����null��ʾ��֧�ֵ�path.
     */
    static HTTPRequestHandler* creat(const URI& uri, const std::string& path)
    {
        auto ev = ParsePath(path);
        if (ev != HandlerType::none) {
            return new DebugRequestHandler(uri, ev);
        }
        return nullptr;
    }

    /**
     * ����.
     *
     * @author daixian
     * @date 2020/3/18
     *
     * @param  uri �����rui.
     * @param  ev  Handler��������.
     */
    DebugRequestHandler(const Poco::URI& uri, const DebugRequestHandler::HandlerType& ev);

    /**
     * ����.
     *
     * @author daixian
     * @date 2020/3/18
     */
    virtual ~DebugRequestHandler();

    /**
     * override����.
     *
     * @author daixian
     * @date 2020/4/30
     *
     * @param [in,out] request  http����.
     * @param [in,out] response http��Ӧ.
     */
    void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response) override;

    // ���ݹ����ĵ�ǰʵ���¼�.
    HandlerType ev;

    /**
     * http��body��DTO,���ù����ڴ�Ľ��.
     *
     * @author daixian
     * @date 2020/6/11
     */
    class DtoMFResultSet : XUEXUE_JSON_OBJECT
    {
      public:
        float Lux = 1.0f;
        float Kelvins = 1.0f;
        float ChromaticityX = 0.5f;
        float ChromaticityY = 0.5f;
        XUEXUE_JSON_OBJECT_M4(Lux, Kelvins, ChromaticityX, ChromaticityY)

      private:
    };

  private:
    // ��ǰ��uri.
    Poco::URI uri;

    void handleRequestMFResultSet(HTTPServerRequest& request, HTTPServerResponse& response);

    // ���س�Ա
    class Impl;
    Impl* _impl;
};
} // namespace dxlib