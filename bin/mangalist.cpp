#include "mangalist.h"

#include "splittor.h"

MangaList::MangaList() {}

void oldCode()
{
    // bool MangaList::sort()
    // {
    //     return false;
    // }
    //
    // bool MangaList::split()
    // {
    //     if(!doublePageSpreadExist)
    //         return true;
    //
    //     return false;
    // }
    //
    // bool MangaList::merge()
    // {
    //     //  纯属构思，先不实现
    //     if(!doublePageSpreadExist)
    //         return true;
    //
    //     return false;
    // }
    //
    // MangaList::MangaList(QDir dir, QStringList nameList)
    // {
    //     for(int i = 0; i < nameList.size(); i++)
    //     {
    //         MangaFiles temp(dir, nameList[i]);
    //         fileList.push_back(temp);
    //     }
    // }
    //
    // bool MangaList::classify()
    // {
    //     //  此时已经排完序
    //     int totalWidth = 0;
    //     int min = 1919810, max = 0; //  粗略的认为一话漫画中，单页和大跨页的宽度分别为最小值与最大值，或许可以搞两点回归，用相关度判断是否为纯粹的单页与大跨页组合
    //     int n = fileList.size();
    //     for(int i = 0; i < n; i++)
    //     {
    //         QImageReader reader(fileList[i].pageFile->fileName());
    //         int width = reader.size().width();
    //         totalWidth += width;
    //         min = (width < min ? width : min);
    //         max = (width > max ? width : max);
    //     }
    //
    //     if(min == max)
    //     {
    //         doublePageSpreadExist = false;
    //         // 思考下错误码怎么解决
    //         errorCode = NoneDoublePageSpread;
    //         return false;
    //     }
    //
    //     double averageWidth = (double)totalWidth / n;
    //     for(int i = 0; i < n; i++)
    //     {
    //         // 分类
    //         QImageReader reader(fileList[i].pageFile->fileName());
    //         int width = reader.size().width();
    //
    //         toolNumber = getToolNumber(min, max);
    //         if(illegal(min, max, width))
    //         {
    //             errorCode = UnexpectedPhotoWidth;
    //             return false;
    //         }
    //         else if(width > averageWidth)
    //             fileList[i].attribute = DoublePageSpread;
    //         else if(width < averageWidth)
    //             fileList[i].attribute = SinglePage;
    //         else
    //         {
    //             errorCode = UnknownException;
    //         }
    //     }
    // }
}

void MangaList::fileDirProcess(QString absoluteFilePath, QLabel *label)
{
    //  录入输入路径
    fileInfo.setFile(absoluteFilePath);

    //  路径不存在
    if(!fileInfo.exists())
    {
        label->setText("路径不存在！");
        return;
    }

    //  只保留路径的目录部分
    if(!fileInfo.dir().exists())    //  防止文件名后面多了个"/"被识别成目录
    {
        fileInfo.setFile(fileInfo.absolutePath());
    }
    if(fileInfo.isFile())
    {
        fileInfo.setFile(fileInfo.absolutePath());
    }

    dir.setPath(fileInfo.absoluteFilePath());

    //  错误检测
    if(!dir.exists())
    {
        label->setText("哪里不对......");
    }

    //  检测路径是否为空
    dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    // qDebug() << dir.entryList().count();
    if(dir.entryList().count() == 0)
    {
        label->setText("当前路径为空！");
        return;
    }

    //  输出当前路径
    label->setText("当前路径为：" + dir.absolutePath());
}

void MangaList::dirClassification()
{
    //  literally doing nothing
    return;
}

int MangaList::getIDFromFileName(QString name)
{
    QString id = name.section('.', 0, 0);
    return id.toInt();
}

void MangaList::fileRename(QFile *file, QString newBaseName)
{
    QFileInfo fileInfo(*file);
    QString newFilePath = fileInfo.path() + "/" + newBaseName + "." + fileInfo.completeSuffix();

    qDebug() << newFilePath;
    file->rename(newFilePath);
}

void MangaList::mangaFilesInitialization(QDir dir)
{
    mangaFiles = new MangaFiles(dir);
}

void MangaList::mangaFilesClassification()
{
    for(int i = 0; i < mangaFiles->mangaFile.size(); i++)
    {
        const int width = mangaFiles->mangaFile[i].image->width();
        const int height = mangaFiles->mangaFile[i].image->height();

        if(Splittor::isDoublePageSpread(width, height))
            mangaFiles->mangaFile[i].attribute = Attribute::DoublePageSpread;
        else
            mangaFiles->mangaFile[i].attribute = Attribute::SinglePage;
    }
}

void MangaList::mangaRename(Method method, BlankPageOrNot blankPage)
{
    if(method == Method::Split)
    {
        Q_UNUSED(blankPage);
        //  切割模式保留原文件名，双页图由 splitDoublePageSpread 输出为 原名-1 / 原名-2
        return;
    }
    if(method == Method::Merge)
    {
        //  literally doing nothing
        return;
    }
}

void MangaList::mangaSplit(Method method, ReadingOrder readingOrder)
{
    if(method == Method::Split)
    {
        for(int i = 0; i < mangaFiles->mangaFile.size(); i++)
            if(mangaFiles->mangaFile[i].attribute == Attribute::DoublePageSpread)
                Splittor::splitDoublePageSpread(mangaFiles->mangaFile[i].pageFile, readingOrder);
    }
    if(method == Method::Merge)
    {
        //  literally doing nothing
        return;
    }
}
