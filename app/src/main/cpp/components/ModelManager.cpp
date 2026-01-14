#include "ModelManager.h"
#include <cassert> // Para assert

// Implementación del nuevo método init()
void ModelManager::init(android_app* app, float designWidth, float designHeight) {
    assert(app != nullptr);
    assetManager_ = app->activity->assetManager;
    designWidth_ = designWidth;
    designHeight_ = designHeight;
}

// Implementación del nuevo método para dibujar todos los modelos
void ModelManager::drawAll(Shader* shader, const float* projectionMatrix, float cameraViewWidth, float cameraViewHeight) const {
    if (!shader || models_.empty() || screenWidth_ == 0.f) {
        return;
    }

    // --- LÓGICA DE ESCALADO CORREGIDA Y LIMPIA ---

    const float designAspect = designWidth_ / designHeight_;
    const float screenAspect = screenWidth_ / screenHeight_;

    // 1. ELIMINAR ESTAS LÍNEAS
    // const float cameraViewHeight = kProjectionHalfHeight * 2.f; // <-- Se va
    // const float cameraViewWidth = cameraViewHeight * screenAspect; // <-- Se va

    // 2. Usar directamente los parámetros recibidos
    float scaleX, scaleY;

    if (screenAspect > designAspect) {
        // La pantalla es más ancha que el diseño. El alto manda.
        scaleY = cameraViewHeight;
        scaleX = scaleY * designAspect;
    } else {
        // La pantalla es más alta o igual que el diseño. El ancho manda.
        scaleX = cameraViewWidth;
        scaleY = scaleX / designAspect;
    }

    // El resto de la función se queda igual...
    float modelMatrix[16];
    Matrix::buildScale(modelMatrix, scaleX, scaleY, 1.f);

    shader->activate();
    shader->setProjectionMatrix(projectionMatrix);

    for (const auto& model : models_) {
        shader->drawModel(model, modelMatrix);
    }
}

// 2. Implementación de updateScreenSize()
void ModelManager::updateScreenSize(float screenWidth, float screenHeight) {
    screenWidth_ = screenWidth;
    screenHeight_ = screenHeight;
}

// Implementación de los métodos para añadir modelos
void ModelManager::addModel(const Model& model) {
    models_.push_back(model);
}

void ModelManager::addModel(const std::string& textureName) {
    assert(assetManager_ != nullptr && "ModelManager::init() must be called first!");

    auto texture = TextureAsset::loadAsset(assetManager_, textureName);
    if (!texture) {
        // La textura no se pudo cargar, no añadir el modelo
        return;
    }

    // ¡IMPORTANTE! La geometría ahora debe estar en el "espacio de diseño".
    // Si tu diseño es 1080x1920, un cuadrado que ocupe toda la pantalla sería:
    const float halfW = designWidth_ / 2.f;
    const float halfH = designHeight_ / 2.f;

    // --- CORRECCIÓN CLAVE ---
    // La geometría ahora se define en un espacio normalizado (de -0.5 a 0.5)
    // Esto crea un cuadrado de 1x1 centrado en el origen.
    std::vector<Vertex> vertices = {
            Vertex({ 0.5f,  0.5f, 0.f}, {1.f, 0.f}),  // Arriba-derecha
            Vertex({-0.5f,  0.5f, 0.f}, {0.f, 0.f}),  // Arriba-izquierda
            Vertex({-0.5f, -0.5f, 0.f}, {0.f, 1.f}),  // Abajo-izquierda
            Vertex({ 0.5f, -0.5f, 0.f}, {1.f, 1.f})   // Abajo-derecha
    };
    std::vector<Index> indices = { 0, 1, 2, 0, 2, 3 };

    models_.emplace_back(vertices, indices, texture);
}

void ModelManager::clear() {
    models_.clear();
}
