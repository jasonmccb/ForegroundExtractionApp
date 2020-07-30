#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "helper.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
    imageToProcess = QImage(DEFAULT_IMAGE_WIDTH, DEFAULT_IMAGE_HEIGHT, QImage::Format::Format_ARGB32);
    imageCleanPlate = QImage(DEFAULT_IMAGE_WIDTH, DEFAULT_IMAGE_HEIGHT, QImage::Format::Format_ARGB32);
    imageExtracted = QImage(DEFAULT_IMAGE_WIDTH/2, DEFAULT_IMAGE_HEIGHT/2, QImage::Format::Format_ARGB32);
    imageToProcessDebayered = QImage(DEFAULT_IMAGE_WIDTH/2, DEFAULT_IMAGE_HEIGHT/2, QImage::Format::Format_ARGB32);
    imageCleanPlateDebayered = QImage(DEFAULT_IMAGE_WIDTH/2, DEFAULT_IMAGE_HEIGHT/2, QImage::Format::Format_ARGB32);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::displayMsg(const char* msg)
{
    QMessageBox msgbox;
    msgbox.setText(msg);
    msgbox.exec();
}

void MainWindow::on_pushButtonChooseImageToProcess_clicked()
{
    //QFileDialog dialog(this, "Select a file", "C:/Users/Jason/Desktop/shortcuts/temppppp", "PNG image (*.png)");
    QFileDialog dialog(this, "Select a file", ".", "PNG image (*.png)");
    dialog.setFileMode(QFileDialog::FileMode::ExistingFile);
    QStringList selectedFilePaths;
    if (dialog.exec())
    {
        selectedFilePaths = dialog.selectedFiles();
        for (const QString i : selectedFilePaths)
        {
            ui->lineEditDirImageToProcess->setText(i);
            break;
        }
    }

}

void MainWindow::on_pushButtonChooseImageCleanPlate_clicked()
{
    //QFileDialog dialog(this, "Select a file", "C:/Users/Jason/Desktop/shortcuts/temppppp", "PNG image (*.png)");
    QFileDialog dialog(this, "Select a file", ".", "PNG image (*.png)");
    dialog.setFileMode(QFileDialog::FileMode::ExistingFile);
    QStringList selectedFilePaths;
    if (dialog.exec())
    {
        selectedFilePaths = dialog.selectedFiles();
        for (const QString i : selectedFilePaths)
        {
            ui->lineEditDirImageCleanPlate->setText(i);
            break;
        }
    }

}

void MainWindow::on_pushButtonLoadImageToProcess_clicked()
{
    QImageReader imgReader(ui->lineEditDirImageToProcess->text());
    //imgReader.setFileName("C:/Users/Jason/Desktop/shortcuts/temppppp/Barbie.png");
    imgReader.read(&imageToProcess);

    int w = imageToProcess.width();
    int h = imageToProcess.height();
    if (h > w)
    {
        QMatrix matrix;
        matrix.rotate(-90.0);
        imageToProcess = imageToProcess.transformed(matrix, Qt::FastTransformation);
    }

    ui->label->pCurrentImage = &imageToProcess;
    ui->label->update();
}

void MainWindow::on_pushButtonLoadImageCleanPlate_clicked()
{
    QImageReader imgReader(ui->lineEditDirImageCleanPlate->text());
    //imgReader.setFileName("C:/Users/Jason/Desktop/shortcuts/temppppp/CleanPlate.png");
    imgReader.read(&imageCleanPlate);

    int w = imageCleanPlate.width();
    int h = imageCleanPlate.height();
    if (h > w)
    {
        QMatrix matrix;
        matrix.rotate(-90.0);
        imageCleanPlate = imageCleanPlate.transformed(matrix, Qt::FastTransformation);
    }

    ui->label->pCurrentImage = &imageCleanPlate;
    ui->label->update();
}

