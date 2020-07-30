#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QFileDialog>
#include <QImageReader>

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
    QVBoxLayout* mainLayout;
    //QPixmap myPix;
    void displayMsg(const char* msg);
    QImage imageToProcess, imageToProcessDebayered;
    QImage imageCleanPlate, imageCleanPlateDebayered;
    QImage imageExtracted;
};

#endif // MAINWINDOW_H
