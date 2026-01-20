#ifndef ANDROIDGLINVESTIGATIONS_MODEL_H
#define ANDROIDGLINVESTIGATIONS_MODEL_H

#include <vector>
#include "TextureAsset.h"

union Vector3 {
    struct {
        float x, y, z;
    };
    float idx[3];
};

union Vector2 {
    struct {
        float x, y;
    };
    struct {
        float u, v;
    };
    float idx[2];
};

struct Vertex {
    constexpr Vertex(const Vector3 &inPosition, const Vector2 &inUV) : position(inPosition),
                                                                       uv(inUV) {}

    Vector3 position;
    Vector2 uv;
};

typedef uint16_t Index;

class Model {
public:
    // Ahora solo declaramos el constructor y añadimos el destructor
    Model(std::vector<Vertex> vertices,
          std::vector<Index> indices,
          std::shared_ptr<TextureAsset> spTexture);

    ~Model();

    inline const Vertex *getVertexData() const {
        return vertices_.data();
    }

    inline const size_t getIndexCount() const {
        return indices_.size();
    }

    inline const Index *getIndexData() const {
        return indices_.data();
    }

    inline const TextureAsset &getTexture() const {
        return *spTexture_;
    }

    // Getters y Setters para transformar el objeto
    void setPosition(float x, float y, float z) { position_ = {x, y, z}; }
    void setScale(float s) { scale_ = s; }

    const Vector3& getPosition() const { return position_; }
    float getScale() const { return scale_; }

    // --- ESTE ES EL MÉTODO ---
    // Devuelve el identificador del Vertex Array Object
    inline GLuint getVao() const { return vao_; }
    inline GLuint getVbo() const { return vbo_; }
    inline GLuint getEbo() const { return ebo_; }

private:
    // ... (otros miembros)
    GLuint vao_ = 0; // El ID del Vertex Array Object
    GLuint vbo_ = 0; // El ID del Vertex Buffer Object (vértices)
    GLuint ebo_ = 0; // El ID del Element Buffer Object (índices)

    std::vector<Vertex> vertices_;
    std::vector<Index> indices_;
    std::shared_ptr<TextureAsset> spTexture_;

    Vector3 position_ = {0.0f, 0.0f, 0.0f}; // Posición inicial en el centro
    float scale_ = 1.0f;                    // Escala inicial 1:1
};

#endif //ANDROIDGLINVESTIGATIONS_MODEL_H