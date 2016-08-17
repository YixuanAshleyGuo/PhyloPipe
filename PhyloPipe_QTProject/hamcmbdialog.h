#ifndef HAMCMBDIALOG_H
#define HAMCMBDIALOG_H

#include "hamstrdialog.h"
#include <QDialog>
#include <QVector>
#include <QLinkedList>
#include <QCheckBox>
#include <QWidget>
#include <QGridLayout>

namespace Ui {
class HamCmbDialog;
}

class HamCmbDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HamCmbDialog(QWidget *parent = 0);
    ~HamCmbDialog();
    QString cresult_dir;

private slots:
    void on_FileDirBtn_clicked();

    void on_ResultDirBtn_clicked();

    void on_CanselBtn_clicked();

    void on_StartCmbBtn_clicked();

    void on_keep_btn_clicked();

    void on_HmmDirBtn_clicked();

private:
    Ui::HamCmbDialog *c_ui;
    QLinkedList<QString> FileDir;
    HamstrDialog hsdlg;

    bool removeDirWithContent(const QString &dirName);
    int NSEQapp;
    QString hmcdir;

    //int nseq;
    QStringList hmmname;
    //int hmc_start,hmc_end;
    bool click;
    QCheckBox **cb;
    int cb_num;
    QGridLayout *orig_ly;
    //int FileName2Int(QString fn);
    void ChangeName(QString filepath, int qline);
};

#endif // HAMCMBDIALOG_H
