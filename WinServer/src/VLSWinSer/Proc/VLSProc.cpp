#include "VLSProc.h"
#include "CVSystem/DShow/UVCCamera.h"
#include "../data/Profile.h"

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
    uvc.setBacklightCompensation(false);

    uvc.disconnectDevice();
}

void VLSProc::onDisable()
{
}

} // namespace dxlib