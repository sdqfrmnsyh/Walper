#ifndef FILEDIALOG_H
#define FILEDIALOG_H

#include <QFileDialog>

class FolderSelectDialog : public QFileDialog
{
    Q_OBJECT
public:
    explicit FolderSelectDialog(QWidget *parent = nullptr, const QString &caption = QString(),
                                const QString &directory = QString())
        : QFileDialog(parent, caption, directory)
    {
        setFileMode(QFileDialog::Directory);
        setOption(QFileDialog::ShowDirsOnly, true);
        setOption(QFileDialog::DontUseNativeDialog, true);
    }

public slots:
    void selectFolder() {
        accept();
    }
};

#endif // FILEDIALOG_H
