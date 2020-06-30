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

void go()
{
    UVCCamera uvc;
    uvc.connectDevice(Profile::GetInst()->cameraName);
    auto capabls = uvc.getCapabilities();

    //ѡ��һ����С�ķֱ���
    for (size_t i = 0; i < capabls.size(); i++) {
    }

    cv::Size scSize = cv::Size(1280, 400);

    LogI("VLSModule.init():��ʼ������...");

    CameraManger::GetInst()->clear();
    CameraManger::GetInst()->add(std::make_shared<Camera>(Profile::GetInst()->cameraName));
}

} // namespace dxlib
