#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <dirent.h>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Rename subs");
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_pushButton_clicked()
{
    QString Dest= ui->lineEdit->text(); // this is the path
    QByteArray ba = Dest.toLatin1();
    const char *dest = ba.data();
    int n=ui->spinBox->value();  // this is the number of episodes
    char ep[4];
    ep[0]='E'; //  <<== the letter before episode number
    ep[3]='\0';
    DIR *dir;	//directory that contains the videos + subs
    struct dirent *ent;	//this is a directory entry
    char text[]="Episode xx added.";
    if ((dir = opendir (dest)) != NULL) {	//test if the directory exist
        FILE* f;	//this is my batch file
        f=fopen("/tmp/rename.sh","w");
        while ((ent = readdir (dir)) != NULL) {	//test if there is an entry in the directory
            if(strstr(ent->d_name,".mkv") || strstr(ent->d_name,".avi") || strstr(ent->d_name,".mp4") || strstr(ent->d_name,".m4v") ){		//test if the entry is a video  // add more extention
                int i;
                for(i=1;i<=n;i++){
                    if(i<10){
                        ep[1]='0';
                        ep[2]=i+48;
                    }
                    else if(i<20){
                        ep[1]='1';
                        ep[2]=i%10+48;
                    }
                    else if(i<30){
                        ep[1]='2';
                        ep[2]=i%10+48;
                    }
                    ent->d_name[strlen(ent->d_name)-3]='s';
                    ent->d_name[strlen(ent->d_name)-2]='r';
                    ent->d_name[strlen(ent->d_name)-1]='t';
                    if(strstr(ent->d_name,ep)){
                        fputs("mv \"",f);
                        fputs(dest,f);
                        fputs("\"/*",f);
                        fputs(ep,f);
                        fputs("*.srt \"",f);
                        fputs(dest,f);
                        fputs("/",f);
                        fputs(ent->d_name,f);
                        fputs("\"\n",f);
                        text[8]=ep[1];
                        text[9]=ep[2];
                        ui->textEdit->append(text);		//print a notice
                    }
                }
            }
          }
          fputs("mkdir \"",f);
          fputs(dest,f);
          fputs("/subs\"\nmv \"",f);
          fputs(dest,f);
          fputs("\"/*srt \"",f);
          fputs(dest,f);
          fputs("/subs\"",f);
          fclose(f);		//close batch file
          closedir (dir);		//close directory
          system("chmod 777 /tmp/rename.sh");
          system(". /tmp/rename.sh");
          system("rm /tmp/rename.sh");
          ui->textEdit->append("\n<span style='color:blue;'>Moving subtitles to subs folder...</span>");
          ui->textEdit->append("<span style='color:blue;'>Removing temprary files...</span>");
          ui->textEdit->append("\n<span style='color:green;'>******** Finished succesfully ********</span>");
          QMessageBox::StandardButton reply= QMessageBox::question(this,"Finish","<p>The operation has finished succesfully.</p><p>Do you want to quit ?</p>",QMessageBox::Yes | QMessageBox::No);
          if(reply==QMessageBox::Yes) QApplication::quit();
        }else
            ui->textEdit->append("<span style='color:red;'>Directory not found...</span>");

}

void MainWindow::on_pushButton_3_clicked()
{
    QMessageBox::information(this,"About","This program was created by <a href='http://fb.com/salah.loukilii'><b>SalahEddine LOUKILI</b></a> using Qt4.");
}

void MainWindow::on_pushButton_4_clicked()
{
    QMessageBox::about(this,"Help","<h3>How to use:</h3><p>This program is used to rename the subtitles files automatically (no need to rename each episode mannually).</p><p>To make this work you should :</p><ol><li>Enter the path of the folder that contain your videos/episodes.</li><li>Make sure that the subtitles files (.srt) are in the same folder.</li><li>The name of the episode and the subs should contain the episode's number (ex: E02).</li><li>Enter the number of episodes you want to rename there subs and press Ok.</li></ol><p style='color:red;'>*Note : This is a linux application, it will not work on other systems !</p>");
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_actionAbout_triggered()
{
    MainWindow::on_pushButton_3_clicked();
}

void MainWindow::on_actionHelp_triggered()
{
    MainWindow::on_pushButton_4_clicked();
}

void MainWindow::on_pushButton_5_clicked()
{
    QString dir_name = QFileDialog::getExistingDirectory(this,"Choose a directory","/home/");
    ui->lineEdit->setText(dir_name);
}
