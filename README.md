Compuesto de un MainActivity que lanza la librería gameEngineLibInC que basicamente es el juego en C++

La librería mencionada ejecuta una función "main" parecida al "main de toda la vida" llamada "android_main" que se encuentra en el fichero main.cpp.

Dentro de dicha función "android_main" se encuentra un bucle de eventos, que es donde transcurre todo, el render, eventos, etc.. el ciclo de vida de la aplicación nace y muere en ese bucle.

El objeto que se encarga de pintar y capturar el input se llama Renderer (.cpp)
