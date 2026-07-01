#include "mainwindow.h"
#include "filedialog.h"
#include <QDir>
#include <QFileInfo>
#include <QProcess>
#include <QMessageBox>
#include <QDirIterator>
#include <QStatusBar>
#include <QFile>
#include <QTextStream>
#include <QMenu>
#include <QAction>
#include <QListView>
#include <QTreeView>
#include <algorithm>
#include <QRandomGenerator>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), folderWatcher(new QFileSystemWatcher(this))
{
    setWindowTitle("Walper");
    resize(500, 450);

    QWidget *central = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(central);

    listWidget = new QListWidget(this);
    listWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(listWidget, &QListWidget::customContextMenuRequested,
            this, &MainWindow::onCustomContextMenu);
    layout->addWidget(listWidget);

    btnAddFolder = new QPushButton(tr("add_folder"), this);
    layout->addWidget(btnAddFolder);

    btnStopLive = new QPushButton(tr("stop_live_wallpaper"), this);
    layout->addWidget(btnStopLive);

    btnApply = new QPushButton(tr("apply_wallpaper"), this);
    layout->addWidget(btnApply);

    setCentralWidget(central);

    connect(btnAddFolder, &QPushButton::clicked, this, &MainWindow::onAddFolder);
    connect(btnStopLive, &QPushButton::clicked, this, &MainWindow::onStopLiveWallpaper);
    connect(btnApply, &QPushButton::clicked, this, &MainWindow::onApplyWallpaper);

    // Muat history folder terakhir
    QString lastFolder = loadHistory();
    if (!lastFolder.isEmpty() && QDir(lastFolder).exists()) {
        scanFolder(lastFolder);
        setupWatcher(lastFolder);
    }
}

MainWindow::~MainWindow() {}

void MainWindow::onAddFolder()
{
    FolderSelectDialog dialog(this, tr("choose_folder"), QDir::homePath());

    QListView *listView = dialog.findChild<QListView*>();
    QTreeView *treeView = dialog.findChild<QTreeView*>();
    QAbstractItemView *view = qobject_cast<QAbstractItemView*>(listView ? (QObject*)listView : (QObject*)treeView);

    if (view) {
        connect(view, &QAbstractItemView::doubleClicked, [&dialog](const QModelIndex &index) {
            if (index.isValid()) {
                QString path = dialog.directory().absolutePath() + "/" + index.data().toString();
                QFileInfo info(path);
                if (info.isDir()) {
                    dialog.setDirectory(info.absoluteFilePath());
                    dialog.selectFolder();
                }
            }
        });
    }

    if (dialog.exec() != QDialog::Accepted)
        return;

    QString dirPath = dialog.selectedFiles().first();
    if (dirPath.isEmpty())
        return;

    saveHistory(dirPath);

    wallpapers.clear();
    listWidget->clear();
    currentFolder = dirPath;
    scanFolder(currentFolder);
    setupWatcher(dirPath);
}

void MainWindow::scanFolder(const QString &path)
{
    QStringList filters;
    filters << "*.jpg" << "*.jpeg" << "*.png" << "*.webp" << "*.bmp" << "*.tiff" << "*.tif"
            << "*.mp4" << "*.mkv" << "*.webm" << "*.avi" << "*.mov"
            << "*.gif";

    QDirIterator it(path, filters, QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        it.next();
        QFileInfo info = it.fileInfo();
        QString filePath = info.absoluteFilePath();
        QString fileName = info.fileName();

        bool exists = false;
        for (const auto &wp : wallpapers) {
            if (wp.second == filePath) {
                exists = true;
                break;
            }
        }
        if (!exists) {
            wallpapers.append(qMakePair(fileName, filePath));
        }
    }

    std::sort(wallpapers.begin(), wallpapers.end(),
              [](const QPair<QString, QString> &a, const QPair<QString, QString> &b) {
                  return a.first.toLower() < b.first.toLower();
              });

    for (const auto &wp : wallpapers) {
        QListWidgetItem *item = new QListWidgetItem(wp.first, listWidget);
        item->setData(Qt::UserRole, wp.second);
    }

    statusBar()->showMessage(tr("folder_info").arg(path).arg(wallpapers.size()), 3000);
}

