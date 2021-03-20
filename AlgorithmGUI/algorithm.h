#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <vector>
#include <deque>
#include <cmath>
#include <algorithm>
#include <fstream>

    /*
    * Класс Algorithm
    * Реализует алгоритм Форда - Фалкерсона
    * Написан без привязки к фреймворку Qt, используются STL контейнеры (std::vector)
    */

class Algorithm
{
public:
    Algorithm();
    ~Algorithm();

        // Инициализация матрицы u для вычислений
    bool init_u(const std::vector<std::vector<int>> &matrix);
        // Вычисления
    int calc();
        // Поиск пути в графе
    bool searc_way(const std::vector<std::vector<int>> &matrix, bool flag = 0);
        // Поиск минимального сечения
    int search_min_s(const std::vector<std::vector<int>> &matrix);

        // Геттеры
    int get_size();
    std::vector<std::vector<int>> get_u();
    std::vector<std::vector<int>> get_f();
    std::vector<int> get_knots();

private:
    int _size;                          // Размер матрицы
    std::vector<std::vector<int>> _u;   // Матрица пропускных способностей (U)
    std::vector<std::vector<int>> _f;   // Матрица потока (f)
    std::vector<int> _knots;            // Узлы графа, образующие путь от истока до стока
                                        //в итоге сюда записывается множество узлов сечения со стороны истока (S с чертой)
    std::vector<bool> _v_kn;            // Массив, используемый для определения пути
                                        // Вынесен с целью использовния в двух разных методах
};

#endif // ALGORITHM_H
