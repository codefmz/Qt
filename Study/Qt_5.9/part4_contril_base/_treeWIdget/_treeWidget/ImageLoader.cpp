#include "ImageLoader.h"
#include <QPixmapCache>
#include <QDebug>
ImageLoader::ImageLoader() {


}

void ImageLoader::loadImages(const QStringList& files)
{
    qDebug().nospace()<<__FILE__<<"("<< __LINE__<<")"<<__FUNCTION__ <<" -- ";
    foreach (auto& file, files)
    {
        QPixmap pix(file);
        QPixmapCache::insert(file, pix);
        qDebug().nospace()<<__FILE__<<"("<< __LINE__<<")"<<__FUNCTION__ <<" -- ";
    }
}

void ImageLoader::run()
{

}
