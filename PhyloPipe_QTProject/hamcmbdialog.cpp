#include "hamcmbdialog.h"
#include "ui_hamcmbdialog.h"

#include <QDir>
#include <QFileDialog>
#include <QTextStream>
#include <QVector>
#include <QMessageBox>
#include <QCheckBox>
#include <QElapsedTimer>
#include <QPalette>
#include <QGridLayout>
#include <QVBoxLayout>

HamCmbDialog::HamCmbDialog(QWidget *parent) :
    QDialog(parent),
    c_ui(new Ui::HamCmbDialog)
{
    c_ui->setupUi(this);
    HamCmbDialog::setAttribute(Qt::WA_QuitOnClose);

    //put the widget into a layout and suit the window size
    QVBoxLayout* hamcmbLayout=new QVBoxLayout;
    hamcmbLayout->addWidget(c_ui->hamcmbwidget);
    setLayout(hamcmbLayout);

    c_ui->FileDirCombox->setCurrentText(hsdlg.result_dir);
    c_ui->FileDirCombox->addItem(hsdlg.result_dir);
    c_ui->ResultDirCombox->setCurrentText(QDir::currentPath());
    c_ui->ResultDirCombox->addItem(QDir::currentPath());
    c_ui->HmmDirCombox->setCurrentText(hsdlg.hmsdir);
    c_ui->HmmDirCombox->addItem(hsdlg.hmsdir);
    cresult_dir=c_ui->ResultDirCombox->currentText();

    click=true;
    cb_num=0;
    //hmc_start=hsdlg.hmm_start;
    //hmc_end=hsdlg.hmm_end;
    //c_ui->HmStart_spinBox->setValue(hmc_start);
    //c_ui->HmEnd_spinBox->setValue(hmc_end);
    /**/
    orig_ly=new QGridLayout();
    orig_ly->setMargin(10);
    orig_ly->setAlignment(Qt::AlignCenter);
    c_ui->groupBox->setLayout(orig_ly);
}

HamCmbDialog::~HamCmbDialog()
{
    delete c_ui;
}

void HamCmbDialog::on_FileDirBtn_clicked()
{
    hmmname.clear();//reset the hmmname stringlist
    QString dir=QFileDialog::getExistingDirectory(this,
                                                  tr("Choosing the Directory"),
                                                  c_ui->FileDirCombox->currentText(),
                                                  QFileDialog::ShowDirsOnly);
    if(!dir.isEmpty())
    {
        c_ui->FileDirCombox->addItem(dir);
        c_ui->FileDirCombox->setCurrentIndex(c_ui->FileDirCombox->currentIndex()+1);
    }

    //QMessageBox tmpbox;//for test

    //find the first .fa file and get the hmmset information
    QString apath=c_ui->FileDirCombox->currentText();
    QDir *adir=new QDir(apath);
    QFileInfoList *aList=new QFileInfoList(adir->entryInfoList(QDir::Dirs|QDir::NoDotAndDotDot));
    QFileInfo afileinfo;
    QString bpath;QDir *bdir;QFileInfoList *bList;
    if(aList->size()>0)
    {
        for(int i=0;i<aList->size();i++)
        {
            afileinfo=aList->at(i);
            bpath=afileinfo.filePath();
            bdir=new QDir(bpath);
            if(QString::compare(bdir->dirName(),"tmp"))//avoid "tmp" directory, there are trash data
            {
                QStringList filter;
                filter<<"*.fa";bdir->setNameFilters(filter);
                bList=new QFileInfoList(bdir->entryInfoList(filter));
                if(bList->count()>0)
                {

                    for(int j=0;j<bList->size();j++)
                    {
                        if(QString::compare(bList->at(j).fileName().right(6),"cds.fa"))
                        {
                            QFile origfile(bList->at(j).absoluteFilePath());
                            int ocont=0;
                            if(!origfile.open(QIODevice::ReadOnly|QIODevice::Text))
                                hsdlg.ErrorMsg(origfile.fileName());
                            //find the 9th line
                            QString oline;
                            while(!origfile.atEnd())
                            {
                                oline=QString(origfile.readLine());
                                ocont++;
                                if(ocont%2)//odd line contains the hmmset information
                                {
                                    //QString qtemp=oline;//"XX|DROME|XX..."
                                    QString qtmp=oline.section('|',1,1);
                                    hmmname<<qtmp;
                               }
                            }
                            //remove the last hmmname which is useful
                            hmmname.removeAt(ocont/2-1);//the last is even line, the odd line (ocont/2)==even line (ocont/2-1)
                            origfile.close();
                            break;
                        }
                    }
                    break;
                }
                delete bList;
            }
        }
        delete adir;delete aList;delete bdir;

        //remove the exist qcheckboxs
        if(0<cb_num)
        {
            QLayoutItem *item;
            while((item=orig_ly->takeAt(0))!=0)
            {
                item->widget()->setParent(0);
                orig_ly->removeItem(item);//delete item;
            }
            delete[] cb;

        }

        //initialize the checkbox
        cb=new QCheckBox *[hmmname.size()];
        for(int k=0;k<hmmname.size();k++)
        {
            cb[k]=new QCheckBox;
            cb[k]->setText(hmmname.at(k));
            cb[k]->setEnabled(false);
            orig_ly->addWidget(cb[k],k/5,k%5);
        }
        cb_num=hmmname.size();
    }
    else{
        QMessageBox::information(NULL,tr("INPUT WARNING!"),tr("Please choose the recognizable input file directory!"));

    }

}


