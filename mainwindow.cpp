#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->about->setText("Vogen采样歌词转XStudio采样歌词 版本"+::_VGL2XSL_Version+"\n©2021 無常.保留所有权利.\nQt版本："+QString(qVersion())+" 编译时间："+QString(__DATE__)+" "+QString(__TIME__));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_getInput_clicked()
{
    QString dir=QFileDialog::getExistingDirectory(this,"选择一个目录",QDir::currentPath());
    if(!dir.isEmpty()){
        QDir::setCurrent(dir);
        ui->InputDir->setText(dir);
    }
}


void MainWindow::on_getOutput_clicked()
{
    QString dir=QFileDialog::getExistingDirectory(this,"选择一个目录",QDir::currentPath());
    if(!dir.isEmpty()){
        QDir::setCurrent(dir);
        ui->OutputDir->setText(dir);
    }
}


void MainWindow::on_Run_clicked()
{
    ui->out->clear();
    ui->out->appendPlainText("["+QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")+"]");
    if(ui->InputDir->text().isEmpty()){
        ui->out->appendPlainText("未选择输入目录！");
        return;
    }
    if(ui->OutputDir->text().isEmpty()){
        ui->out->appendPlainText("未选择输出目录！");
        return;
    }
    int modef=0;
    QDir dir(ui->InputDir->text());
    dir.setFilter(QDir::Files);
    for(uint i=0;i<dir.count();i++){
        if(dir[i].endsWith(".txt")){
            QFile infile(dir.path()+"/"+dir[i]);
            if(infile.open(QIODevice::ReadOnly|QIODevice::Text)){
                ui->out->appendPlainText("读取文件："+dir.path()+"/"+dir[i]);
                QString indata=infile.readAll();
                infile.close();

                QStringList indatasplit=indata.split("\n",Qt::SkipEmptyParts);
                QString outdata;
                for(int j=0;j<indatasplit.size();j++){
                    QStringList indatachar=indatasplit.at(j).split(" ",Qt::SkipEmptyParts);
                    QString outstemp;
                    for(int k=0;k<indatachar.size();k++){
                        if(indatachar.at(k).size()>=2){
                            outstemp+=indatachar.at(k).at(0);
                        }
                    }
                    outdata+=outstemp;
                    outdata+="\n";
                }

                QFile outfile(ui->OutputDir->text()+"/"+dir[i]);
                bool okf=true;

                if(outfile.exists()){
                    if(modef==-1){
                        okf=false;
                    }else if(modef==1){
                        okf=true;
                    }else{
                        QMessageBox::StandardButton result=QMessageBox::warning(this,"覆盖文件","文件"+ui->OutputDir->text()+"/"+dir[i]+"已经存在！\n是否覆盖？",QMessageBox::StandardButtons(QMessageBox::Yes|QMessageBox::YesToAll|QMessageBox::No|QMessageBox::NoToAll|QMessageBox::Cancel),QMessageBox::Cancel);
                        if(result==QMessageBox::Yes){
                            okf=true;
                        }else if(result==QMessageBox::YesToAll){
                            okf=true;
                            modef=1;
                        }else if(result==QMessageBox::No){
                            okf=false;
                        }else if(result==QMessageBox::NoToAll){
                            okf=false;
                            modef=-1;
                        }else{
                            ui->out->appendPlainText("操作取消！");
                            break;
                        }
                    }
                }
                if(okf){
                    if(outfile.open(QIODevice::WriteOnly|QIODevice::Text|QIODevice::Truncate)){
                        ui->out->appendPlainText("写入文件："+ui->OutputDir->text()+"/"+dir[i]);
                        outfile.write(outdata.toUtf8());
                        outfile.close();
                    }else{
                        ui->out->appendPlainText("未能写入文件："+ui->OutputDir->text()+"/"+dir[i]);
                    }
                }else{
                    ui->out->appendPlainText("跳过文件："+ui->OutputDir->text()+"/"+dir[i]);
                }


            }else{
                ui->out->appendPlainText("未能读取文件："+dir.path()+"/"+dir[i]);
            }
        }
    }
}

