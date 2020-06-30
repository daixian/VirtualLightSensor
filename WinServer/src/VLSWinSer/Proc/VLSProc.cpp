#include "VLSProc.h"

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
}

void VLSProc::onDisable()
{
}
} // namespace dxlib