#include "concatenatedialog.h"
#include "ui_concatenatedialog.h"
#include "aliscoredialog.h"
#include "ui_aliscoredialog.h"

#include <QMessageBox>
#include <QString>
#include <QFile>
#include <QFileDialog>
#include <QDir>
#include <QVBoxLayout>

ConcatenateDialog::ConcatenateDialog(QWidget *parent) :
    QDialog(parent),
    con_ui(new Ui::ConcatenateDialog)
{
    con_ui->setupUi(this);

    //put the widget into a layout and suit the window size
    QVBoxLayout* concatenateLayout=new QVBoxLayout;
    concatenateLayout->addWidget(con_ui->concatewidget);
    setLayout(concatenateLayout);

    AliscoreDialog alidlg;
    con_ui->FileDirCombox->setCurrentText(alidlg.aliresultdir);
    con_ui->FileDirCombox->addItem(alidlg.aliresultdir);
}

ConcatenateDialog::~ConcatenateDialog()
{
    delete con_ui;
}

void ConcatenateDialog::on_FileDirBtn_clicked()
{
    QString dir=QFileDialog::getExistingDirectory(this,
                                                  tr("Choosing the Input File Directory"),
                                                  con_ui->FileDirCombox->currentText(),
                                                  QFileDialog::ShowDirsOnly);
    if(!dir.isEmpty())
    {
        con_ui->FileDirCombox->addItem(dir);
        con_ui->FileDirCombox->setCurrentIndex(con_ui->FileDirCombox->currentIndex()+1);
    }
}

void ConcatenateDialog::on_CancelBtn_clicked()
{
    close();
}

void ConcatenateDialog::on_ApplyBtn_clicked()
{
    QMessageBox conmsg;
    QString conpath,conpath2,content;
    const char *cmd;
    conpath=QDir::currentPath();
    //conpath.remove(conpath.lastIndexOf('/'),conpath.size()-conpath.lastIndexOf('/'));
    conpath.append("/SystemFiles");
    conpath2=conpath;
    conpath2.append("/ruby_methods.rb");
    //copy the two files to the file directory
    content="cp ";
    content.append(conpath2);
    content.append(" ");
    content.append(con_ui->FileDirCombox->currentText());
    cmd=qPrintable(content);
    system(cmd);

    conpath2=conpath;
    conpath2.append("/concatenator.rb");
    content="cp ";
    content.append(conpath2);
    content.append(" ");
    content.append(con_ui->FileDirCombox->currentText());
    cmd=qPrintable(content);
    system(cmd);

    content="cd ";
    content.append(con_ui->FileDirCombox->currentText());
    content.append(";ruby concatenator.rb ./");
    //content.append(sco_ui->ResDirCombox->currentText());
    content.append(" ");
    content.append(con_ui->Matrix_lineEdit->text());
    content.append(".fa ");
    content.append(con_ui->Charset_lineEdit->text());
    content.append(".txt");
    cmd=qPrintable(content);system(cmd);

    conmsg.setText("The files have been changed, please check the result!");
    conmsg.exec();

    close();
}
