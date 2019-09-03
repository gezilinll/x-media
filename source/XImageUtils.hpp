//
// Created by 林炳河 on 2019/7/21.
//

#ifndef GPUIMAGE_X_XIMAGEUTILS_HPP
#define GPUIMAGE_X_XIMAGEUTILS_HPP

#include "bgfx/bgfx.h"
#include "bgfx_utils.h"
#include "XMacros.hpp"
#include <string>

NS_X_IMAGE_BEGIN

/**
 * @brief XImage's utils to make develop easier.
 */
class XImageUtils {
public:
    /**
     * @brief wrap float value to vec4 needed form
     * @param[in] value float value
     * @return float array, length:4
     */
    static float* wrapFloatToVec4(float value);

    /**
     * @brief wrap three float value to vec4 needed form
     * @param[in] x first float value
     * @param[in] y second float value
     * @param[in] z third float value
     * @return float array, length is 4
     */
    static float* wrapVec3ToVec4(float x, float y, float z);

    /**
     * @brief wrap three float value to vec4 needed form
     * @param[in] xyz length is 3
     * @return float array, length is 4
     */
    static float* wrapVec3ToVec4(float* xyz);

    /**
     * @brief destroy program loaded by bgfx safely
     * @param[in] handle loaded program handle
     */
    static void destroy(bgfx::ProgramHandle &handle);

    /**
     * @brief destroy frame buffer loaded by bgfx safely
     * @param[in] handle loaded frame buffer handle
     */
    static void destroy(bgfx::FrameBufferHandle &handle);

    /**
     * @brief destroy texture loaded by bgfx safely
     * @param[in] handle loaded texture handle
     */
    static void destroy(bgfx::TextureHandle &handle);

    /**
     * @brief destroy vertex buffer loaded by bgfx safely
     * @param[in] handle loaded vertex buffer handle
     */
    static void destroy(bgfx::VertexBufferHandle &handle);



    /**
     * @brief destroy index buffer loaded by bgfx safely
     * @param[in] handle loaded index buffer handle
     */
    static void destroy(bgfx::IndexBufferHandle &handle);

    /**
     * @brief destroy uniform created by bgfx safely
     * @param[in] handle created uniform handle
     */
    static void destroy(bgfx::UniformHandle &handle);

};
NS_X_IMAGE_END

#endif //GPUIMAGE_X_XIMAGEUTILS_HPP
