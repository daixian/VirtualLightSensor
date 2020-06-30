#include "DebugRequestHandler.h"
#include <iostream>
#include "dlog/dlog.h"
#include "xuexuejson/Serialize.hpp"

#include "Poco/URI.h"
#include <algorithm>
#include "../extension/ParserQueryParameters.hpp"
#include "CVSystem/CVSystem.h"
#include "xuexue/math/math.h"

#include "../../Shared/MappingFile.h"

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

        default:
            break;
        }
    }
    catch (const Poco::Exception& pe) {
        LogE("DebugRequestHandler.handleRequest():“Ï≥£:%s,%s ", pe.what(), pe.message().c_str());
    }
    catch (const std::exception& e) {
        LogE("DebugRequestHandler.handleRequest():“Ï≥£:%s", e.what());
    }
}

void DebugRequestHandler::handleRequestMFResultSet(HTTPServerRequest& request, HTTPServerResponse& response)
{
    auto dto = xuexue::json::JsonMapper::toObject<DtoMFResultSet>(request.stream());
    MappingFile::GetInst()->init();
    LightSensorData* LSData = MappingFile::GetInst()->LSData;
    if (LSData != nullptr) {
        LSData->Lux = dto.Lux;
        LSData->Kelvins = dto.Kelvins;
        LSData->ChromaticityX = dto.ChromaticityX;
        LSData->ChromaticityY = dto.ChromaticityY;
    }
    response.redirect("/event_done.html");
}

} // namespace dxlib