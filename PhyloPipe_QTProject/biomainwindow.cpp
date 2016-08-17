#include "biomainwindow.h"
#include "ui_biomainwindow.h"

#include <qfiledialog.h>
#include <qfile.h>
#include <qmessagebox.h>
#include <qwidget.h>
#include <qtextstream.h>
#include <qstringlist.h>
#include <qsettings.h>
#include <QAction>

BIOMainWindow::BIOMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BIOMainWindow)
{
    ui->setupUi(this);
    QCoreApplication::setOrganizationName("NBJL LAB");
    //QCoreApplication::setOrganizationDomain("mysoft.com");
    QCoreApplication::setApplicationName("PhyloPipeApp");
    settingsdlg=new SettingsDialog;
}

BIOMainWindow::~BIOMainWindow()
{
    delete ui;
}

bool BIOMainWindow::maybeSave()
{
    if(ui->plainTextEdit->document()->isModified())
    {
        QMessageBox::StandardButton ret;
        ret=QMessageBox::warning(this,tr("IMPORTANT!"),
                                 tr("The document has been modified.\n"
                                    "Do you want to save your changes?"),
                                 QMessageBox::Save|QMessageBox::Discard|QMessageBox::Cancel);
        if(ret==QMessageBox::Save)
            return on_actionSave_triggered();
        if(ret==QMessageBox::Cancel)
            return false;
    }
    return true;
}

void BIOMainWindow::openFile()
{
    QString fileName=QFileDialog::getOpenFileName(this);
    if(!fileName.isEmpty())
    {
       loadFile(fileName);
    }
}

void BIOMainWindow::loadFile(const QString &fileName)
{
    QFile file(fileName);
        if (!file.open(QFile::ReadOnly | QFile::Text)) {
            QMessageBox::warning(this, tr("IMPORTANT!"),
                                 tr("Cannot read file %1:\n%2.")
                                 .arg(fileName)
                                 .arg(file.errorString()));
            return;
        }

        QTextStream in(&file);
    #ifndef QT_NO_CURSOR
        QApplication::setOverrideCursor(Qt::WaitCursor);
    #endif
        ui->plainTextEdit->setPlainText(in.readAll());
    #ifndef QT_NO_CURSOR
        QApplication::restoreOverrideCursor();
    #endif
       setCurrentFile(fileName);
       ui->statusBar->showMessage(tr("File loaded"), 20000);

       //ui->OpenFileBtn->close();

}

bool BIOMainWindow::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QFile::WriteOnly|QFile::Text))
    {
        QMessageBox::warning(this,tr("IMPORTANT!"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }
    QTextStream out(&file);
    #ifndef QT_NO_CURSOR
        QApplication::setOverrideCursor(Qt::WaitCursor);
    #endif
        out << ui->plainTextEdit->toPlainText();
    #ifndef QT_NO_CURSOR
        QApplication::restoreOverrideCursor();
    #endif

        setCurrentFile(fileName);
        ui->statusBar->showMessage(tr("File saved"), 20000);
        return true;
}

void BIOMainWindow::setCurrentFile(const QString &fileName)
{
    curFile=fileName;
    ui->plainTextEdit->document()->setModified(false);
    setWindowModified(false);
    QString shownName=curFile;
    if(curFile.isEmpty())
        shownName="untitled.txt";
    setWindowFilePath(shownName);
}

void BIOMainWindow::on_actionQuit_triggered()
{
    if(maybeSave())
    {

    }
    close();
}

void BIOMainWindow::on_actionOpen_triggered()
{
    if(maybeSave())
    {
        QString fileName=QFileDialog::getOpenFileName(this);
        if(!fileName.isEmpty())
        {
           loadFile(fileName);
        }
    }

    //ui->OpenFileBtn->close();
}

void BIOMainWindow::on_actionNew_triggered()
{
    if(maybeSave())
    {
        ui->plainTextEdit->clear();
        setCurrentFile("");
    }

    //ui->OpenFileBtn->close();
}

bool BIOMainWindow::on_actionSave_triggered()
{
    if(curFile.isEmpty())
    {
        return on_actionSave_as_triggered();
    }
    else
        return saveFile(curFile);
}

bool BIOMainWindow::on_actionSave_as_triggered()
{
    QFileDialog dialog(this);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.exec();
    QStringList files=dialog.selectedFiles();
    if(files.isEmpty())
        return false;
    return saveFile(files.at(0));
}

void BIOMainWindow::on_actionRun_triggered()
{
    if(settingsdlg->hamstrpath=="")
        QMessageBox::information(NULL,tr("HaMStR WARNING!"),tr("Please go to the settings dialog to set the HaMStR software path!"));
    else{
    hamstrdlg=new HamstrDialog;
    hamstrdlg->show();
    }
}

void BIOMainWindow::on_actionResults_triggered()
{
    hamcmbdlg=new HamCmbDialog;    
    hamcmbdlg->setWindowModality(Qt::ApplicationModal);
    hamcmbdlg->show();
}

void BIOMainWindow::on_actionSettings_triggered()
{
    //settingsdlg=new SettingsDialog;
    settingsdlg->show();
}

void BIOMainWindow::on_actionMafft_triggered()
{
    if(settingsdlg->mafftpath=="")
        QMessageBox::information(NULL,tr("MAFFT WARNING!"),tr("Please go to the settings dialog to set the Mafft software path!"));
    else{
    mafftdlg=new MafftDialog;
    mafftdlg->show();
    }
}

void BIOMainWindow::on_actionRun_2_triggered()
{
    if(settingsdlg->raxmlpath=="")
        QMessageBox::information(NULL,tr("raxmlGUI WARNING!"),tr("Please go to the settings dialog to set the raxmlGUI software path!"));
    else{
    QString content,content_tmp;
    const char *cmd;
    content="cd ";
    //../home/GUOYIXUAN/BioinformSW/raxmlGUI1.3.1/raxmlGUI.py
    content_tmp=settingsdlg->raxmlpath;
    content.append(content_tmp.remove(content_tmp.lastIndexOf('/'),content_tmp.size()-content_tmp.lastIndexOf('/')));
    content.append(";");
    content.append(settingsdlg->raxmlgui);
    content.append("&");

    cmd=qPrintable(content);
    system(cmd);
    }
}

void BIOMainWindow::on_actionMuscle_triggered()
{
    if(settingsdlg->musclepath=="")
        QMessageBox::information(NULL,tr("MUSCLE WARNING!"),tr("Please go to the settings dialog to set the Muscle software path!"));
    else{
    muscledlg=new MuscleDialog;
    muscledlg->show();
    }
}


void BIOMainWindow::on_actionMatrixMasking_triggered()
{
    if(settingsdlg->aliscorepath=="")
        QMessageBox::information(NULL,tr("MatrixMasking WARNING!"),tr("Please go to the settings dialog to set the Aliscore software path!"));
    else{
        aliscoredlg=new AliscoreDialog;
        aliscoredlg->show();
    }
}

void BIOMainWindow::on_actionConcatenate_triggered()
{
    concatedlg=new ConcatenateDialog;
    concatedlg->show();
}
