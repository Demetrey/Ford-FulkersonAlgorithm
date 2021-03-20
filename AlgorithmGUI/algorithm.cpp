#include "algorithm.h"

Algorithm::Algorithm()
{
    this->_size = 0;
}

Algorithm::~Algorithm()
{
}

    /*
     * Инициализация данных для вычилений
     * IN: matrix - матрица пропускных способностей сети
     * OUT: bool
     *      true если матрица является квадратной и инициализация прошла успешно
     *      false если матрица не является квадратной
     */
bool Algorithm::init_u(const std::vector<std::vector<int>> &matrix)
{
        // Очистка матриц
    _u.clear();
    _f.clear();
    _v_kn.clear();
    _knots.clear();

        // Проверяем матрицу на соответствие (что она квадратная)
    for (unsigned int i = 0; i < matrix.size(); i++)
        if (matrix.size() != matrix[i].size())
            return 0;

    _u = matrix;
    _size = _u.size();

        // Инициализируем матрицу потока нулями
    for (int i = 0; i < _size; i++)
    {
        _f.push_back(std::vector<int>());
        for (int j = 0; j < _size; j++)
        {
            _f[i].push_back(0);
        }
    }

    return 1;
}

    /*
     * Непосредственно метод для вычисления
     * IN: void
     * OUT: int
     *      -1 в случае, если не удалось найти максимальный поток и минимальное сечение
     *      значение мощности максимального потока, равное пропускной способности минимаьного сечения
     */
