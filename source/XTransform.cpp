//
// Created by 林炳河 on 2019/9/28.
//

#include "XTransform.hpp"

NS_X_IMAGE_BEGIN
XTransform::XTransform() {
    mFilter = nullptr;
    mCameraEye = {0, 0, 0};
    mCameraAt = {0, 0, 0};
    mCameraUp = {0, 0, 0};
    mFovy = 0;
    mAspect = 0;
    mNear = 0;
    mFar = 0;
    mHandness = bx::Handness::Left;
}

XInputOutput* XTransform::get() {
    if (mFilter == nullptr) {
        mFilter = new XFilter("vs_default", "fs_default");
    }
//    float view[16];
//    bx::mtxLookAt(view, bx::load<bx::Vec3>(&mCameraEye.x), bx::load<bx::Vec3>(&mCameraAt.x), bx::load<bx::Vec3>(&mCameraUp.x));
//    float projection[16];
//    bx::mtxProj(projection, mFovy, mAspect, mNear, mFar, bgfx::getCaps()->homogeneousDepth, mHandness);
//    mFilter->setTransform(view, projection);
    return mFilter;
}

void XTransform::setCamera(bx::Vec3 eye, bx::Vec3 at, bx::Vec3 up) {
    mCameraEye = eye;
    mCameraAt = at;
    mCameraUp = up;
}

void XTransform::setProjection(float fovy, float aspect, float near, float far, bx::Handness::Enum handness) {
    mFovy = fovy;
    mAspect = aspect;
    mNear = near;
    mFar = far;
    mHandness = handness;
}

NS_X_IMAGE_END