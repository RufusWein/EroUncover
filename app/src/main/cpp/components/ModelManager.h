#ifndef EROUNCOVER_MODELMANAGER_H
#define EROUNCOVER_MODELMANAGER_H

#include <unordered_map>
#include <memory>
#include <game-activity/native_app_glue/android_native_app_glue.h>
#include "Model.h"
#include "Shader.h"
#include "../tools/Matrix.h" // Necesitaremos esto para las matrices

class ModelManager {
public:
    // Constructor por defecto
    ModelManager() = default;
    void init(android_app* app, float designWidth, float designHeight);
    void updateScreenSize(float screenWidth, float screenHeight);

    // Ahora pedimos un ID para registrar el modelo
    void addModel(const std::string& id, const std::string& textureName);

    // Nuevo método para obtener un modelo específico por su ID
    Model* getModel(const std::string& id);

    // Nuevo método para eliminar un modelo si ya no lo quieres
    void removeModel(const std::string& id);

    void drawAll(Shader* shader, const float* projectionMatrix, float cameraViewWidth, float cameraViewHeight);

    // Método para limpiar todos los modelos si es necesario
    void clear();

private:
    void calculateCanvasScale(float& outScaleX, float& outScaleY) const;

    AAssetManager* assetManager_ = nullptr; // Se inicializará con init()

    //std::unordered_map<std::string, Model> models_;
    std::unordered_map<std::string, std::unique_ptr<Model>> models_;

    Aspect aspect_;
};
#endif //EROUNCOVER_MODELMANAGER_H
