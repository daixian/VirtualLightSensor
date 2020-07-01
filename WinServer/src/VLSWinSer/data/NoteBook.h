#pragma once
#include "CVSystem/DShow/UVCCamera.h"

namespace dxlib {

/**
 * 笔记本硬件信息.
 *
 * @author daixian
 * @date 2020/5/26
 */
class NoteBook : XUEXUE_JSON_OBJECT
{
  public:
    NoteBook();
    ~NoteBook();

    static NoteBook* GetInst();

    /**
     * 载入配置文件
     *
     * @author daixian
     * @date 2020/5/26
     *
     * @param  path 配置文件路径.
     */
    void load(std::string path);

    // 笔记本设备型号
    std::string noteBookType;

    // 创建者
    std::string creator;

    // 相机的名字
    std::string cameraName;

    // 相机的默认属性信息(实际只看默认信息)
    std::vector<UVCProp> defaultCameraProp;

    XUEXUE_JSON_OBJECT_M3(noteBookType, creator, cameraName)

  private:
    static NoteBook* m_pInstance;

    class Impl;
    Impl* _impl;
};
} // namespace dxlib