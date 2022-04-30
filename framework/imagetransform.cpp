#include "imagetransform.h"

#include <QPainter>
#include <utility>

ImageTransform::ImageTransform() {}
ImageTransform::ImageTransform(const QPointF &pos) {}

const QImage &ImageTransform::getImage() { return this->image; }
bool ImageTransform::setImage(const char *path) {
  bool succeeded = true;
  if (path != nullptr)
    succeeded = this->image.load(path);
  else
    this->image = QImage{};
  updateImageRect();
  return succeeded;
}
bool ImageTransform::setImage(const QString &path) {
  bool succeeded = this->image.load(path);
  updateImageRect();
  return succeeded;
}

void ImageTransform::setOffset(const QPointF &offset) {
  this->offset = offset;
  updateImageRect();
}
void ImageTransform::setAlignment(Qt::Alignment alignment) {
  this->alignment = alignment;
  updateImageRect();
}

void ImageTransform::updateImageRect() {
  this->prepareGeometryChange();
  imageRect = this->image.rect();
  imageRect.translate(this->offset);
  QPointF alignmentPoint;
  switch (this->alignment & Qt::AlignHorizontal_Mask) {
    case Qt::AlignLeft:
      alignmentPoint.setX(0);
      break;
    case Qt::AlignHCenter:
      alignmentPoint.setX(this->image.width() / 2);
      break;
    case Qt::AlignRight:
      alignmentPoint.setX(this->image.width());
      break;
  }
  switch (this->alignment & Qt::AlignVertical_Mask) {
    case Qt::AlignTop:
      alignmentPoint.setY(0);
      break;
    case Qt::AlignVCenter:
      alignmentPoint.setY(this->image.height() / 2);
      break;
    case Qt::AlignBottom:
      alignmentPoint.setY(this->image.height());
      break;
  }
  imageRect.translate(-alignmentPoint);
}

QRectF ImageTransform::boundingRect() const { return this->imageRect; }
void ImageTransform::paint(QPainter *painter,
                           const QStyleOptionGraphicsItem *option,
                           QWidget *widget) {
  if (this->image.isNull()) return;
  Q_UNUSED(option);
  Q_UNUSED(widget);
  painter->drawImage(this->boundingRect(), this->image);
}
