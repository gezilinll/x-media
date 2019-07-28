//
// Created by 林炳河 on 2019/7/21.
//

#include "XImageUtils.hpp"
#include "XLog.hpp"
#include "bx/commandline.h"

NS_X_IMAGE_BEGIN

static bx::FileReaderI *sFileReader = nullptr;

extern bx::AllocatorI *getDefaultAllocator();

bx::AllocatorI *allocator = getDefaultAllocator();

typedef bx::StringT<&allocator> String;

class FileReader : public bx::FileReader {
    typedef bx::FileReader super;

public:
    virtual bool open(const bx::FilePath &filePath, bx::Error *err) override {
        String resultFilePath(ENTRY_CONFIG_WORKING_DIRECTORY);
        resultFilePath.append(filePath);
        return super::open(resultFilePath.getPtr(), err);
    }
};

#if ENTRY_CONFIG_IMPLEMENT_DEFAULT_ALLOCATOR

bx::AllocatorI *getDefaultAllocator() {
    BX_PRAGMA_DIAGNOSTIC_PUSH();
    BX_PRAGMA_DIAGNOSTIC_IGNORED_MSVC(4459); // warning C4459: declaration of 'sAllocator' hides global declaration
    BX_PRAGMA_DIAGNOSTIC_IGNORED_CLANG_GCC("-Wshadow");
    static bx::DefaultAllocator sAllocator;
    return &sAllocator;
}

#endif // ENTRY_CONFIG_IMPLEMENT_DEFAULT_ALLOCATOR

void *load(bx::FileReaderI *reader, bx::AllocatorI *allocator, const char *filePath, uint32_t *size) {
    if (bx::open(reader, filePath)) {
        uint32_t readSize = (uint32_t) bx::getSize(reader);
        void *data = BX_ALLOC(allocator, readSize);
        bx::read(reader, data, readSize);
        bx::close(reader);
        if (NULL != size) {
            *size = readSize;
        }
        return data;
    }

    if (NULL != size) {
        *size = 0;
    }

    return NULL;
}

static void imageReleaseCb(void *ptr, void *userData) {
    BX_UNUSED(ptr);
    bimg::ImageContainer *imageContainer = (bimg::ImageContainer *) userData;
    bimg::imageFree(imageContainer);
}

void unload(void *_ptr) {
    BX_FREE(allocator, _ptr);
}

bgfx::TextureHandle
XImageUtils::loadTexture(bx::FileReaderI *reader, const char *filePath, uint64_t flags, uint8_t skip,
                         bgfx::TextureInfo *info, bimg::Orientation::Enum *orientation) {
    BX_UNUSED(skip);
    bgfx::TextureHandle handle = BGFX_INVALID_HANDLE;

    uint32_t size;
    if (NULL == allocator) {
        allocator = getDefaultAllocator();
    }
    void *data = load(reader, allocator, filePath, &size);
    if (NULL != data) {
        bimg::ImageContainer *imageContainer = bimg::imageParse(allocator, data, size);

        if (NULL != imageContainer) {
            if (NULL != orientation) {
                *orientation = imageContainer->m_orientation;
            }

            const bgfx::Memory *mem = bgfx::makeRef(
                    imageContainer->m_data, imageContainer->m_size, imageReleaseCb, imageContainer
            );
            unload(data);

            if (imageContainer->m_cubeMap) {
                handle = bgfx::createTextureCube(
                        uint16_t(imageContainer->m_width), 1 < imageContainer->m_numMips, imageContainer->m_numLayers,
                        bgfx::TextureFormat::Enum(imageContainer->m_format), flags, mem
                );
            } else if (1 < imageContainer->m_depth) {
                handle = bgfx::createTexture3D(
                        uint16_t(imageContainer->m_width), uint16_t(imageContainer->m_height),
                        uint16_t(imageContainer->m_depth), 1 < imageContainer->m_numMips,
                        bgfx::TextureFormat::Enum(imageContainer->m_format), flags, mem
                );
            } else if (bgfx::isTextureValid(0, false, imageContainer->m_numLayers,
                                            bgfx::TextureFormat::Enum(imageContainer->m_format), flags)) {
                handle = bgfx::createTexture2D(
                        uint16_t(imageContainer->m_width), uint16_t(imageContainer->m_height),
                        1 < imageContainer->m_numMips, imageContainer->m_numLayers,
                        bgfx::TextureFormat::Enum(imageContainer->m_format), flags, mem
                );
            }

            if (bgfx::isValid(handle)) {
                bgfx::setName(handle, filePath);
            }

            if (NULL != info) {
                bgfx::calcTextureSize(
                        *info, uint16_t(imageContainer->m_width), uint16_t(imageContainer->m_height),
                        uint16_t(imageContainer->m_depth), imageContainer->m_cubeMap, 1 < imageContainer->m_numMips,
                        imageContainer->m_numLayers, bgfx::TextureFormat::Enum(imageContainer->m_format)
                );
            }
        }
    }

    return handle;
}

