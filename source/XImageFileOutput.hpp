//
// Created by 林炳河 on 2019/7/14.
//

#ifndef GPUIMAGE_X_XIMAGEFILEOUTPUT_HPP
#define GPUIMAGE_X_XIMAGEFILEOUTPUT_HPP

#include "XImageOutput.hpp"
#include <string>

NS_X_IMAGE_BEGIN
/**
 * @brief Picture source object.
 */
class XImageFileOutput: public XImageOutput {
public:
    /**
     * @brief load picture or other file type that can be loaded to output frame buffer from its path
     * @param[in] path file's absolute path
     */
    void initWithPath(std::string path);
};
NS_X_IMAGE_END

#endif //GPUIMAGE_X_XIMAGEFILEOUTPUT_HPP
