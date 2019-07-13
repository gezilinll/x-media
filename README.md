# GPUImage-X

## Building

目前主要在Mac OS上基于Xcode进行开发，因此可通过以下步骤编译出Xcode工程和运行Demo，其他平台的后续补上

```
git submodule update --init --recursive
mkdir build
cd build
cmake -G Xcode ..
open GPUImage-X.xcodeproj
```

做完上述操作后你还需要设置一下Demo的scheme：

1. 打开 *“Edit scheme...”* 
2. 选择 *“Run”* 
3. 勾选 *“Use custom working directory”* ，然后在下面的框中尾部文件选择中选择以下文件夹：

```
examples/runtime
```

