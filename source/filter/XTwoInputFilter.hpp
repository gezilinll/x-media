//
// Created by 林炳河 on 2019-09-25.
//

#ifndef GPUIMAGE_X_XTWOINPUTFILTER_HPP
#define GPUIMAGE_X_XTWOINPUTFILTER_HPP

#include "XFilter.hpp"

NS_X_IMAGE_BEGIN
class XTwoInputFilter : public XFilter {
public:
    XTwoInputFilter(const std::string vertex, const std::string fragment);

    /**
     * @brief 设置二号输入
     * @param secondFrame 二号输入
     */
    void setSecondInputFrameBuffer(XFrameBuffer *secondFrame);

protected:
    void updateParams() override ;

    bool isValid() override ;

private:
    XFrameBuffer* mSecondInputFrameBuffer;
    bgfx::UniformHandle mTexture2; /// 二号纹理
};
NS_X_IMAGE_END

#endif //GPUIMAGE_X_XTWOINPUTFILTER_HPP