void HamCmbDialog::on_ResultDirBtn_clicked()
{
    QString dir=QFileDialog::getExistingDirectory(this,
                                                  tr("Choosing the Result Directory"),
                                                  c_ui->ResultDirCombox->currentText(),
                                                  QFileDialog::ShowDirsOnly);
    if(!dir.isEmpty())
    {
        c_ui->ResultDirCombox->addItem(dir);
        c_ui->ResultDirCombox->setCurrentIndex(c_ui->ResultDirCombox->currentIndex()+1);
    }
    cresult_dir=dir;
}

void HamCmbDialog::on_HmmDirBtn_clicked()
{
    QString dir=QFileDialog::getExistingDirectory(this,
                                                  tr("Choosing the Result Directory"),
                                                  c_ui->HmmDirCombox->currentText(),
                                                  QFileDialog::ShowDirsOnly);
    if(!dir.isEmpty())
    {
        c_ui->HmmDirCombox->addItem(dir);
        c_ui->HmmDirCombox->setCurrentIndex(c_ui->HmmDirCombox->currentIndex()+1);
    }
}

void HamCmbDialog::on_CanselBtn_clicked()
{
    close();
}

void HamCmbDialog::on_StartCmbBtn_clicked()
{
    bool quit_bool=false;
    QMessageBox hmsmsg;
    //update the values that might be changed by users
    //hmc_start=c_ui->HmStart_spinBox->text().toInt();
    //hmc_end=c_ui->HmEnd_spinBox->text().toInt();
    hmcdir=c_ui->HmmDirCombox->currentText();
    QVector<QString> hmmname_str(0);
    //find the hmm files and put them into a QStringList
    QStringList filter;
    QDir *hmc_dir=new QDir(hmcdir);
    filter<<"*.hmm";hmc_dir->setNameFilters(filter);
    QFileInfoList *hmc_list=new QFileInfoList(hmc_dir->entryInfoList(filter));

    if(hmc_list->count()>0)
    {
        for(int j=0;j<hmc_list->size();j++)
            hmmname_str.append(hmc_list->at(j).fileName().replace(hmc_list->at(j).fileName().size()-4,4,".fa"));
    }
    else{
        hmsmsg.setText("There is no hmm file in the directory, please check it!");
        hmsmsg.exec();
        quit_bool=true;
    }
    if(quit_bool)
    {
        return;
        close();//close the dialog
    }

    //FileCont is for the count of existence compared to threshold

    QVector<int> FileCont(hmc_list->count());
    QString path=c_ui->FileDirCombox->currentText();
    QDir *dir=new QDir(path);
    QFileInfoList *fList=new QFileInfoList(dir->entryInfoList(QDir::Dirs|QDir::NoDotAndDotDot));
    QFileInfo fileInfo;
    QString subpath;QDir *subdir;QFileInfoList *subfList;//=new QFileInfoList;

    delete hmc_dir;delete hmc_list;
    //remove the "tmp" directory if it exists
    QString tdir;
    tdir=path;
    tdir.append("/tmp");
    removeDirWithContent(tdir);//remove all the file in the "tdir" directory
    //store the names of species that users might want to keep
    for(int i=0;i<fList->size();i++)
    {
            fileInfo=fList->at(i);
            subpath=fileInfo.filePath();
            subdir=new QDir(subpath);

            filter<<"*.fa"; subdir->setNameFilters(filter);
            subfList=new QFileInfoList(subdir->entryInfoList(filter));
            // Keep those files with ".fa" files
            if(subfList->count()>0)
            {
                FileDir.append(subpath);//add to the filedirectory linklist
                int start=0;
                for(int j=0;j<subfList->size();j++)
                {
                    if(QString::compare(subfList->at(j).fileName().right(6),"cds.fa"))//returns 0 if the two strings are equal
                    {
                        for(int k=start;k<hmmname_str.size();k++)
                        {
                            if(QString::compare(subfList->at(j).fileName(),hmmname_str.at(k))==0)
                            {
                                FileCont[k]+=1;
                                start=k+1;
                                quit_bool=true;//this means that the filename has a match in the hmmset
                                break;
                            }                           
                        }
                        if(quit_bool)//there is a match
                        {
                            quit_bool=false;//set to false to avoid closing the dialog
                        }
                        else{
                            hmsmsg.setText("There is file that has no match of the hmmset, please check the hmm directory!");
                            hmsmsg.exec();
                            close();
                            quit_bool=true;//set to true to close the dialog
                        }
                        //int cont=FileName2Int(subfList->at(j).fileName());
                        //FileCont[cont-hmc_start]+=1;//文件名对应的数字count++
                    }
                    if(quit_bool)
                        break;
                }
            }
            if(quit_bool)
                break;
            delete subdir;delete subfList;
    }
    if(quit_bool)
    {
        return;
        close();
    }
    delete dir; delete fList;

    int threshold=FileDir.size()*(c_ui->CmbFacSpinbox->text().toInt())/100;
    QLinkedListIterator<QString> listItrator (FileDir);


    //新建文件夹用于存放处理好的.fa文件
    QString content;
    const char *cmd;

    //remove directory with the same name, overlay
    QString hresult=c_ui->ResultDirCombox->currentText();
    hresult.append("/HaMStR_result");
    //remove if already exist one
    removeDirWithContent(hresult);
    //create a new one
    content="mkdir ";
    content.append(hresult);
    cmd=qPrintable(content);
    system(cmd);

    //遍历整个文件夹列表，将出现次数>阈值的.fa文件去前10行，合并到新文件中
    while (listItrator.hasNext()) {
        QDir *tempDir=new QDir (listItrator.next());
        QStringList tempfilter; tempfilter<<"*.fa";

        tempDir->setNameFilters(tempfilter);
        QFileInfoList *tempfList=new QFileInfoList(tempDir->entryInfoList(tempfilter));

        int start=0;
        for(int k=0;k<tempfList->size();k++)
        {
            //去掉cds.fa结尾的文件
            int tempcont=-1;//the position of at(k)
            QString tfilename=tempfList->at(k).fileName();
            if(QString::compare(tfilename.right(6),"cds.fa"))//if the two string are equal, then return 0, else, continue to do next operations
            {
                for(int j=start;j<hmmname_str.size();j++)
                {
                    if(QString::compare(tempfList->at(k).fileName(),hmmname_str.at(j))==0)
                    {
                        tempcont=j;
                        start=j+1;
                        break;
                    }
                }
                if(tempcont==-1)//ERROR
                {
                    hmsmsg.setText("There exist a file that has no corresponding hmm file!");
                    continue;
                }

                if((FileCont[tempcont])>=threshold||FileCont[tempcont]==-1)
                {
                    QFile tfile(tempfList->at(k).absoluteFilePath());
                    //符合条件的.fa文件，如果有12行，则删除前10行【这里一定要删除前10行吗->方便cat调用，cat是将一个文件内容合并到另一个文件中】
                    //并且将最后两行合并到新文件夹下的新建.fa文件当中
                    int lcont=0;
                    if(!tfile.open(QIODevice::ReadOnly|QIODevice::Text))
                        hsdlg.ErrorMsg(tfile.fileName());
                    while(!tfile.atEnd())
                    {
                        QByteArray tline=tfile.readLine();
                        lcont++;
                    }
                    tfile.close();
                    if(lcont>2)
                    {
                        if(FileCont[tempcont]==-1)//exist XX.fa file
                        {
                            /*content="sed -i \"1,+9 d\" ";
                            content.append(tempfList->at(k).absoluteFilePath());
                            cmd=qPrintable(content);
                            system(cmd);*/
                            char cmds[512];
                            QByteArray ba=tempfList->at(k).absoluteFilePath().toLatin1();
                            sprintf(cmds,"sed -i \"1,+%d d\" %s",lcont-3,ba.data());
                            system(cmds);
                            lcont=2;//update the line count
                        }
                        else//new XX.fa file, keep the chosen original data
                        {
                            QStringList origname;
                            int lcont2=0;//int lcont3;
                            QString strline,tmpline;
                            for(int a=0;a<hmmname.size();a++)
                                if(cb[a]->isChecked())
                                    origname<<cb[a]->text();
                            if(!tfile.open(QIODevice::ReadOnly|QIODevice::Text))
                                hsdlg.ErrorMsg(tfile.fileName());
                            if(origname.size()==0)
                            {
                                char cmds[1024];
                                QByteArray ba=tempfList->at(k).absoluteFilePath().toLatin1();
                                sprintf(cmds,"sed -i \"1,+%d d\" %s",lcont-3,ba.data());//delete the (1~lcont-2) lines
                                system(cmds);
                                lcont=2;//update the line count;
                            }
                            else if(hmmname.size()-origname.size())//there is some line left unchecked
                            {
                                int orignum[(hmmname.size()-origname.size())];//keep the line number of the chosen original name
                                int a=0;
                                while(!tfile.atEnd()&&lcont2<(lcont-2))//don't do anything to the last 2
                                {
                                    strline=QString(tfile.readLine());
                                    lcont2++;
                                    if(lcont2%2)//odd line, contains the name information
                                    {
                                        tmpline=strline.section("|",1,1);
                                        //int QStringList::indexOf(const QString & value, int from = 0) const
                                        if(origname.indexOf(tmpline)==-1)//no matches,delete this line
                                        {
                                            orignum[a++]=lcont2;
                                        }
                                    }
                                }
                                //tfile.close();
                                for(a=(hmmname.size()-origname.size()-1);a>=0;a--)//从后往前删除，从前往后前删除
                                {
                                    lcont2=orignum[a];
                                    char cmds[1024];
                                    QByteArray ba=tempfList->at(k).absoluteFilePath().toLatin1();
                                    sprintf(cmds,"sed -i \"%d,+1 d\" %s",lcont2,ba.data());
                                    system(cmds);
                                    lcont-=2;
                                }
                            }
                            tfile.close();
                        }

                    }
                    //change the species name of odd lines
                    ChangeName(tempfList->at(k).absoluteFilePath(),lcont);
                    /*content="sync";
                    cmd=qPrintable(content);
                    system(cmd);*/

                    //combine the files
                    if(!tfile.open(QIODevice::ReadOnly|QIODevice::Text))
                        hsdlg.ErrorMsg(tfile.fileName());
                    QByteArray catcontent=tfile.readAll();
                    tfile.close();

                    content=hresult;
                    content.append("/");
                    content.append(tempfList->at(k).fileName());
                    QFile catfile(content);
                    if(FileCont[tempcont]>-1)//new file, need to close() to create the file.
                    {
                        catfile.open(QIODevice::WriteOnly);
                        catfile.close();
                        FileCont[tempcont]=-1;
                    }

                    QTextStream catstream(&catfile);
                    if(catfile.open(QIODevice::WriteOnly|QIODevice::Text|QIODevice::Append))
                    {
                        //catfile.write(catcontent);
                        catstream<<catcontent;
                        catfile.close();
                    }
                    else
                        hsdlg.ErrorMsg(catfile.fileName());
                }
            }
            //break;
        }
        delete tempDir;delete tempfList;
    }

    if(!quit_bool)
    {
        hmsmsg.setText("The combine work is done, please check the results!");
        hmsmsg.exec();
        close();//close the window while the operation has finished
    }

}

