#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
      currentPercentage(100)
{
    ui->setupUi(this);
    connect(ui->actionCopy, &QAction::triggered, this,
            &MainWindow::copy);
    connect(ui->actionCut, &QAction::triggered, this,
            &MainWindow::cut);
    connect(ui->actionPaste, &QAction::triggered, this,
            &MainWindow::paste);
    connect(ui->actionRedimensionner, &QAction::triggered, this, &MainWindow::resizeImage);
    connect(ui->actionSupprimer, &QAction::triggered, this, &MainWindow::deleteImage);
    connect(ui->textEdit, &QTextEdit::textChanged, this, &MainWindow::textChanged);
    connect(ui->actionRedimensionnerBar, &QAction::triggered, this, &MainWindow::openResizeDialog);

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


void MainWindow::on_actionRendu_triggered()
{
    QString htmlContent = ui->textEdit->toHtml();
    ui->textEdit_2->setPlainText(htmlContent);
}

void MainWindow::on_actionRendu_HTML_preview_triggered()
{
    QString htmlContent = ui->textEdit_2->toPlainText();
    ui->textEdit->setHtml(htmlContent);
}


void MainWindow::textChanged()
{
    QString htmlContent = ui->textEdit->toHtml();
    ui->textEdit_2->setPlainText(htmlContent);
}

QSize MainWindow::getImageSize(const QString& imagePath)
{
    QImage image(imagePath);
    return image.size();
}


void MainWindow::openResizeDialog()
{
    QTextCursor cursor = ui->textEdit->textCursor();
    QTextCharFormat charFormat = cursor.charFormat();
    charFormat.setObjectIndex(-1); // Ajoutez cette ligne pour supprimer l'index de l'objet

    if (charFormat.isImageFormat())
    {
        QTextImageFormat imgFormat = charFormat.toImageFormat();
        QString imgSrc = imgFormat.name();
        if (!imgSrc.isEmpty())
        {
            // Créez une boîte de dialogue pour redimensionner l'image
            QDialog* dialog = new QDialog(this);
            dialog->setWindowTitle("Redimensionner l'image");

            // Créez un slider pour ajuster la taille
            QSlider* slider = new QSlider(Qt::Horizontal);
            slider->setRange(1, 200);
            slider->setSliderPosition(currentPercentage);



            // Créez une étiquette pour afficher la valeur actuelle du slider
            QLabel* label = new QLabel(QString("%1%").arg(currentPercentage)); // Mettre à jour le label

            // Connectez le signal `valueChanged` du slider à la mise à jour de l'étiquette
            connect(slider, &QSlider::valueChanged, [label](int value) {
                label->setText(QString("%1%").arg(value));
            });


            // Créez un layout vertical pour organiser les widgets dans la boîte de dialogue
            QVBoxLayout* layout = new QVBoxLayout;
            layout->addWidget(slider);
            layout->addWidget(label);

            // Ajoutez des boutons OK et Annuler à la boîte de dialogue
            QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
            layout->addWidget(buttonBox);




            // Connectez le signal `accepted` du bouton OK à la fonction de redimensionnement de l'image
            connect(buttonBox, &QDialogButtonBox::accepted, [dialog, slider, imgFormat, cursor, this]() {
                QTextCursor cursor = ui->textEdit->textCursor();
                QTextCharFormat charFormat = cursor.charFormat();
                charFormat.setObjectIndex(-1); // Ajoutez cette ligne pour supprimer l'index de l'objet
                if (charFormat.isImageFormat())
                {
                    QTextImageFormat imgFormat = charFormat.toImageFormat();
                    QString imgSrc = imgFormat.name();


                QSize imageSize = getImageSize(imgSrc);
                int value = slider->value();
                int width = imageSize.width();
                int height = imageSize.height();
                int newWidth = width * value / 100;
                int newHeight = height * value / 100;

                const_cast<QTextImageFormat&>(imgFormat).setWidth(newWidth); // Utilisez const_cast pour supprimer la constance
                const_cast<QTextImageFormat&>(imgFormat).setHeight(newHeight); // Utilisez const_cast pour supprimer la constance

                const_cast<QTextCursor&>(cursor).mergeCharFormat(imgFormat);

                ui->textEdit->setTextCursor(cursor);

                currentPercentage = slider->value();

                dialog->accept();

                }
            });

            // Connectez le signal `rejected` du bouton Annuler pour fermer la boîte de dialogue
            connect(buttonBox, &QDialogButtonBox::rejected, dialog, &QDialog::reject);

            // Définissez le layout sur la boîte de dialogue
            dialog->setLayout(layout);

            // Affichez la boîte de dialogue modale
            dialog->exec();
        }
    }
    else
    {
        QMessageBox::information(this, "Aucune image sélectionnée", "Veuillez sélectionner une image à redimensionner.");
    }
}

void MainWindow::on_actionRedimensionnerBar_triggered()
{

}


void MainWindow::on_actionCentrer_triggered()
{
    QTextCursor cursor = ui->textEdit->textCursor();
    if (cursor.hasSelection()) {
        QTextBlockFormat blockFormat;
        blockFormat.setAlignment(Qt::AlignHCenter); // Aligner le texte au centre
        cursor.mergeBlockFormat(blockFormat);
        ui->textEdit->setTextCursor(cursor);
    } else {
        QTextCursor newCursor = ui->textEdit->textCursor();
        newCursor.movePosition(QTextCursor::StartOfLine); // Déplacer le curseur au début de la ligne
        QTextBlockFormat blockFormat;
        blockFormat.setAlignment(Qt::AlignHCenter); // Aligner le texte au centre
        newCursor.setBlockFormat(blockFormat);
        ui->textEdit->setTextCursor(newCursor);
    }
}


void MainWindow::on_actionEcrire_a_droite_triggered()
{
    QTextCursor cursor = ui->textEdit->textCursor();
    if (cursor.hasSelection()) {
        QTextBlockFormat blockFormat;
        blockFormat.setAlignment(Qt::AlignRight); // Aligner le texte à droite
        cursor.mergeBlockFormat(blockFormat);
        ui->textEdit->setTextCursor(cursor);
    } else {
        QTextCursor newCursor = ui->textEdit->textCursor();
        newCursor.movePosition(QTextCursor::EndOfLine); // Déplacer le curseur à la fin de la ligne
        QTextBlockFormat blockFormat;
        blockFormat.setAlignment(Qt::AlignRight); // Aligner le texte à droite
        newCursor.setBlockFormat(blockFormat);
        ui->textEdit->setTextCursor(newCursor);
    }
}


void MainWindow::on_actionEcrire_a_gauche_triggered()
{
    QTextCursor cursor = ui->textEdit->textCursor();
        if (cursor.hasSelection()) {
            QTextBlockFormat blockFormat;
            blockFormat.setAlignment(Qt::AlignLeft); // Aligner le texte à gauche
            cursor.mergeBlockFormat(blockFormat);
            ui->textEdit->setTextCursor(cursor);
        } else {
            QTextCursor newCursor = ui->textEdit->textCursor();
            newCursor.movePosition(QTextCursor::StartOfLine); // Déplacer le curseur au début de la ligne
            QTextBlockFormat blockFormat;
            blockFormat.setAlignment(Qt::AlignLeft); // Aligner le texte à gauche
            newCursor.setBlockFormat(blockFormat);
            ui->textEdit->setTextCursor(newCursor);
        }
}




