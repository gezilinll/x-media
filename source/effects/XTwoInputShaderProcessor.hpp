//
// Created by 林炳河 on 2019-09-25.
//

#ifndef GPUIMAGE_X_XTWOINPUTFILTER_HPP
#define GPUIMAGE_X_XTWOINPUTFILTER_HPP

#include "XShaderProcessor.hpp"

NS_X_IMAGE_BEGIN
/**
 * @brief 控制双输入类型滤镜渲染逻辑的基类，例如混合、抠图等效果
 *
 * 该类继承自 #XFilter ，使用方式、逻辑、能力等基本一致，同样支持形成链式效果
 * 需要通过 #setSecondInputFrameBuffer 来设置二号纹理输入
 */
class XTwoInputShaderProcessor : public XShaderProcessor {
public:
    XTwoInputShaderProcessor(const std::string vertex, const std::string fragment);

    ~XTwoInputShaderProcessor();

    /**
     * @brief 设置二号纹理输入
     * @param secondFrame 二号纹理输入
     */
    void setSecondInputFrameBuffer(XFrameBuffer *secondFrame);

protected:
    void updateParams() override ;

    bool isValid() override ;

private:
    XFrameBuffer* mSecondInputFrameBuffer; /// 二号纹理
    bgfx::UniformHandle mTexture2; /// 二号纹理标识
};
NS_X_IMAGE_END

#endif //GPUIMAGE_X_XTWOINPUTFILTER_HPP
