#include "logview.h"
#include "ui_logview.h"

LogView::LogView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LogView)
{
    ui->setupUi(this);
    load_file();
}

LogView::~LogView()
{
    delete ui;
}

void LogView::load_file()
{
    std::ifstream logread("logfile.log");
    if (!logread.is_open())
    {
        logread.close();
        return ;
    }

    while (!logread.eof())
    {
        std::string temp = "";
        getline(logread, temp);
        ui->textEdit->moveCursor(QTextCursor::End);
        ui->textEdit->insertPlainText(QString::fromStdString(temp) + "\n");
    }
    logread.close();
}
