#include <napi.h>
#include "VideoDecoder.hpp"

using namespace Napi;

Napi::String Method(const Napi::CallbackInfo &info)
{
    auto decoder = new VideoDecoder("/Users/linbinghe/Desktop/1.mp4");

    Napi::Env env = info.Env();
    return Napi::String::New(env, "world");
}

Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    exports.Set(Napi::String::New(env, "HelloWorld"),
                Napi::Function::New(env, Method));
    return exports;
}

NODE_API_MODULE(addon, Init)