#!/usr/bin/env python
# coding=UTF-8

import os


def list_all_dirs(rootdir):
    import os
    _files = []
    list = os.listdir(rootdir)  # 列出文件夹下所有的目录与文件
    for i in range(0, len(list)):
        path = os.path.join(rootdir, list[i])
        if os.path.isdir(path):
            _files.append(path)
    return _files


os.chdir("shaders")
config = open("config", "r")
for line in config.readlines():
    line = line.strip()
    if line:
        config_type = line.split(":")[0].split("_")[0]
        type = "fragment" if config_type == "FS" else "vertex"
        name = line.split(":")[0].split("_")[1]
        print "----------compile: " + name + " " + type + "----------"
        shader_file = line.split(":")[1].split(",")[0]
        shader_file_name = shader_file.split(".")[0]
        varying_def = line.split(":")[1].split(",")[1]

        # METAL
        command = "../../../github/bgfx/tools/bin/darwin/shaderc -f " + shader_file + " -o " + shader_file_name \
                  + ".bin --depends -i ../../../github/bgfx/src --varyingdef " + varying_def \
                  + " --platform osx -p metal --type " + type
        print "【Metal】: " + command
        os.system(command)
        os.system("cp " + shader_file_name + ".bin ../examples/runtime/shaders/metal/" + shader_file_name + ".bin")
        os.system("rm " + shader_file_name + ".bin")
        os.system("rm " + shader_file_name + ".bin.d")

        # ESSL
        command = "../../../github/bgfx/tools/bin/darwin/shaderc -f " + shader_file + " -o " + shader_file_name \
                  + ".bin --depends -i ../../../github/bgfx/src --varyingdef " + varying_def \
                  + " --platform linux -p 120 --type " + type
        print "【ESSL】: " + command
        os.system(command)
        os.system("cp " + shader_file_name + ".bin ../examples/runtime/shaders/essl/" + shader_file_name + ".bin")
        os.system("rm " + shader_file_name + ".bin")
        os.system("rm " + shader_file_name + ".bin.d")

        # DX9
        p_type = "ps_3_0" if config_type == "FS" else "vs_3_0"
        command = "../../../github/bgfx/tools/bin/darwin/shaderc -f " + shader_file + " -o " + shader_file_name \
                  + ".bin --depends -i ../../../github/bgfx/src --varyingdef " + varying_def \
                  + " --platform windows -p " + p_type + " --type " + type
        print "【DX9】: " + command
        os.system(command)
        os.system("cp " + shader_file_name + ".bin ../examples/runtime/shaders/dx9/" + shader_file_name + ".bin")
        os.system("rm " + shader_file_name + ".bin")
        os.system("rm " + shader_file_name + ".bin.d")

        # DX11
        p_type = "ps_5_0" if config_type == "FS" else "vs_5_0"
        command = "../../../github/bgfx/tools/bin/darwin/shaderc -f " + shader_file + " -o " + shader_file_name \
                  + ".bin --depends -i ../../../github/bgfx/src --varyingdef " + varying_def \
                  + " --platform linux -p " + p_type + " --type " + type
        print "【DX11】: " + command
        os.system(command)
        os.system("cp " + shader_file_name + ".bin ../examples/runtime/shaders/dx11/" + shader_file_name + ".bin")
        os.system("rm " + shader_file_name + ".bin")
        os.system("rm " + shader_file_name + ".bin.d")

        # PSSL
        command = "../../../github/bgfx/tools/bin/darwin/shaderc -f " + shader_file + " -o " + shader_file_name \
                  + ".bin --depends -i ../../../github/bgfx/src --varyingdef " + varying_def \
                  + " --platform orbis -p pssl --type " + type
        print "【PSSL】: " + command
        os.system(command)
        os.system("cp " + shader_file_name + ".bin ../examples/runtime/shaders/pssl/" + shader_file_name + ".bin")
        os.system("rm " + shader_file_name + ".bin")
        os.system("rm " + shader_file_name + ".bin.d")

        # spirv
        command = "../../../github/bgfx/tools/bin/darwin/shaderc -f " + shader_file + " -o " + shader_file_name \
                  + ".bin --depends -i ../../../github/bgfx/src --varyingdef " + varying_def \
                  + " --platform linux -p spirv --type " + type
        print "【SPIRV】: " + command
        os.system(command)
        os.system("cp " + shader_file_name + ".bin ../examples/runtime/shaders/spirv/" + shader_file_name + ".bin")
        os.system("rm " + shader_file_name + ".bin")
        os.system("rm " + shader_file_name + ".bin.d")
