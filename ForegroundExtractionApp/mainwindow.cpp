#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "helper.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    //mainLayout = new QVBoxLayout;
    //mainLayout->addWidget(ui->label);
    //setLayout(mainLayout);

    //QPixmap mypix ("C:/Users/Jason/Desktop/shortcuts/temppppp/Barbie.png");
    //double h_w_ratio = mypix.height() * 1.0 / mypix.width();
    //QSize qs(ui->centralwidget->rect().width(), ui->centralwidget->rect().height());
    //newPix = mypix.scaled(500, (int) (h_w_ratio*500), Qt::AspectRatioMode::KeepAspectRatio);
    //ui->label->setPixmap(newPix);

    imageToProcess = QImage(DEFAULT_IMAGE_WIDTH, DEFAULT_IMAGE_HEIGHT, QImage::Format::Format_RGB32);
    imageCleanPlate = QImage(DEFAULT_IMAGE_WIDTH, DEFAULT_IMAGE_HEIGHT, QImage::Format::Format_RGB32);
    imageExtracted = QImage(DEFAULT_IMAGE_WIDTH, DEFAULT_IMAGE_HEIGHT, QImage::Format::Format_RGB32);
    imageToProcessDebayered = QImage(DEFAULT_IMAGE_WIDTH, DEFAULT_IMAGE_HEIGHT, QImage::Format::Format_RGB32);
    imageCleanPlateDebayered = QImage(DEFAULT_IMAGE_WIDTH, DEFAULT_IMAGE_HEIGHT, QImage::Format::Format_RGB32);
}

MainWindow::~MainWindow()
{
    delete ui;
    //delete mainLayout;
}

void MainWindow::displayMsg(const char* msg)
{
    QMessageBox msgbox;
    msgbox.setText(msg);
    msgbox.exec();
}

void MainWindow::on_pushButtonChooseImageToProcess_clicked()
{
    QFileDialog dialog(this);
    dialog.setNameFilter(tr("Image (*.png)"));
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
    QFileDialog dialog(this);
    dialog.setNameFilter(tr("Image (*.png)"));
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
    QImageReader imgReader("C:/Users/Jason/Desktop/shortcuts/temppppp/Barbie.png");
    //QImageReader imgReader(ui->lineEditDirImageToProcess->text());
    
    //imgReader.setScaledSize(QSize(500, 500));
    //imgReader.setAutoTransform(false);
    imgReader.read(&imageToProcess);

    //QExifImageHeader qeih;


    int w = imageToProcess.width();
    int h = imageToProcess.height();
    if (h > w)
    {
        QMatrix matrix;
        matrix.rotate(-90.0);
        imageToProcess = imageToProcess.transformed(matrix, Qt::FastTransformation);
    }

    ui->label->pCurrentImage = &imageToProcess;
    //int w2 = imageToProcess.width();
    //int h2 = imageToProcess.height();

    QRgb* line = (QRgb*)imageToProcess.scanLine(0);
    int red = qRed(line[0]);
    int green = qGreen(line[0]);
    int blue = qBlue(line[0]);

    int alpha;
}

void MainWindow::on_pushButtonLoadImageCleanPlate_clicked()
{
    displayMsg("on_pushButtonLoadImageCleanPlate_clicked");
}

void MainWindow::on_pushButtonDebayerImageToProcess_clicked()
{
    displayMsg("on_pushButtonDebayerImageToProcess_clicked");
}

void MainWindow::on_pushButtonDebayerImageCleanPlate_clicked()
{
    displayMsg("on_pushButtonDebayerImageCleanPlate_clicked");
}

void MainWindow::on_pushButtonSaveBelowImage_clicked()
{
    displayMsg("on_pushButtonSaveBelowImage_clicked");;
}

void MainWindow::on_pushButtonExtractForeGround_clicked()
{
    displayMsg("on_pushButtonExtractForeGround_clicked");
}
