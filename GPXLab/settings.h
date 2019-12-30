#ifndef SETTINGS_H
#define SETTINGS_H

/**
 * @ingroup GPXLab
 * @{
 */

#include <QObject>
#include <QMainWindow>
#include <QString>
#include <QDate>

/**
 * @class Settings
 *
 * @brief Manage the settings
 */
class Settings : public QObject
{
    Q_OBJECT

public:

    /**
     * @brief Constructor
     */
    Settings(QMainWindow *parent);

    /**
     * @brief Saves the settings
     */
    void save();

    /**
     * @brief Saves the settings
     */
    void load();

    /**
     * @brief Restores the window layout
     */
    void restoreLayout();

    /**
     * @brief Sets a settings
     * @param key Key
     * @param value Value
     */
    void setValue(const QString &key, const QVariant &value);

    /**
     * @brief Gets a settings
     * @param key Key
     * @return Value
     */
    QVariant getValue(const QString &key);

    /**
     * @brief Adds a file to the recent file list
     * @param fileName File name
     */
    void addToRecentFile(const QString &fileName);

    /**
     * @brief Removes a file from the recent file list
     * @param fileName File name
     */
    void removeFromRecentFile(const QString &fileName);

    /**
     * @brief Clears the cache
     */
    void clearCache();

    /**
     * @brief Returns the default cache path
     * @return Default cache path
     */
    QString defaultCachePath();

    /**
     * @brief Returns the default tiles URL
     * @return Default tiles URL
     */
    QString defaultTilesURL();

    /**
     * @brief Maximal number of undo commands stored
     */
    const int undoLimit;

    /**
     * @brief Maximal number of recent files shown in the file menu
     */
    const int maxRecentFiles;

    /**
     * @brief List of the recent files shown in the file menu
     */
    QStringList recentFiles;

    /**
     * @brief Do persistent caching of map tiles
     */
    bool doPersistentCaching;

    /**
     * @brief Path to the tiles cache directory
     */
    QString cachePath;

    /**
     * @brief Loads the last opened file on startup
     */
    bool autoLoadLastFile;

    /**
     * @brief Checks for update at the start of the program
     */
    bool checkUpdate;

    /**
     * @brief URL to check for updates
     */
    QString checkUpdateUrl;

    /**
     * @brief Last date of the update check
     */
    QDate checkUpdateLastDate;

    /**
     * @brief Tiles URL
     */
    QString tilesURL;

signals:

    /**
     * @brief Signal when settings are loaded or saved
     */
    void settingsChanged(bool loaded);

private:

    QMainWindow *parent;
    QByteArray  defaultState;
    QByteArray  defaultGeometry;
};

/** @} GPXLab */

#endif // SETTINGS_H
