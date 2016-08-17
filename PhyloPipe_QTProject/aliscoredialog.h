#ifndef ALISCOREDIALOG_H
#define ALISCOREDIALOG_H

#include "hamstrdialog.h"
#include <QDialog>
#include <QString>
#include <QSettings>

namespace Ui {
class AliscoreDialog;
}

class AliscoreDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AliscoreDialog(QWidget *parent = 0);
    ~AliscoreDialog();
    QString aliresultdir;

private slots:
    void on_FileDirBtn_clicked();

    void on_ApplyBtn_clicked();

    void on_CansleBtn_clicked();

    void on_ResDirBtn_clicked();

    void on_r_checkBox_clicked();

private:
    Ui::AliscoreDialog *sco_ui;
    QSettings settings;
    HamstrDialog hsdlg;
};

#endif // ALISCOREDIALOG_H
