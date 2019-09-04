//
// Created by 林炳河 on 2019/7/14.
//

#include "XImageFileOutput.hpp"

NS_X_IMAGE_BEGIN
void XImageFileOutput::initWithPath(std::string path) {
    mOutputFrameBuffer->loadFromImage(path);
}
NS_X_IMAGE_END