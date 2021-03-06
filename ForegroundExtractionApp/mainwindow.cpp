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

    ui->pushButtonExtractForeGround->setEnabled(false);
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
        imageToProcessDebayeredReady = true;
        ui->pushButtonExtractForeGround->setEnabled(imageCleanPlateDebayeredReady);
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
        imageCleanPlateDebayeredReady = true;
        ui->pushButtonExtractForeGround->setEnabled(imageToProcessDebayeredReady);
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
            {
                int y_diff = std::abs(YUV_getY(Rgb2Yuv(lineBackground[x])) - YUV_getY(Rgb2Yuv(lineForeground[x])));
                int u_diff = std::abs(YUV_getU(Rgb2Yuv(lineBackground[x])) - YUV_getU(Rgb2Yuv(lineForeground[x])));
                int v_diff = std::abs(YUV_getV(Rgb2Yuv(lineBackground[x])) - YUV_getV(Rgb2Yuv(lineForeground[x])));

                bool isShadow = y_diff < YUV_DIFF_SHADOW_THRESHOLD_Y && u_diff < YUV_DIFF_SHADOW_THRESHOLD_UV && v_diff < YUV_DIFF_SHADOW_THRESHOLD_UV;
                if(!isShadow)
                    imageExtracted.setPixel(x, y, lineForeground[x]);
            }
        }
    }
    QImage imageDenoisedTemp;
    int denoisedPixcelNumber = 0;
    int denoiseCounter = 0;
    do
    {
        imageDenoisedTemp = denoise(&imageExtracted, DENOISE_PIXEL_EDGE_SIZE, denoisedPixcelNumber);
        denoisedPixcelNumber = 0;
        imageExtracted = denoise(&imageDenoisedTemp, DENOISE_PIXEL_EDGE_SIZE, denoisedPixcelNumber);
        denoiseCounter += 2;
    } 
    while (denoisedPixcelNumber>0 && denoiseCounter <= MAX_DENOISE_COUNT);

    ui->label->pCurrentImage = &imageExtracted;
    ui->label->update();
}

int MainWindow::pixColorDiff(const QRgb pix1, const QRgb pix2)
{
    int ret = 0;
    ret += std::abs(qRed(pix1) - qRed(pix2));
    ret += std::abs(qGreen(pix1) - qGreen(pix2));
    ret += std::abs(qBlue(pix1) - qBlue(pix2));
    return ret;
}

YUV MainWindow::Rgb2Yuv(QRgb rgb)
{
    int r = qRed(rgb);
    int g = qGreen(rgb);
    int b = qBlue(rgb);
    int y = r * 0.299 + g * 0.587 + b * 0.114;
    int u = r * (-0.168736) + g * (-0.331264) + b * 0.5 + 128;
    int v = r * 0.5 + g * (-0.418688) + b * (-0.081312) + 128;
    return qRgb(y, u, v);
}

QImage MainWindow::denoise(const QImage* pOldImage, const int denoiseEdge, int& denoisedPixelCount)
{
    QImage newImage = QImage(*pOldImage);
    
    int offSetRadius = (denoiseEdge - 1) / 2;
    int examineLineCount = denoiseEdge % 2 == 0 ? denoiseEdge - 1 : denoiseEdge;
    int rotatedDenoiseLineNumber = examineLineCount / 2;
    int rotatedScanLineNumber = 0;
    std::vector<QRgb*> lines;
    lines.resize(examineLineCount);

    for (int y = 0; y < pOldImage->height(); y++)
    {
        lines[rotatedScanLineNumber] = (QRgb*) (pOldImage->scanLine(y));
        if (y + 1 - examineLineCount >= 0)
        {
            for (int x = offSetRadius; x < pOldImage->width() - offSetRadius; x++)
            {
                QRgb oldPix = lines[rotatedDenoiseLineNumber][x];
                //std::map<int, QRgb> pixMediums;
                //int avgRgb;
                
                int sumOfRed = 0;
                int sumOfGreen = 0;
                int sumOfBlue = 0;
                int count = 0;
                for (int lineNumber=0; lineNumber<examineLineCount; lineNumber++)
                {
                    for (int x_to_examine = x - offSetRadius; x_to_examine <= x + offSetRadius; x_to_examine++)
                    {
                        QRgb nearPix = lines[lineNumber][x_to_examine];
                        if (x_to_examine != x || lineNumber != rotatedDenoiseLineNumber)
                        {
                            sumOfRed += qRed(nearPix);
                            sumOfGreen += qGreen(nearPix);
                            sumOfBlue += qBlue(nearPix);
                            count++;
                            
                            // Calculate medium instead of mean
                            /*avgRgb = (qRed(nearPix) + qGreen(nearPix) + qBlue(nearPix)) / 3;
                            if (pixMediums.size() <= 4)
                                pixMediums.insert( std::pair<int,QRgb> (avgRgb, nearPix));
                            if (pixMediums.size() == 5)
                            {
                                pixMediums.erase(pixMediums.begin()); 
                                pixMediums.erase(--pixMediums.end());
                            }*/
                        }
                    }
                }
                
                QRgb avg = qRgb(sumOfRed / count, sumOfGreen / count, sumOfBlue / count);
                //QRgb medium = pixMediums[pixMediums.size() / 2];
                if (pixColorDiff(oldPix, avg) > RGB_DIFF_DENOISE_THRESHOLD)
                    //newImage.setPixel(x, y - offSetRadius, medium);
                    newImage.setPixel(x, y - offSetRadius, avg);
            }
        }

        if (y >= examineLineCount)
            rotatedDenoiseLineNumber = (rotatedDenoiseLineNumber + 1) % examineLineCount;
        rotatedScanLineNumber = (rotatedScanLineNumber + 1) % examineLineCount;
    }
    return newImage;
}