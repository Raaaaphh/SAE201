#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionCopy, &QAction::triggered, this,
            &MainWindow::copy);
    connect(ui->actionCut, &QAction::triggered, this,
            &MainWindow::cut);
    connect(ui->actionPaste, &QAction::triggered, this,
            &MainWindow::paste);





}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionNew_triggered()
{
    currentFile.clear();
    ui->textEdit_2->setText(QString());

}


void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open the file");
    if (fileName.isEmpty())
        return;
    QFile file(fileName);
    currentFile = fileName;
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot open file: " +
                                                  file.errorString());
        return;
    }
    setWindowTitle(fileName);
    QTextStream in(&file);
    QString text = in.readAll();
    ui->textEdit_2->setText(text);
    file.close();
}


void MainWindow::on_actionSave_triggered()
{
    QString fileToSave;
    if(currentFile.size()==0){
        fileToSave = QFileDialog::getSaveFileName(this, "Choose a file name");
    }else{
        fileToSave =currentFile;
    }
    QFile file(fileToSave+".txt");
    QString fileContent = ui->textEdit_2->toPlainText();
    if(file.open(QFile::WriteOnly | QFile::Text)){
        QTextStream in(&file);
        in<<fileContent;
        file.close();
    }
}


void MainWindow::on_actionSave_as_triggered()
{
    QString fileToSave = QFileDialog::getSaveFileName(this, "Choose a file name");
    QFile file(fileToSave+".txt");
    QString fileContent = ui->textEdit_2->toPlainText();
    if(file.open(QFile::WriteOnly | QFile::Text)){
        QTextStream in(&file);
        in<<fileContent;
        file.close();
    }
}


void MainWindow::on_actionExit_triggered()
{
    QApplication::exit();
}


void MainWindow::on_actionFont_triggered()
{
    bool fontSelected;
    QFont font = QFontDialog::getFont(&fontSelected, this);
    if (fontSelected)
        ui->textEdit_2->setCurrentFont(font);
}

void MainWindow::setItalic(bool italic)
{
    ui->textEdit_2->setFontItalic(italic);
}

void MainWindow::setUnderline(bool underline)
{
    ui->textEdit_2->setFontUnderline(underline);
}

void MainWindow::setBold(bool bold)
{
    ui->textEdit_2->setFontWeight(bold ? QFont::Bold: QFont::Normal);
}

void MainWindow::about(){
    QMessageBox::about(this, "About",tr("Ceci est un about"));
}

void MainWindow::on_actionAbout_triggered()
{
    about();
}

void MainWindow::copy(){
    ui->textEdit_2->copy();
}

void MainWindow::paste(){
    ui->textEdit_2->paste();
}
    QString selectedImageUrl;
void MainWindow::cut(){
    ui->textEdit_2->cut();
}

void MainWindow::on_actionColor_triggered()
{
    ui->textEdit_2->setTextColor(QColorDialog::getColor(Qt::white, this, "Couleur",QColorDialog::ColorDialogOptions()));
}


void MainWindow::on_actionImporter_triggered()
{
    QString imagePath = QFileDialog::getOpenFileName(this, "Sélectionner une image", "", "Images (*.png *.jpg *.jpeg)");
    QString htmlImageTag = QString("<img src=\"%1\">").arg(imagePath);
    ui->textEdit_2->insertHtml(htmlImageTag);
}


void MainWindow::on_actionRendu_triggered()
{
    QString htmlContent = ui->textEdit->toHtml();
    ui->textEdit_2->setPlainText(htmlContent);
}

void MainWindow::on_actionRendu_HTML_preview_triggered()
{
    QString htmlContent = ui->textEdit_2->toPlainText();
    ui->textBrowser->setHtml(htmlContent);
}

