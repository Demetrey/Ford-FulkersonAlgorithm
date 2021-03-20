#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QProcess>
#include <QDebug>

#include "interlayer.h"
#include "logview.h"
#include "vizualizer.h"
#include "vizualizerform.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void add_data_to_form();
    auto runProcessWaited(const std::string &processname) -> int;

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_action_triggered();

    void on_action_3_triggered();

    void on_action_4_triggered();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_lineEdit_3_textChanged(const QString &arg1);

    void on_action_2_triggered();

    void on_action_5_triggered();

private:
    Ui::MainWindow *ui;
    Interlayer _interlayer;
    LogView *_log_view_window;
    VizualizerForm *_vf;

    bool flag_save;

    QAction *action_save;
    QAction *action_save_answer;
};
#endif // MAINWINDOW_H
