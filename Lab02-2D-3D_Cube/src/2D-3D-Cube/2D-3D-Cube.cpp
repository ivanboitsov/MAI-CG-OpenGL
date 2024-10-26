#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/System.hpp>
#include <gl/GLU.h>
#include <cmath>
#include <iostream>

using namespace sf;
using namespace std;

// Параметры окна
const unsigned int windowWidth = 1200;
const unsigned int windowHeight = 800;
const float aspectRatio = static_cast<float>(windowWidth) / windowHeight;

// Центр окна
const Vector2f windowCenter(windowWidth / 2, windowHeight / 2);

// Переменные для проекций
bool isPerspective = true;

// Функция для настройки проекции
void setupProjection() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float aspect = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);
    if (isPerspective) {
        // Настройки перспективной проекции
        gluPerspective(60.0f, aspect, 1.0f, 100.0f);
    }
    else {
        // Настройки ортографической проекции
        glOrtho(-2.0 * aspect, 2.0 * aspect, -2.0, 2.0, 1.0, 100.0);
    }
    glMatrixMode(GL_MODELVIEW);
}

// Функция для отрисовки куба
void drawCube() {
    glBegin(GL_QUADS);

    // Верхняя грань (y = 1.0f)
    glColor3f(1.0f, 0.0f, 0.0f);  // Красный
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    // Нижняя грань (y = -1.0f)
    glColor3f(0.0f, 1.0f, 0.0f);  // Зеленый
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    // Передняя грань (z = 1.0f)
    glColor3f(0.0f, 0.0f, 1.0f);  // Синий
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    // Задняя грань (z = -1.0f)
    glColor3f(1.0f, 1.0f, 0.0f);  // Желтый
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);

    // Левая грань (x = -1.0f)
    glColor3f(1.0f, 0.0f, 1.0f);  // Фиолетовый
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);

    // Правая грань (x = 1.0f)
    glColor3f(0.0f, 1.0f, 1.0f);  // Голубой
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);

    glEnd();
}

int main() {
    // Настройки контекста OpenGL: 24 бита на буфер глубины
    ContextSettings settings;
    settings.depthBits = 24;

    // Создаем окно с использованием OpenGL
    Window window(VideoMode(windowWidth, windowHeight), "3D Cube", Style::Titlebar | Style::Close, settings);
    window.setFramerateLimit(120);
    glEnable(GL_DEPTH_TEST);

    glClearColor(0.8f, 0.8f, 0.8f, 1.0f);

    // Настраиваем начальную проекцию
    setupProjection();

    // Основной цикл программы
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }

            // Обработка нажатия пробела для переключения проекций
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Space) {
                isPerspective = !isPerspective;
                setupProjection();
            }
        }

        // Очистка экрана
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Настройка камеры
        glLoadIdentity();
        glTranslatef(0.0f, 0.0f, -5.0f);  // Отодвигаем камеру назад, чтобы увидеть куб

        // Вращение куба, только если активна перспективная проекция
        if (isPerspective) {
            static float angle = 0.0f;
            angle += 0.5f;
            glRotatef(angle, 1.0f, 1.0f, 0.0f); // Вращаем куб по всем осям
        }

        // Отрисовка куба
        drawCube();

        // Отображение результата на экране
        window.display();
    }

    return 0;
}