int Algorithm::calc()
{
    std::ofstream logfile("logfile.log");

    if(!logfile.is_open())
    {
        logfile.close();
    }

    logfile << "START ALGORITHM:\n\tALGORITHM STEP 0:\n";
                        // Подготовительная часть
    int fi = INT_MAX;   // Пропускная способность потока (сначала инициализируем максимальным значением int (2147483647))

                        // Ищем путь, он будет записан в _knots
    if (!searc_way(_u))
    {
        logfile << "No Way\n";
        logfile.close();
        return -1;      // Если путя нет - расходимся
    }


    //---------------FOR_LOG--------------------------
    std::string tempstr = "";
    logfile << "\nMATRIX U:\n";
    for(unsigned int i = 0; i < _u.size(); i++)
    {
        for(unsigned int j = 0; j < _u[i].size(); j++)
            tempstr += std::to_string( _u[i][j]) + "\t";
        tempstr += "\n"; // fro file input line
        logfile << tempstr.c_str();
        tempstr = "";
    }

    tempstr = "";
    logfile << "\nWAY:\n";
    for(unsigned int i = 0; i < _knots.size(); i++)
        tempstr += std::to_string(_knots[i]) + "->";
    tempstr = tempstr.substr(0, tempstr.size() - 2);
    tempstr += "\n";
    logfile << tempstr.c_str();
    tempstr = "";
    logfile << "\n------------------------------------------------------\n\tALGORITHM STEP 1:\n\nF0 = MIN(";
    //---------------FOR_LOG--------------------------

                        //определяем f1 как минимальное значение пропускной способности среди найденного начального пути
    for (unsigned int i = 0; i < _knots.size() - 1; i++)
    {
        // --------------------------- FOR_LOG ---------------------------
        tempstr += std::to_string(_u[_knots[i]][_knots[i + 1]] + 1) + ", ";
        // --------------------------- FOR_LOG ---------------------------

        if (_u[_knots[i]][_knots[i + 1]] < fi)
            fi = _u[_knots[i]][_knots[i + 1]];
    }

    // --------------------------- FOR_LOG ---------------------------
    tempstr = tempstr.substr(0, tempstr.size() - 2);
    tempstr += ") = ";
    logfile << tempstr;
    logfile << std::to_string(fi) + ";\n";
    tempstr = "";
    // --------------------------- FOR_LOG ---------------------------

                        // Расставим найенное значение в матрице потока f1
    for (unsigned int i = 0; i < _knots.size() - 1; i++)
    {
        _f[_knots[i]][_knots[i + 1]] = fi;
        _f[_knots[i + 1]][_knots[i]] = -fi;
    }

    // --------------------------- FOR_LOG ---------------------------
    logfile << "\nMATRIX F0:\n";
    for(unsigned int i = 0; i < _f.size(); i++)
    {
        for(unsigned int j = 0; j < _f[i].size(); j++)
            tempstr += std::to_string( _f[i][j]) + "\t";
        tempstr += "\n"; // fro file input line
        logfile << tempstr.c_str();
        tempstr = "";
    }
    tempstr = "";
    // --------------------------- FOR_LOG ---------------------------

                        // Находим U1 как разность U и F1, временно используем матрицу fi под ui (экономия памяти)
    for (int i = 0; i < _size; i++)
        for (int j = 0; j < _size; j++)
            _f[i][j] = _u[i][j] - _f[i][j];

    // --------------------------- FOR_LOG ---------------------------
    int iteration_count = 1;
    logfile << "------------------------------------------------------\n\tALGORITHM STEP "
                    + std::to_string(iteration_count + 1);
    logfile << "\n\nMATRIX U1:\n";
    for(unsigned int i = 0; i < _f.size(); i++)
    {
        for(unsigned int j = 0; j < _f[i].size(); j++)
            tempstr += std::to_string( _f[i][j]) + "\t";
        tempstr += "\n"; // fro file input line
        logfile << tempstr.c_str();
        tempstr = "";
    }
    tempstr = "";
    // --------------------------- FOR_LOG ---------------------------


    while (1)
    {
        // --------------------------- FOR_LOG ---------------------------
        iteration_count++;
        // --------------------------- FOR_LOG ---------------------------


                        // Ищем путь в полученной матрице
                        // Если путь не будет найден, то мы нашли минимальное сечение
        if (!searc_way(_f))
        {
            logfile << "\nWAY: NO WAY\n";
            break;      // Если путя нет, то мы уже нашли минимальное сечение, уходим
        }


        // --------------------------- FOR_LOG ---------------------------
        tempstr = "";
        logfile << "\nWAY:\n";
        for(unsigned int i = 0; i < _knots.size(); i++)
            tempstr += std::to_string(_knots[i]) + "->";
        tempstr = tempstr.substr(0, tempstr.size() - 2);
        tempstr += "\n";
        logfile << tempstr.c_str();
        tempstr = "";
        logfile << "\nSIGMA " + std::to_string(iteration_count) + " = MIN(";
        // --------------------------- FOR_LOG ---------------------------


                        //определяем sigma как минимальное значение пропускной способности среди найденного нового пути
        int sigma = INT_MAX;
        for (unsigned int i = 0; i < _knots.size() - 1; i++)
        {
            // --------------------------- FOR_LOG ---------------------------
            tempstr += std::to_string(_f[_knots[i]][_knots[i + 1]] + 1) + ", ";
            // --------------------------- FOR_LOG ---------------------------

            if (_f[_knots[i]][_knots[i + 1]] < sigma)
                sigma = _f[_knots[i]][_knots[i + 1]];
        }

        // --------------------------- FOR_LOG ---------------------------
        tempstr = tempstr.substr(0, tempstr.size() - 2);
        tempstr += ") = ";
        logfile << tempstr;
        logfile << std::to_string(sigma) + ";\n";
        tempstr = "";
        // --------------------------- FOR_LOG ---------------------------

        fi += sigma;    // Определяем мощность потока

        // --------------------------- FOR_LOG ---------------------------
        logfile << "\nF" + std::to_string(iteration_count) + " = " + std::to_string(fi) + "\n";
        logfile << "------------------------------------------------------\n\tALGORITHM STEP "
                        + std::to_string(iteration_count + 1);
        logfile << "\n\nMATRIX U" + std::to_string(iteration_count) + ":\n";
        // --------------------------- FOR_LOG ---------------------------


        // Изменяем соответствующие значения на соответствующую величину (+/-sigma) в ui
        for (unsigned int i = 0; i < _knots.size() - 1; i++)
        {
            _f[_knots[i]][_knots[i + 1]] -= sigma;
            _f[_knots[i + 1]][_knots[i]] += sigma;
        }

        // --------------------------- FOR_LOG ---------------------------
        for(unsigned int i = 0; i < _f.size(); i++)
        {
            for(unsigned int j = 0; j < _f[i].size(); j++)
                tempstr += std::to_string( _f[i][j]) + "\t";
            tempstr += "\n"; // fro file input line
            logfile << tempstr.c_str();
            tempstr = "";
        }
        tempstr = "";
        // --------------------------- FOR_LOG ---------------------------

    }

    int u = search_min_s(_f);

    // --------------------------- FOR_LOG ---------------------------
    logfile << "MINIMUM CROSS-SECTION THROUGHPUT: " + std::to_string(u) + "\n";
    logfile << "MAXIMUM FLOW POWER: " + std::to_string(u) + "\n";
    tempstr = "";
    logfile << "\nS : {";
    for(unsigned int i = 0; i < _knots.size(); i++)
    {
        tempstr += std::to_string(_knots[i]) + ", ";
    }
    tempstr = tempstr.substr(0, tempstr.size() - 2);
    tempstr += "}\n";
    logfile << tempstr;
    tempstr = "";
    logfile << "S\' : {";
    for(int i = 0; i < _size; i++)
    {
        bool flag = 0;

        for(unsigned int j = 0; j < _knots.size(); j++)
        {
            if(i == _knots[j])
            {
                flag = 1;
                break;
            }
        }

        if(!flag)
            tempstr += std::to_string(i) + ", ";
    }
    tempstr = tempstr.substr(0, tempstr.size() - 2);
    tempstr += "}\n\n";
    logfile << tempstr;
    tempstr = "";
    // --------------------------- FOR_LOG ---------------------------

                        // Находим FN как разность U и UN
    for (int i = 0; i < _size; i++)
        for (int j = 0; j < _size; j++)
            _f[i][j] = _u[i][j] - _f[i][j];

    // --------------------------- FOR_LOG ---------------------------
    logfile << "F" + std::to_string(iteration_count) + " MATRIX:\n";
    for(unsigned int i = 0; i < _f.size(); i++)
    {
        for(unsigned int j = 0; j < _f[i].size(); j++)
            tempstr += std::to_string( _f[i][j]) + "\t";
        tempstr += "\n"; // fro file input line
        logfile << tempstr.c_str();
        tempstr = "";
    }
    tempstr = "";
    // --------------------------- FOR_LOG ---------------------------

    if (fi == u)
    {
        logfile.close();
        return fi;
    }

    logfile.close();
    return -1;
}

    /*
     * Метод поиска пути в графе
     * IN:  matrix - матрица, в которой необходимо найти путь
     *      flag
     *      true - не убираем посещенные вершины (составляем S с чертой)
     *      false - убираем посещенные вершины из списка (ищем путь)
     * OUT: bool
     *      true - путь найден
     *      false - путь не удалось найти
     */
