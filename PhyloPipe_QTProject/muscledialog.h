#ifndef MUSCLEDIALOG_H
#define MUSCLEDIALOG_H

#include "hamstrdialog.h"
#include <QDialog>
#include <QSettings>

namespace Ui {
class MuscleDialog;
}

class MuscleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MuscleDialog(QWidget *parent = 0);
    ~MuscleDialog();
    QString musresult_dir;

private slots:
    void on_FileDirBtn_clicked();

    void on_ResultDirBtn_clicked();

    void on_Muscle_pushButton_clicked();

    void on_Cansle_pushButton_clicked();

private:
    Ui::MuscleDialog *mus_ui;
    QSettings settings;
    HamstrDialog hsdlg;
};

#endif // MUSCLEDIALOG_H
