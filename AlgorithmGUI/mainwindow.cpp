#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    flag_save = 0;
    ui->setupUi(this);
    ui->label_3->setToolTip("Указывается при задаче матрицы вручную");
    ui->pushButton->setEnabled(false);

    if(!ui->menu->actions().empty())
    {
        for(int i = 0; i < ui->menu->actions().size(); i++)
        {
            if(ui->menu->actions().at(i)->objectName() == "")
            {
                action_save = ui->menu->actions().at(i);
                for(int j = 0; j  < ui->menu->actions().at(i)->menu()->actions().size(); j++)
                {
                    if(ui->menu->actions().at(i)->menu()->actions().at(j)->objectName() == "action_4")
                        action_save_answer = ui->menu->actions().at(i)->menu()->actions().at(j);
                }
            }
        }
    }

    action_save->setEnabled(false);
    action_save_answer->setEnabled(false);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::add_data_to_form()
{
    ui->lineEdit->setText(QString::number(_interlayer.get_f()));
    ui->lineEdit_2->setText(QString::number(_interlayer.get_f()));

    ui->textEdit->clear();
    ui->textEdit->insertPlainText("Минимальное сечение делит узлы на 2 подмножества:\n S : {");
    for(int i = 0; i < _interlayer.get_sech().size(); i++)
    {
        ui->textEdit->moveCursor(QTextCursor::End);
        ui->textEdit->insertPlainText(QString::number(_interlayer.get_sech().at(i) + 1) + "; ");
    }
    ui->textEdit->moveCursor(QTextCursor::End);
    ui->textEdit->insertPlainText("}\nS\' : {");
    for(int i = 0; i < ui->tableWidget->columnCount(); i++)
    {
        bool k = 0;
        for(int j = 0; j < _interlayer.get_sech().size(); j++)
            if(i == _interlayer.get_sech().at(j))
            {
                k = 1;
                break;
            }
        if(!k)
        {
            ui->textEdit->moveCursor(QTextCursor::End);
            ui->textEdit->insertPlainText(QString::number(i + 1) + "; ");
        }
    }
    ui->textEdit->moveCursor(QTextCursor::End);
    ui->textEdit->insertPlainText("}");
}

int MainWindow::runProcessWaited(const std::string &processname)
{
    QProcess Process;
    auto Command = QString("cmd.exe");
    auto Arguments = QStringList{
                QString("/C"),
                QString::fromStdString(processname)
    };

    Process.start(Command, Arguments);
    Process.waitForFinished(-1);

    return Process.exitCode();
}


void MainWindow::on_pushButton_clicked()
{
    QString matr_size = ui->lineEdit_3->text();

    for(int i = 0; i < matr_size.size(); i++)
        if(matr_size[i] < "0" || matr_size[i] > "9")
            return;

    int size = matr_size.toInt();
    ui->tableWidget->setColumnCount(size);
    ui->tableWidget->setRowCount(size);
    ui->tableWidget_2->setColumnCount(size);
    ui->tableWidget_2->setRowCount(size);

    ui->tableWidget->clear();
    ui->tableWidget_2->clear();
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->textEdit->clear();

    action_save_answer->setEnabled(false);
    action_save->setEnabled(false);

}

void MainWindow::on_pushButton_2_clicked()
{
    try
    {

        QVector<QVector<int>> temp;

        if(ui->tableWidget->rowCount() == 0)
            throw -1;

        action_save->setEnabled(true);
        action_save_answer->setEnabled(true);

        for(int i = 0; i < ui->tableWidget->rowCount(); i++)
        {
            temp.push_back(QVector<int>());
            for(int j = 0; j < ui->tableWidget->columnCount(); j++)
            {
                if(ui->tableWidget->item(i, j) == NULL)
                {
                    temp[i].push_back(0);
                }
                else
                {
                    temp[i].push_back((ui->tableWidget->item(i, j))->text().toInt());
                }
            }
        }

        if(!_interlayer.set_from_matrix(temp))
            return;

        if(_interlayer.get_f() == -1)
        {
            action_save_answer->setEnabled(false);
            QMessageBox::information(this, tr("ФАТАЛЬ ЕРРОРЬ"), tr("Решение не было найдено!"));
            return;
        }

        temp = _interlayer.get_matrix(0);

        for(int i = 0; i < temp.size(); i++)
        {
            for(int j = 0; j < temp[i].size(); j++)
            {
                ui->tableWidget_2->setItem(i, j, new QTableWidgetItem(QString::number(temp[i][j])));
            }
        }

        ui->lineEdit_3->setText(QString::number(temp.size()));

        add_data_to_form();
    }
    catch(...)
    {
        action_save->setEnabled(false);
        QMessageBox::information(this, tr("ФАТАЛЬ ЕРРОРЬ"), tr("Была задана некорректная матрица пропускных способностей"));
        return;
    }

}


