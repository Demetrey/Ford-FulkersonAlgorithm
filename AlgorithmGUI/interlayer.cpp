#include "interlayer.h"

Interlayer::Interlayer()
{
    _res = 0;
}

Interlayer::~Interlayer()
{
}

bool Interlayer::reader(std::vector<std::vector<int> > &matrix, const std::string &filename)
{
    std::string temp = "";
    std::ifstream inf(filename);

    // Проверяем на is_open
    if (!inf.is_open())
    {
        inf.close();
        return 0;
    }

    int i = 0;

    while (!inf.eof())
    {
        matrix.push_back(std::vector<int>());

        getline(inf, temp);
        std::string cifera = "";

        for (unsigned int j = 0; j < temp.size(); j++)
        {
            if (temp[j] >= '0' && temp[j] <= '9')
                cifera += temp[j];
            if (temp[j] == ';')
            {
                matrix[i].push_back(atoi(cifera.c_str()));
                cifera = "";
            }
        }

        matrix[i].push_back(atoi(cifera.c_str()));

        i++;
    }

    matrix.pop_back();

    inf.close();
    return 1;
}

bool Interlayer::set_from_file(const QString &filename)
{
    // Преобразование типов QString to std::string
    std::string way = filename.toLocal8Bit().constData();
    std::vector<std::vector<int>> temp;
    if(!reader(temp, way))
        return 0;
    if(!_ff.init_u(temp))
        return 0;
    _res = _ff.calc();
    return 1;
}

bool Interlayer::set_from_matrix(const QVector<QVector<int> > &matrix)
{
    // Преобразуем QVector<QVector<int>> в std::vector<std::vector<int>>
    std::vector<std::vector<int>> temp;
    for(int i = 0; i < matrix.size(); i++)
    {
        temp.push_back(std::vector<int>());
        for(int j = 0; j < matrix[i].size(); j++)
            temp[i].push_back(matrix[i][j]);
    }
    if(!_ff.init_u(temp))
        return 0;
    _res = _ff.calc();
    return 1;
}

int Interlayer::get_f()
{
    return _res;
}

QVector<QVector<int> > Interlayer::get_matrix(const bool &what)
{
    QVector<QVector<int>> temp;
    // return U
    if(what)
    {
        for(int i = 0; i < _ff.get_size(); i++)
        {
            temp.push_back(QVector<int>());
            for(int j = 0; j < _ff.get_size(); j++)
                temp[i].push_back(_ff.get_u()[i][j]);
        }
    }
    // return F
    else
        for(int i = 0; i < _ff.get_size(); i++)
        {
            temp.push_back(QVector<int>());
            for(int j = 0; j < _ff.get_size(); j++)
                temp[i].push_back(_ff.get_f()[i][j]);
        }

    return temp;
}

QVector<int> Interlayer::get_sech()
{
    QVector<int> temp;

    for(unsigned int i = 0; i < _ff.get_knots().size(); i++)
    {
        temp.push_back(_ff.get_knots()[i]);
    }

    return temp;
}

bool Interlayer::save_file(const QString &filename, const bool &what)
{
    std::ofstream ofile(filename.toStdString());
    if(!ofile.is_open())
    {
        ofile.close();
        return 0;
    }

    // Сохраняем исходную матрицу
    if(what)
    {
        for(unsigned int i = 0; i < _ff.get_u().size(); i++)
        {
            QString temp = "";
            for(unsigned int j = 0; j < _ff.get_u()[i].size(); j++)
            {
                temp += QString::number(_ff.get_u()[i][j]) + ";";
            }
            temp.remove(temp.size() - 1, 1);
            temp += "\n";
            ofile.write(temp.toUtf8(),temp.size());
        }

    }
    //Сохраянем решение
    else
    {
        ofile << "Matrix F:\n";
        for(unsigned int i = 0; i < _ff.get_f().size(); i++)
        {
            QString temp = "";
            for(unsigned int j = 0; j < _ff.get_f()[i].size(); j++)
            {
                temp += QString::number(_ff.get_f()[i][j] + 1) + ";";
            }
            temp.remove(temp.size() - 1, 1);
            temp += "\n";
            ofile.write(temp.toUtf8(), temp.size());
        }
        ofile << "F = U(S, S\') = ;";
        QString temp = "";
        temp = QString::number(_res) + "\n";
        ofile.write(temp.toUtf8(), temp.size());
        temp = "";
        ofile << "S:;";
        for(unsigned int i = 0; i < _ff.get_knots().size(); i++)
        {
            temp += QString::number(_ff.get_knots()[i] + 1) + ";";
        }
        temp.remove(temp.size() - 1, 1);
        temp += "\n";
        ofile.write(temp.toUtf8(), temp.size());
        temp = "";
        ofile << "S\':;";
        for(int j = 0; j < _ff.get_size(); j++)
        {
            bool flag = 0;
            for(unsigned int i = 0; i < _ff.get_knots().size(); i++)
            {
                if(j == _ff.get_knots()[i])
                {
                    flag = 1;
                    break;
                }
            }
            if(!flag)
                temp += QString::number(j) + ";";
        }
        temp.remove(temp.size() - 1, 1);
        temp += "\n";
        ofile.write(temp.toUtf8(), temp.size());

    }
    ofile.close();
    return 1;
}
