#ifndef LOGVIEW_H
#define LOGVIEW_H

#include <QMainWindow>

#include <fstream>
#include <string>

namespace Ui {
class LogView;
}

class LogView : public QMainWindow
{
    Q_OBJECT

public:
    explicit LogView(QWidget *parent = nullptr);
    ~LogView();

    void load_file();

private:
    Ui::LogView *ui;
};

#endif // LOGVIEW_H
