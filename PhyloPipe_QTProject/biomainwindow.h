#ifndef BIOMAINWINDOW_H
#define BIOMAINWINDOW_H

#include <QMainWindow>
#include "hamstrdialog.h"
#include "hamcmbdialog.h"
#include "mafftdialog.h"
#include "muscledialog.h"
#include "settingsdialog.h"
#include "aliscoredialog.h"
#include "concatenatedialog.h"
#include <QSettings>
#include <QDialog>
#include <QMessageBox>

namespace Ui {
class BIOMainWindow;
}

class BIOMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit BIOMainWindow(QWidget *parent = 0);
    ~BIOMainWindow();

private slots:
    //void on_OpenFileBtn_clicked();

    void on_actionQuit_triggered();

    void on_actionOpen_triggered();

    void on_actionNew_triggered();

    bool on_actionSave_triggered();

    bool on_actionSave_as_triggered();

    void on_actionRun_triggered();

    void on_actionResults_triggered();

    void on_actionSettings_triggered();

    void on_actionMafft_triggered();

    void on_actionRun_2_triggered();

    void on_actionMuscle_triggered();

    void on_actionMatrixMasking_triggered();

    void on_actionConcatenate_triggered();

private:
    Ui::BIOMainWindow *ui;
    HamstrDialog *hamstrdlg;
    HamCmbDialog *hamcmbdlg;
    MafftDialog *mafftdlg;
    MuscleDialog *muscledlg;
    SettingsDialog *settingsdlg;
    AliscoreDialog *aliscoredlg;
    ConcatenateDialog *concatedlg;
    QSettings settings;

    QString curFile;
    QMessageBox mainmsg;

    bool maybeSave();
    void openFile();
    void loadFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);

};

#endif // BIOMAINWINDOW_H
