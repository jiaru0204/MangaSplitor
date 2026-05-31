#include "splittor.h"

Splittor::Splittor() {}

void Splittor::splitDoublePageSpread(QFile *file, ReadingOrder readingOrder)
{
    QFileInfo fileInfo(*file);

    QImage imageDoublePageSpread;
    imageDoublePageSpread.load(fileInfo.absoluteFilePath());

    const int width = imageDoublePageSpread.width();
    const int height = imageDoublePageSpread.height();
    const int halfWidth = width / 2;

    QImage leftPartImage = imageDoublePageSpread.copy(0, 0, halfWidth, height);
    QImage rightPartImage = imageDoublePageSpread.copy(halfWidth, 0, width - halfWidth, height);

    const QString baseName = fileInfo.baseName();
    QString baseNameLeft;
    QString baseNameRight;
    if(readingOrder == ReadingOrder::RightToLeft)
    {
        baseNameLeft = baseName + "-2";
        baseNameRight = baseName + "-1";
    }
    if(readingOrder == ReadingOrder::LeftToRight)
    {
        baseNameLeft = baseName + "-1";
        baseNameRight = baseName + "-2";
    }
    qDebug() << baseNameLeft;
    qDebug() << baseNameRight;

    QString doublePageSpreadName = fileInfo.path() + "/" + fileInfo.baseName() + "_doublePageSpread" + "." + fileInfo.completeSuffix();
    QString leftName = fileInfo.path() + "/" + baseNameLeft + "." + fileInfo.completeSuffix();
    QString rightName = fileInfo.path() + "/" + baseNameRight + "." + fileInfo.completeSuffix();

    file->rename(doublePageSpreadName);
    leftPartImage.save(leftName);
    rightPartImage.save(rightName);
    file->remove();
}