bool Algorithm::searc_way(const std::vector<std::vector<int>>& matrix, bool flag)
{
    _knots.clear();         // Удоли старый путь
    bool is_way = 0;        // Флаг нахождения пути
    _v_kn.clear();
    std::deque<int> dque;   // Очередь из вершин

    for (int i = 0; i < _size; i++)
        _v_kn.push_back(0);

    dque.push_back(0);      // Заносим в очередь исток

                            // Пока очередь не пуста
    while (!dque.empty())
    {
        int current = dque.back();  // Записываем индекс последнего узла в очереди
        if (_v_kn[current] == 0)
        {
            _v_kn[current] = 1;  // ставим этот узел как посещенный
            _knots.push_back(current);
        }

                            // Если текущий элемент - сток, то сваливаем
        if (current == _size - 1)
        {
            is_way = 1;
            break;
        }

                            // Для каждого узла, до которого можем добраться из текущего положения
        for (int i = 0; i < _size; i++)
        {
                            // Смотрим, не посещали ли мы его и можем ли до него добраться
            if (matrix[current][i] != 0 && _v_kn[i] == 0)
                dque.push_back(i);  // Если да, то записываем ужел в очередь
        }

                            // Если последний в очереди элемент мы уже посещали (зашли в тупик)
        if (_v_kn[dque.back()] == 1)
        {
            if(!flag)
                _knots.erase(std::remove(_knots.begin(), _knots.end(), dque.back()), _knots.end());
            dque.pop_back();
        }
    }

    return is_way;
}

    /*
     * Поиск минимального сечения и вычисление его пропускной способности
     * Действует аналогично поиску пути в графе, но не убирает ранее посещенные
     * узлы из списка, тем самым составляя сечение. В случае наличия пути
     * от узла 0 до узла N (max index) находит путь, но не всегда корректно из-
     * за отсцтствия части необходимого функционала
     *
     * IN: matrix - марица Un, в которой необходимо определить минимальное сечение
     * OUT: int
     *      значение пропускной способности минимального сечения, вычисленное по матрице U
     */
int Algorithm::search_min_s(const std::vector<std::vector<int>>& matrix)
{
    int u = 0;              // пропускная способность минимального потока

    searc_way(matrix, 1);

    for (unsigned int i = 0; i < _knots.size(); i++)
    {
        for (int j = 0; j < _size; j++)
        {
            if (_u[_knots[i]][j] != 0 && _v_kn[j] == 0)
                u += _u[_knots[i]][j];
        }
    }

    return u;
}




int Algorithm::get_size()
{
    return this->_size;
}

std::vector<std::vector<int>> Algorithm::get_u()
{
    return this->_u;
}

std::vector<std::vector<int>> Algorithm::get_f()
{
    return _f;
}

std::vector<int> Algorithm::get_knots()
{
    return _knots;
}

