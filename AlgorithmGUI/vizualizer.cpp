#include "vizualizer.h"

Vizualizer::Vizualizer(const QVector<QVector<int>> &matrix, const QVector<int> &arr)
{
    this->matrix = matrix;
    this->arr = arr;
}

Vizualizer::~Vizualizer()
{
}

bool Vizualizer::vizualize()
{
    std::ofstream outfile("input\\outf.gv");

    if(!outfile.is_open())
    {
        return 0;
    }

    outfile << "digraph CFG {\n";
    outfile << "node[style=wedged, color=blue, fontcolor=black]\n";

    std::string temp = "";

    outfile << "{";
    for(int i = 0; i < arr.size(); i++)
    {
        temp += std::to_string(arr[i] + 1) + ", ";
    }
    temp = temp.substr(0, temp.size() - 2);
    temp += "}\n";
    outfile << temp;
    outfile << "node[style=wedged, color=black, fontcolor=black]\n";
    temp = "";


    for(int i = 0; i < this->matrix.size(); i++)
    {
        for(int j = 0; j < this->matrix[i].size(); j++)
        {
            if(j <= i)
            {
                continue;
            }

            if(this->matrix[i][j] != 0)
            {
                std::string temp = "";

                temp += std::to_string(i + 1) + "->" +
                        std::to_string(j + 1) + "[label = \"(" +
                        std::to_string(this->matrix[i][j]) + ", " +
                        std::to_string(this->matrix[j][i]) + ")\"]";

                bool f = 1;
                for(int k = 0; k < arr.size(); k++)
                {
                    if(i == arr[k])
                    {
                        f = 0;
                        for(int m = 0; m < arr.size(); m++)
                            if(j == arr[m])
                                f = 1;
                    }
                    if(j == arr[k])
                    {
                        f = 0;
                        for(int m = 0; m < arr.size(); m++)
                            if(i == arr[m])
                                f = 1;
                    }
                }
                if(!f)
                {
                    temp += "[fillcolor = red]";
                }
                temp += " [dir=both];\n";

                outfile << temp;
            }
        }
    }

    outfile << "}";
    outfile.close();

    return 1;
}
