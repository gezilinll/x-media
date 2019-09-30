//
// Created by 林炳河 on 2019/9/28.
//

#ifndef GPUIMAGE_X_XTRANSFORM_HPP
#define GPUIMAGE_X_XTRANSFORM_HPP

#include "effects/XFilterEffect.hpp"
#include "bx/math.h"

NS_X_IMAGE_BEGIN
/**
 * @brief 变换效果，主要提供相机视角变换以及投影变换能力
 */
class XTransform : public XEffect {
public:
    XTransform();

    ~XTransform();

    XInputOutput *get() override ;

    /**
     * @brief 设置相机参数
     * @param eye 相机镜头朝向
     * @param at 相机位置
     * @param up 相机顶部方向
     */
    void setCamera(bx::Vec3 eye, bx::Vec3 at, bx::Vec3 up);

    /**
     * @brief 设置透视投影变换参数
     * @param fovy 视角范围角度
     * @param aspect 裁剪平面宽高比
     * @param near 裁剪近平面
     * @param far 裁剪远平面
     * @param handness 坐标系统类型
     * @attention 该变换与正交投影互斥，最终取最后设置的投影类型
     */
    void setPerspective(float fovy, float aspect, float near, float far,
                        bx::Handness::Enum handness = bx::Handness::Left);

    /**
     * @brief 设置正交投影变换参数
     * @param left 左位置
     * @param right 右位置
     * @param bottom 底位置
     * @param top 顶位置
     * @param near 近位置
     * @param far 远位置
     * @param offset 偏移值
     * @param handness 坐标系统类型
     * @attention 该变换与透视投影互斥，最终取最后设置的投影类型
     */
    void setOrthogonal(float left, float right, float bottom, float top, float near, float far,
            float offset = 0, bx::Handness::Enum handness = bx::Handness::Left);

private:
    /**
     * @brief 相机信息
     */
    struct XCameraInfo {
        bx::Vec3 eye;
        bx::Vec3 at;
        bx::Vec3 up;
    };

    /**
     * @brief 透视投影信息
     */
    struct XPerspectiveInfo {
        float fovy;
        float aspect;
        float near;
        float far;
        bx::Handness::Enum handness;
    };

    /**
     * @brief 正交投影信息
     */
    struct XOrthogonalInfo {
        float left;
        float right;
        float bottom;
        float top;
        float near;
        float far;
        float offset;
        bx::Handness::Enum handness;
    };

private:
    XFilter *mFilter; /// 变换效果滤镜
    XCameraInfo mCameraInfo; /// 相机信息
    XPerspectiveInfo mPerspectiveInfo; /// 透视信息
    XOrthogonalInfo mOrthogonalInfo; /// 正交信息
    float *mViewMatrix; /// 相机变换矩阵
    float *mProjectionMatrix; /// 投影矩阵
    int mMatrixMask; /// 需要计算的矩阵的mask标示
};
NS_X_IMAGE_END

#endif //GPUIMAGE_X_XTRANSFORM_HPP
