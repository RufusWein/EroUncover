#include "Model.h"
#include <GLES3/gl3.h>
#include <cstddef>

Model::Model(
        std::vector<Vertex> vertices,
        std::vector<Index> indices,
        std::shared_ptr<TextureAsset> spTexture)
        : vertices_(std::move(vertices)),
          indices_(std::move(indices)),
          spTexture_(std::move(spTexture)) {

    // 1. Generar los nombres (IDs) de los buffers
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glGenBuffers(1, &ebo_);

    // 2. Enlazar el VAO (Vertex Array Object) para empezar a grabar la configuración
    glBindVertexArray(vao_);

    // 3. Subir los datos de los VÉRTICES (VBO)
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER,
                 vertices_.size() * sizeof(Vertex),
                 vertices_.data(),
                 GL_STATIC_DRAW);

    // 4. Subir los datos de los ÍNDICES (EBO)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 indices_.size() * sizeof(Index),
                 indices_.data(),
                 GL_STATIC_DRAW);

    // 5. Configurar los atributos para el Shader
    // Atributo 0: position (coincide con inPosition en el shader)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, position));

    // Atributo 1: uv (coincide con inUV en el shader)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, uv));

    // 6. Desenlazar el VAO para evitar modificaciones accidentales
    glBindVertexArray(0);

    // Opcional: una vez subidos a la GPU, si no vas a modificar los vértices en CPU,
    // podrías limpiar los vectores vertices_ e indices_ para ahorrar RAM.
}

Model::~Model() {
    // Es muy importante liberar la memoria de la GPU cuando el modelo se destruya
    if (vbo_) glDeleteBuffers(1, &vbo_);
    if (ebo_) glDeleteBuffers(1, &ebo_);
    if (vao_) glDeleteVertexArrays(1, &vao_);
}//