void MainWindow::onApplyWallpaper()
{
    QListWidgetItem *current = listWidget->currentItem();

    // Jika tidak ada yang dipilih, pilih acak dari daftar
    if (!current) {
        if (wallpapers.isEmpty()) {
            QMessageBox::warning(this, tr("warning"), tr("no_wallpapers"));
            return;
        }
        int randomIndex = QRandomGenerator::global()->bounded(wallpapers.size());
        QString selectedPath = wallpapers.at(randomIndex).second;

        // Setel item yang sesuai di listWidget sebagai terpilih
        for (int i = 0; i < listWidget->count(); ++i) {
            QListWidgetItem *item = listWidget->item(i);
            if (item->data(Qt::UserRole).toString() == selectedPath) {
                listWidget->setCurrentItem(item);
                current = item;
                break;
            }
        }
        statusBar()->showMessage(tr("randomly_selected").arg(current->text()), 2000);
    }

    // --- Sisa kode apply seperti biasa (tidak berubah) ---
    QString wallpaperPath = current->data(Qt::UserRole).toString();
    QFileInfo fileInfo(wallpaperPath);
    QString suffix = fileInfo.suffix().toLower();
    QStringList staticExtensions = {"jpg", "jpeg", "png", "webp", "bmp", "tiff", "tif"};

    QProcess::execute("bash", {"-c", "killall -q xwinwrap mpv 2>/dev/null; sleep 0.2"});

    if (staticExtensions.contains(suffix)) {
        QStringList fehArgs;
        fehArgs << "--bg-fill" << wallpaperPath;
        int exitCode = QProcess::execute("feh", fehArgs);
        if (exitCode != 0) {
            QMessageBox::critical(this, tr("error"), tr("feh_failed"));
        } else {
            statusBar()->showMessage(tr("static_applied").arg(wallpaperPath), 3000);
        }
    } else {
        QString scriptPath = QDir::homePath() + "/.config/walper/hy.sh";
        if (!QFile::exists(scriptPath)) {
            QMessageBox::critical(this, tr("error"), tr("script_not_found").arg(scriptPath));
            return;
        }
        QStringList args;
        args << scriptPath << wallpaperPath;
        bool started = QProcess::startDetached("bash", args);
        if (!started) {
            QMessageBox::critical(this, tr("error"), tr("script_failed"));
        } else {
            statusBar()->showMessage(tr("live_applied").arg(wallpaperPath), 3000);
        }
    }
}

void MainWindow::onStopLiveWallpaper()
{
    QProcess::execute("bash", {"-c", "killall -q xwinwrap mpv 2>/dev/null"});
    statusBar()->showMessage(tr("live_stopped"), 2000);
}

void MainWindow::onFolderChanged(const QString &path)
{
    if (path != currentFolder) return;
    wallpapers.clear();
    listWidget->clear();
    scanFolder(currentFolder);
}

void MainWindow::onCustomContextMenu(const QPoint &pos)
{
    QListWidgetItem *item = listWidget->itemAt(pos);
    if (!item) return;

    QMenu menu(this);
    QAction *applyAction = menu.addAction(tr("apply"));
    QAction *deleteAction = menu.addAction(tr("delete"));
    QAction *chosen = menu.exec(listWidget->viewport()->mapToGlobal(pos));
    if (chosen == applyAction) {
        listWidget->setCurrentItem(item);
        onApplyWallpaper();
    } else if (chosen == deleteAction) {
        listWidget->setCurrentItem(item);
        deleteSelectedFile();
    }
}

void MainWindow::deleteSelectedFile()
{
    QListWidgetItem *current = listWidget->currentItem();
    if (!current) return;
    QString filePath = current->data(Qt::UserRole).toString();
    QString fileName = current->text();
    int ret = QMessageBox::question(this, tr("delete_file"),
                tr("confirm_delete").arg(fileName),
                QMessageBox::Yes | QMessageBox::No);
    if (ret != QMessageBox::Yes) return;

    QFile file(filePath);
    if (file.remove()) {
        statusBar()->showMessage(tr("file_deleted").arg(fileName), 3000);
        int idx = wallpapers.indexOf(qMakePair(fileName, filePath));
        if (idx >= 0) wallpapers.removeAt(idx);
        delete current;
    } else {
        QMessageBox::critical(this, tr("error"),
            tr("delete_failed").arg(filePath));
    }
}

void MainWindow::setupWatcher(const QString &path)
{
    currentFolder = path;
    folderWatcher->removePaths(folderWatcher->directories());
    if (!path.isEmpty()) {
        folderWatcher->addPath(path);
        connect(folderWatcher, &QFileSystemWatcher::directoryChanged,
                this, &MainWindow::onFolderChanged, Qt::UniqueConnection);
    }
}

QString MainWindow::loadHistory()
{
    QString historyFile = QDir::homePath() + "/.config/walper/history";
    QFile file(historyFile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return QString();
    QTextStream in(&file);
    QString folder = in.readLine().trimmed();
    file.close();
    return folder;
}

void MainWindow::saveHistory(const QString &folder)
{
    QString historyFile = QDir::homePath() + "/.config/walper/history";
    QFile file(historyFile);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("error"),
            tr("cannot_save_history").arg(historyFile));
        return;
    }
    QTextStream out(&file);
    out << folder << "\n";
    file.close();
}
