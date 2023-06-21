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

private:
    Ui::MainWindow *ui;
    QString currentFile;
    QString filePath;
    QString htmlContent;
    QString selectedImageUrl;
    void createHtmlFile(const QString& filePath, const QString& htmlContent);
};
#endif // MAINWINDOW_H
