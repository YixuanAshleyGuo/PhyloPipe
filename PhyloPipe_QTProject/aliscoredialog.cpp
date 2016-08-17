#include "aliscoredialog.h"
#include "ui_aliscoredialog.h"
#include "muscledialog.h"
#include "ui_muscledialog.h"

#include <QString>
#include <QStringList>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QVBoxLayout>

AliscoreDialog::AliscoreDialog(QWidget *parent) :
    QDialog(parent),
    sco_ui(new Ui::AliscoreDialog)
{
    sco_ui->setupUi(this);
    QVBoxLayout* aliscoreLayout=new QVBoxLayout;
    aliscoreLayout->addWidget(sco_ui->aliscorewidget);
    setLayout(aliscoreLayout);

    MuscleDialog musdlg;
    //musdlg.musresult_dir="/home/GUOYIXUAN/temp/temp4s";
    sco_ui->FileDirCombox->setCurrentText(musdlg.musresult_dir);
    sco_ui->FileDirCombox->addItem(musdlg.musresult_dir);
    sco_ui->ResDirCombox->setCurrentText(QDir::currentPath());
    sco_ui->ResDirCombox->addItem(QDir::currentPath());
}

AliscoreDialog::~AliscoreDialog()
{
    delete sco_ui;
}

void AliscoreDialog::on_FileDirBtn_clicked()
{
    QString dir=QFileDialog::getExistingDirectory(this,
                                                  tr("Choosing the Input File Directory"),
                                                  sco_ui->FileDirCombox->currentText(),
                                                  QFileDialog::ShowDirsOnly);
    if(!dir.isEmpty())
    {
        sco_ui->FileDirCombox->addItem(dir);
        sco_ui->FileDirCombox->setCurrentIndex(sco_ui->FileDirCombox->currentIndex()+1);
    }
}
void AliscoreDialog::on_ResDirBtn_clicked()
{
    QString dir=QFileDialog::getExistingDirectory(this,
                                                  tr("Choosing the Input File Directory"),
                                                  sco_ui->ResDirCombox->currentText(),
                                                  QFileDialog::ShowDirsOnly);
    if(!dir.isEmpty())
    {
        sco_ui->ResDirCombox->addItem(dir);
        sco_ui->ResDirCombox->setCurrentIndex(sco_ui->ResDirCombox->currentIndex()+1);
    }
    aliresultdir=dir;
}

