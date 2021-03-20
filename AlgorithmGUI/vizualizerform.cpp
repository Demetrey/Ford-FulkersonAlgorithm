#include "vizualizerform.h"
#include "ui_vizualizerform.h"

VizualizerForm::VizualizerForm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::VizualizerForm)
{
    ui->setupUi(this);

    mypixmap = new ScaledPixmap();


    QPixmap pix("output\\outf.png");
    ui->gridLayout->addWidget(mypixmap);
    mypixmap->setScaledPixmap(pix);
}

VizualizerForm::~VizualizerForm()
{
    delete ui;
    delete mypixmap;
}
