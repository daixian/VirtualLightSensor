#include "VLSProc.h"
#include "CVSystem/DShow/UVCCamera.h"
#include "../data/Profile.h"
#include "../Shared/MappingFile.h"

namespace dxlib {

class VLSProc::Impl
{
  public:
    Impl() {}
    ~Impl() {}

  private:
};

VLSProc::VLSProc()
{
    _impl = new Impl();
}

VLSProc::~VLSProc()
{
    delete _impl;
    onDisable();
}

void VLSProc::process(pCameraImage camImage, int& key)
{
    //如果是手动的
    if (isManual) {
        return;
    }

    //应该只有一个相机
    CV_Assert(camImage->vImage.size() == 1);

    if (camImage->vImage[0].isSuccess) {
        cv::Mat& image = camImage->vImage[0].image;

        cv::Mat gray;
        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY); //COLOR_BGR2HLS
        cv::Scalar mean = cv::mean(gray);
        float value = mean[0];

        if (value < 25) {
            MappingFile::GetInst()->writeLux(value);
        }
        else {
            MappingFile::GetInst()->writeLux(value * 3);
        }
    }
}

void VLSProc::onLightSleep(int& key)
{
}

void VLSProc::onEnable()
{
    UVCCamera uvc;
    uvc.connectDevice(Profile::GetInst()->cameraName);

    //关闭摄像头的自动曝光
    uvc.setAutoExposure(false);
    uvc.setLowLightCompensation(false);

    //调试用,所有的当前属性
    auto allProp = uvc.getAllProp();
    uvc.disconnectDevice();
}

void VLSProc::onDisable()
{
}

} // namespace dxlib