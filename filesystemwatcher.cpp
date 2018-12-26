#include "filesystenwatcher.h"


FileSystenWatcher* FileSystenWatcher::m_pInstance = NULL;

FileSystenWatcher::FileSystenWatcher(QObject *parent) :
    QObject(parent)
{
}

FileSystenWatcher::~FileSystenWatcher()
{

}

//监控文件或目录
void FileSystenWatcher::addWatchPath(QString path)
{
    qDebug()<< QString("Add to watch: %1").arg(path);

    if (m_pInstance == NULL)
    {
        m_pInstance = new FileSystenWatcher();
        m_pInstance->m_pSystemWatcher = new QFileSystemWatcher();

        // 连接QFileSystemWatcher的directoryChanged和fileChanged信号到相应的槽
        connect(m_pInstance->m_pSystemWatcher, SIGNAL(directoryChanged(QString)), m_pInstance, SLOT(directoryUpdated(QString)));
        connect(m_pInstance->m_pSystemWatcher, SIGNAL(fileChanged(QString)), m_pInstance, SLOT(fileUpdated(QString)));
    }

    // 添加监控路径
    m_pInstance->m_pSystemWatcher->addPath(path);

    // 如果添加路径是一个目录，保存当前内容列表
    QFileInfo file(path);
    if (file.isDir())
    {
        const QDir dirw(path);
        m_pInstance->m_currentContentsMap[path] = dirw.entryList(QDir::NoDotAndDotDot|QDir::AllDirs|QDir::Files,QDir::DirsFirst);
    }
}

// 只要任何监控的目录更新（添加、删除、重命名），就会调用。
void FileSystenWatcher::directoryUpdated(const QString &path)
{
    qDebug() << QString("Directory updated: %1").arg(path);


    // 比较最新的内容和保存的内容找出区别(变化)
    QStringList currEntryList = m_currentContentsMap[path];
    const QDir dir(path);

    QStringList newEntryList = dir.entryList(QDir::NoDotAndDotDot|QDir::AllDirs|QDir::Files,QDir::DirsFirst);

    QSet<QString> newDirSet = QSet<QString>::fromList(newEntryList);
    QSet<QString> currentDirSet = QSet<QString>::fromList(currEntryList);

    // 添加了文件
    QSet<QString> newFiles = newDirSet - currentDirSet;
    QStringList newFile = newFiles.toList();

    //文件已被移除
    QSet<QString>deletedFiles = currentDirSet - newDirSet;
    QStringList deleteFile = deletedFiles.toList();

    //更新当前设置
    m_currentContentsMap[path] = newEntryList;

    if (!newFile.isEmpty() && !deleteFile.isEmpty())
    {
        //文件/目录重命名
        if ((newFile.count() == 1) && (deleteFile.count() == 1))
        {
            qDebug() << QString("File Rename from %1 to %2").arg(deleteFile.first()).arg(newFile.first());
        }
    }
    else
    {
        //添加新文件/目录至Dir
        if (!newFile.isEmpty())
        {
            qDebug() <<"New Files/Dirs added: "<<newFile;

            foreach (QString file, newFile)
            {
             //处理操作每个新文件...
//            qDebug()<<qApp->applicationDirPath();
            QString programAddress = "C:/Program Files (x86)/2345Explorer/2345Explorer.exe";
//            qDebug()<<programAddress;
            QStringList arguments;
//            QString fileName = "/你最棒.jpg";
//            arguments << "file:///"+qApp->applicationDirPath()+fileName;
            arguments << "file:///"+path+"/"+file;
            QProcess *chromeProcess = new QProcess(this);
            chromeProcess->start(programAddress, arguments);
            }
        }
        //从Dir中删除文件/目录
        if (!deleteFile.isEmpty())
        {
            qDebug() << "Files/Dirs deleted:"<<deleteFile;

            foreach (QString file, deleteFile)
            {
            //处理操作每个被删除的文件...
            }
        }
    }
}

//文件修改时调用
void FileSystenWatcher::fileUpdated(const QString &path)
{
    QFileInfo file(path);
    QString strPath = file.absolutePath();
    QString strName = file.fileName();

    qDebug()<<QString("The file %1 at path %2 is updated").arg(strName).arg(strPath);
}
