#include "mafftdialog.h"
#include "ui_mafftdialog.h"
#include "hamcmbdialog.h"
#include "ui_hamcmbdialog.h"

#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QSettings>
#include <QString>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QTextStream>

MafftDialog::MafftDialog(QWidget *parent) :
    QDialog(parent),
    maf_ui(new Ui::MafftDialog)
{
    maf_ui->setupUi(this);

    //put the widget into a layout and suit the window size
    QVBoxLayout* mafftLayout=new QVBoxLayout;
    mafftLayout->addWidget(maf_ui->mafftwidget);
    setLayout(mafftLayout);

    HamCmbDialog hcdlg;
    maf_ui->FileDirCombox->setCurrentText(hcdlg.cresult_dir);
    maf_ui->FileDirCombox->addItem(hcdlg.cresult_dir);
    maf_ui->ResultDirCombox->setCurrentText(QDir::currentPath());
    maf_ui->ResultDirCombox->addItem(QDir::currentPath());

}

MafftDialog::~MafftDialog()
{
    delete maf_ui;
}



void MafftDialog::on_FileDirBtn_clicked()
{
    QString dir=QFileDialog::getExistingDirectory(this,
                                                  tr("Choosing the Input File Directory"),
                                                  maf_ui->FileDirCombox->currentText(),
                                                  QFileDialog::ShowDirsOnly);
    if(!dir.isEmpty())
    {
        maf_ui->FileDirCombox->addItem(dir);
        maf_ui->FileDirCombox->setCurrentIndex(maf_ui->FileDirCombox->currentIndex()+1);
    }
}

void MafftDialog::on_ResultDirBtn_clicked()
{
    QString dir=QFileDialog::getExistingDirectory(this,
                                                  tr("Choosing the Output File Directory"),
                                                  maf_ui->ResultDirCombox->currentText(),
                                                  QFileDialog::ShowDirsOnly);
    if(!dir.isEmpty())
    {
        maf_ui->ResultDirCombox->addItem(dir);
        maf_ui->ResultDirCombox->setCurrentIndex(maf_ui->ResultDirCombox->currentIndex()+1);
    }
    mafresult_dir=maf_ui->ResultDirCombox->currentText();
}

void MafftDialog::on_MafftBtn_clicked()
{
    //QMessageBox::information(NULL,tr("Mafft Information"),tr("Please check the result directory, the computer is executating Mafft program..."));

    QString mafftpath,syspath,content;
    //QString ;
    const char *cmd;
    mafftpath=settings.value("SWsetting/MafftPath").toString();
/*
    QMessageBox mafmsg;

    QDir *mafdir=new QDir(maf_ui->FileDirCombox->currentText());
    QStringList filter;filter<<"*.fa";mafdir->setNameFilters(filter);
    QFileInfoList *maflist=new QFileInfoList(mafdir->entryInfoList(filter));
*/
    //QString ac_sp="";
    //content.append(" ");

    if(maf_ui->SpeedRadioBtn->isChecked())//high speed
    {
        //if(maf_ui->ThreadspinBox->text().toInt(&ok,10)!=-1)
        //{
            mafftpath.append(" --thread ");
            mafftpath.append(maf_ui->ThreadspinBox->text());
        //}
    }
    else//high accuracy
    {
        mafftpath.append(" --maxiterate 1000");
        if(maf_ui->LocalRadioBtn->isChecked())
            mafftpath.append(" --localpair");
        else if(maf_ui->GenafRadioBtn->isChecked())
            mafftpath.append(" --genafpair");
        else
            mafftpath.append(" --globalpair");

    }

    syspath=QDir::currentPath();
    syspath.append("/SystemFiles/SysTempData.txt");
    QFile mafftfile(syspath);
    QTextStream mafftstream(&mafftfile);
    if (mafftfile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        int chos=1;
        mafftstream<<chos<<"\n";
        //set the mafftpath,parameters and input&output paths
        mafftstream<<mafftpath<<"\n";
        mafftstream<<maf_ui->FileDirCombox->currentText()<<"\n";
        mafftstream<<maf_ui->ResultDirCombox->currentText()<<"\n";
        mafftfile.close();
    }
    else
        hsdlg.ErrorMsg(mafftfile.fileName());

    content="cd ";
    content.append(QDir::currentPath());
    content.append("/SystemFiles; ./SysExec&");
    cmd=qPrintable(content);
    system(cmd);
    QMessageBox::information(NULL,tr("Mafft Information"),tr("Please check the result directory, the computer is executating Mafft program..."));

    //QMessageBox::information(NULL,tr("Mafft Information"),tr("Mafft program has finished, please check the result directory!"));


    //int commandint=0;
    //bool ok;
/*    if(maflist->count()>0)
    {
        for(int i=0;i<maflist->count();i++)
        {
            content=mafftpath;
            content.append(ac_sp);
            content.append(" ");
            content.append(maflist->at(i).absoluteFilePath());
            content.append(" > ");
            content.append(maf_ui->ResultDirCombox->currentText());
            content.append("/");
            content.append(maflist->at(i).fileName());

            //cmd=new char;
            cmd=qPrintable(content);
            system(cmd);
            //delete cmd;

            //cmd=NULL;
        }
        mafmsg.setText("Mafft is done, please check the results!");
        mafmsg.exec();
    }
    else
    {
        mafmsg.setText("There is no file in the FileDirectory, check your input!");
        mafmsg.exec();
    }

    delete mafdir;delete maflist;*/
    close();

}

void MafftDialog::on_CanselBtn_clicked()
{
    close();
}

void MafftDialog::on_AccuracyRadioBtn_clicked()
{
    if(maf_ui->AccuracyRadioBtn->isChecked())
    {
        maf_ui->LocalRadioBtn->setCheckable(true);maf_ui->LocalRadioBtn->setChecked(true);
        maf_ui->GenafRadioBtn->setCheckable(true);
        maf_ui->GlobalRadioBtn->setCheckable(true);
    }
    else
    {
        maf_ui->LocalRadioBtn->setCheckable(false);
        maf_ui->GenafRadioBtn->setCheckable(false);
        maf_ui->GlobalRadioBtn->setCheckable(false);
    }
}