bgfx::TextureHandle
XImageUtils::loadTexture(const char *path, uint64_t flags, uint8_t skip, bgfx::TextureInfo *info,
                         bimg::Orientation::Enum *orientation) {
    return loadTexture(getFileReader(), path, flags, skip, info, orientation);
}

bgfx::ProgramHandle XImageUtils::loadProgram(const char *vsPath, const char *fsPath) {
    return loadProgram(getFileReader(), vsPath, fsPath);
}

bgfx::ProgramHandle XImageUtils::loadProgram(bx::FileReaderI *reader, const char *vsPath, const char *fsPath) {
    bgfx::ShaderHandle vsh = loadShader(reader, vsPath);
    bgfx::ShaderHandle fsh = BGFX_INVALID_HANDLE;
    if (NULL != fsPath) {
        fsh = loadShader(reader, fsPath);
    }

    return bgfx::createProgram(vsh, fsh, true /* destroy shaders when program is destroyed */);
}

bgfx::ShaderHandle XImageUtils::loadShader(bx::FileReaderI *reader, const char *path) {
    bgfx::ShaderHandle handle = bgfx::createShader(loadMem(reader, path));
    bgfx::setName(handle, path);

    return handle;
}

bimg::ImageContainer* XImageUtils::loadImage(const char *filePath, bgfx::TextureFormat::Enum dstFormat) {
    uint32_t size = 0;
    void* data = loadMem(getFileReader(), getAllocator(), filePath, &size);

    return bimg::imageParse(getAllocator(), data, size, bimg::TextureFormat::Enum(dstFormat) );
}

const bgfx::Memory *XImageUtils::loadMem(bx::FileReaderI *reader, const char *filePath) {
    if (bx::open(reader, filePath)) {
        uint32_t size = (uint32_t) bx::getSize(reader);
        const bgfx::Memory *mem = bgfx::alloc(size + 1);
        bx::read(reader, mem->data, size);
        bx::close(reader);
        mem->data[mem->size - 1] = '\0';
        return mem;
    }

    LOGE("XImageUtils::loadMem load failed. %s", filePath);
    return nullptr;
}

void* XImageUtils::loadMem(bx::FileReaderI *reader, bx::AllocatorI *allocator, const char *filePath, uint32_t *size) {
    if (bx::open(reader, filePath) )
    {
        uint32_t fileSize = (uint32_t)bx::getSize(reader);
        void* data = BX_ALLOC(getAllocator(), fileSize);
        bx::read(reader, data, fileSize);
        bx::close(reader);

        if (NULL != size)
        {
            *size = fileSize;
        }
        return data;
    }

    LOGE("XImageUtils::loadMem(with size) load failed. %s", filePath);
    return NULL;
}

bx::FileReaderI *XImageUtils::getFileReader() {
    /// @todo concurrent
    if (sFileReader == nullptr) {
        LOGD("XImageUtils::getFileReader ENTRY_CONFIG_WORKING_DIRECTORY=%s", ENTRY_CONFIG_WORKING_DIRECTORY);
        sFileReader = BX_NEW(allocator, FileReader);
    }
    return sFileReader;
}

