#pragma once

namespace dxlib {

/**
 * 共享内存数据定义.
 *
 * @author daixian
 * @date 2020/6/24
 */
struct LightSensorData
{
    // 驱动版本
    int DriverVer;

    // windows服务程序状态，当它不为0之后表示有windows服务器对接上了。
    int WinServerStatus;

    float Lux;
    float Kelvins;
    float ChromaticityX;
    float ChromaticityY;
    bool IsValid;
};

/**
 * 一个公共的MappingFile单例.
 *
 * @author daixian
 * @date 2020/6/24
 */
class MappingFile
{
  public:
    MappingFile();
    ~MappingFile();

    static MappingFile* GetInst();

    // 全局的LightSensorData.
    LightSensorData* LSData = nullptr;

    /**
     * 初始化的时候打开内存共享文件.
     *
     * @author daixian
     * @date 2020/6/24
     *
     * @returns 成功返回0,失败返回-1.
     */
    int init();

    /**
     * 初始化一些结果的初始值.
     *
     * @author daixian
     * @date 2020/6/24
     */
    void reset();

    /**
     * 写Lux值到共享内存.
     *
     * @author daixian
     * @date 2020/6/24
     *
     * @param  lux 要写的lux值.
     */
    void writeLux(float lux);

    /**
     * 从共享内存读Lux值.
     *
     * @author daixian
     * @date 2020/6/24
     *
     * @returns The lux.
     */
    float readLux();

  private:
    static MappingFile* m_pInstance;

    class Impl;
    Impl* _impl;
};
} // namespace dxlib