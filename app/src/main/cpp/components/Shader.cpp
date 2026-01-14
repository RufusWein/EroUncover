#include "Shader.h"
#include "AndroidOut.h"
#include "Model.h"
#include "Utility.h"
#include <android/log.h>

// --- ESTE ES EL CONSTRUCTOR QUE DEBES AÑADIR/MODIFICAR EN SHADER.H ---
// Shader::Shader(GLuint program, GLint position, GLint uv, GLint projectionMatrix, GLint modelMatrix)
//    : program_(program),
//      position_(position),
//      uv_(uv),
//      projectionMatrix_(projectionMatrix),
//      modelMatrix_(modelMatrix) {}

// --- Y ESTOS SON LOS MIEMBROS QUE DEBES TENER EN SHADER.H ---
// GLuint program_ = 0;
// GLint position_ = -1;
// GLint uv_ = -1;
// GLint projectionMatrix_ = -1;
// GLint modelMatrix_ = -1;
// --- ¡AÑADE ESTE CÓDIGO AQUÍ! ---
// Esta es la implementación del constructor que falta.
Shader::Shader(GLuint program, GLint position, GLint uv, GLint projectionMatrix, GLint modelMatrix)
        : program_(program),
          position_(position),
          uv_(uv),
          projectionMatrix_(projectionMatrix),
          modelMatrix_(modelMatrix) {
    // El cuerpo del constructor está vacío, ya que todo el trabajo se hace
    // en la lista de inicialización de arriba (los ': ...').
}

// Esta es la función principal que crea el PROGRAMA completo a partir de los dos shaders.
Shader *Shader::loadShader(
        const std::string &vertexSource,
        const std::string &fragmentSource,
        const std::string &positionAttributeName,
        const std::string &uvAttributeName,
        const std::string &projectionMatrixUniformName) {
    Shader *shader = nullptr;

    // 1. Compilar los shaders individuales
    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, vertexSource);
    if (!vertexShader) {
        aout << "Error al cargar el vertex shader." << std::endl;
        return nullptr;
    }

    GLuint fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentSource);
    if (!fragmentShader) {
        glDeleteShader(vertexShader);
        aout << "Error al cargar el fragment shader." << std::endl;
        return nullptr;
    }

    // 2. Crear y enlazar el programa
    GLuint program = glCreateProgram();
    if (program) {
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);

        glLinkProgram(program);
        GLint linkStatus = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);

        if (linkStatus != GL_TRUE) {
            GLint logLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
            if (logLength) {
                GLchar *log = new GLchar[logLength];
                glGetProgramInfoLog(program, logLength, nullptr, log);
                aout << "Error al enlazar el programa del shader:\n" << log << std::endl;
                delete[] log;
            }
            aout << "linkStatus != GL_TRUE por lo que el programa es eliminado:\n" << std::endl;
            glDeleteProgram(program);
        } else {
            // --- ¡LUGAR CORRECTO PARA OBTENER UNIFORMS! ---
            // 3. Obtener la ubicación de atributos y uniforms
            GLint positionAttribute = glGetAttribLocation(program, positionAttributeName.c_str());
            GLint uvAttribute = glGetAttribLocation(program, uvAttributeName.c_str());
            GLint projectionMatrixUniform = glGetUniformLocation(program, projectionMatrixUniformName.c_str());

            // 3.1. Obtener el nuevo uniform "uModel" que añadimos en el shader GLSL
            GLint modelMatrixUniform = glGetUniformLocation(program, "uModel");

            // 4. Comprobar que TODO se ha encontrado
            if (positionAttribute != -1 && uvAttribute != -1 && projectionMatrixUniform != -1 && modelMatrixUniform != -1) {
                // 5. Crear el objeto de nuestra clase Shader y pasarle todas las ubicaciones
                // (Asegúrate de que el constructor en Shader.h acepta 5 parámetros)
                shader = new Shader(
                        program,
                        positionAttribute,
                        uvAttribute,
                        projectionMatrixUniform,
                        modelMatrixUniform); // <- Parámetro añadido
            } else {
                // Si algo falla, es útil saber el qué.
                if (positionAttribute == -1) aout << "Error: No se encontró el atributo '" << positionAttributeName << "'." << std::endl;
                if (uvAttribute == -1) aout << "Error: No se encontró el atributo '" << uvAttributeName << "'." << std::endl;
                if (projectionMatrixUniform == -1) aout << "Error: No se encontró el uniform '" << projectionMatrixUniformName << "'." << std::endl;
                if (modelMatrixUniform == -1) aout << "Error: No se encontró el uniform 'uModel'." << std::endl;
                aout << "ALGO FALLA por lo que el programa es eliminado:\n" << std::endl;
                glDeleteProgram(program);
            }
        }
    }

    // 6. Los shaders individuales ya no son necesarios, están dentro del programa
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    if (shader == nullptr) {
        aout <<"La función loadShader va a devolver nullptr." << std::endl;
    }

    return shader;
}


