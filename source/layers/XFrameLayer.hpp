//
// Created by 林炳河 on 2019/9/20.
//

#ifndef GPUIMAGE_X_XFRAMELAYER_HPP
#define GPUIMAGE_X_XFRAMELAYER_HPP

#include "XLayer.hpp"

NS_X_IMAGE_BEGIN
/**
 * @brief 帧类型图层，可代表图片、视频帧等类型的图层
 * 通过 #setPath 可设置图片路径
 */
class XFrameLayer : public XLayer {
public:
    XFrameLayer(int id) : XLayer(id) {}

    /**
     * 设置图片路径
     * @param framePath 图片路径
     */
    void setPath(std::string framePath);

protected:
    void updateSource() override ;

private:
    std::string mFramePath;
};
NS_X_IMAGE_END

#endif //GPUIMAGE_X_XFRAMELAYER_HPP
