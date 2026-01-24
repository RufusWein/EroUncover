#include "ModelManager.h"
#include <cassert> // Para assert

// Implementación del nuevo método init()
void ModelManager::init(android_app* app, float designWidth, float designHeight) {
    assert(app != nullptr);
    assetManager_ = app->activity->assetManager;

    aspect_.designWidth = designWidth;
    aspect_.designHeight = designHeight;
    aspect_.designAspect = aspect_.designWidth / aspect_.designHeight;
}

// Implementación del nuevo método para dibujar todos los modelos
void ModelManager::drawAll(Shader* shader, const float* projectionMatrix, float cameraViewWidth, float cameraViewHeight) {
    if (!shader || models_.empty() || aspect_.screenWidth == 0.f) {
        return;
    }

    // Actualizamos solo lo que depende de la cámara (si cambia la resolución)
    aspect_.cameraViewWidth = cameraViewWidth;
    aspect_.canvasScale = cameraViewWidth;

    shader->activate();
    shader->setProjectionMatrix(projectionMatrix);

    // Iteramos sobre el mapa de modelos
    for (const auto& item : models_) {
        const Model& model = *(item.second);

        shader->drawModel(model, aspect_);
    }
}

// 2. Implementación de updateScreenSize()
void ModelManager::updateScreenSize(float screenWidth, float screenHeight) {
    aspect_.screenWidth = screenWidth;
    aspect_.screenHeight = screenHeight;
}

// Creamos un método privado para obtener la matriz de "Lienzo"
// Esto limpia el drawAll de cálculos matemáticos pesados
void ModelManager::calculateCanvasScale(float& outScaleX, float& outScaleY) const {
    // Calculamos cuánto espacio de la cámara ocupa nuestro ancho de diseño
    // Como el ancho predomina:
    outScaleX = aspect_.screenWidth * (aspect_.designWidth / aspect_.screenWidth);

    // Para evitar que se estire, la escala Y DEBE ser proporcional a la X
    // basándose en el aspect ratio del diseño original.
    float designAspect = aspect_.designWidth / aspect_.designHeight;
    outScaleX = aspect_.screenWidth; // El ancho del diseño ocupa todo el ancho de pantalla
    outScaleY = outScaleX / designAspect;
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