void MainWindow::on_pushButtonDebayerImageToProcess_clicked()
{
    if(imageToProcess.isNull())
        return;
    int w = imageToProcess.width();
    int h = imageToProcess.height();

    for (int y = 0; y+1 < h; y += 2)
    {
        QRgb* line1 = (QRgb*)imageToProcess.scanLine(y);
        QRgb* line2 = (QRgb*)imageToProcess.scanLine(y+1);

        int x = 0;
        for (x = 0; x + 1 < w; x += 2)
        {
            int red = qRed(line1[x]);
            int green = (qGreen(line1[x+1]) + qGreen(line2[x])) / 2;
            int blue = qBlue(line2[x+1]);
            //int alpha;
            imageToProcessDebayered.setPixel(x/2, y/2, qRgb(red, green, blue));
        }
    }
    if (!imageToProcessDebayered.isNull())
    {
        ui->label->pCurrentImage = &imageToProcessDebayered;
        ui->label->update();
    }
}

void MainWindow::on_pushButtonDebayerImageCleanPlate_clicked()
{
    if (imageCleanPlate.isNull())
        return;
    int w = imageCleanPlate.width();
    int h = imageCleanPlate.height();

    for (int y = 0; y + 1 < h; y += 2)
    {
        QRgb* line1 = (QRgb*)imageCleanPlate.scanLine(y);
        QRgb* line2 = (QRgb*)imageCleanPlate.scanLine(y + 1);

        int x = 0;
        for (x = 0; x + 1 < w; x += 2)
        {
            int red = qRed(line1[x]);
            int green = (qGreen(line1[x + 1]) + qGreen(line2[x])) / 2;
            int blue = qBlue(line2[x + 1]);
            //int alpha;
            imageCleanPlateDebayered.setPixel(x / 2, y / 2, qRgb(red, green, blue));
        }
    }
    if (!imageCleanPlateDebayered.isNull())
    {
        ui->label->pCurrentImage = &imageCleanPlateDebayered;
        ui->label->update();
    }
}

void MainWindow::on_pushButtonSaveBelowImage_clicked()
{
    if (!ui->label->pCurrentImage)
    {
        displayMsg("There is no image to save.");
        return;
    }

    QFileDialog dialog(this);
    //dialog.setNameFilter("PNG-Files (*.png)");
    dialog.setDefaultSuffix("png");
    if (ui->label->pCurrentImage->save(dialog.getSaveFileName(this, "Save file", ".", "PNG image (*.png)")))
        displayMsg("Successful.");
    else
        displayMsg("Failed!");
}

void MainWindow::on_pushButtonExtractForeGround_clicked()
{
    imageExtracted.fill(qRgb(0, 0, 0));
    if (imageToProcessDebayered.isNull() || imageCleanPlateDebayered.isNull())
        return;
    int w = std::min(imageToProcessDebayered.width(), imageCleanPlateDebayered.width());
    int h = std::min(imageToProcessDebayered.height(), imageCleanPlateDebayered.height());

    for (int y = 0; y < h; y++)
    {
        QRgb* lineBackground = (QRgb*)imageCleanPlateDebayered.scanLine(y);
        QRgb* lineForeground = (QRgb*)imageToProcessDebayered.scanLine(y);
        int x = 0;
        for (x = 0; x < w; x++)
        {
            if (pixColorDiff(lineBackground[x], lineForeground[x]) > RGB_DIFF_THRESHOLD)
                imageExtracted.setPixel(x, y, lineForeground[x]);
        }
    }
    ui->label->pCurrentImage = &imageExtracted;
    ui->label->update();
}

int MainWindow::pixColorDiff(const QRgb pPix1, const QRgb pPix2)
{
    int ret = 0;
    ret += std::abs(qRed(pPix1) - qRed(pPix2));
    ret += std::abs(qGreen(pPix1) - qGreen(pPix2));
    ret += std::abs(qBlue(pPix1) - qBlue(pPix2));
    return ret;
}
