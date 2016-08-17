#ifndef CONCATENATEDIALOG_H
#define CONCATENATEDIALOG_H

#include <QDialog>

namespace Ui {
class ConcatenateDialog;
}

class ConcatenateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConcatenateDialog(QWidget *parent = 0);
    ~ConcatenateDialog();

private slots:
    void on_FileDirBtn_clicked();

    void on_CancelBtn_clicked();

    void on_ApplyBtn_clicked();

private:
    Ui::ConcatenateDialog *con_ui;
};

#endif // CONCATENATEDIALOG_H
