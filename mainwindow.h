#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Version.h"
#include <QDir>
#include <QDateTime>
#include <QFile>
#include <QStringList>
#include <QFileDialog>
#include <QMessageBox>

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
    void on_getInput_clicked();

    void on_getOutput_clicked();

    void on_Run_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
