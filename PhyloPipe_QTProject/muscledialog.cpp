#include "muscledialog.h"
#include "ui_muscledialog.h"
#include "mafftdialog.h"
#include "ui_mafftdialog.h"

#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QSettings>
#include <QString>
#include <QMessageBox>
#include <QVBoxLayout>

MuscleDialog::MuscleDialog(QWidget *parent) :
    QDialog(parent),
    mus_ui(new Ui::MuscleDialog)
{
    mus_ui->setupUi(this);

    //put the widget into a layout and suit the window size
    QVBoxLayout* muscleLayout=new QVBoxLayout;
    muscleLayout->addWidget(mus_ui->musclewidget);
    setLayout(muscleLayout);

    MafftDialog mafftdlg;
    mus_ui->FileDirCombox->setCurrentText(mafftdlg.mafresult_dir);
    mus_ui->FileDirCombox->addItem(mafftdlg.mafresult_dir);
    mus_ui->ResultDirCombox->setCurrentText(QDir::currentPath());
    mus_ui->ResultDirCombox->addItem(QDir::currentPath());

}

MuscleDialog::~MuscleDialog()
{
    delete mus_ui;
}

void MuscleDialog::on_FileDirBtn_clicked()
{
    QString dir=QFileDialog::getExistingDirectory(this,
                                                  tr("Choosing the Input File Directory"),
                                                  mus_ui->FileDirCombox->currentText(),
                                                  QFileDialog::ShowDirsOnly);
    if(!dir.isEmpty())
    {
        mus_ui->FileDirCombox->addItem(dir);
        mus_ui->FileDirCombox->setCurrentIndex(mus_ui->FileDirCombox->currentIndex()+1);
    }
}

void MuscleDialog::on_ResultDirBtn_clicked()
{
    QString dir=QFileDialog::getExistingDirectory(this,
                                                  tr("Choosing the Output File Directory"),
                                                  mus_ui->ResultDirCombox->currentText(),
                                                  QFileDialog::ShowDirsOnly);
    if(!dir.isEmpty())
    {
        mus_ui->ResultDirCombox->addItem(dir);
        mus_ui->ResultDirCombox->setCurrentIndex(mus_ui->ResultDirCombox->currentIndex()+1);
    }
    musresult_dir=dir;
}

void MuscleDialog::on_Muscle_pushButton_clicked()
{
    //QMessageBox::information(NULL,tr("Muscle Information"),tr("Please check the result directory, the computer is executating Muscle program..."));

    QString musclepath,syspath,content;
    const char *cmd;
    bool ok;

    musclepath=settings.value("SWsetting/MusclePath").toString();
    if(((mus_ui->MaxitersSpinBox->text().toInt(&ok,10)))!=16)//the initiate value is 16
    {
        musclepath.append(" -maxiters ");
        //str2int.number();str2int
        musclepath.append(mus_ui->MaxitersSpinBox->text());
    }
    if((mus_ui->MaxhoursSpinBox->text().toInt(&ok,10)))//the initiate value is 0, no maxhour. if is not 0,then the value should be put to the command.
    {
        musclepath.append(" -maxhours ");
        //str2int.number();
        musclepath.append(mus_ui->MaxhoursSpinBox->text());
    }

    if(mus_ui->RefineRadioBtn->isChecked())
        musclepath.append(" -refine");


    syspath=QDir::currentPath();
    syspath.append("/SystemFiles/SysTempData.txt");
    QFile musclefile(syspath);
    QTextStream musclestream(&musclefile);
    if(musclefile.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        int chos=2;
        musclestream<<chos<<"\n";
        musclestream<<musclepath<<"\n";
        musclestream<<mus_ui->FileDirCombox->currentText()<<"\n";
        musclestream<<mus_ui->ResultDirCombox->currentText()<<"\n";
        musclefile.close();
    }
    else
        hsdlg.ErrorMsg(musclefile.fileName());

    content="cd ";
    content.append(QDir::currentPath());
    content.append("/SystemFiles; ./SysExec&");
    cmd=qPrintable(content);system(cmd);
    QMessageBox::information(NULL,tr("Muscle Information"),tr("Please check the result directory, the computer is executating Muscle program..."));

    //QMessageBox::information(NULL,tr("Muscle Information"),tr("Muscle program has finished, please check the result directory!"));


/*    QMessageBox musmsg;

    QDir *musdir=new QDir(mus_ui->FileDirCombox->currentText());
    QStringList filter;filter<<"*.fa";musdir->setNameFilters(filter);
    QFileInfoList *muslist=new QFileInfoList(musdir->entryInfoList(filter));
*/    //QString str2int;


/*    if(muslist->count()>0)
    {
        for(int i=0;i<muslist->size();i++)
        {
            content=musclepath;
            content.append(" -in ");
            content.append(muslist->at(i).absoluteFilePath());
            content.append(" -out ");
            content.append(mus_ui->ResultDirCombox->currentText());
            content.append("/");
            content.append(muslist->at(i).fileName());



            cmd=qPrintable(content);
            system(cmd);
        }
        musmsg.setText("Muscle has done, please check the results!");
        musmsg.exec();
    }
    else
    {
        musmsg.setText("There is no file in the FileDirectory, check your input!");
        musmsg.exec();
    }
    delete musdir;delete muslist;*/
    close();
}

void MuscleDialog::on_Cansle_pushButton_clicked()
{
    close();
}
