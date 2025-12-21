#include "ModelManager.h"
#include <cassert> // Para assert

// Implementación del nuevo método init()
void ModelManager::init(android_app* app) {
    // Nos aseguramos de que el puntero a 'app' es válido y guardamos el asset manager
    assert(app != nullptr);
    assetManager_ = app->activity->assetManager;
}

// Implementación del nuevo método para dibujar todos los modelos
void ModelManager::drawAll(Shader* shader) const {
    if (!shader || models_.empty()) {
        return; // No hacer nada si no hay shader o modelos
    }

    for (const auto& model : models_) {
        shader->drawModel(model);
    }
}

// Implementación de los métodos para añadir modelos
void ModelManager::addModel(const Model& model) {
    models_.push_back(model);
}

void ModelManager::addModel(const std::string& textureName) {
    /*
     * This is a square:
     * 0 --- 1
     * | \   |
     * |  \  |
     * |   \ |
     * 3 --- 2
     */
    std::vector<Vertex> vertices = {
            Vertex(Vector3{1, 1, 0}, Vector2{0, 0}), // 0
            Vertex(Vector3{-1, 1, 0}, Vector2{1, 0}), // 1
            Vertex(Vector3{-1, -1, 0}, Vector2{1, 1}), // 2
            Vertex(Vector3{1, -1, 0}, Vector2{0, 1}) // 3
    };
    std::vector<Index> indices = {
            0, 1, 2, 0, 2, 3
    };

    // Es una buena práctica asegurarse de que init() ha sido llamado antes de usar el assetManager_
    assert(assetManager_ != nullptr && "ModelManager::init() must be called before addModel()!");

    // 1. Cargar la textura
    auto texture = TextureAsset::loadAsset(assetManager_, textureName);

    models_.emplace_back(vertices, indices, texture);
}

void ModelManager::clear() {
    models_.clear();
}
