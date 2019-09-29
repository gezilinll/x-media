//
// Created by 林炳河 on 2019/9/28.
//

#ifndef GPUIMAGE_X_XTRANSFORM_HPP
#define GPUIMAGE_X_XTRANSFORM_HPP

#include "XFilterEffect.hpp"
#include "bx/math.h"

NS_X_IMAGE_BEGIN
/**
 * @brief 变换效果，主要提供相机视角变换以及投影变换能力
 */
class XTransform : public XEffect {
public:
    XTransform();

    XInputOutput *get() override ;

    /**
     * @brief 设置相机参数
     * @param eye 相机镜头朝向
     * @param at 相机位置
     * @param up 相机顶部方向
     */
    void setCamera(bx::Vec3 eye, bx::Vec3 at, bx::Vec3 up);

    /**
     * @brief 设置投影变换参数
     * @param fovy 视角范围角度
     * @param aspect 裁剪平面宽高比
     * @param near 裁剪近平面
     * @param far 裁剪远平面
     * @param handness 坐标系统类型
     */
    void setProjection(float fovy, float aspect, float near, float far, bx::Handness::Enum handness = bx::Handness::Left);

private:
    XFilter *mFilter;
    bx::Vec3 mCameraEye;
    bx::Vec3 mCameraAt;
    bx::Vec3 mCameraUp;
    float mFovy;
    float mAspect;
    float mNear;
    float mFar;
    bx::Handness::Enum mHandness;
};
NS_X_IMAGE_END

#endif //GPUIMAGE_X_XTRANSFORM_HPP
