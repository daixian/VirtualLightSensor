#pragma once
#include "CVSystem/CVSystem.h"

namespace dxlib {

/**
 * 虚拟光线传感器的Proc.
 *
 * @author daixian
 * @date 2020/6/30
 */
class VLSProc : public FrameProc
{
  public:
    VLSProc();

    ~VLSProc();

    /**
     * 处理函数接口,返回值作为一个key值,如果没有按下按键,那么这里返回-1(和waitkey()的无按键返回值相同).
     *
     * @author daixian
     * @date 2019/3/19
     *
     * @param          camImage 采集的处理图像.
     * @param [in,out] key      这里waitkey()得到的按键key值.
     */
    void process(pCameraImage camImage, int& key) override;

    /**
     * 浅睡眠,此时相机没有抓图,没有采图.但是会定时300ms传出这个事件来可以执行一些函数.
     *
     * @author daixian
     * @date 2019/4/14
     *
     * @param [in,out] key The key.
     */
    void onLightSleep(int& key) override;

    /**
     * 被使能的时候的响应.
     *
     * @author daixian
     * @date 2018/10/18
     */
    void onEnable() override;

    /**
     * 不使能的时候的响应.
     *
     * @author daixian
     * @date 2018/9/28
     */
    void onDisable() override;

    /**
     * 当前proc的标示名.
     *
     * @author daixian
     * @date 2019/3/19
     *
     * @returns 当前proc的标示名.
     */
    const char* name() override
    {
        return "VLSProc";
    }

  private:
    class Impl;
    Impl* _impl = nullptr;
};

} // namespace dxlib