#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <gl/GLU.h>
#include <cmath>
#include <iostream>

using namespace sf;
using namespace std;

// Параметры окна
const unsigned int windowWidth = 1200;
const unsigned int windowHeight = 800;

// Переменные для управления
bool isPerspective = true; // Текущая проекция
bool isDragging = false;
Vector2i lastMousePosition;
float cameraDistance = 5.0f;
float angleX = 0.0f, angleY = 0.0f;
float orthoScale = 2.0f; // Масштаб ортографической проекции

// Настройка проекции
void setupProjection() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspectRatio = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);

    if (isPerspective) {
        gluPerspective(60.0f, aspectRatio, 1.0f, 100.0f); // Перспективная проекция
    }
    else {
        glOrtho(-aspectRatio * orthoScale, aspectRatio * orthoScale, -1.0f * orthoScale, 1.0f * orthoScale, 1.0f, 100.0f); // Ортографическая проекция
    }

    glMatrixMode(GL_MODELVIEW);
}

// Отрисовка куба
void drawCube() {
    glBegin(GL_QUADS);

    // Верхняя грань
    glColor3f(1.0f, 0.0f, 0.0f); // Красная
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    // Нижняя грань
    glColor3f(0.0f, 1.0f, 0.0f); // Зелёная
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    // Передняя грань
    glColor3f(0.0f, 0.0f, 1.0f); // Синяя
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    // Задняя грань
    glColor3f(1.0f, 1.0f, 0.0f); // Жёлтая
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);

    // Левая грань
    glColor3f(1.0f, 0.0f, 1.0f); // Фиолетовая
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);

    // Правая грань
    glColor3f(0.0f, 1.0f, 1.0f); // Голубая
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);

    glEnd();
}

// Главная функция
int main() {
    ContextSettings settings;
    settings.depthBits = 24;

    Window window(VideoMode(windowWidth, windowHeight), "3D Cube", Style::Titlebar | Style::Close, settings);
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
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Space) {
                isPerspective = !isPerspective;
                setupProjection();
            }
            if (event.type == Event::MouseWheelScrolled) {
                if (!isPerspective) {
                    orthoScale *= (event.mouseWheelScroll.delta > 0) ? 0.9f : 1.1f;
                    setupProjection();
                }
            }
            if (event.type == Event::MouseButtonPressed) {
                if (isPerspective) {
                    if (event.mouseButton.button == Mouse::Left) {
                        isDragging = true;
                        lastMousePosition = Mouse::getPosition(window);
                    }
                }
            }
            if (event.type == Event::MouseButtonReleased) {
                if (event.mouseButton.button == Mouse::Left) {
                    isDragging = false;
                }
            }
            if (event.type == Event::MouseMoved && isDragging) {
                Vector2i currentMousePosition = Mouse::getPosition(window);
                angleY += (currentMousePosition.x - lastMousePosition.x) * 0.5f;
                angleX += (currentMousePosition.y - lastMousePosition.y) * 0.5f;
                lastMousePosition = currentMousePosition;
            }
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
        gluLookAt(0.0f, 0.0f, cameraDistance, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

        glRotatef(angleX, 1.0f, 0.0f, 0.0f);
        glRotatef(angleY, 0.0f, 1.0f, 0.0f);

        drawCube();
        window.display();
    }

    return 0;
}