#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QString>
#include <QSettings>
#include <qcombobox.h>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();
    QString raxmlpath,hamstrpath,mafftpath,musclepath,aliscorepath,raxmlgui;

private slots:
    void on_ham_btn_clicked();

    void on_mus_btn_clicked();

    void on_sco_btn_clicked();

    void on_rax_btn_clicked();

    void on_aply_btn_clicked();

    void on_csl_btn_clicked();

    void on_maf_btn_clicked();

    //void on_cate_btn_clicked();

private:
    Ui::SettingsDialog *s_ui;
    QComboBox *tempCB;
    QSettings settings;

    void ChosSWPath(int index);
};

#endif // SETTINGSDIALOG_H
