#ifndef FILESYSTENWATCHER_H
#define FILESYSTENWATCHER_H

#include <QObject>
#include <QMap>
#include <QDir>
#include <QDebug>
#include <QFileInfo>
#include <QProcess>
#include <QFileSystemWatcher>

class FileSystenWatcher : public QObject
{
    Q_OBJECT
public:

    ~FileSystenWatcher();

    static void addWatchPath(QString path);
signals:

public slots:
    void directoryUpdated(const QString &path);// 目录更新时调用，path是监控的路径
    void fileUpdated(const QString &path);// 文件被修改时调用，path是监控的路径

private:
    explicit FileSystenWatcher(QObject *parent = 0);

    static FileSystenWatcher *m_pInstance;// 单例
    QFileSystemWatcher *m_pSystemWatcher;// QFileSystemWatcher变量
    QMap<QString, QStringList> m_currentContentsMap;// 当前每个监控的内容目录列表

};

#endif // FILESYSTENWATCHER_H
