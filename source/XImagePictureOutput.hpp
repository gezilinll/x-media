//
// Created by 林炳河 on 2019/7/14.
//

#ifndef GPUIMAGE_X_XIMAGEPICTUREOUTPUT_HPP
#define GPUIMAGE_X_XIMAGEPICTUREOUTPUT_HPP

#include "XImageOutput.hpp"
#include <string>

NS_X_IMAGE_BEGIN
class XImagePictureOutput: public XImageOutput {
public:
    void initWithPath(std::string path);

    void processPicture();
};
NS_X_IMAGE_END

#endif //GPUIMAGE_X_XIMAGEPICTUREOUTPUT_HPP
