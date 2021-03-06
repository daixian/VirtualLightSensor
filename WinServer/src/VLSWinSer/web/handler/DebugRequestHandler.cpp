﻿#include "DebugRequestHandler.h"
#include <iostream>
#include "dlog/dlog.h"
#include "xuexuejson/Serialize.hpp"

#include "Poco/URI.h"
#include <algorithm>
#include "../extension/ParserQueryParameters.hpp"
#include "CVSystem/CVSystem.h"
#include "xuexue/math/math.h"

#include "../../Shared/MappingFile.h"
#include "../../Proc/VLSProc.h"

using namespace xuexue;
using namespace xuexue::json;
using namespace Poco::Net;
using namespace Poco;

namespace dxlib {

class DebugRequestHandler::Impl
{
  public:
    Impl() {}
    ~Impl() {}

  private:
};

DebugRequestHandler::DebugRequestHandler(const Poco::URI& uri, const DebugRequestHandler::HandlerType& ev) : uri(uri), ev(ev)
{
    _impl = new DebugRequestHandler::Impl();
}

DebugRequestHandler::~DebugRequestHandler()
{
    delete _impl;
}

void DebugRequestHandler::handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
{
    try {
        switch (ev) {
        case HandlerType::none:
            break;
        case HandlerType::MFResultSet:
            handleRequestMFResultSet(request, response);
            break;
        case HandlerType::AppStatus:
            handleRequestAppStatus(request, response);
            break;
        case HandlerType::ConfigManual:
            handleRequestConfigManual(request, response);
            break;
        default:
            break;
        }
    }
    catch (const Poco::Exception& pe) {
        LogE("DebugRequestHandler.handleRequest():异常:%s,%s ", pe.what(), pe.message().c_str());
    }
    catch (const std::exception& e) {
        LogE("DebugRequestHandler.handleRequest():异常:%s", e.what());
    }
}

void DebugRequestHandler::handleRequestMFResultSet(HTTPServerRequest& request, HTTPServerResponse& response)
{
    auto dto = JsonMapper::toObject<DtoMFResultSet>(request.stream());
    MappingFile::GetInst()->init_c();
    LightSensorData* LSData = MappingFile::GetInst()->LSData;
    if (LSData != nullptr) {
        LSData->Lux = dto.Lux;
        LSData->Kelvins = dto.Kelvins;
        LSData->ChromaticityX = dto.ChromaticityX;
        LSData->ChromaticityY = dto.ChromaticityY;
    }
    response.redirect("/event_done.html");
}

void DebugRequestHandler::handleRequestAppStatus(HTTPServerRequest& request, HTTPServerResponse& response)
{
    LightSensorData* LSData = MappingFile::GetInst()->LSData;
    DtoAppStatus rep;
    rep.Lux = LSData->Lux;
    rep.runCount = LSData->runCount;
    rep.fps = MultiCamera::GetInst()->fps();

    response.setStatus(HTTPResponse::HTTP_OK);
    response.setContentType("application/json");
    std::ostream& out = response.send();
    out << xuexue::json::JsonMapper::toJson(rep);
}

void DebugRequestHandler::handleRequestConfigManual(HTTPServerRequest& request, HTTPServerResponse& response)
{
    auto dto = JsonMapper::toObject<DtoConfigManual>(request.stream());

    auto* ptr = MultiCamera::GetInst()->getProc(0);
    if (ptr != nullptr) {
        poco_assert(std::string(ptr->name()) == "VLSProc");
        VLSProc* proc = (VLSProc*)ptr;
        proc->isManual = dto.isManual;
    }
    response.redirect("/event_done.html");
}
} // namespace dxlib