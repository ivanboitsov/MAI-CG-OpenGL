#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <GL/gl.h>
#include <GL/glu.h>
#include <cmath>
#include <iostream>

using namespace sf;
using namespace std;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Параметры окна
const unsigned int windowWidth = 1200;
const unsigned int windowHeight = 800;

// Переменные для управления камерой
float cameraDistance = 5.0f; // Расстояние от камеры до центра сцены
float angleAroundCube = 0.0f; // Угол вращения камеры вокруг куба
float angleX = 20.0f; // Угол по оси X (для наклона камеры)
float cameraHeight = 2.0f; // Высота камеры

// Флаг для отслеживания движения мыши
bool isLeftDragging = false;
bool isRightDragging = false;
Vector2i lastMousePosition;

// Настройка проекции
void setupProjection() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspectRatio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
    gluPerspective(60.0f, aspectRatio, 1.0f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

// Обновление матрицы вида камеры
void updateCamera() {
    glLoadIdentity();

    float cameraX = cameraDistance * cosf(angleAroundCube * M_PI / 180.0f) * cosf(angleX * M_PI / 180.0f);
    float cameraZ = cameraDistance * sinf(angleAroundCube * M_PI / 180.0f) * cosf(angleX * M_PI / 180.0f);
    float cameraY = cameraHeight;

    // Камера смотрит на центр сцены (0, 0, 0)
    gluLookAt(cameraX, cameraY, cameraZ, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
}

// Отрисовка куба
void drawCube() {
    glBegin(GL_QUADS);

    // Верхняя грань
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    // Нижняя грань
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    // Передняя грань
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    // Задняя грань
    glColor3f(1.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);

    // Левая грань
    glColor3f(1.0f, 0.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);

    // Правая грань
    glColor3f(0.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

    glEnd();
}

// Главная функция
int main() {
    ContextSettings settings;
    settings.depthBits = 24;

    Window window(VideoMode(windowWidth, windowHeight), "3D Camera Demo", Style::Titlebar | Style::Close, settings);
    window.setFramerateLimit(60);

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.8f, 0.8f, 0.8f, 1.0f);

    setupProjection();

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }

            if (event.type == Event::MouseButtonPressed) {
                if (event.mouseButton.button == Mouse::Left) {
                    isLeftDragging = true;
                    lastMousePosition = Mouse::getPosition(window);
                }
                else if (event.mouseButton.button == Mouse::Right) {
                    isRightDragging = true;
                    lastMousePosition = Mouse::getPosition(window);
                }
            }

            if (event.type == Event::MouseButtonReleased) {
                if (event.mouseButton.button == Mouse::Left) {
                    isLeftDragging = false;
                }
                else if (event.mouseButton.button == Mouse::Right) {
                    isRightDragging = false;
                }
            }

            if (event.type == Event::MouseMoved) {
                if (isLeftDragging) {
                    Vector2i currentMousePosition = Mouse::getPosition(window);
                    angleAroundCube += (currentMousePosition.x - lastMousePosition.x) * 0.5f;
                    angleX -= (currentMousePosition.y - lastMousePosition.y) * 0.5f;
                    lastMousePosition = currentMousePosition;
                }
                else if (isRightDragging) {
                    Vector2i currentMousePosition = Mouse::getPosition(window);
                    float deltaY = currentMousePosition.y - lastMousePosition.y;
                    cameraHeight += deltaY * 0.1f; // Регулируем высоту камеры
                    lastMousePosition = currentMousePosition;
                }
            }

            if (event.type == Event::MouseWheelScrolled) {
                cameraDistance += event.mouseWheelScroll.delta * 0.2f; // Регулировка расстояния
            }
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        updateCamera();
        drawCube();

        window.display();
    }

    return 0;
}