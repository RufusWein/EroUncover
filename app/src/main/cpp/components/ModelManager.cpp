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

    const float designAspect = designWidth_ / designHeight_;
    const float screenAspect = screenWidth_ / screenHeight_;

    float baseScaleX, baseScaleY;

    if (screenAspect > designAspect) {
        baseScaleY = cameraViewHeight;
        baseScaleX = baseScaleY * designAspect;
    } else {
        baseScaleX = cameraViewWidth;
        baseScaleY = baseScaleX / designAspect;
    }

    shader->activate();
    shader->setProjectionMatrix(projectionMatrix);

    // Iteramos sobre el mapa de modelos
    for (const auto& item : models_) {
        const Model& model = *(item.second);

        // 1. Calculamos la escala final:
        // (Escala del sistema) * (Escala individual del modelo)
        float finalScaleX = baseScaleX * model.getScale();
        float finalScaleY = baseScaleY * model.getScale();

        // 2. Calculamos la posición:
        // Convertimos las coordenadas de "espacio de diseño" a "espacio de cámara"
        // Como el cuadrado 1x1 está centrado, (0,0) en tu diseño será el centro de la pantalla.
        float posX = model.getPosition().x * (baseScaleX / designWidth_);
        float posY = model.getPosition().y * (baseScaleY / designHeight_);
        float posZ = model.getPosition().z; // La Z sirve para el orden de capas (Z-Order)

        // 3. Construimos la matriz de transformación (Model Matrix)
        float modelMatrix[16];

        // Primero escalamos el cuadrado 1x1
        Matrix::buildScale(modelMatrix, finalScaleX, finalScaleY, 1.f);

        // Luego aplicamos la traslación (posición) directamente en los componentes de la matriz
        // m[12]=X, m[13]=Y, m[14]=Z
        modelMatrix[12] = posX;
        modelMatrix[13] = posY;
        modelMatrix[14] = posZ;

        shader->drawModel(model, modelMatrix);
    }
}

// 2. Implementación de updateScreenSize()
void ModelManager::updateScreenSize(float screenWidth, float screenHeight) {
    screenWidth_ = screenWidth;
    screenHeight_ = screenHeight;
}

void ModelManager::addModel(const std::string& id, const std::string& textureName) {
    assert(assetManager_ != nullptr && "ModelManager::init() must be called first!");

    auto texture = TextureAsset::loadAsset(assetManager_, textureName);
    if (!texture) {
        // La textura no se pudo cargar, no añadir el modelo
        return;
    }

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

    models_[id] = std::unique_ptr<Model>(new Model(vertices, indices, texture));
}

Model* ModelManager::getModel(const std::string& id) {
    auto it = models_.find(id);
    if (it != models_.end()) {
        return it->second.get(); // Retornamos puntero al modelo encontrado
    }
    return nullptr; // No existe
}

void ModelManager::removeModel(const std::string& id) {
    models_.erase(id);
}

void ModelManager::clear() {
    models_.clear();
}
