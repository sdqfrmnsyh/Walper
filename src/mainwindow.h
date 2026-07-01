#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QVector>
#include <QPair>
#include <QString>
#include <QFileSystemWatcher>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onAddFolder();
    void onApplyWallpaper();
    void onStopLiveWallpaper();
    void onFolderChanged(const QString &path);
    void onCustomContextMenu(const QPoint &pos);
    void deleteSelectedFile();
    
private:
    void scanFolder(const QString &path);
    QString loadHistory();
    void saveHistory(const QString &folder);
    void setupWatcher(const QString &path);   // <-- tambahkan ini

    QListWidget *listWidget;
    QPushButton *btnAddFolder;
    QPushButton *btnStopLive;
    QPushButton *btnApply;
    QVector<QPair<QString, QString>> wallpapers;
    QFileSystemWatcher *folderWatcher;
    QString currentFolder;
};

#endif // MAINWINDOW_H
