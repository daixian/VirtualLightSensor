#include "MathRequestHandler.h"
#include <iostream>
#include "dlog/dlog.h"
#include "xuexuejson/Serialize.hpp"

#include "Poco/URI.h"
#include <algorithm>
#include "../extension/ParserQueryParameters.hpp"
#include "CVSystem/CVSystem.h"
#include "xuexue/math/math.h"

using namespace xuexue;
using namespace xuexue::json;
using namespace Poco::Net;
using namespace Poco;

namespace dxlib {

class MathRequestHandler::Impl
{
  public:
    Impl() {}
    ~Impl() {}

  private:
};

MathRequestHandler::MathRequestHandler(const Poco::URI& uri, const MathRequestHandler::HandlerType& ev) : uri(uri), ev(ev)
{
    _impl = new MathRequestHandler::Impl();
}

MathRequestHandler::~MathRequestHandler()
{
    delete _impl;
}

void MathRequestHandler::handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
{
    try {
        //计算本地到世界矩阵
        if (ev == HandlerType::TRS) {
            ParserQueryParameters querys(uri);

            auto dto = xuexue::json::JsonMapper::toObject<DtoTRS>(request.stream());
            cv::Mat l2w;
            cv::eigen2cv(Math::TRSMat4x4(dto.position, dto.rotation, dto.scale), l2w);

            response.setStatus(HTTPResponse::HTTP_OK);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            out << JsonMapper::toJson(l2w);
        }
        //使用u3d中的欧垃角来计算结果
        else if (ev == HandlerType::TRSU3D) {
            ParserQueryParameters querys(uri);

            auto dto = xuexue::json::JsonMapper::toObject<DtoTRSU3D>(request.stream());
            cv::Mat l2w;
            Eigen::Quaterniond q = Math::QuatU3D(dto.euler);
            cv::eigen2cv(Math::TRSMat4x4(dto.position, q, dto.scale), l2w);

            //保存一个yml文件
            cv::FileStorage fs;
            fs.open("DtoTRSU3D.json", cv::FileStorage::WRITE);
            if (fs.isOpened()) {
                fs << "CamTR" << l2w;
                fs.release();
            }
            else {
                LogE("Error: 无法保存DtoTRSU3D.json");
            }

            response.setStatus(HTTPResponse::HTTP_OK);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            out << JsonMapper::toJson(l2w);
        }
        else if (ev == HandlerType::FOV) {
            ParserQueryParameters querys(uri);

            auto dto = xuexue::json::JsonMapper::toObject<DtoFOV>(request.stream());
            Camera camera("cam1");
            cv::Mat M;
            cv::Mat D;
            //如果有参数文件路径
            if (!dto.filePath.empty()) {
                cv::FileStorage fs;
                fs.open(dto.filePath, cv::FileStorage::READ);

                if (fs.isOpened()) {
                    fs["targetSize"] >> camera.paramSize;
                    camera.size = camera.paramSize;
                    fs["M1"] >> camera.camMatrix;
                    fs["D1"] >> camera.distCoeffs;
                    fs["R1"] >> camera.R;
                    fs["P1"] >> camera.P;
                    fs.release();
                }
                else {
                    LogE("Error: 无法打开%s", dto.filePath.c_str());
                }
            }
            camera.initUndistortRectifyMap();

            //成像区域：3896*2453um；
            //封装尺寸：5237*4463um
            double fovx, fovy;
            double focalLength;         //镜头的焦距,以毫米为单位
            cv::Point2d principalPoint; //以毫米为单位的主点
            double aspectRatio;         //f_y/f_x
            cv::calibrationMatrixValues(camera.camMatrix, camera.paramSize, 3.896, 2.453, fovx, fovy, focalLength, principalPoint, aspectRatio);

            response.setStatus(HTTPResponse::HTTP_OK);
            response.setContentType("application/json");
            std::ostream& out = response.send();
            out << "{" << std::endl;
            out << "\"fov_x\": " << fovx << "," << std::endl;
            out << "\"fov_y\": " << fovy << std::endl;
            out << "}" << std::endl;
        }
    }
    catch (const std::exception& e) {
        LogE("MathRequestHandler.handleRequest():异常:%s", e.what());
    }

    response.redirect("/event_done.html");
}

} // namespace dxlib