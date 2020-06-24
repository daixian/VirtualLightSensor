# 记录
## 一、项目简介
### 目标
买了一台某神船的战神笔记本，可是笔记本居然没有光线传感器，无法自动调节屏幕亮度。考虑利用笔记本的前置摄像头来感知当前环境光亮度，写一个驱动程序来虚拟光线传感器，最终实现一定程度的自动调节亮度。

### 实现
项目大致分为两个部分，一部分是虚拟的光线传感器驱动。一部分是使用opencv来调用摄像头的windows服务程序。它们之间使用共享内存通信数据（不一定是最佳实现）。windows服务程序通过前置摄像头来计算出一个亮度结果lux，将这个结果提供给光线传感器驱动使用。

它应该会存在如下问题：
* 实时性：无法实时。由于使用前置摄像头必然会带来一定的CPU开销。所以不会一直打开摄像头来感光，只会隔一段时间（如10分钟或更长）打开摄像头来看一下当前的环境亮度。
* 亮度的精度：不精确。这只是简单的使用摄像头来估计一个环境光亮度，它也不精确。

不过它解决了这些笔记本光线传感器的有无问题。。。

### 文档
* [环境光传感器白皮书（必读）](https://docs.microsoft.com/en-us/windows-hardware/design/whitepapers/integrating-ambient-light-sensors-with-computers-running-windows-10-creators-update)

## 二、Windows驱动开发相关
第一次接触Windows驱动开发，简单记录一下。。。
### 开发环境安装
1. 首先安装vs2019(安装C++开发相关部分)。
2. 还需要安装win10 SDK和WDK。安装程序下载下来分别是两个文件winsdksetup.exe和wdksetup.exe，先安装Win10SDK再安装WD。
    * WIN10 SDK下载:https://developer.microsoft.com/zh-cn/windows/downloads/windows-10-sdk/
    * WDK的下载安装(不需要EWDK):https://docs.microsoft.com/en-us/windows-hardware/drivers/download-the-wdk

### 准备要预配的目标计算机
1. 在目标计算机上，安装将用于运行和测试驱动程序的操作系统。
2. 安装 WDK。 不过，你不需要安装 Visual Studio，除非你计划在目标计算机上进行驱动程序开发。
3. 在目标计算机上，运行与目标计算机平台匹配的 WDK 测试目标设置 MSI。 可以在 Remote 下的 Windows 驱动程序工具包 (WDK) 安装目录中找到该 MSI。
例如：C:\Program Files (x86)\Windows Kits\10\Remote\x64\WDK Test Target Setup x64-x64_en-us.msi
*  安装好了之后注意在服务里有一个服务WDK Remote Communication Service，需要确认它启动了才能Remote连上。

### 文档
* [Windows 驱动程序入门](https://docs.microsoft.com/zh-cn/windows-hardware/drivers/gettingstarted/)
* [基于模板编写通用 Windows 驱动程序 (UMDF 2)](https://docs.microsoft.com/zh-cn/windows-hardware/drivers/gettingstarted/writing-a-umdf-driver-based-on-a-template) 
* [Windows-driver-samples](https://github.com/microsoft/Windows-driver-samples)
* [INF 节摘要](https://docs.microsoft.com/zh-cn/windows-hardware/drivers/install/summary-of-inf-sections)

## 三、开发记录
* 遇到了无法打开文件的问题，使用C的fopen()创建任何文件，试了多个路径均没成功。有[说法](https://community.osr.com/discussion/198727/accessing-file-system-from-with-a-umdf)说UMDF驱动程序在LocalService帐户下运行。如果此帐户可以访问文件，则可以访问该文件。
    * 后来决定直接使用共享内存算了。
* 创建国际INF文件没有成功，不知道是什么原因。
    ``` ini
    [Strings]
    ;Localizable Strings
    MediaDescription           = "Virtual Light Sensor Driver"
    PROVIDER                   = "daixian"
    VirtualLightSensorDriver_DevDesc = "VirtualLightSensor"
    WudfRdDisplayName          = "Windows Driver Foundation - User-mode Driver Framework Reflector"
    ;Non-Localizable Strings
    SERVICE_KERNEL_DRIVER    = 1
    SERVICE_DEMAND_START     = 3
    SERVICE_ERROR_NORMAL     = 1

    [Strings.0804]
    ;Localizable Strings
    MediaDescription           = "Virtual Light Sensor Driver"
    PROVIDER                   = "daixian"
    VirtualLightSensorDriver_DevDesc = "VirtualLightSensor"
    WudfRdDisplayName          = "Windows Driver Foundation - User-mode Driver Framework Reflector"
    ;Non-Localizable Strings
    SERVICE_KERNEL_DRIVER    = 1
    SERVICE_DEMAND_START     = 3
    SERVICE_ERROR_NORMAL     = 1
    ```
    报错error 1295: Section [strings] contains a different number of strings than section [strings.0804].

### 文档
* [OSLangCodes](https://www.autoitscript.com/autoit3/docs/appendix/OSLangCodes.htm)