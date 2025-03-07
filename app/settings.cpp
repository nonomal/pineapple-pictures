#include "settings.h"

#include <QApplication>
#include <QStandardPaths>
#include <QDebug>
#include <QDir>
#include <QMetaEnum>

namespace QEnumHelper
{
    template <typename E>
    E fromString(const QString &text, const E defaultValue)
    {
        bool ok;
        E result = static_cast<E>(QMetaEnum::fromType<E>().keyToValue(text.toUtf8(), &ok));
        if (!ok) {
            return defaultValue;
        }
        return result;
    }

    template <typename E>
    QString toString(E value)
    {
        const int intValue = static_cast<int>(value);
        return QString::fromUtf8(QMetaEnum::fromType<E>().valueToKey(intValue));
    }
}

Settings *Settings::m_settings_instance = nullptr;

Settings *Settings::instance()
{
    if (!m_settings_instance) {
        m_settings_instance = new Settings;
    }

    return m_settings_instance;
}

bool Settings::stayOnTop()
{
    return m_qsettings->value("stay_on_top", true).toBool();
}

Settings::DoubleClickBehavior Settings::doubleClickBehavior() const
{
    QString result = m_qsettings->value("double_click_behavior", "Close").toString();

    return QEnumHelper::fromString<DoubleClickBehavior>(result, DoubleClickBehavior::Close);
}

Settings::MouseWheelBehavior Settings::mouseWheelBehavior() const
{
    QString result = m_qsettings->value("mouse_wheel_behavior", "Zoom").toString();

    return QEnumHelper::fromString<MouseWheelBehavior>(result, MouseWheelBehavior::Zoom);
}

Settings::WindowSizeBehavior Settings::initWindowSizeBehavior() const
{
    QString result = m_qsettings->value("init_window_size_behavior", "Auto").toString();

    return QEnumHelper::fromString<WindowSizeBehavior>(result, WindowSizeBehavior::Auto);
}

void Settings::setStayOnTop(bool on)
{
    m_qsettings->setValue("stay_on_top", on);
    m_qsettings->sync();
}

void Settings::setDoubleClickBehavior(DoubleClickBehavior dcb)
{
    m_qsettings->setValue("double_click_behavior", QEnumHelper::toString(dcb));
    m_qsettings->sync();
}

void Settings::setMouseWheelBehavior(MouseWheelBehavior mwb)
{
    m_qsettings->setValue("mouse_wheel_behavior", QEnumHelper::toString(mwb));
    m_qsettings->sync();
}

void Settings::setInitWindowSizeBehavior(WindowSizeBehavior wsb)
{
    m_qsettings->setValue("init_window_size_behavior", QEnumHelper::toString(wsb));
    m_qsettings->sync();
}

Settings::Settings()
    : QObject(qApp)
{
    QString configPath;

#ifdef FLAG_PORTABLE_MODE_SUPPORT
    QString portableConfigDirPath = QDir(QCoreApplication::applicationDirPath()).absoluteFilePath("data");
    QFileInfo portableConfigDirInfo(portableConfigDirPath);
    if (portableConfigDirInfo.exists() && portableConfigDirInfo.isDir() && portableConfigDirInfo.isWritable()) {
        // we can use it.
        configPath = portableConfigDirPath;
    }
#endif // FLAG_PORTABLE_MODE_SUPPORT

    // %LOCALAPPDATA% under Windows.
    if (configPath.isEmpty()) {
        configPath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
    }

    m_qsettings = new QSettings(QDir(configPath).absoluteFilePath("config.ini"), QSettings::IniFormat, this);
}