// Esta es la función auxiliar que compila UN solo shader (vertex o fragment).
// DEVUELVE UN GLuint, NO UN Shader*.
// La dejo exactamente como estaba originalmente, que es la forma correcta.
GLuint Shader::loadShader(GLenum shaderType, const std::string &shaderSource) {
    Utility::assertGlError();
    GLuint shader = glCreateShader(shaderType);
    if (shader) {
        auto *shaderRawString = (GLchar *) shaderSource.c_str();
        GLint shaderLength = shaderSource.length();
        glShaderSource(shader, 1, &shaderRawString, &shaderLength);
        glCompileShader(shader);

        GLint shaderCompiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &shaderCompiled);
        if (!shaderCompiled) {
            GLint infoLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLength);
            if (infoLength) {
                auto *infoLog = new GLchar[infoLength];
                glGetShaderInfoLog(shader, infoLength, nullptr, infoLog);
                aout << "Error al compilar el shader de tipo " << shaderType << ":\n" << infoLog << std::endl;
                delete[] infoLog;
            }
            glDeleteShader(shader);
            shader = 0;
        }
    }
    return shader;
}


void Shader::activate() const {
    glUseProgram(program_);
}

void Shader::deactivate() const {
    glUseProgram(0);
}

// Implementación del nuevo drawModel que acepta la matriz del modelo
void Shader::drawModel(const Model& model, const float* modelMatrix) const {
    aout << "Dibujando: Activando Shader" << std::endl;
    glUseProgram(program_);

    glUniformMatrix4fv(modelMatrix_, 1, GL_FALSE, modelMatrix);


    // ¡AHORA EL VAO SÍ TIENE DATOS!
    glBindVertexArray(model.getVao());

    aout << "Dibujando: Configurando Atributos" << std::endl;
    glEnableVertexAttribArray(position_);
    glVertexAttribPointer(position_, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, position));

    glEnableVertexAttribArray(uv_);
    glVertexAttribPointer(uv_, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, uv));

    aout << "Dibujando: Enlazando Textura" << std::endl;
    glActiveTexture(GL_TEXTURE0);
    // IMPORTANTE: Verifica que la textura no sea nula antes de esto
    glBindTexture(GL_TEXTURE_2D, model.getTexture().getTextureID());

    aout << "Dibujando: Llamando a glDrawElements con " << model.getIndexCount() << " indices" << std::endl;

    // Si peta aquí, es el tipo de dato (GL_UNSIGNED_INT vs SHORT) o el EBO está vacío
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(model.getIndexCount()), GL_UNSIGNED_SHORT, nullptr);

    aout << "Dibujando: Finalizado" << std::endl;

    glDisableVertexAttribArray(position_);
    glDisableVertexAttribArray(uv_);
}


void Shader::setProjectionMatrix(const float *projectionMatrix) const {
    // Asegurarse de que el programa está activo antes de establecer el uniform
    glUseProgram(program_);
    glUniformMatrix4fv(projectionMatrix_, 1, GL_FALSE, projectionMatrix);
}