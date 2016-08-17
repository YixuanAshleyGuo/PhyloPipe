#include "hamstrdialog.h"
#include "ui_hamstrdialog.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <QChar>
#include <QIntegerForSize>
#include <QSettings>
#include <QString>
#include <QByteArray>
#include <QVBoxLayout>
#include <QGridLayout>

HamstrDialog::HamstrDialog(QWidget *parent) :
    QDialog(parent),
    h_ui(new Ui::HamstrDialog)
{
    h_ui->setupUi(this);

    //put the widget into a layout and suit the window size
    QVBoxLayout* hamstrLayout=new QVBoxLayout;
    hamstrLayout->addWidget(h_ui->hamstrwidget);
    setLayout(hamstrLayout);

    h_ui->dir_comboBox->setCurrentIndex(-1);
    h_ui->out_comboBox->setCurrentText(QDir::currentPath());
    h_ui->out_comboBox->addItem(QDir::currentPath());
    result_dir=h_ui->out_comboBox->currentText();

    //initiate the qcheckboxes' layout
    cb_num=0;
    refspec_ly=new QGridLayout();
    refspec_ly->setMargin(10);
    refspec_ly->setAlignment(Qt::AlignCenter);
    h_ui->RefSpec_groupBox->setLayout(refspec_ly);

    //initiate the hmmset
    //home/GUOYIXUAN/BioinformSW/hamstr.v13.2.2---/bin/hamstr
    hmmtemp=settings.value("SWsetting/HamstrPath").toString();
    //get away of the last two '/' -> /bin/hamstr
    int indexlast=hmmtemp.lastIndexOf('/');
    indexlast=hmmtemp.lastIndexOf('/',indexlast-1);
    hmmtemp2=hmmtemp.remove(indexlast,hmmtemp.size()-indexlast);
    hmmtemp2.append("/core_orthologs");

    QDir *hmmdir=new QDir(hmmtemp2);
    QFileInfoList *hmmlist=new QFileInfoList(hmmdir->entryInfoList(QDir::Dirs|QDir::NoDotAndDotDot));
    //QFileInfo hmminfo;QString hmmsub;

    for(int i=0;i<hmmlist->size();i++)
    {
        if(i==0)
            h_ui->hms_comboBox->setCurrentText(hmmlist->at(i).fileName());//call FindRefspec() while currenttext is changed
        h_ui->hms_comboBox->addItem(hmmlist->at(i).fileName());
    }
    delete hmmlist;
    delete hmmdir;
    HamstrDialog::setAttribute(Qt::WA_QuitOnClose,false);
}

HamstrDialog::~HamstrDialog()
{
    delete h_ui;
    //cmd.close();
}

