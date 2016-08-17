#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include "qfiledialog.h"
#include "qmessagebox.h"
#include <QVBoxLayout>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QMessageBox>

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    s_ui(new Ui::SettingsDialog)
{
    s_ui->setupUi(this);

    //put the widget into a layout and suit the window size
    QVBoxLayout* settingsLayout=new QVBoxLayout;
    settingsLayout->addWidget(s_ui->settingswidget);
    setLayout(settingsLayout);

    QString inipath=QDir::currentPath();
    inipath.append("/SystemFiles/Settings.ini");
    QFile inifile(inipath);
    QByteArray iniline[6];int i=0;
    if(inifile.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        while(!inifile.atEnd())
            iniline[i++]=inifile.readLine();
        inifile.close();
    }
    hamstrpath=QString(iniline[0].left(iniline[0].length()-1));
    mafftpath=QString(iniline[1].left(iniline[1].length()-1));
    musclepath=QString(iniline[2].left(iniline[2].length()-1));
    aliscorepath=QString(iniline[3].left(iniline[3].length()-1));
    raxmlpath=QString(iniline[4].left(iniline[4].length()-1));
    raxmlgui=QString(iniline[5].left(iniline[5].length()-1));


    QMessageBox setmsg;

    s_ui->ham_comboBox->setCurrentIndex(-1);
    if(QString::compare(hamstrpath,""))
    {
        s_ui->ham_comboBox->setCurrentText(hamstrpath);
        s_ui->ham_comboBox->addItem(hamstrpath);
    }

    s_ui->maf_comboBox->setCurrentIndex(-1);   
    if(QString::compare(mafftpath,""))
    {
        s_ui->maf_comboBox->setCurrentText(mafftpath);
        s_ui->maf_comboBox->addItem(mafftpath);
    }

    s_ui->mus_comboBox->setCurrentIndex(-1);
    if(QString::compare(musclepath,""))
    {
        s_ui->mus_comboBox->setCurrentText(musclepath);
        s_ui->mus_comboBox->addItem(musclepath);
    }

    s_ui->rax_comboBox->setCurrentIndex(-1);
    if(QString::compare(raxmlpath,""))
    {
        s_ui->rax_comboBox->setCurrentText(raxmlpath);
        s_ui->rax_comboBox->addItem(raxmlpath);
    }
    if(QString::compare(raxmlgui,""))
    {
        s_ui->raxcmd_comboBox->setCurrentText(raxmlgui);
    }

    s_ui->sco_comboBox->setCurrentIndex(-1);
    if(QString::compare(aliscorepath,""))
    {
        s_ui->sco_comboBox->setCurrentText(aliscorepath);
        s_ui->sco_comboBox->addItem(aliscorepath);
    }

    //raxmlpath=s_ui->rax_comboBox->currentText();
}

SettingsDialog::~SettingsDialog()
{
    delete s_ui;
}

void SettingsDialog::ChosSWPath(int index)
{
    if(index==2)
        tempCB=s_ui->ham_comboBox;
    else if(index==3)
        tempCB=s_ui->maf_comboBox;
    else if(index==4)
        tempCB=s_ui->mus_comboBox;
    else if(index==5)
        tempCB=s_ui->rax_comboBox;
//    else if(index==6)
//        tempCB=s_ui->sco_comboBox;

    QString fileName=QFileDialog::getOpenFileName(this,
                                                  tr("Choosing the SoftWare file"),
                                                  tempCB->currentText());
                                                  //tr("FASTA Files(*.fa)")
    if(fileName.length()==0)
        QMessageBox::information(NULL,tr("Path"),tr("You didn't select any files."));
    else{
        //QMessageBox::information(NULL,tr("currentIndex"),tr("h_ui->dir_comboBox->currentIndex()"));
        if(tempCB->currentIndex()==-1)
        {
            tempCB->setCurrentText(fileName);
            tempCB->addItem(fileName);
        }
        else
        {
            tempCB->addItem(fileName);
            tempCB->setCurrentIndex(tempCB->currentIndex()+1);
        }
    }
}

void SettingsDialog::on_ham_btn_clicked()
{
    ChosSWPath(2);
    //hamstrpath=s_ui->ham_comboBox->currentText();
}

