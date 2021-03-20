#ifndef SCALEDPIXMAP_H
#define SCALEDPIXMAP_H

#include <QWidget>
#include <QPainter>

class ScaledPixmap : public QWidget
{
    Q_OBJECT
public:
    explicit ScaledPixmap(QWidget *parent = nullptr);
    void setScaledPixmap(const QPixmap &pixmap);

protected:
  void paintEvent(QPaintEvent *event);

private:
  QPixmap m_pixmap;

signals:

};

#endif // SCALEDPIXMAP_H