void HamstrDialog::on_ChoSeqBtn_clicked()
{
    tempCB=h_ui->dir_comboBox;
    QString fileName=QFileDialog::getOpenFileName(this,
                                                  tr("Choosing the file"),
                                                  h_ui->dir_comboBox->currentText(),
                                                  tr("FASTA Files(*.fa)"));
    if(fileName.length()==0)
        QMessageBox::information(NULL,tr("Path"),tr("You didn't select any files."));
    else{

        //QMessageBox::information(NULL,tr("currentIndex"),tr("h_ui->dir_comboBox->currentIndex()"));
        if(h_ui->dir_comboBox->currentIndex()==-1)
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
    //QMessageBox::information(NULL,tr("currentIndex"),printf("%s",h_ui->dir_comboBox->currentIndex()));
}
void HamstrDialog::on_BrwOutBtn_clicked()
{
    ChosPath(2);
}

void HamstrDialog::on_canselBtn_clicked()
{
    close();
}


void HamstrDialog::ChosPath(int index)
{
    //QComboBox *tempCB;
    if(index==1)
        tempCB=h_ui->dir_comboBox;
    else if(index==2)
        tempCB=h_ui->out_comboBox;

    QString dir=QFileDialog::getExistingDirectory(this,
                                                  tr("Choosing the Directory"),
                                                  QDir::currentPath(),
                                                  QFileDialog::ShowDirsOnly);
    if(!dir.isEmpty())
    {
        tempCB->addItem(dir);
        tempCB->setCurrentIndex(tempCB->currentIndex()+1);
    }

}

void HamstrDialog::FindRefspec()
{
    refspecname.clear();
    //remove the exist qcheckboxes
    if(cb_num>0)
    {
        QLayoutItem *item;
        while((item=refspec_ly->takeAt(0))!=0)
        {
            item->widget()->setParent(0);
            delete item;//refspec_ly->removeItem(item);
        }
        delete[] cb;
    }

    //find the NSEQ
    hmmtemp=hmmtemp2;
    hmmtemp.append("/");
    hmmtemp.append(h_ui->hms_comboBox->currentText());
    hmmtemp.append("/hmm_dir");
    hmsdir=hmmtemp;
    QDir *tdir;
    tdir=new QDir(hmmtemp);
    QStringList filter;filter<<"*.hmm";
    QFileInfoList *tlist;
    tlist=new QFileInfoList(tdir->entryInfoList(filter));

    hmmtemp.append("/");
    hmmtemp.append(tlist->at(2).fileName());

    delete tlist;delete tdir;

    QFile hfile(hmmtemp);
    int hcont=0;
    if(!hfile.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        ErrorMsg(hfile.fileName());
    }//find the 9th line
    QByteArray hline;
    while(!hfile.atEnd()&&(hcont<9))
    {
        hline=hfile.readLine();
        hcont++;
    }
    //hline="NSEQ 5\n\0";
    QByteArray hline2=hline.mid(5,hline.size()-6);//hline.size=all bytes+2
    bool ok=true;
    NSEQ=hline2.toInt(&ok,10);

    //find the hmm files' names and attach to the hmm file combobox
    HmmcomboxEnabled();

    //get the hmmset value
    //the location:/home/GUOYIXUAN/BioinformSW/hamstr.v13.2.2/core_orthologs/modelorganisms_hmmer3/modelorganisms_hmmer3.fa
    QString hmmtemp3;
    hmmtemp3=hmmtemp2;
    hmmtemp3.append("/");
    hmmtemp3.append(h_ui->hms_comboBox->currentText());
    hmmtemp3.append("/");
    hmmtemp3.append(h_ui->hms_comboBox->currentText());
    hmmtemp3.append(".fa");
    QFile hmsfile(hmmtemp3);
    hcont=1;
    if(!hmsfile.open(QIODevice::ReadOnly|QIODevice::Text))
        ErrorMsg(hmsfile.fileName());
    //find the odd lines of first 2*NSEQ lines, get the names of refspecies
    QString hmsline;
    while(!hmsfile.atEnd()&&hcont<(2*NSEQ))
    {
        hmsline=QString(hmsfile.readLine());
        if(hcont%2)//odd lines
        {
            QString htemp=hmsline.section('|',1,1);
            refspecname<<htemp;
            /*if(hcont==1)
                h_ui->ref_comboBox->setCurrentText(htemp);
            h_ui->ref_comboBox->addItem(htemp);*/
        }
        hcont++;
    }
    cb_num=refspecname.size();
    cb=new QCheckBox *[cb_num];
    for(int k=0;k<cb_num;k++)
    {
        cb[k]=new QCheckBox;
        cb[k]->setText(refspecname.at(k));
        refspec_ly->addWidget(cb[k],k/5,k%5);
    }
}

void HamstrDialog::on_applyBtn_clicked()
{
    //把要用的文件拷贝到当前目录下
    QString content;const char *cmd;
    content="cp ";
    content.append(h_ui->dir_comboBox->currentText());
    content.append(" ");
    content.append(QDir::currentPath());
    cmd=qPrintable(content);
    system(cmd);
    //system("/home/GUOYIXUAN/BioinformSW/hamstr.v13.2.2/bin/hamstr -sequence_file=testset-cDNA.fa -taxon=TEST -hmmset=modelorganisms_hmmer3 -refspec=DROME  -hmm=317.hmm");
    /*
    content="cd ";
    QString dirtmp=h_ui->dir_comboBox->currentText();
    content.append(dirtmp.left(dirtmp.lastIndexOf('/')));
    content.append(";");*/
    content="";
    content.append(settings.value("SWsetting/HamstrPath").toString());
    //hamstr start
    //content="/home/GUOYIXUAN/BioinformSW/hamstr.v13.2.2/bin/hamstr ";
    content.append(" -sequence_file=");
    //将当前文件路径中的文件名读取出来
    QFileInfo fi=QFileInfo(h_ui->dir_comboBox->currentText());
    content.append(fi.fileName());

    //taxon
    content.append(" -taxon=");
    content.append(h_ui->tax_comboBox->currentText());

    //hmmset
    content.append(" -hmmset=");
    content.append(h_ui->hms_comboBox->currentText());

    //refspec
    content.append(" -refspec=");
    int firstcheck=false;
    /*content.append(h_ui->ref_comboBox->currentText());*/
    for(int i=0;i<cb_num;i++)
    {
        if(cb[i]->isChecked())
        {
            if(firstcheck)
                content.append(",");//if there are more than one box checked, put ; between them
            content.append(cb[i]->text());
            firstcheck=true;
            if(cb_num==1)//if find the only checked one, break
                break;
        }
    }

    //hmm
    QAbstractButton *RBtn=(h_ui->buttonGroup->checkedButton());
    if(!QString::compare(RBtn->objectName(),"hmmRBtn_m"))
    {
       content.append(" -hmm=");
       content.append(h_ui->hmm_combobox->currentText());
    }

    //outpath
    content.append(" -outpath=");
    content.append(h_ui->out_comboBox->currentText());

    //datatype
    RBtn=(h_ui->buttonGroup_3->checkedButton());
    if(!QString::compare(RBtn->objectName(),"ProRBtn"))
        content.append(" -protein");
    else
        content.append(" -est");

    //concat
    RBtn=qobject_cast<QAbstractButton *>(h_ui->buttonGroup_4->checkedButton());
    if(!QString::compare(RBtn->objectName(),"concatBtn_y"))
        content.append(" -concat");

    //eval_blase
    RBtn=qobject_cast<QAbstractButton *>(h_ui->buttonGroup_2->checkedButton());
    if(!QString::compare(RBtn->objectName(),"eblastRBtn_m"))
    {
        content.append(" -eval_blast=");
        content.append(h_ui->eblast_comboBox->currentText());
    }

    //eval_hmmer
    RBtn=qobject_cast<QAbstractButton *>(h_ui->buttonGroup_5->checkedButton());
    if(!QString::compare(RBtn->objectName(),"ehmmerRBtn_m"))
    {
        content.append(" -eval_hmmer=");
        content.append(h_ui->ehmmer_comboBox->currentText());
    }

    //rbh
    RBtn=qobject_cast<QAbstractButton *>(h_ui->buttonGroup_6->checkedButton());
    if(!QString::compare(RBtn->objectName(),"rbhBtn_y"))
        content.append(" -rbh");

    //flag
    RBtn=qobject_cast<QAbstractButton *>(h_ui->buttonGroup_7->checkedButton());
    if(!QString::compare(RBtn->objectName(),"flag_relaxRBtn"))
        content.append(" -relaxed");
    else
        content.append(" -strict");

    //representative
    RBtn=qobject_cast<QAbstractButton *>(h_ui->buttonGroup_8->checkedButton());
    if(!QString::compare(RBtn->objectName(),"representBtn_y"))
        content.append(" -representative");

    //longhead
    RBtn=qobject_cast<QAbstractButton *>(h_ui->buttonGroup_9->checkedButton());
    if(!QString::compare(RBtn->objectName(),"lheadBtn_y"))
        content.append(" -longhead");

    content.append("&");//put the command to run in the back

    /*QMessageBox msg;
    msg.setText(content);
    msg.exec();*/

    cmd=qPrintable(content);
    system(cmd);

    close();//close the dialog after hamstr execution
}

void HamstrDialog::on_hmmRBtn_m_clicked()
{
    h_ui->hmm_combobox->setEnabled(true);
    HmmcomboxEnabled();
}

void HamstrDialog::on_hmmRBtn_d_clicked()
{
    h_ui->hmm_combobox->setDisabled(true);
}

void HamstrDialog::on_eblastRBtn_m_clicked()
{
    h_ui->eblast_comboBox->setEnabled(true);
}

void HamstrDialog::on_eblastRBtn_d_clicked()
{
    h_ui->eblast_comboBox->setDisabled(true);
}

void HamstrDialog::on_ehmmerRBtn_m_clicked()
{
    h_ui->ehmmer_comboBox->setEnabled(true);
}

void HamstrDialog::on_ehmmerRBtn_d_clicked()
{
    h_ui->ehmmer_comboBox->setDisabled(true);
}

void HamstrDialog::HmmcomboxEnabled()
{
    hmmtemp=hmmtemp2;
    hmmtemp.append("/");
    hmmtemp.append(h_ui->hms_comboBox->currentText());
    hmmtemp.append("/hmm_dir");
    QDir *tdir;
    tdir=new QDir(hmmtemp);
    QStringList filter;filter<<"*.hmm";
    QFileInfoList *tlist;
    tlist=new QFileInfoList(tdir->entryInfoList(filter));
    for(int i=0;i<tlist->size();i++)
    {
        if(i==0)
            h_ui->hmm_combobox->setCurrentText(tlist->at(i).fileName());
        h_ui->hmm_combobox->addItem(tlist->at(i).fileName());
    }
    delete tlist;delete tdir;
}

void HamstrDialog::ErrorMsg(QString filename)
{
    QMessageBox errmsg;
    QString tmp;
    tmp="ERROR occured while opening the data!Check your file directory:";
    tmp.append(filename);
    errmsg.setText(tmp);
    errmsg.exec();
    close();
}

void HamstrDialog::on_hms_comboBox_currentTextChanged()
{
    if(h_ui->hmm_combobox->currentIndex()!=-1)
        h_ui->hmm_combobox->clear();
    FindRefspec();
}
