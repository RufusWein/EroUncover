#include <android/imagedecoder.h>
#include "TextureAsset.h"
#include "AndroidOut.h"
#include "Utility.h"

std::shared_ptr<TextureAsset>
TextureAsset::loadAsset(AAssetManager *gestorDeRecursos, const std::string &assetPath) {
    // Get the image from asset manager
    auto pAndroidRobotPng = AAssetManager_open(
            gestorDeRecursos,
            assetPath.c_str(),
            AASSET_MODE_BUFFER);

    // Make a decoder to turn it into a texture
    AImageDecoder *pAndroidDecoder = nullptr;
    auto result = AImageDecoder_createFromAAsset(pAndroidRobotPng, &pAndroidDecoder);
    assert(result == ANDROID_IMAGE_DECODER_SUCCESS);

    // make sure we get 8 bits per channel out. RGBA order.
    AImageDecoder_setAndroidBitmapFormat(pAndroidDecoder, ANDROID_BITMAP_FORMAT_RGBA_8888);

    // Get the image header, to help set everything up
    const AImageDecoderHeaderInfo *pAndroidHeader = nullptr;
    pAndroidHeader = AImageDecoder_getHeaderInfo(pAndroidDecoder);

    // important metrics for sending to GL
    auto ancho = AImageDecoderHeaderInfo_getWidth(pAndroidHeader);
    auto altura = AImageDecoderHeaderInfo_getHeight(pAndroidHeader);
    auto profundidad = AImageDecoder_getMinimumStride(pAndroidDecoder);

    // Get the bitmap data of the image
    auto imagenDeAndroid = std::unique_ptr<std::vector<uint8_t>>(new std::vector<uint8_t>(altura * profundidad));

    auto decodeResult = AImageDecoder_decodeImage(
            pAndroidDecoder,
            imagenDeAndroid->data(),
            profundidad,
            imagenDeAndroid->size());
    assert(decodeResult == ANDROID_IMAGE_DECODER_SUCCESS);

    // Get an opengl texture
    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    // Clamp to the edge, you'll get odd results alpha blending if you don't
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load the texture into VRAM
    glTexImage2D(
    GL_TEXTURE_2D, // target
    0, // mip level
    GL_RGBA, // internal format, often advisable to use BGR
    ancho, // ancho of the texture
    altura, // altura of the texture
    0, // border (always 0)
    GL_RGBA, // format
    GL_UNSIGNED_BYTE, // type
    imagenDeAndroid->data() // Data to upload
    );

    // generate mip levels. Not really needed for 2D, but good to do
    glGenerateMipmap(GL_TEXTURE_2D);

    // cleanup helpers
    AImageDecoder_delete(pAndroidDecoder);
    AAsset_close(pAndroidRobotPng);

    // Create a shared pointer so it can be cleaned up easily/automatically
    return std::shared_ptr<TextureAsset>(new TextureAsset(textureId));
}

TextureAsset::~TextureAsset() {
    // return texture resources
    glDeleteTextures(1, &textureID_);
    textureID_ = 0;
}