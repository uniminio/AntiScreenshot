# 禁止截图此应用程序

实现方式为Hook Window API BitBlt

**32位程序只能禁止32位程序的截图，比如QQ截图**

**64位程序只能禁止64位程序的截图，比如Windows自带的截图工具**

使用Detours库实现

# 环境需求

编译环境:VS2017，需安装MFC

```sh
vcpkg install detours:x86-windows detours:x64-windows
```




