#ifndef EROUNCOVER_MODELMANAGER_H
#define EROUNCOVER_MODELMANAGER_H

#include <vector>
#include <memory>
#include <game-activity/native_app_glue/android_native_app_glue.h>
#include "Model.h"
#include "Shader.h"

class ModelManager {
public:
    // Constructor por defecto
    ModelManager() = default;

    // Nuevo método de inicialización
    void init(android_app* app);

    // Método para añadir un nuevo modelo a la colección
    void addModel(const Model& model);

    // Sobrecarga para añadir un modelo usando sus componentes
    void addModel(const std::string& textureName);

    // El nuevo método que pediste: dibuja todos los modelos usando un shader
    void drawAll(Shader* shader) const;

    // Método para limpiar todos los modelos si es necesario
    void clear();

private:
    AAssetManager* assetManager_ = nullptr; // Se inicializará con init()

    std::vector<Model> models_;
};

#endif //EROUNCOVER_MODELMANAGER_H
