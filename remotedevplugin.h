#ifndef REMOTEDEV_H
#define REMOTEDEV_H

#pragma once

#include "remotedev_global.h"

#include <extensionsystem/iplugin.h>

#include "connection.h"

QT_BEGIN_NAMESPACE
class QAction;
class QActionGroup;
QT_END_NAMESPACE

namespace Core { class IEditor; class ActionContainer; }
namespace ProjectExplorer { class Project; }

namespace RemoteDev {
namespace Internal {

class OptionsPage;
class ConnectionHelper;
class ConnectionManager;
class Mapping;
class MappingsManager;
class DeviceManager;

class RemoteDevPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "RemoteDev.json")

public:
    RemoteDevPlugin();
    ~RemoteDevPlugin();

    bool initialize(const QStringList &arguments, QString *errorString) override;
    void extensionsInitialized() override;
    bool delayedInitialize() override;
    ShutdownFlag aboutToShutdown() override;

public slots:
    void uploadCurrentDocument();

    void uploadCurrentNode();

    void downloadCurrentNodeUsingActiveMapping();
    void downloadCurrentNode(const Mapping &mapping);

private slots:
    void triggerAction();

    // ConnectionManager
    void onConnectionError(Connection::Ptr connection);

    void onDownloadMenuAboutToShow();

private:
    using UploadMethod = RemoteJobId (Connection::*)(Utils::FileName,
                                                     Utils::FileName,
                                                     const Utils::FileName &,
                                                     OverwriteMode);

    using DownloadMethod = UploadMethod;

private:
    void createOptionsPage();
    void createProjectSettingsPage();
    void createFileMenus();

    void showDebug(const QString &string) const;


    void upload(const Utils::FileName &file,
                ProjectExplorer::Project &project,
                UploadMethod uploadMethod);

    void download(const Utils::FileName &file,
                  ProjectExplorer::Project &project,
                  DownloadMethod downloadMethod,
                  const Mapping &mapping);

    ConnectionHelper &getConnectionHelper(Connection &connection,
                                          const QString &mapping,
                                          const QString &target) const;

private:
    OptionsPage *m_optionsPage;

    QAction *m_uploadFile;
    QAction *m_downloadFile;
    QAction *m_uploadDirectory;

    ConnectionManager *m_connManager;
    MappingsManager   *m_mapManager;
    DeviceManager     *m_devManager;

    Core::ActionContainer *m_downloadMenu;
    QActionGroup *m_downloadActionGroup;
};

} // namespace Internal
} // namespace RemoteDev

#endif // REMOTEDEV_H