bx::AllocatorI* XImageUtils::getAllocator() {
    if (nullptr == allocator)
    {
        allocator = getDefaultAllocator();
    }

    return allocator;
}

float* XImageUtils::wrapFloatToVec4(float value) {
    return new float[4]{value, 1.0f, 1.0f, 1.0f};
}

float* XImageUtils::wrapVec3ToVec4(float x, float y, float z) {
    return new float[4]{x, y, z, 1.0f};
}

float* XImageUtils::wrapVec3ToVec4(float *xyz) {
    return new float[4]{xyz[0], xyz[1], xyz[2], 1.0f};
}

void XImageUtils::destroy(bgfx::ProgramHandle &handle) {
    if (bgfx::isValid(handle)) {
        bgfx::destroy(handle);
        handle = BGFX_INVALID_HANDLE;
    }
}

void XImageUtils::destroy(bgfx::TextureHandle &handle) {
    if (bgfx::isValid(handle)) {
        bgfx::destroy(handle);
        handle = BGFX_INVALID_HANDLE;
    }
}

void XImageUtils::destroy(bgfx::FrameBufferHandle &handle) {
    if (bgfx::isValid(handle)) {
        bgfx::destroy(handle);
        handle = BGFX_INVALID_HANDLE;
    }
}

void XImageUtils::destroy(bgfx::VertexBufferHandle &handle) {
    if (bgfx::isValid(handle)) {
        bgfx::destroy(handle);
        handle = BGFX_INVALID_HANDLE;
    }
}

void XImageUtils::destroy(bgfx::IndexBufferHandle &handle) {
    if (bgfx::isValid(handle)) {
        bgfx::destroy(handle);
        handle = BGFX_INVALID_HANDLE;
    }
}

void XImageUtils::destroy(bgfx::UniformHandle &handle) {
    if (bgfx::isValid(handle)) {
        bgfx::destroy(handle);
        handle = BGFX_INVALID_HANDLE;
    }
}

XImageUtils::Args::Args(int argc, const char *const *argv)
        : rendererType(bgfx::RendererType::Count), pciId(BGFX_PCI_ID_NONE) {
    bx::CommandLine cmdLine(argc, (const char**)argv);

    if (cmdLine.hasArg("gl") )
    {
        rendererType = bgfx::RendererType::OpenGL;
    }
    else if (cmdLine.hasArg("vk") )
    {
        rendererType = bgfx::RendererType::Vulkan;
    }
    else if (cmdLine.hasArg("noop") )
    {
        rendererType = bgfx::RendererType::Noop;
    }
    else if (BX_ENABLED(BX_PLATFORM_WINDOWS|BX_PLATFORM_WINRT|BX_PLATFORM_XBOXONE) )
    {
        if (cmdLine.hasArg("d3d9") )
        {
            rendererType = bgfx::RendererType::Direct3D9;
        }
        else if (cmdLine.hasArg("d3d11") )
        {
            rendererType = bgfx::RendererType::Direct3D11;
        }
        else if (cmdLine.hasArg("d3d12") )
        {
            rendererType = bgfx::RendererType::Direct3D12;
        }
    }
    else if (BX_ENABLED(BX_PLATFORM_OSX) )
    {
        if (cmdLine.hasArg("mtl") )
        {
            rendererType = bgfx::RendererType::Metal;
        }
    }

    if (cmdLine.hasArg("amd") )
    {
        pciId = BGFX_PCI_ID_AMD;
    }
    else if (cmdLine.hasArg("nvidia") )
    {
        pciId = BGFX_PCI_ID_NVIDIA;
    }
    else if (cmdLine.hasArg("intel") )
    {
        pciId = BGFX_PCI_ID_INTEL;
    }
    else if (cmdLine.hasArg("sw") )
    {
        pciId = BGFX_PCI_ID_SOFTWARE_RASTERIZER;
    }
}

NS_X_IMAGE_END
