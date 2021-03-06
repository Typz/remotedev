#ifndef CONNECTION_H
#define CONNECTION_H

#pragma once

#include "remotedev_global.h"

#include <QObject>

#include <ssh/sftpdefs.h>

namespace Utils {
    class FileName;
}

namespace RemoteDev {

using RemoteJobId = QSsh::SftpJobId;

enum OverwriteMode {
    OverwriteExisting   = QSsh::SftpOverwriteExisting,
    AppendToExisting    = QSsh::SftpAppendToExisting,
    SkipExisting        = QSsh::SftpSkipExisting
};

/**
 * @brief RemoteInvalidJob - value for indicating an invalid job ID
 */
extern REMOTEDEVSHARED_EXPORT const RemoteJobId REMOTE_INVALID_JOB;

/**
 * @brief The RemoteConnection class
 * This class is intended to be a polymorphic wrapper around
 * different connection types: SSH, FTP etc...
 */
class Connection : public QObject
{
    Q_OBJECT
public:
    typedef QSharedPointer<Connection> Ptr;

    virtual ~Connection();

    /**
     * @brief alias - get a short recognizable name for this connection
     * @return the short name
     */
    const QString &alias() const;

    /**
     * @brief uploadFile - upload a file using this connection
     * @param local     Local base directory
     * @param remote    Remote base directory
     * @param file      Relative file path (to both local and remote)
     * @param mode      OverwriteMode
     * @return Reference job ID
     */
    virtual RemoteJobId uploadFile(Utils::FileName local,
                                   Utils::FileName remote,
                                   const Utils::FileName &file,
                                   OverwriteMode mode) = 0;

    virtual RemoteJobId uploadDirectory(Utils::FileName local,
                                        Utils::FileName remote,
                                        const Utils::FileName &directory,
                                        OverwriteMode mode) = 0;

    virtual RemoteJobId downloadFile(Utils::FileName localDir,
                                     Utils::FileName remoteDir,
                                     const Utils::FileName &file,
                                     OverwriteMode mode) = 0;

    /**
     * @brief errorString - get a description for the last error
     * @return the description
     */
    virtual QString errorString() const = 0;

signals:
    void connected();

    /**
     * @brief error - is emitted when connection error occurs
     * TODO this signal should be emitted together with error information
     */
    void error(const QString &error = QString());
    void uploadError(RemoteJobId job, const QString &reason);
    void uploadFinished(RemoteJobId job);

    void disconnected();

protected:
    explicit Connection(const QString &alias, QObject *parent = nullptr);

    RemoteJobId createJobId() const;

protected:
    mutable RemoteJobId m_jobIdCounter = REMOTE_INVALID_JOB;
    const QString m_alias;
};

} // namespace RemoteDev

#endif // CONNECTION_H
