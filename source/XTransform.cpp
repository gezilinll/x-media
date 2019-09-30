//
// Created by 林炳河 on 2019/9/28.
//

#include "XTransform.hpp"

NS_X_IMAGE_BEGIN
struct MatrixType {
    enum Enum {
        DEFAULT = 1 << 0,
        ORTHOGONAL = 1 << 1,
        PERSPECTIVE = 1 << 2,
        CAMERA = 1 << 3,
    };
};

XTransform::XTransform() {
    mFilter = nullptr;

    mMatrixMask = MatrixType::Enum::DEFAULT;

    mViewMatrix = new float[16];
    mProjectionMatrix = new float[16];
    bx::mtxIdentity(mViewMatrix);
    bx::mtxIdentity(mProjectionMatrix);
}

XInputOutput* XTransform::get() {
    if (mFilter == nullptr) {
        mFilter = new XFilter("vs_default", "fs_default");
    }
    if (mMatrixMask & MatrixType::Enum::CAMERA) {
        mMatrixMask = mMatrixMask ^ MatrixType::Enum::CAMERA;
        bx::mtxLookAt(mViewMatrix, bx::load<bx::Vec3>(&mCameraInfo.eye.x),
                bx::load<bx::Vec3>(&mCameraInfo.at.x), bx::load<bx::Vec3>(&mCameraInfo.up.x));
    }
    if (mMatrixMask & MatrixType::Enum::PERSPECTIVE) {
        mMatrixMask = mMatrixMask ^ MatrixType::Enum::PERSPECTIVE;
        bx::mtxProj(mProjectionMatrix, mPerspectiveInfo.fovy, mPerspectiveInfo.aspect,
                mPerspectiveInfo.near, mPerspectiveInfo.far, bgfx::getCaps()->homogeneousDepth, mPerspectiveInfo.handness);
    }
    if (mMatrixMask & MatrixType::Enum::ORTHOGONAL) {
        mMatrixMask = mMatrixMask ^ MatrixType::Enum::ORTHOGONAL;
        bx::mtxOrtho(mProjectionMatrix, mOrthogonalInfo.left, mOrthogonalInfo.right,
                mOrthogonalInfo.bottom, mOrthogonalInfo.top, mOrthogonalInfo.near, mOrthogonalInfo.far,
                mOrthogonalInfo.offset, bgfx::getCaps()->homogeneousDepth, mOrthogonalInfo.handness);
    }
    mFilter->setTransform(mViewMatrix, mProjectionMatrix);
    return mFilter;
}

void XTransform::setCamera(bx::Vec3 eye, bx::Vec3 at, bx::Vec3 up) {
    mMatrixMask = mMatrixMask | MatrixType::Enum::CAMERA;
    mCameraInfo = {eye, at, up};
}

void XTransform::setPerspective(float fovy, float aspect, float near, float far, bx::Handness::Enum handness) {
    if (mMatrixMask & MatrixType::Enum::ORTHOGONAL) {
        mMatrixMask = mMatrixMask ^ MatrixType::Enum::ORTHOGONAL;
    }
    mMatrixMask = mMatrixMask | MatrixType::Enum::PERSPECTIVE;
    mPerspectiveInfo = {fovy, aspect, near, far, handness};

}

void XTransform::setOrthogonal(float left, float right, float bottom, float top, float near, float far, float offset,
                               bx::Handness::Enum handness) {
    if (mMatrixMask & MatrixType::Enum::PERSPECTIVE) {
        mMatrixMask = mMatrixMask ^ MatrixType::Enum::PERSPECTIVE;
    }
    mMatrixMask = mMatrixMask | MatrixType::Enum::ORTHOGONAL;
    mOrthogonalInfo = {left, right, bottom, top, near, far, offset, handness};
}

NS_X_IMAGE_END