void SettingsDialog::on_mus_btn_clicked()
{
    ChosSWPath(4);
    //musclepath=s_ui->mus_comboBox->currentText();
}

void SettingsDialog::on_maf_btn_clicked()
{
    ChosSWPath(3);
    //mafftpath=s_ui->maf_comboBox->currentText();
}

void SettingsDialog::on_sco_btn_clicked()
{
    //ChosSWPath(6);
    tempCB=s_ui->sco_comboBox;
    QStringList fileNames=QFileDialog::getOpenFileNames(this,
                                                  tr("Choosing the SoftWare file"),
                                                  tempCB->currentText());
                                                  //tr("FASTA Files(*.fa)")
    if(fileNames.size()==0)
        QMessageBox::information(NULL,tr("Path"),tr("You didn't select any files."));
    else{
        //QMessageBox::information(NULL,tr("currentIndex"),tr("h_ui->dir_comboBox->currentIndex()"));
        QString strnames=fileNames.join(";");
        if(tempCB->currentIndex()==-1)
        {
                tempCB->setCurrentText(strnames);
                tempCB->addItem(strnames);
        }
        else
        {
                tempCB->addItem(strnames);
                tempCB->setCurrentIndex(tempCB->currentIndex()+1);
        }
        //aliscorepath=tempCB->currentText();
    }
}

/*void SettingsDialog::on_cate_btn_clicked()
{
    tempCB=s_ui->cate_comboBox;
    QStringList fileNames=QFileDialog::getOpenFileNames(this,
                                                  tr("Choosing the SoftWare file"),
                                                  tempCB->currentText());
                                                  //tr("FASTA Files(*.fa)")
    if(fileNames.size()==0)
        QMessageBox::information(NULL,tr("Path"),tr("You didn't select any files."));
    else{
        //QMessageBox::information(NULL,tr("currentIndex"),tr("h_ui->dir_comboBox->currentIndex()"));
        QString strnames=fileNames.join(";");
        if(tempCB->currentIndex()==-1)
        {
                tempCB->setCurrentText(strnames);
                tempCB->addItem(strnames);
        }
        else
        {
                tempCB->addItem(strnames);
                tempCB->setCurrentIndex(tempCB->currentIndex()+1);
        }
        concatepath=tempCB->currentText();
    }
}*/

void SettingsDialog::on_rax_btn_clicked()
{
    ChosSWPath(5);
    raxmlpath=s_ui->rax_comboBox->currentText();
}

void SettingsDialog::on_aply_btn_clicked()
{
    settings.beginGroup("SWsetting");
    settings.setValue("HamstrPath", s_ui->ham_comboBox->currentText());
    settings.setValue("MusclePath", s_ui->mus_comboBox->currentText());
    settings.setValue("MafftPath",s_ui->maf_comboBox->currentText());
    settings.setValue("AliscorePath",s_ui->sco_comboBox->currentText());
    //settings.setValue("ConcatePath",s_ui->cate_comboBox->currentText());
    settings.setValue("RaxmlPath",s_ui->rax_comboBox->currentText());
    settings.setValue("RaxmlCmd",s_ui->raxcmd_comboBox->currentText());
    settings.endGroup();
    hamstrpath=s_ui->ham_comboBox->currentText();
    musclepath=s_ui->mus_comboBox->currentText();
    mafftpath=s_ui->maf_comboBox->currentText();
    aliscorepath=s_ui->sco_comboBox->currentText();
    raxmlpath=s_ui->rax_comboBox->currentText();
    raxmlgui=s_ui->raxcmd_comboBox->currentText();

        QString inipath=QDir::currentPath();
        inipath.append("/SystemFiles/Settings.ini");
        QFile inifile(inipath);
        if(inifile.open(QIODevice::WriteOnly|QIODevice::Text))
        {
            QTextStream inistream(&inifile);
            inistream<<hamstrpath<<"\n";
            inistream<<mafftpath<<"\n";
            inistream<<musclepath<<"\n";
            inistream<<aliscorepath<<"\n";
            inistream<<raxmlpath<<"\n";
            inistream<<raxmlgui<<"\n";
            inifile.close();
        }

    close();
}

void SettingsDialog::on_csl_btn_clicked()
{
    close();
}


