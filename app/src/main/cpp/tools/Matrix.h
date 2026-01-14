#ifndef EROUNCOVER_MATRIX_H
#define EROUNCOVER_MATRIX_H

#include <cmath>
#include <cstring> // Para memset

/*
 * Clase de utilidad para operaciones con matrices de 4x4 en formato de columna mayor (column-major),
 * que es el formato que espera OpenGL.
 *
 * Una matriz de 4x4 se representa como un array de 16 floats:
 *
 *   | m[0]  m[4]  m[8]  m[12] |
 *   | m[1]  m[5]  m[9]  m[13] |
 *   | m[2]  m[6]  m[10] m[14] |
 *   | m[3]  m[7]  m[11] m[15] |
 *
 * m[12], m[13], m[14] son los componentes de traslación (x, y, z).
 */
class Matrix {
public:
    /**
     * Construye una matriz de escala.
     * @param result La matriz resultante de 16 floats.
     * @param sx Factor de escala en el eje X.
     * @param sy Factor de escala en el eje Y.
     * @param sz Factor de escala en el eje Z.
     */
    static void buildScale(float* result, float sx, float sy, float sz) {
        // Inicializa a matriz identidad
        memset(result, 0, sizeof(float) * 16);
        result[0] = sx;
        result[5] = sy;
        result[10] = sz;
        result[15] = 1.0f;
    }

    /**
     * Multiplica dos matrices de 4x4 (a * b) y almacena el resultado en 'result'.
     * @param result Matriz donde se guardará el resultado.
     * @param a Matriz izquierda de la multiplicación.
     * @param b Matriz derecha de la multiplicación.
     */
    static void multiply(float* result, const float* a, const float* b) {
        float temp[16];
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                float sum = 0;
                for (int k = 0; k < 4; ++k) {
                    sum += a[j + k * 4] * b[i * 4 + k];
                }
                temp[j + i * 4] = sum;
            }
        }
        memcpy(result, temp, sizeof(float) * 16);
    }
};

#endif //EROUNCOVER_MATRIX_H
