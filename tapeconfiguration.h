#ifndef TAPECONFIGURATION_H
#define TAPECONFIGURATION_H

#include <QWidget>

namespace Ui {
class TapeConfiguration;
}

class TapeConfiguration : public QWidget
{
    Q_OBJECT

public:
    explicit TapeConfiguration(QWidget *parent = nullptr);
    ~TapeConfiguration();

signals:
    void saved();

private slots:
    void on_btClose_clicked();

    void on_btApply_clicked();

    void on_tbBrowse_clicked();

private:
    Ui::TapeConfiguration *ui;

    bool notify = true;
    bool native = true;
    QString path;
};

#endif // TAPECONFIGURATION_H
