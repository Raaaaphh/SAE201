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
    connect(ui->actionRedimensioner, &QAction::triggered, this, &MainWindow::resizeImage);
    connect(ui->actionD_placer, &QAction::triggered, this, &MainWindow::moveImage);
    connect(ui->actionSuprimmer, &QAction::triggered, this, &MainWindow::deleteImage);






}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionNew_triggered()
{
    currentFile.clear();
    ui->textEdit->setText(QString());

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
    ui->textEdit->setText(text);
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
    QString fileContent = ui->textEdit->toPlainText();
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
    QString fileContent = ui->textEdit->toPlainText();
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
        ui->textEdit->setCurrentFont(font);
}

void MainWindow::setItalic(bool italic)
{
    ui->textEdit->setFontItalic(italic);
}

void MainWindow::setUnderline(bool underline)
{
    ui->textEdit->setFontUnderline(underline);
}

void MainWindow::setBold(bool bold)
{
    ui->textEdit->setFontWeight(bold ? QFont::Bold: QFont::Normal);
}

void MainWindow::about(){
    QMessageBox::about(this, "About",tr("Ceci est un about"));
}

void MainWindow::on_actionAbout_triggered()
{
    about();
}

void MainWindow::copy(){
    ui->textEdit->copy();
}

void MainWindow::paste(){
    ui->textEdit->paste();
}
    QString selectedImageUrl;
void MainWindow::cut(){
    ui->textEdit->cut();
}

void MainWindow::on_actionColor_triggered()
{
    ui->textEdit->setTextColor(QColorDialog::getColor(Qt::white, this, "Couleur",QColorDialog::ColorDialogOptions()));
}


void MainWindow::on_actionImporter_triggered()
{
    QString imagePath = QFileDialog::getOpenFileName(this, "Sélectionner une image", "", "Images (*.png *.jpg *.jpeg)");
    QString htmlImageTag = QString("<img src=\"%1\">").arg(imagePath);
    ui->textEdit->insertHtml(htmlImageTag);
}


void MainWindow::on_actionRendu_triggered()
{
    QString htmlContent = ui->textEdit->toHtml();
    ui->textBrowser->setPlainText(htmlContent);
}

void MainWindow::resizeImage()
{
    QTextCursor cursor = ui->textEdit->textCursor();
    QTextCharFormat charFormat = cursor.charFormat();
    if (charFormat.isImageFormat())
    {
        QTextImageFormat imgFormat = charFormat.toImageFormat();
        QString imgSrc = imgFormat.name();
        if (!imgSrc.isEmpty())
        {
            bool ok;
            QString newSize = QInputDialog::getText(this, "Redimensioner l'image", "Entrer une taille (weight x height)", QLineEdit::Normal, "", &ok);
            if (ok && !newSize.isEmpty())
            {
                QStringList sizeParts = newSize.split("x");
                if (sizeParts.size() == 2)
                {
                    int width = sizeParts[0].toInt();
                    int height = sizeParts[1].toInt();

                    imgFormat.setWidth(width);
                    imgFormat.setHeight(height);

                    cursor.mergeCharFormat(imgFormat);
                    ui->textEdit->setTextCursor(cursor);
                }
            }
        }
    }
    else
    {
        QMessageBox::information(this, "No Image Selected", "Please select an image to resize.");
    }
}



void MainWindow::moveImage()
{
    QTextCursor cursor = ui->textEdit->textCursor();
    cursor.select(QTextCursor::Document);
    QString selectedText = cursor.selectedText();

    if (selectedText.contains("<img"))
    {
        // Code pour déplacer l'image
        // Ici, tu peux mettre en œuvre la logique permettant de détecter le déplacement de l'image
        // et de mettre à jour les coordonnées de l'image dans la balise <img>
        // Une approche possible est d'ajouter des gestionnaires d'événements de souris
        // pour suivre le déplacement de l'image et mettre à jour les coordonnées en conséquence.
        // Par exemple, tu peux utiliser des signaux tels que mousePressEvent, mouseMoveEvent et mouseReleaseEvent
    }
    else
    {
        QMessageBox::information(this, "No Image Selected", "Please select an image to move.");
    }
}

void MainWindow::deleteImage()
{
    QTextCursor cursor = ui->textEdit->textCursor();
    cursor.select(QTextCursor::Document);
    QString selectedText = cursor.selectedText();

    if (selectedText.contains("<img"))
    {
        cursor.removeSelectedText();
    }
    else
    {
        QMessageBox::information(this, "No Image Selected", "Please select an image to delete.");
    }
}

void MainWindow::on_actionExporter_triggered()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Enregistrer le fichier", "", "Fichiers HTML (*.html)");
    if (filePath.isEmpty())
        return;

    QString htmlContent = ui->textEdit->toHtml();

    createHtmlFile(filePath, htmlContent);

    QMessageBox::information(this, "Exportation réussie", "Le fichier a été exporté avec succès.");
}

void MainWindow::createHtmlFile(const QString& filePath, const QString& htmlContent)
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::critical(this, "Erreur d'exportation", "Impossible de créer le fichier HTML : " + file.errorString());
        return;
    }

    QTextStream out(&file);
    out << htmlContent;

    file.close();
}