/*int HamCmbDialog::FileName2Int(QString fn)
{
    //char* =>int
    QString tmp=fn.left(fn.length()-3);
    bool ok;
    return tmp.toInt(&ok,10);
}*/

void HamCmbDialog::ChangeName(QString filepath,int qline)
{
    QFile qfile(filepath);
    if(!qfile.open(QIODevice::ReadWrite|QIODevice::Text))
        hsdlg.ErrorMsg(qfile.fileName());
    QString linedata;QStringList linename;
    int linecont=0;
    while(!qfile.atEnd())
    {
        linedata=qfile.readLine();
        linecont++;
        if(linecont==qline-1)
        {
            linename<<linedata.section("|",2,2);
            break;
        }
        else if(linecont%2)
            linename<<linedata.section("|",1,1);
    }
    char cmds[512];
    QByteArray ba=filepath.toLatin1();
    QByteArray ba2;
    for(int i=1;i<qline;i+=2)
    {
        ba2=linename.at(i/2).toLatin1();
        sprintf(cmds,"sed -i '%dc >%s' %s",i,ba2.data(),ba.data());
        system(cmds);
    }
}

/*递归删除文件夹,即使里面有内容
**
**dir :文件夹的名字,绝对路径和相对路径均可
**
**返回值 :成功后返回true;否则返回false
*/
bool HamCmbDialog::removeDirWithContent(const QString &dirName)
{
    static QVector<QString> dirNames;
    QDir dir;
    QFileInfoList filst;
    QFileInfoList::iterator curFi;
    //初始化
    dirNames.clear();
    if(dir.exists()){
        dirNames<<dirName;
    }
    else{
        return true;
    }
    //遍历各级文件夹，并将这些文件夹中的文件删除
    for(int i=0;i<dirNames.size();++i)
    {
        dir.setPath(dirNames[i]);
        filst=dir.entryInfoList(QDir::Dirs|QDir::Files
                                |QDir::Readable|QDir::Writable
                                |QDir::Hidden|QDir::NoDotAndDotDot
                                ,QDir::Name);
        if(filst.size()>0){
            curFi=filst.begin();
            while(curFi!=filst.end()){
                 //遇到文件夹,则添加至文件夹列表dirs尾部
                if(curFi->isDir()){
                    dirNames.append(curFi->filePath());
                }else if(curFi->isFile()){
                    //遇到文件,则删除之
                    if(!dir.remove(curFi->fileName())){
                        return false;
                    }
                }
                curFi++;
            }//end of while
        }
    }
    //删除文件夹
    for(int i=dirNames.size()-1;i>=0;--i)
    {
        dir.setPath(dirNames[i]);

        /* QMessageBox msgBox;
        msgBox.setText(dir.absolutePath());
        msgBox.exec();*/

        if(!dir.rmdir(dir.absolutePath())){
            return false;
        }
    }
    return true;
}


void HamCmbDialog::on_keep_btn_clicked()
{
    if(click)//click to enable choose original data
    {
        for(int c=0;c<hmmname.size();c++)
        {
            cb[c]->setEnabled(true);
            cb[c]->setChecked(true);
        }
        click=false;
    }
    else{//click to cansel keeping original data
        for(int c=0;c<hmmname.size();c++)
        {
            cb[c]->setChecked(false);
            cb[c]->setEnabled(false);
        }
        click=true;
    }
}


