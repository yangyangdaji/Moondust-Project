#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QPixmap>
#include <QAbstractListModel>
#include <QList>
#include <QPoint>
#include <QString>
#include <QStringList>

QT_BEGIN_NAMESPACE
class QMimeData;
QT_END_NAMESPACE

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QMdiArea *parent = 0);
    ~MainWindow();
    
private slots:
    void on_OpenFile_activated();

    void on_Exit_activated();

    void on_actionAbout_activated();

    void on_LevelToolBox_visibilityChanged(bool visible);

    void on_actionLVLToolBox_activated();

    void on_actionWLDToolBox_activated();

    void on_WorldToolBox_visibilityChanged(bool visible);

    void on_actionLevelProp_activated();

    void on_pushButton_4_clicked();

    void on_actionSection_Settings_activated();

    void on_LevelSectionSettings_visibilityChanged(bool visible);

    bool ReadLevelFile(QFile &inf);

    bool ReadNpcTXTFile(QFile &inf);

    bool ReadWorldFile(QFile &inf);

private:
    Ui::MainWindow *ui;
};

void BadFileMsg(MainWindow *window, QString fileName_DATA, int str_count, QString line);

////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////

#endif // MAINWINDOW_H
