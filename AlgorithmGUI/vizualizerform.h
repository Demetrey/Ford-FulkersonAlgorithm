#ifndef VIZUALIZERFORM_H
#define VIZUALIZERFORM_H

#include <QMainWindow>

#include <QPixmap>

#include "scaledpixmap.h"

namespace Ui {
class VizualizerForm;
}

class VizualizerForm : public QMainWindow
{
    Q_OBJECT

public:
    explicit VizualizerForm(QWidget *parent = nullptr);
    ~VizualizerForm();

private slots:

private:
    Ui::VizualizerForm *ui;
    ScaledPixmap *mypixmap;
};

#endif // VIZUALIZERFORM_H