void MainWindow::on_action_triggered()
{
    action_save->setEnabled(true);
    action_save_answer->setEnabled(true);
    QString filedata = QFileDialog::getOpenFileName(this, tr("Открыть файл"), "", tr("CSV (*.csv)"));
    QFile file(filedata);
    if(!file.open(QIODevice::ReadOnly))
        return;
    file.close();

    if(!_interlayer.set_from_file(filedata))
    {
        action_save_answer->setEnabled(false);
        QMessageBox::information(this, tr("ФАТАЛЬ ЕРРОРЬ"), tr("Данные в файле некорректны!"));
        return;
    }

    QVector<QVector<int>> temp = _interlayer.get_matrix(1);

    int size = temp.size();
    ui->tableWidget->setColumnCount(size);
    ui->tableWidget->setRowCount(size);
    ui->tableWidget_2->setColumnCount(size);
    ui->tableWidget_2->setRowCount(size);

    for(int i = 0; i < temp.size(); i++)
    {
        for(int j = 0; j < temp[i].size(); j++)
        {
            ui->tableWidget->setItem(i, j, new QTableWidgetItem(QString::number(temp[i][j])));
        }
    }

    if(_interlayer.get_f() == -1)
    {
        action_save_answer->setEnabled(false);
        QMessageBox::information(this, tr("ФАТАЛЬ ЕРРОРЬ"), tr("Решение не было найдено!"));
        return;
    }

    temp = _interlayer.get_matrix(0);

    for(int i = 0; i < temp.size(); i++)
    {
        for(int j = 0; j < temp[i].size(); j++)
        {
            ui->tableWidget_2->setItem(i, j, new QTableWidgetItem(QString::number(temp[i][j])));
        }
    }

    ui->lineEdit_3->setText(QString::number(temp.size()));

    add_data_to_form();
    action_save->setEnabled(true);
}

void MainWindow::on_action_3_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Сохранить исходные данные"), "", tr("CSV (*.csv)"));
    if(filename.isEmpty())
        return;
    else
    {
        if(!_interlayer.save_file(filename, 1))
            QMessageBox::information(this, tr("ФАТАЛЬ ЕРРОР!"), tr("Не получилось сохранить файл"));
    }
}

void MainWindow::on_action_4_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Сохранить решение"), "", tr("CSV (*.csv)"));
    if(filename.isEmpty())
        return;
    else
    {
        if(!_interlayer.save_file(filename, 0))
            QMessageBox::information(this, tr("ФАТАЛЬ ЕРРОР!"), tr("Не получилось сохранить файл"));
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    _log_view_window = new LogView(this);
    _log_view_window->setWindowTitle("Ход вычисления");
    _log_view_window->show();
}

void MainWindow::on_pushButton_4_clicked()
{
    Vizualizer viz(_interlayer.get_matrix(1), _interlayer.get_sech());
    if(!viz.vizualize())
        return;

    runProcessWaited("rundot.bat");
    _vf = new VizualizerForm(this);
    _vf->setWindowTitle("Граф");
    _vf->show();
}

void MainWindow::on_lineEdit_3_textChanged(const QString &arg1)
{
    if(arg1 == "" || arg1.toInt() <= 0)
        ui->pushButton->setEnabled(false);
    else
        ui->pushButton->setEnabled(true);
}

void MainWindow::on_action_2_triggered()
{
    QString data;
    QFile aboutfile(":/data/Resources/About.txt");
    if(!aboutfile.open(QIODevice::ReadOnly))
    {
        return;
    }
    while (!aboutfile.atEnd())
    {
        data += aboutfile.readLine();
    }
    aboutfile.close();
    data.replace("\n", "<br>");
    QMessageBox m_box;
    m_box.setIcon(QMessageBox::Information);
    m_box.setWindowTitle("О программе");
    m_box.setTextFormat(Qt::RichText);
    m_box.setText(data);
    m_box.setStandardButtons(QMessageBox::Ok);
    m_box.exec();

}

void MainWindow::on_action_5_triggered()
{
    QString data;
    QFile aboutfile(":/data/Resources/HowToUse.txt");
    if(!aboutfile.open(QIODevice::ReadOnly))
    {
        return;
    }
    while (!aboutfile.atEnd())
    {
        data += aboutfile.readLine();
    }
    aboutfile.close();
    data.replace("\n", "<br>");
    QMessageBox m_box;
    m_box.setIcon(QMessageBox::Information);
    m_box.setWindowTitle("Использование");
    m_box.setTextFormat(Qt::RichText);
    m_box.setText(data);
    m_box.setStandardButtons(QMessageBox::Ok);
    m_box.exec();
}
