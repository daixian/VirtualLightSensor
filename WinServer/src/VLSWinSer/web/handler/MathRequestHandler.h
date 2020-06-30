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
 * 处理http消息:数学计算.
 *
 * @author daixian
 * @date 2020/6/11
 */
class MathRequestHandler : public HTTPRequestHandler
{
  public:
    enum class HandlerType
    {
        // 没有支持的处理.
        none,

        // 物体的l->w矩阵.
        TRS,

        // 计算u3d的l->w矩阵.
        TRSU3D,

        // 根据相机参数计算大致fov.
        FOV,
    };

    /**
     * 判断是否有支持的Path处理.
     *
     * @author daixian
     * @date 2020/3/18
     *
     * @param  path 解码之后的path.
     *
     * @returns 解析出来的处理类型.
     */
    static HandlerType ParsePath(const std::string& path)
    {
        if (path == "/math/trs") {
            return HandlerType::TRS;
        }
        if (path == "/math/trs-u3d") {
            return HandlerType::TRSU3D;
        }
        if (path == "/math/fov") {
            return HandlerType::FOV;
        }
        return HandlerType::none;
    }

    /**
     * 工厂方法,如果能够包含支持的path就成功创建对象,否则返回null.
     *
     * @author daixian
     * @date 2020/3/18
     *
     * @param  uri  完整uri.
     * @param  path 解码之后的path.
     *
     * @returns 返回null表示不支持的path.
     */
    static HTTPRequestHandler* creat(const URI& uri, const std::string& path)
    {
        auto ev = ParsePath(path);
        if (ev != HandlerType::none) {
            return new MathRequestHandler(uri, ev);
        }
        return nullptr;
    }

    /**
     * 构造.
     *
     * @author daixian
     * @date 2020/3/18
     *
     * @param  uri 请求的rui.
     * @param  ev  Handler处理类型.
     */
    MathRequestHandler(const Poco::URI& uri, const MathRequestHandler::HandlerType& ev);

    /**
     * 析构.
     *
     * @author daixian
     * @date 2020/3/18
     */
    virtual ~MathRequestHandler();

    /**
     * override处理.
     *
     * @author daixian
     * @date 2020/4/30
     *
     * @param [in,out] request  http请求.
     * @param [in,out] response http响应.
     */
    void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response) override;

    /// <summary> 传递过来的当前实际事件. </summary>
    HandlerType ev;

    /**
     * http的body的DTO.
     *
     * @author daixian
     * @date 2020/6/11
     */
    class DtoTRS : XUEXUE_JSON_OBJECT
    {
      public:
        Eigen::Vector3d position;
        Eigen::Quaterniond rotation;
        Eigen::Vector3d scale;
        XUEXUE_JSON_OBJECT_M3(position, rotation, scale)

      private:
    };

    class DtoTRSU3D : XUEXUE_JSON_OBJECT
    {
      public:
        Eigen::Vector3d position;
        Eigen::Vector3d euler;
        Eigen::Vector3d scale;
        XUEXUE_JSON_OBJECT_M3(position, euler, scale)

      private:
    };

    class DtoFOV : XUEXUE_JSON_OBJECT
    {
      public:
        int width;
        int height;
        std::string filePath;
        int f;
        XUEXUE_JSON_OBJECT_M4(width, height, filePath, f)

      private:
    };

  private:
    // 当前的uri.
    Poco::URI uri;

    // 隐藏成员
    class Impl;
    Impl* _impl;
};
} // namespace dxlib