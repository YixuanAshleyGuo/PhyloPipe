#ifndef MAFFTDIALOG_H
#define MAFFTDIALOG_H

#include "hamstrdialog.h"
#include <QDialog>
#include <QSettings>

namespace Ui {
class MafftDialog;
}

class MafftDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MafftDialog(QWidget *parent = 0);
    ~MafftDialog();
    QString mafresult_dir;

private slots:
    void on_CanselBtn_clicked();

    void on_FileDirBtn_clicked();

    void on_ResultDirBtn_clicked();

    void on_MafftBtn_clicked();

    void on_AccuracyRadioBtn_clicked();

private:
    Ui::MafftDialog *maf_ui;

    QSettings settings;
    HamstrDialog hsdlg;
};

#endif // MAFFTDIALOG_H
