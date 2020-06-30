#include "VLSModule.h"
#include "CVSystem/DShow/UVCCamera.h"
#include "../data/Profile.h"

namespace dxlib {

VLSModule::VLSModule()
{
}

VLSModule::~VLSModule()
{
}

void VLSModule::go()
{
    UVCCamera uvc;
    uvc.connectDevice(Profile::GetInst()->cameraName);
    auto capabls = uvc.getCapabilities();

    //选择一个最小的分辨率
    for (size_t i = 0; i < capabls.size(); i++) {
    }

    cv::Size scSize = cv::Size(1280, 400);

    LogI("VLSModule.init():初始化设置...");

    CameraManger::GetInst()->clear();
    CameraManger::GetInst()->add(std::make_shared<Camera>(Profile::GetInst()->cameraName));
}

} // namespace dxlib
