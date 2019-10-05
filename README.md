# GPUImage-X

## Building

目前主要在Mac OS上基于Clion进行开发，其他工具及平台暂未进行验证、适配，因此目前效果查看等建议使用CLion直接打开根目录项目。

## Shader 编译

1. 在 shaders 目录下添加着色器文件
2. 在 shaders/config 文件里添加对应文件的配置信息
3. 在项目根目录下运行python脚本: *python tools/shader_compiler.py*
4. 按需要删除 .idea 以及 cmake-build-debug 目录重新打开项目