#ifndef VIZUALIZER_H
#define VIZUALIZER_H

#include <QObject>

#include <fstream>
#include <string>

class Vizualizer
{
public:
    Vizualizer(const QVector<QVector<int>> &matrix, const QVector<int> &arr);
    ~Vizualizer();

    bool vizualize();

private:
    QVector<QVector<int>> matrix;
    QVector<int> arr;

};

#endif // VIZUALIZER_H
