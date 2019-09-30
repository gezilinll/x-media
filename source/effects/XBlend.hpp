//
// Created by 林炳河 on 2019-09-26.
//

#ifndef GPUIMAGE_X_XBLEND_HPP
#define GPUIMAGE_X_XBLEND_HPP

#include "effects/XFilterEffect.hpp"
#include "effects/XTwoInputFilter.hpp"
#include <unordered_map>

NS_X_IMAGE_BEGIN
struct XBlend {
    enum Type {
        NORMAL,
        MULTIPLY,
        ADD
    };
    XBlend::Type type;
    std::unordered_map<std::string, glm::vec4> params;
};

class XBlender : public XFilterEffect {
public:
    XBlender(XBlend blend) : XFilterEffect() {
        mBlend = blend;
        if (blend.type == XBlend::Type::NORMAL) {
            setFragmentShaderName("fs_blend_normal");
        } else if (blend.type == XBlend::Type::MULTIPLY) {
            setFragmentShaderName("fs_blend_multiply");
        } else if (blend.type == XBlend::Type::ADD) {
            setFragmentShaderName("fs_blend_add");
        }
    }

protected:
    void init() override {
        if (mFilter == nullptr) {
            mFilter = new XTwoInputFilter(mVertexShaderName, mFragmentShaderName);
        }
    }

private:
    XBlend mBlend;
};
NS_X_IMAGE_END

#endif //GPUIMAGE_X_XBLEND_HPP
