#pragma once
#include "CVSystem/CVSystem.h"

namespace dxlib {

/**
 * ������ߴ�������Proc.
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
     * �������ӿ�,����ֵ��Ϊһ��keyֵ,���û�а��°���,��ô���ﷵ��-1(��waitkey()���ް�������ֵ��ͬ).
     *
     * @author daixian
     * @date 2019/3/19
     *
     * @param          camImage �ɼ��Ĵ���ͼ��.
     * @param [in,out] key      ����waitkey()�õ��İ���keyֵ.
     */
    void process(pCameraImage camImage, int& key) override;

    /**
     * ǳ˯��,��ʱ���û��ץͼ,û�в�ͼ.���ǻᶨʱ300ms��������¼�������ִ��һЩ����.
     *
     * @author daixian
     * @date 2019/4/14
     *
     * @param [in,out] key The key.
     */
    void onLightSleep(int& key) override;

    /**
     * ��ʹ�ܵ�ʱ�����Ӧ.
     *
     * @author daixian
     * @date 2018/10/18
     */
    void onEnable() override;

    /**
     * ��ʹ�ܵ�ʱ�����Ӧ.
     *
     * @author daixian
     * @date 2018/9/28
     */
    void onDisable() override;

    /**
     * ��ǰproc�ı�ʾ��.
     *
     * @author daixian
     * @date 2019/3/19
     *
     * @returns ��ǰproc�ı�ʾ��.
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