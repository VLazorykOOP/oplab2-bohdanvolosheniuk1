#include <iostream>
#include <thread>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <mutex>

using namespace std;

const double V = 1.0; // Швидкість
const double X_LIMIT = 100.0; // Межа руху по осі X
const double Y_LIMIT = 100.0; // Межа руху по осі Y
const int GOLDFISH_COUNT = 5;
const int GUPPY_COUNT = 5;

mutex mtx; // М'ютекс для потокобезпечного виводу

void goldfish_movement(int id) {
    srand(time(0) + id); // Ініціалізація генератора випадкових чисел для зміни напрямку
    double x = 0.0; // Початкова позиція по осі X
    bool moving_right = true; // Прапорець напрямку руху

    while (true) {
        // Рух у поточному напрямку
        x += moving_right ? V : -V;

        // Зміна напрямку при досягненні межі
        if (x >= X_LIMIT || x <= -X_LIMIT) {
            moving_right = !moving_right;
        }

        // Вивід поточної позиції
        mtx.lock();
        cout << "Goldfish " << id << ": (" << x << ", 0)\n";
        mtx.unlock();

        // Затримка для імітації безперервного руху
        this_thread::sleep_for(chrono::milliseconds(100));
    }
}

void guppy_movement(int id) {
    srand(time(0) + id); // Ініціалізація генератора випадкових чисел для зміни напрямку
    double y = 0.0; // Початкова позиція по осі Y
    bool moving_up = true; // Прапорець напрямку руху

    while (true) {
        // Рух у поточному напрямку
        y += moving_up ? V : -V;

        // Зміна напрямку при досягненні межі
        if (y >= Y_LIMIT || y <= -Y_LIMIT) {
            moving_up = !moving_up;
        }

        // Вивід поточної позиції
        mtx.lock();
        cout << "Guppy " << id << ": (0, " << y << ")\n";
        mtx.unlock();

        // Затримка для імітації безперервного руху
        this_thread::sleep_for(chrono::milliseconds(100));
    }
}

int main() {
    vector<thread> threads;

    // Створення потоків для золотих рибок
    for (int i = 0; i < GOLDFISH_COUNT; ++i) {
        threads.push_back(thread(goldfish_movement, i));
    }

    // Створення потоків для гуппі
    for (int i = 0; i < GUPPY_COUNT; ++i) {
        threads.push_back(thread(guppy_movement, i));
    }

    // Очікування завершення всіх потоків
    for (auto& thread : threads) {
        thread.join();
    }

    return 0;
}
