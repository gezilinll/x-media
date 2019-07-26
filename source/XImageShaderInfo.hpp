//
// Created by 林炳河 on 2019/7/24.
//

#ifndef GPUIMAGE_X_XIMAGESHADERINFO_HPP
#define GPUIMAGE_X_XIMAGESHADERINFO_HPP

#include <limits>

struct Shader {
    std::string vsName = "vs_filter_normal";
    std::string fsName = "fs_filter_normal";
};

/**
 * @brief Adjusts the saturation of an image
 * `saturation` ranges from 0.0 (fully desaturated) to 2.0 (max saturation), with 1.0 as the normal level
 */
struct Saturation {
    std::string fsName = "fs_filter_saturation";
    std::string paramName = "saturation";
    float paramMin = 0.0f;
    float paramMax = 2.0f;
    float paramNormal = 1.0f;
};

/**
 * @brief Adjusts the contrast of the image
 *
 * `contrast` ranges from 0.0 to 4.0 (max contrast), with 1.0 as the normal level
 */
struct Contrast {
    std::string fsName = "fs_filter_contrast";
    std::string paramName = "contrast";
    float paramMin = 0.0f;
    float paramMax = 4.0f;
    float paramNormal = 1.0f;
};

/**
 * @brief Adjusts the brightness of the image
 *
 * `brightness` ranges from -1.0 to 1.0, with 0.0 as the normal level
 */
struct Brightness {
    std::string fsName = "fs_filter_brightness";
    std::string paramName = "brightness";
    float paramMin = -1.0f;
    float paramMax = 1.0f;
    float paramNormal = 0.0f;
};

/**
 * @brief Levels like Photoshop.
 *
 * The `min`, `max`, `minOut` and `maxOut` parameters are floats in the range [0, 1].
 * If you have parameters from Photoshop in the range [0, 255] you must first
 * convert them to be [0, 1].
 * The `mid(gamma)` parameter is a float >= 0. This matches the value from Photoshop.
 *
 * If you want to apply levels to RGB as well as individual channels you need to use
 * this filter twice - first for the individual channels and then for all channels.
 *
 * \todo confirm middle level param max value
 */
struct Levels {
    std::string fsName = "fs_filter_levels";
    std::string paramMinLevelName = "levelMinimum";
    std::string paramMiddleLevelName = "levelMiddle";
    std::string paramMaxLevelName = "levelMaximum";
    std::string paramMinOutName = "minOutput";
    std::string paramMaxOutName = "maxOutput";
    float paramMin[3] = {0.0f, 0.0f, 0.0f};
    float paramMax[3] = {1.0f, 1.0f, 1.0f};
    float paramMiddleLevelMin[3] = {0.0f, 0.0f, 0.0f};
    float paramMinNormal[3] = {0.0f, 0.0f, 0.0f};
    float paramMaxNormal[3] = {1.0f, 1.0f, 1.0f};
    float paramMiddleNormal[3] = {1.0f, 1.0f, 1.0f};
};

/**
 * @brief Adjusts the exposure of the image
 *
 * `exposure` ranges from -10.0 to 10.0, with 0.0 as the normal level
 */
struct Exposure {
    std::string fsName = "fs_filter_exposure";
    std::string paramName = "exposure";
    float paramMin = -10.0f;
    float paramMax = 10.0f;
    float paramNormal = 0.0f;
};

/**
 * @brief \todo To be added
 *
 * Normalized values by which each color channel is multiplied. The range is from 0.0 up, with 1.0 as the default.
 */
struct RGB {
    std::string fsName = "fs_filter_rgb";
    std::string paramRedName = "redAdjustment";
    std::string paramGreenName = "greenAdjustment";
    std::string paramBlueName = "blueAdjustment";
    float paramMin = 0.0f;
    float paramNormal = 1.0;
};

/**
 * @brief \todo To be added and confirm param value
 */
struct HUE {
    std::string fsName = "fs_filter_hue";
    std::string paramName = "hueAdjust";
    float paramMin = 0.0f;
    float paramMax = 2 * M_PI;
    float paramNormal = 0.0f;
};

/**
 * @brief Allows adjustment of color temperature in terms of what an image was effectively shot in. This means higher Kelvin values will warm the image, while lower values will cool it.
 *
 * Created by Alaric Cole
 */
struct WhiteBalance {
    std::string fsName = "fs_filter_white_balance";
    std::string paramTemperatureName = "temperature"; /// choose color temperature, in degrees Kelvin
    std::string paramTintName = "tint"; /// adjust tint to compensate
    float paramTemperatureDefault = 0.0f;
    float paramTintDefault = 0.0f;
};

/**
 * @brief \todo To be added
 */
struct Monochrome {
    std::string fsName = "fs_filter_monochrome";
    std::string paramIntensityName = "intensity";
    std::string paramFilterColorName = "filterColor";
    float paramIntensityMin = 0.0f;
    float paramIntensityMax = 1.0f;
    float paramIntensityNormal = 0.0f;
    float paramFilterColorMin[3] = {0.0f, 0.0f, 0.0f};
    float paramFilterColorMax[3] = {1.0f, 1.0f, 1.0f};
};

#endif //GPUIMAGE_X_XIMAGESHADERINFO_HPP