void AliscoreDialog::on_ApplyBtn_clicked()
{
    QMessageBox::information(NULL,tr("MatrixMasking Information"),tr("Please check the input directory, the computer is executaing aliscore program..."));

    QMessageBox scomsg;
    QString aliscorpath,content,scopath,syspath;
    QStringList fileNames;
    const char *cmd;

        //put the aliscore files to input files directories
        aliscorpath=settings.value("SWsetting/AliscorePath").toString();
        int i=0;
        while(aliscorpath.section(";",i,i)!="")
        {
            fileNames<<aliscorpath.section(";",i,i);
            i++;
        }
        for(i=0;i<fileNames.size();i++)
        {
            content="cp ";
            content.append(fileNames.at(i));
            content.append(" ");
            content.append(sco_ui->FileDirCombox->currentText());
            //find the commend
            if(QString::compare(fileNames.at(i).right(2),"pl")==0)
            {
                scopath=fileNames.at(i);
                QFileInfo tmpfile(scopath);
                scopath=tmpfile.fileName();
            }
            //TEST
            //scomsg.setText(content);scomsg.exec();
            //TEST

            cmd=qPrintable(content);
            system(cmd);
        }

        //aliscorpath="cd ";
        //aliscorpath.append(sco_ui->FileDirCombox->currentText());
        //aliscorpath.append(";perl ");
        aliscorpath="perl ";
        aliscorpath.append(scopath);
        if(sco_ui->r_checkBox->isChecked())
        {
            aliscorpath.append(" -r");
            aliscorpath.append(sco_ui->r_spinBox->text());
        }
        if(sco_ui->e_checkBox->isChecked())
            aliscorpath.append(" -e");

        syspath=QDir::currentPath();
        syspath.append("/SystemFiles/SysTempData.txt");
        QFile aliscorefile(syspath);
        QTextStream aliscorestream(&aliscorefile);
        if(aliscorefile.open(QIODevice::WriteOnly|QIODevice::Text))
        {
            int chos=3;
            aliscorestream<<chos<<"\n";
            aliscorestream<<aliscorpath<<"\n";
            aliscorestream<<sco_ui->FileDirCombox->currentText()<<"\n";
            aliscorestream<<sco_ui->ResDirCombox->currentText()<<"\n";
            aliscorefile.close();
        }
        else
        {
            hsdlg.ErrorMsg(aliscorefile.fileName());
            close();
        }

        content="cd ";
        content.append(QDir::currentPath());
        content.append("/SystemFiles; ./SysExec");

        //TEST
        //scomsg.setText(content);scomsg.exec();
        //TEST
        cmd=qPrintable(content);
        system(cmd);

        QMessageBox::information(NULL,tr("MatrixMasking Information"),tr("Aliscore program has finished, the computer is executating result adjusting step..."));

        //aliscorpath.append(" -i ");
        //aliscorpath.append(sco_list->at(i).fileName());

        QDir *sco_dir;
        sco_dir=new QDir(sco_ui->FileDirCombox->currentText());
        QStringList filter;filter<<"*.fa";sco_dir->setNameFilters(filter);
        QFileInfoList *sco_list=new QFileInfoList(sco_dir->entryInfoList(filter));

/*        if(sco_list->count()>0)
        {
            for(int i=0;i<sco_list->size();i++)
            {
                //cd to the input files dir, perl aliscore ---
                content="cd ";
                content.append(sco_ui->FileDirCombox->currentText());
                content.append(";perl ");
                content.append(scopath);//perl aliscore.02.2.pl -i ");
                if(sco_ui->r_checkBox->isChecked())
                {
                    content.append(" -r ");
                    content.append(sco_ui->r_spinBox->text());
                }
                if(sco_ui->e_checkBox->isChecked())
                    content.append(" -e");
                content.append(" -i ");
                content.append(sco_list->at(i).fileName());

                //TEST
                //scomsg.setText(content);scomsg.exec();
                //TEST

                cmd=qPrintable(content);
                system(cmd);

                //TEST
                //content="echo test";cmd=qPrintable(content);system(cmd);
                //TEST
            }
            scomsg.setText("Aliscore has done, please check the results!");
            scomsg.exec();
        }
        delete sco_dir;
*/
        //cut species: the same function as alicut
        QByteArray spec_gene,spec_line,pos_gene;
        //linepos is the line position in the whole file, linecont is the line count of a gene, linebyte is the total bytes of a gene.
        int spec_bool=1,linecont=0,linebyte=0,linepos=0;//,linestart;
        QString origname,resultname,posname;

        //START the alicut(self-written code)
        bool mafftok=true;
        if(sco_list->count()>0)
        {
            for(int i=0;i<sco_list->size();i++)
            {
                origname=sco_list->at(i).absoluteFilePath();
                //QByteArray bafile=posname.toLatin1();
                QFile qfile(origname);
                //get the position file data
                posname=origname.append("_List_random.txt");
                QFile posfile(posname);

                resultname=sco_list->at(i).fileName();
                resultname.insert(resultname.size()-3,"_Adjust");
                resultname.prepend("/");
                resultname.prepend(sco_ui->ResDirCombox->currentText());
                QFile resultfile(resultname);
                resultfile.open(QIODevice::ReadWrite|QIODevice::Text);
                resultfile.close();//create the resultfile
                QString spec;

                if(!posfile.open(QIODevice::ReadOnly|QIODevice::Text))
                {
                    hsdlg.ErrorMsg(posfile.fileName());mafftok=false;
                    break;
                }
                pos_gene=posfile.readAll();
                posfile.close();

                if(QString::compare(pos_gene,"")==0)//posfile contains nothing,copy the file to the result directory directly.
                {
                    if(!QFile::copy(origname,resultname))
                    {
                        QMessageBox::information(NULL,tr("MatrixMasking Information"),tr("Cannot copy the file from input directory to the output directory while the cut information is NULL!"));
                        mafftok=false;break;
                    }
                }
                else//posfile contains postion information and needs cutting data
                {
                    //open the species file
                    if(!qfile.open(QIODevice::ReadOnly|QIODevice::Text))
                        hsdlg.ErrorMsg(qfile.fileName());

                    QStringList pos_nums=QString(pos_gene).split(' ');
                    bool firstline=true;//linebyte should be the total byte of first line of every species
                    while(!qfile.atEnd())
                    {
                        //linebyte=spec_line.size();//the bytes every line
                        spec_line=qfile.readLine();
                        linepos++;
                        if(spec_line.left(1)==">")
                        {
                            spec=QString(spec_line);
                            //change the spec_bool
                            spec_bool*=-1;
                            if(spec_bool<0)//the start line has occured(start with">")
                                continue;//linestart=linepos;

                        }
                        linecont++;
                        //get rid of the last character /n/0
                        //spec_line.remove(spec_line.size()+1,2); no effect
                        if(spec_bool<0)
                        {
                            if(firstline)
                            {
                                //attach the >species to the new file
                                if(resultfile.open(QIODevice::WriteOnly|QIODevice::Append|QIODevice::Text))
                                {
                                    QTextStream catstream(&resultfile);
                                    catstream<<spec;
                                    //catstream.flush();
                                    resultfile.close();
                                }
                                else
                                    hsdlg.ErrorMsg(resultfile.fileName());

                                linebyte=spec_line.size()-1;
                                firstline=false;
                            }
                            //spec_line.simplified();spec_line.trimmed();spec_line.size()=the charactors number+1(\n)
                            spec_gene.append(spec_line.left(spec_line.size()-1));
                        }
                       else//the end line (start with">")
                        {
                            linecont-=1;
                            //at(linebyte-1)=" ",at(linebyte)="G"(start of the 2nd line)
                            //959YES
                            //cut the species' protein data
                            int j;
                            for(j=pos_nums.size()-1;j>=0;j--)
                            {
                                spec_gene.remove(pos_nums.at(j).toInt()-1,1);
                            }

                            for(j=0;j<spec_gene.size();)
                            {
                                if((j+linebyte)<=spec_gene.size())
                                {
                                    //attach the >species to the new file
                                    if(resultfile.open(QIODevice::WriteOnly|QIODevice::Append|QIODevice::Text))
                                    {
                                        QTextStream catstream(&resultfile);
                                        catstream<<spec_gene.mid(j,linebyte)<<"\n";
                                        //catstream.flush();
                                        resultfile.close();
                                    }
                                    else
                                        hsdlg.ErrorMsg(resultfile.fileName());
                                }
                                else
                                    if(j<=(spec_gene.size()-1))
                                    {
                                        if(resultfile.open(QIODevice::WriteOnly|QIODevice::Append|QIODevice::Text))
                                        {
                                            QTextStream catstream(&resultfile);
                                            catstream<<spec_gene.mid(j,linebyte)<<"\n";
                                            //catstream.flush();
                                            resultfile.close();
                                        }
                                        else
                                            hsdlg.ErrorMsg(resultfile.fileName());
                                    }
                                j+=linebyte;
                            }
                            //660Yes
                            //YES!!!result is right
                            spec_bool*=-1;
                            //linestart=linepos;
                            spec_gene="";
                            linecont=0;
                            firstline=true;
                            //break;//for the first species
                        }
                    }
                    //for the last species, it has no '>'to process the reduction
                    //attach the last species to the new file
                    int j;
                    for(j=pos_nums.size()-1;j>=0;j--)
                    {
                        spec_gene.remove(pos_nums.at(j).toInt()-1,1);
                    }
                    //scomsg.setText(spec_gene);
                    //scomsg.exec();
                    for(j=0;j<spec_gene.size();)
                    {
                        if((j+linebyte)<=spec_gene.size())
                        {
                            //attach the >species to the new file
                            if(resultfile.open(QIODevice::WriteOnly|QIODevice::Append|QIODevice::Text))
                            {
                                QTextStream catstream(&resultfile);
                                catstream<<spec_gene.mid(j,linebyte)<<"\n";
                                //catstream.flush();
                                resultfile.close();
                            }
                            else
                                hsdlg.ErrorMsg(resultfile.fileName());
                        }
                        else
                            if(j<=(spec_gene.size()-1))
                            {
                                if(resultfile.open(QIODevice::WriteOnly|QIODevice::Append|QIODevice::Text))
                                {
                                    QTextStream catstream(&resultfile);
                                    catstream<<spec_gene.mid(j,linebyte)<<"\n";
                                    //catstream.flush();
                                    resultfile.close();
                                }
                                else
                                    hsdlg.ErrorMsg(resultfile.fileName());
                            }
                        j+=linebyte;
                    }
                    spec_gene="";
                    spec_bool=1;//set to the origin value
                    qfile.close();
                    //break;//for the first file
                }

            }
            if(mafftok)
            {
                QMessageBox::information(NULL,tr("Matrix Information"),tr("Please check the result directory, the MatrixMasking step has been finished."));
            }
            else{
                scomsg.setText("Check for the mafft results, there are some problem!");
                scomsg.exec();
            }

        close();
        }
        else
        {
            scomsg.setText("There is no file in the input file directory, CHECK your input!");
            scomsg.exec();
        }
        delete sco_list;
}

void AliscoreDialog::on_CansleBtn_clicked()
{
    close();
}

void AliscoreDialog::on_r_checkBox_clicked()
{
    if(sco_ui->r_checkBox->isChecked())
    {
        sco_ui->r_spinBox->setEnabled(true);
    }
    else
    {
        sco_ui->r_spinBox->setEnabled(false);
    }
}
