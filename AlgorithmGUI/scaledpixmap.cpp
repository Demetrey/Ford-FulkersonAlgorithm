#include "scaledpixmap.h"

ScaledPixmap::ScaledPixmap(QWidget *parent) : QWidget(parent)
{
}

void ScaledPixmap::setScaledPixmap(const QPixmap &pixmap)
{
  m_pixmap = pixmap;
  update();
}

void ScaledPixmap::paintEvent(QPaintEvent *event)
{
  QPainter painter(this);
  if (false == m_pixmap.isNull())
  {
    QSize widgetSize = size();
    QPixmap scaledPixmap = m_pixmap.scaled(widgetSize, Qt::KeepAspectRatio);
    QPoint center((widgetSize.width() - scaledPixmap.width())/2,
                  (widgetSize.height() - scaledPixmap.height())/2);
    painter.drawPixmap(center, scaledPixmap);
  }
  QWidget::paintEvent(event);
}
