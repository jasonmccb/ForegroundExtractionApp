#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QFileDialog>
#include <QImageReader>

typedef QRgb YUV;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButtonChooseImageToProcess_clicked();

    void on_pushButtonChooseImageCleanPlate_clicked();

    void on_pushButtonLoadImageToProcess_clicked();

    void on_pushButtonLoadImageCleanPlate_clicked();

    void on_pushButtonDebayerImageToProcess_clicked();

    void on_pushButtonDebayerImageCleanPlate_clicked();

    void on_pushButtonSaveBelowImage_clicked();

    void on_pushButtonExtractForeGround_clicked();

private:
    Ui::MainWindow *ui;
    void displayMsg(const char* msg);
    QImage imageToProcess, imageToProcessDebayered;
    QImage imageCleanPlate, imageCleanPlateDebayered;
    QImage imageExtracted;
    int pixColorDiff(const QRgb pPix1, const QRgb pPix2);
    QImage denoise(const QImage* pOldImage, const int denoiseEdge, int & denoisedPixelCount);
    inline int YUV_getY(YUV yuv) { return qRed(yuv); }
    inline int YUV_getU(YUV yuv) { return qGreen(yuv); }
    inline int YUV_getV(YUV yuv) { return qBlue(yuv); }
    YUV Rgb2Yuv(QRgb rgb);
    bool imageToProcessDebayeredReady = false;
    bool imageCleanPlateDebayeredReady = false;
};

#endif // MAINWINDOW_H
