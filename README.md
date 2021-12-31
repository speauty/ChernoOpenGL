## Cherno 系列教程代码项目仓库

### 当前环境

* 主机

  ```
  版本	Windows 10 专业版
  版本号	20H2
  安装日期	2021-05-12
  操作系统内部版本	19042.1415
  体验	Windows Feature Experience Pack 120.2212.3920.0
  
  ```

* IDE 

  ```
  Microsoft Visual Studio Community 2022 (64 位) 版本 17.0.4
  ```

### P2. 在VS中创建项目, 配置GLFW和OpenGL

* 配置 [GLFW](https://www.glfw.org/)

  [下载转送门](https://github.com/glfw/glfw/releases/download/3.3.6/glfw-3.3.6.zip)

  ```c
  /**
   * 设置附加包含目录
   * 项目 => OpenGL属性 => C\C++ => 常规 => 附加包含目录
   * 添加: $(SolutionDir)Dependencies\GLFW\include
   * 设置附加库目录
   * 项目 => OpenGL属性 => 链接器 => 常规 => 附加库目录
   * 添加: $(SolutionDir)Dependencies\GLFW\lib-vc2022
   * 设置附加依赖项
   * 项目 => OpenGL属性 => 链接器 => 输入 => 附加依赖项
   * 添加: glfw3.lib
   ** /
  ```
  
* 处理错误

  * `Application.obj : error LNK2019: 无法解析的外部符号 __imp__glClear@4，函数 _main 中引用了该符号`

    ```c
    /**
     * 补充附加依赖项(opengl32.lib)
     * 项目 => OpenGL属性 => 链接器 => 输入 => 附加依赖项
     * 添加: opengl32.lib
     **/
    ```
  
  * `glfw3.lib(win32_init.obj) : error LNK2019: 无法解析的外部符号 __imp__RegisterDeviceNotificationW@12，函数 __glfwPlatformInit 中引用了该符号`
  
    ```c
    /**
     * 补充附加依赖项(user32.lib)
     * 项目 => OpenGL属性 => 链接器 => 输入 => 附加依赖项
     * 添加: user32.lib
     **/
  	```

  * `glfw3.lib(win32_monitor.obj) : error LNK2019: 无法解析的外部符号 __imp__CreateDCW@16，函数 __glfwPlatformGetGammaRamp 中引用了该符号`

    ```c
    /**
     * 补充附加依赖项(gdi32.lib)
     * 项目 => OpenGL属性 => 链接器 => 输入 => 附加依赖项
     * 添加: gdi32.lib
     **/
    ```

  * `glfw3.lib(win32_window.obj) : error LNK2019: 无法解析的外部符号 __imp__DragQueryFileW@16，函数 _windowProc@16 中引用了该符号`

    ```c
    /**
     * 补充附加依赖项(shell32.lib)
     * 项目 => OpenGL属性 => 链接器 => 输入 => 附加依赖项
     * 添加: shell32.lib
     **/
    ```

  * `LINK : warning LNK4098: 默认库“MSVCRT”与其他库的使用冲突；请使用 /NODEFAULTLIB:library`

    ```c
    /**
     * 忽略特定默认库(msvcrt.lib)
     * 项目 => OpenGL属性 => 链接器 => 输入 => 忽略特定默认库
     * 添加: msvcrt.lib
     **/
    ```

* 当前附加依赖库 `glfw3.lib;opengl32.lib;user32.lib;gdi32.lib;shell32.lib`

### P3. 配置GLEW
下载[传送门](https://sourceforge.net/projects/glew/files/glew/2.1.0/glew-2.1.0.zip/download)
```c
  /**
   * 设置附加包含目录
   * 项目 => OpenGL属性 => C\C++ => 常规 => 附加包含目录
   * 添加: $(SolutionDir)Dependencies\GLEW\include
   * 设置附加库目录
   * 项目 => OpenGL属性 => 链接器 => 常规 => 附加库目录
   * 添加: $(SolutionDir)Dependencies\GLEW\lib\Release\Win32
   * 设置附加依赖项
   * 项目 => OpenGL属性 => 链接器 => 输入 => 附加依赖项
   * 添加: glew32s.lib
   *
   * 设置预处理器定义
   * 项目 => OpenGL属性 => C\C++ => 预处理器 => 预处理器定义
   * 添加: GLEW_STATIC
   ** /
```

* 错误处理
  
   * `fatal error C1189: #error:  gl.h included before glew.h`
   
     ```c
     /**
      * 交换glew.h和glfw3.h的包含顺序
      **/
     ```
   
   * `Application.obj : error LNK2019: 无法解析的外部符号 __imp__glewInit@0，函数 _main 中引用了该符号`
   
     ```c
     /**
      * 添加预处理器定义
      * 项目 => OpenGL属性 => C\C++ => 预处理器 => 预处理器定义
      * 添加: GLEW_STATIC
      **/
     ```
   
   * `LINK : warning LNK4098: 默认库“LIBCMT”与其他库的使用冲突；请使用 /NODEFAULTLIB:library`
   
     ```c
     /**
      * 忽略特定默认库(libcmt.lib)
      * 项目 => OpenGL属性 => 链接器 => 输入 => 忽略特定默认库
      * 添加: libcmt.lib
      **/
     ```
   
   * `glew32s.lib(glew.obj) : warning LNK4099: 未找到 PDB“vc120.pdb”(使用glew32s.lib(glew.obj)`
   
     ```c
     /**
      * 关闭生成调试信息
      * 项目 => OpenGL属性 => 链接器 => 调试 => 生成调试信息(否, 默认值: 生成调试信息 (/DEBUG))
      **/
     ```
   
* 切换NIVDIA显卡

   ```c
   /**
    * NVIDIA控制面板 => 管理3D设置 => 程序设置 => 添加(选择VS)
    * NVIDIA控制面板 => 管理3D设置 => 全局设置 => 首先图形处理器(选择高性能NVIDIA处理器)
    * glGetString(GL_VERSION)输出: 4.6.0 NVIDIA 471.41
    * 默认显卡(Inter)时输出: 4.6.0 - Build 26.20.100.8141
    *
    * 可在 计算机 => 属性 => 设备管理器 => 显示适配器 查看当前支持的显卡
    **/
   ```