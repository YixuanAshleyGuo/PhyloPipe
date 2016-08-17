#ifndef HAMSTRDIALOG_H
#define HAMSTRDIALOG_H

#include <QDialog>
#include <QProcess>
#include <QComboBox>
#include <QSettings>
#include <QString>
#include <QStringList>
#include <QCheckBox>
#include <QGridLayout>

namespace Ui {
class HamstrDialog;
}

class HamstrDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HamstrDialog(QWidget *parent = 0);

    ~HamstrDialog();
    QString result_dir;
    QString hmsdir;
    //int hmm_start,hmm_end;
    void ErrorMsg(QString filename);

private slots:
    void on_ChoSeqBtn_clicked();

    void on_canselBtn_clicked();

    void on_BrwOutBtn_clicked();

    void on_applyBtn_clicked();

    void on_hmmRBtn_m_clicked();

    void on_hmmRBtn_d_clicked();

    void on_eblastRBtn_m_clicked();

    void on_eblastRBtn_d_clicked();

    void on_ehmmerRBtn_m_clicked();

    void on_ehmmerRBtn_d_clicked();

    //int FileName2Int(QString fn);

    void on_hms_comboBox_currentTextChanged();

private:
    Ui::HamstrDialog *h_ui;
    void ChosPath(int index);
    void FindRefspec();
    void HmmcomboxEnabled();

    QComboBox *tempCB;
    int NSEQ;
    QString output;
    QSettings settings;
    QStringList hmmset;
    QString hmmtemp,hmmtemp2;
    //for the refspec checkboxes
    QStringList refspecname;
    QCheckBox **cb;
    int cb_num;
    QGridLayout *refspec_ly;
};

#endif // HAMSTRDIALOG_H
