#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/System.hpp>
#include <cmath>
#include <iostream>

using namespace sf;
using namespace std;

// Параметры окна
const unsigned int windowWidth = 1200;
const unsigned int windowHeight = 800;

// Размер точки для визуализации
const float pointRadius = 5.0f;

// Центр окна
const Vector2f windowCenter(windowWidth / 2, windowHeight / 2);

// Инициализация контрольных точек относительно центра окна
Vector2f controlPoints[3] = {
    Vector2f(windowCenter.x - 300.0f, windowCenter.y),   // Левая точка
    Vector2f(windowCenter.x, windowCenter.y - 200.0f),   // Верхняя точка
    Vector2f(windowCenter.x + 300.0f, windowCenter.y)    // Правая точка
};

// Переменная для отслеживания перетаскивания
int selectedPoint = -1;  // -1 если ни одна точка не выбрана

// Функция для проверки, находится ли мышь на контрольной точке
bool isMouseOverPoint(const Vector2f& mousePos, const Vector2f& point) {
    return (abs(mousePos.x - point.x) < pointRadius) && (abs(mousePos.y - point.y) < pointRadius);
}

// Функция для вычисления точки на кривой Безье для параметра t
Vector2f calculateBezierPoint(float t, const Vector2f& P0, const Vector2f& P1, const Vector2f& P2) {
    float u = 1 - t;
    float tt = t * t;
    float uu = u * u;

    Vector2f point = uu * P0;            // (1-t)^2 * P0
    point += 2 * u * t * P1;             // 2*(1-t)*t*P1
    point += tt * P2;                    // t^2 * P2

    return point;
}

int main()
{
    // Настройки контекста OpenGL: 24 бита на буфер глубины
    ContextSettings settings;
    settings.depthBits = 24;

    // Создаем окно с использованием OpenGL
    Window window(VideoMode(windowWidth, windowHeight), "Bezier Curve", Style::Titlebar | Style::Close, settings);
    window.setFramerateLimit(120);

    // Устанавливаем светло-серый цвет фона (RGB: 0.8, 0.8, 0.8)
    glClearColor(0.8f, 0.8f, 0.8f, 1.0f);


    // Настройка 2D проекции
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, windowWidth, windowHeight, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);

    // Включаем антиалиасинг линий и точек
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

    // Основной цикл приложения
    bool running = true;
    while (running)
    {
        // Обрабатываем события
        Event event;
        while (running)
        {
            Event event;
            while (window.pollEvent(event))
            {
                if (event.type == Event::Closed)
                {
                    running = false;
                }
                // Если кнопка мыши нажата
                if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
                {
                    Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
                    // Проверяем, попали ли мы в одну из контрольных точек
                    for (int i = 0; i < 3; ++i)
                    {
                        if (isMouseOverPoint(mousePos, controlPoints[i])) {
                            selectedPoint = i;  // Запоминаем, какую точку начали двигать
                            break;
                        }
                    }
                }
                // Если кнопка мыши отпущена
                if (event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left)
                {
                    selectedPoint = -1;  // Прекращаем перемещение
                }

                // Если мышь двигается и точка выбрана
                if (event.type == Event::MouseMoved && selectedPoint != -1)
                {
                    controlPoints[selectedPoint] = Vector2f(event.mouseMove.x, event.mouseMove.y);
                }
            }

            // Очищаем экран
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Рисуем контрольные точки
            glColor3f(0.0f, 0.0f, 0.0f);
            glPointSize(pointRadius * 2);
            glBegin(GL_POINTS);
            for (int i = 0; i < 3; ++i)
            {
                glVertex2f(controlPoints[i].x, controlPoints[i].y);
            }
            glEnd();

            // Рисуем линию кривой Безье
            glLineWidth(2.0f);  // Толщина линии кривой
            glBegin(GL_LINE_STRIP);
            for (float t = 0; t <= 1; t += 0.001f) {
                Vector2f bezierPoint = calculateBezierPoint(t, controlPoints[0], controlPoints[1], controlPoints[2]);
                glVertex2f(bezierPoint.x, bezierPoint.y);
            }
            glEnd();

            // Меняем буферы местами для отображения следующего кадра
            window.display();
        }
    }

    return 0;
}