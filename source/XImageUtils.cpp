//
// Created by 林炳河 on 2019/7/21.
//

#include "XImageUtils.hpp"
#include "XLog.hpp"

NS_X_IMAGE_BEGIN
#ifndef ENTRY_CONFIG_IMPLEMENT_DEFAULT_ALLOCATOR
#    define ENTRY_CONFIG_IMPLEMENT_DEFAULT_ALLOCATOR 1
#endif // ENTRY_CONFIG_IMPLEMENT_DEFAULT_ALLOCATOR
// bgfx utils

static bx::FileReaderI *sFileReader = nullptr;

extern bx::AllocatorI *getDefaultAllocator();

bx::AllocatorI *allocator = getDefaultAllocator();

typedef bx::StringT<&allocator> String;

class FileReader : public bx::FileReader {
    typedef bx::FileReader super;

public:
    virtual bool open(const bx::FilePath &filePath, bx::Error *err) override {
        String resultFilePath("");
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
    BX_PRAGMA_DIAGNOSTIC_POP();
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
    } else {

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

const bgfx::Memory *XImageUtils::loadMem(bx::FileReaderI *reader, const char *filePath) {
    if (bx::open(reader, filePath)) {
        uint32_t size = (uint32_t) bx::getSize(reader);
        const bgfx::Memory *mem = bgfx::alloc(size + 1);
        bx::read(reader, mem->data, size);
        bx::close(reader);
        mem->data[mem->size - 1] = '\0';
        return mem;
    }

    LOGE("XImageUtils::loadMem open failed.");
    return nullptr;
}

bx::FileReaderI *XImageUtils::getFileReader() {
    /// @todo concurrent
    if (sFileReader == nullptr) {
        sFileReader = BX_NEW(allocator, FileReader);
    }
    return sFileReader;
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
NS_X_IMAGE_END