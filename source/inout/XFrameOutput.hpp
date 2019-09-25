//
// Created by 林炳河 on 2019/7/14.
//

#ifndef GPUIMAGE_X_XIMAGEFILEOUTPUT_HPP
#define GPUIMAGE_X_XIMAGEFILEOUTPUT_HPP

#include "XOutput.hpp"
#include <string>

NS_X_IMAGE_BEGIN
/**
 * @brief 帧数据输出类，可控制图片、视频帧等类型数据的输出
 * 通过 #setPath 可设置图片路径控制图片数据输出
 */
class XFrameOutput: public XOutput {
public:
    XFrameOutput();

    void init() override ;

    /**
     * @brief 设置图片路径
     * @param path 图片路径
     * @attention 当图片路径与之前设置一致时，不会重新刷新数据
     */
    void setPath(std::string path);

    /**
     * @brief 设置输出尺寸
     * @param width 宽度
     * @param height 高度
     * @note 宽高 <= 0 时使用帧数据的狂傲
     */
    void setOutputSize(int width, int height);
private:
    bool mOutputUpdated; /// 帧数据是否已更新
    std::string mFramePath; /// 图片类型数据路径
    int mOutputWidth;
    int mOutputHeight;
};
NS_X_IMAGE_END

#endif //GPUIMAGE_X_XIMAGEFILEOUTPUT_HPP
