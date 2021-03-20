#ifndef INTERLAYER_H
#define INTERLAYER_H

#include <QObject>

#include <fstream>
#include <string>
#include <QFile>
#include <QDataStream>

#include "algorithm.h"

/*
 * Класс является прослойкой (переходником) между
 * STL и QtFramework
 */

class Interlayer
{
public:
    Interlayer();
    ~Interlayer();

    bool reader(std::vector<std::vector<int>> &matrix, const std::string &filename); // Чтение из файла матрицы

    bool set_from_file(const QString &filename); // Инициализация матрицей из файла с именем filename
    bool set_from_matrix(const QVector<QVector<int>> &matrix); // Инициализация из матрицы

    int get_f(); // Возвращает мощность максимального потока | пропускную способность минимального сечения
    QVector<QVector<int>> get_matrix(const bool &what); // Возвращает матрицу U или F
    QVector<int> get_sech(); // Возвращает узлы минимального сечения S' или S-'

    // Сохранение матрицы в файл
    bool save_file(const QString &filename, const bool &what);

private:
    Algorithm _ff;
    int _res;

};

#endif // INTERLAYER_H
