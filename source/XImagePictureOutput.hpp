//
// Created by 林炳河 on 2019/7/14.
//

#ifndef GPUIMAGE_X_XIMAGEPICTUREOUTPUT_HPP
#define GPUIMAGE_X_XIMAGEPICTUREOUTPUT_HPP

#include "XImageOutput.hpp"
#include <string>

class XImagePictureOutput: public XImageOutput {
public:
    void initWithPath(std::string path);

    void processPicture();
};


#endif //GPUIMAGE_X_XIMAGEPICTUREOUTPUT_HPP
