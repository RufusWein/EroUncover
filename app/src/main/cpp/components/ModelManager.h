#ifndef EROUNCOVER_MODELMANAGER_H
#define EROUNCOVER_MODELMANAGER_H

#include <vector>
#include <memory>
#include <game-activity/native_app_glue/android_native_app_glue.h>
#include "Model.h"
#include "Shader.h"
#include "../tools/Matrix.h" // Necesitaremos esto para las matrices

class ModelManager {
public:
    // Constructor por defecto
    ModelManager() = default;

    // Nuevo método de inicialización
    void init(android_app* app, float designWidth, float designHeight);

    // Método para añadir un nuevo modelo a la colección
    void addModel(const Model& model);

    // Sobrecarga para añadir un modelo usando sus componentes
    void addModel(const std::string& textureName);

    // Método para actualizar la resolución real de la pantalla
    void updateScreenSize(float screenWidth, float screenHeight);

    // El nuevo método que pediste: dibuja todos los modelos usando un shader
    void drawAll(Shader* shader, const float* projectionMatrix, float cameraViewWidth, float cameraViewHeight) const;

    // Método para limpiar todos los modelos si es necesario
    void clear();

private:
    AAssetManager* assetManager_ = nullptr; // Se inicializará con init()

    std::vector<Model> models_;

    // Nuevos miembros para gestionar la escala y resolución
    float designWidth_ = 0.f;
    float designHeight_ = 0.f;
    float screenWidth_ = 0.f;
    float screenHeight_ = 0.f;
};

#endif //EROUNCOVER_MODELMANAGER_H
