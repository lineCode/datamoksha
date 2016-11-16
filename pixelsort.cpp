#include "pixelsort.h"

Pixelsort::Pixelsort(Direction direction, CompareFunction compareFunction, QImage *edges)
{
    this->direction = direction;
    this->compareFunction = compareFunction;
    this->edges = edges;
}

int compare_red(const void *a, const void *b) { return QColor::fromRgb(*(QRgb*)a).red() - QColor::fromRgb(*(QRgb*)b).red(); }
int compare_green(const void *a, const void *b) { return QColor::fromRgb(*(QRgb*)a).green() - QColor::fromRgb(*(QRgb*)b).green(); }
int compare_blue(const void *a, const void *b) { return QColor::fromRgb(*(QRgb*)a).blue() - QColor::fromRgb(*(QRgb*)b).blue(); }
int compare_hue(const void *a, const void *b) { return QColor::fromRgb(*(QRgb*)a).hue() - QColor::fromRgb(*(QRgb*)b).hue(); }
int compare_saturation(const void *a, const void *b) { return QColor::fromRgb(*(QRgb*)a).saturation() - QColor::fromRgb(*(QRgb*)b).saturation(); }
int compare_value(const void *a, const void *b) { return QColor::fromRgb(*(QRgb*)a).value() - QColor::fromRgb(*(QRgb*)b).value(); }
int compare_cyan(const void *a, const void *b) { return QColor::fromRgb(*(QRgb*)a).cyan() - QColor::fromRgb(*(QRgb*)b).cyan(); }
int compare_magenta(const void *a, const void *b) { return QColor::fromRgb(*(QRgb*)a).magenta() - QColor::fromRgb(*(QRgb*)b).magenta(); }
int compare_yellow(const void *a, const void *b) { return QColor::fromRgb(*(QRgb*)a).yellow() - QColor::fromRgb(*(QRgb*)b).yellow(); }
int compare_black(const void *a, const void *b) { return QColor::fromRgb(*(QRgb*)a).black() - QColor::fromRgb(*(QRgb*)b).black(); }

int (*Pixelsort::getCompareFunctionPointer())(const void *a, const void *b)
{
    switch (this->compareFunction)
    {
    case CompareHue: return &compare_hue;
    case CompareSaturation: return &compare_saturation;
    case CompareValue: return &compare_value;
    case CompareRed: return &compare_red;
    case CompareGreen: return &compare_green;
    case CompareBlue: return &compare_blue;
    case CompareCyan: return &compare_cyan;
    case CompareMagenta: return &compare_magenta;
    case CompareYellow: return &compare_yellow;
    case CompareBlack: return &compare_black;
    }
}

QString Pixelsort::compareFunctionLabel(CompareFunction compareFunction)
{
    switch (compareFunction)
    {
    case CompareHue: return "HSV Hue";
    case CompareSaturation: return "HSV Saturation";
    case CompareValue: return "HSV Value";
    case CompareRed: return "RGB Red";
    case CompareGreen: return "RGB Green";
    case CompareBlue: return "RGB Blue";
    case CompareCyan: return "CYMK Cyan";
    case CompareMagenta: return "CMYK Magenta";
    case CompareYellow: return "CMYK Yellow";
    case CompareBlack: return "CMYK Black";
    }
}

QStringList Pixelsort::compareFunctionLabels()
{
    QStringList list;
    for (int i = CompareFirst; i <= CompareLast; i++)
    {
        list << compareFunctionLabel((CompareFunction)i);
    }
    return list;
}

QString Pixelsort::directionLabel(Direction direction)
{
    switch (direction)
    {
    case DirectionHorizontal: return "Horizontal";
    case DirectionVertical: return "Vertical";
    }
}


QStringList Pixelsort::directionLabels()
{
    QStringList list;
    list << directionLabel(DirectionHorizontal);
    list << directionLabel(DirectionVertical);
    return list;
}

void Pixelsort::sortRow(QImage &img, int y, int start, int length)
{
    int numBytes = img.depth() / 8;

    qsort(img.scanLine(y) + start * numBytes, length, numBytes, getCompareFunctionPointer());
}

void Pixelsort::sortColumn(QImage &img, int x, int start, int length)
{
    int numBytes = img.depth() / 8;

    QRgb *column = (QRgb*)malloc(sizeof(QRgb) * length);
    for (int i = 0; i < length; i++) { column[i] = img.pixel(x, start + i); }

    qsort(column, length, numBytes, getCompareFunctionPointer());

    for (int i = 0; i < length; i++) { img.setPixel(x, start + i, column[i]); }

    free(column);
}

void Pixelsort::sortHorizontal(QImage &img)
{
    for (int i = 0; i < img.height(); i++)
    {
        if (edges != NULL)
        {
            int start = 0;
            while (start + 1 < img.width())
            {
                int j = start + 1;
                for (; j < img.width(); j++)
                {
                    if (edges->pixel(j, i) == qRgb(255, 255, 255))
                    {
                        sortRow(img, i, start, j - start);
                        start = j + 1;
                        break;
                    }
                }
                if (j == img.width())
                    break;
            }
        }
        else
        {
            sortRow(img, i, 0, img.width());
        }
    }
}

void Pixelsort::sortVertical(QImage &img)
{
    int numLines = img.width();
    int start = 0;
    int length = img.height();

    for (int i = 0; i < numLines; i++)
    {
        sortColumn(img, i, start, length);
    }
}

void Pixelsort::run(QImage &img)
{
    switch (this->direction)
    {
    case DirectionHorizontal:
        sortHorizontal(img);
        break;
    case DirectionVertical:
        sortVertical(img);
        break;
    }
}
