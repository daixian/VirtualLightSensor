#pragma once
#define XUEXUE_JSON_SUPPORT_OPENCV
#define XUEXUE_JSON_SUPPORT_EIGEN
#include "xuexuejson/Serialize.hpp"

namespace dxlib {

/**
 * 全局的配置文件.
 *
 * @author daixian
 * @date 2020/5/26
 */
class Profile : XUEXUE_JSON_OBJECT
{
  public:
    Profile();
    ~Profile();

    static Profile* GetInst();

    /**
     * 载入配置文件
     *
     * @author daixian
     * @date 2020/5/26
     *
     * @param  path 配置文件路径.
     */
    void load(std::string path);

    // 相机的名字
    std::string cameraName;

    XUEXUE_JSON_OBJECT_M1(cameraName)

  private:
    static Profile* m_pInstance;

    class Impl;
    Impl* _impl;
};
} // namespace dxlib