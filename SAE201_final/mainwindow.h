#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"QFileDialog"
#include<QFile>
#include<QMessageBox>
#include<QTextEdit>
#include<QIODevice>
#include<iostream>
#include<QFont>
#include<QFontDialog>
#include<QColorDialog>
#include <QTextStream>
#include <QLabel>
#include <QInputDialog>
#include <QDebug>
#include <QPainter>
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QWidget>
#include <QDialog>
#include <QVBoxLayout>
#include <QSlider>
#include <QLabel>
#include <QDialogButtonBox>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionSave_as_triggered();
    void on_actionExit_triggered();
    void on_actionFont_triggered();
    void setItalic(bool italic);
    void setUnderline(bool underline);
    void setBold(bool bold);
    void about();
    void on_actionAbout_triggered();
    void copy();
    void cut();
    void paste();
    void on_actionColor_triggered();
    void on_actionImporter_triggered();
    void on_actionRendu_triggered();
    void resizeImage();
    //void moveImage();
    void deleteImage();
    void on_actionRendu_HTML_preview_triggered();
    void on_actionExporter_triggered();
    void textChanged();
    void on_actionDebut_triggered();
    void on_actionFIn_triggered();

    void on_ajouterPage_clicked();

    void on_comboBox_activated(int index);

    void on_actionTitre_triggered();

    void on_Suivant_clicked();

    void on_Precedant_clicked();

    void openResizeDialog();
    void on_actionRedimensionnerBar_triggered();


    void on_actionCentrer_triggered();

    void on_actionEcrire_a_gauche_triggered();

    void on_actionEcrire_a_droite_triggered();

private:
    Ui::MainWindow *ui;
    QString currentFile;
    QString filePath;
    QString htmlContent;
    QString selectedImageUrl;
    void createHtmlFile(const QString& filePath, const QString& htmlContent);
    QString lastBalise;
    std::vector<QString> listeBalise;
    std::vector<QString*> listePage;
    int nbPages = 0;
    int PageActuel;

    QSize getImageSize(const QString& imagePath);
    int currentPercentage;
};
#endif // MAINWINDOW_H
