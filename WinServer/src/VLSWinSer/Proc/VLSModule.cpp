#include "VLSModule.h"
#include "CVSystem/DShow/UVCCamera.h"
#include "../data/Profile.h"
#include "VLSProc.h"

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

    std::vector<std::string> devList;
    UVCCamera::listDevices(devList);

    Profile* profile = Profile::GetInst();

    //如果只有一个设备,那么就使用这一个?(为了前期调试方便)
    if (devList.size() == 1) {
        profile->cameraName = devList[0];
    }

    bool res = uvc.connectDevice(profile->cameraName);
    if (res) {
        //配一下相机名
        for (size_t i = 0; i < devList.size(); i++) {
            if (std::string(devList[i]).find(profile->cameraName) != std::string::npos) {
                profile->cameraName = devList[i];
            }
        }

        auto capabls = uvc.getCapabilities();
        //选择一个最小的分辨率
        int miniSizeIndex = 0;
        long long minSize = 4096 * 4096;
        for (int i = 0; i < capabls.size(); i++) {
            long long inputSize = capabls[i].InputSize.cx * capabls[i].InputSize.cy;
            if (inputSize < minSize) {
                minSize = inputSize;
                miniSizeIndex = i;
            }
        }

        cv::Size camSize = cv::Size(capabls[miniSizeIndex].InputSize.cx, capabls[miniSizeIndex].InputSize.cy);

        LogI("VLSModule.go():初始化设置...");

        CameraManger::GetInst()->clear();
        auto cam = CameraManger::GetInst()->add(
            std::make_shared<Camera>(profile->cameraName,
                                     camSize,
                                     uvc.getBrightness().Default));

        //cam->setProp(CV_CAP_PROP_AUTO_EXPOSURE, 0);
        cam->setProp(cv::CAP_PROP_EXPOSURE, uvc.getExposure().Default);
        cam->setFourcc("MJPG"); //使用MJPG因为它的帧率更高曝光时间更确定

        MultiCamera::GetInst()->clearProc();
        MultiCamera::GetInst()->addProc(pFrameProc(new VLSProc()));

        //打开相机
        MultiCamera::GetInst()->openCamera();

        uvc.setAutoFocus(false);
        uvc.setAutoExposure(false);
        uvc.setLowLightCompensation(false);

        uvc.disconnectDevice();

        //开始处理线程
        MultiCamera::GetInst()->start();
    }
    else {
        LogE("VLSModule.go():无法连接摄像头%s", profile->cameraName.c_str());
    }
}

} // namespace dxlib
