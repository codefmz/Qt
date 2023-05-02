#ifndef IMAGELOADER_H
#define IMAGELOADER_H

#include <QThread>
class ImageLoader:public QThread
{
    Q_OBJECT
public:
    ImageLoader();
    void loadImages(const QStringList & );
    void run() override;
};

#endif // IMAGELOADER_H
