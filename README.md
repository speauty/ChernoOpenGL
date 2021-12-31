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