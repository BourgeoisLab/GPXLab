#ifndef QTABLEWIDGETPOINTS_H
#define QTABLEWIDGETPOINTS_H

#include <QTableView>
#include <QAbstractTableModel>
#include <QUndoStack>
#include "gpx_wrapper.h"

/**
 * @addtogroup Widgets Widgets
 * @brief Widgets related functions
 * @{
 */

/**
 * @class PointTableModel
 *
 * @brief Model for point table
 *
 * Implements the QAbstractTableModel abstract class.
 *
 * The data for the table is taken directly from the GPX_model.
 *
 * @see QAbstractTableModel
 */
class PointTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:

    /**
     * @brief Constructor
     * @param parent Parent
     */
    PointTableModel(QObject *parent = 0);

    /**
     * @brief Returns the number of rows ( = number of track points)
     * @param parent Parent
     * @return Number of rows
     */
    int rowCount(const QModelIndex &parent = QModelIndex()) const ;

    /**
     * @brief Returns the number of columns
     * @param parent Parent
     * @return Number of columns
     */
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    /**
     * @brief Returns the data stored under the given role for the item referred to by the index
     * @param index Index
     * @param role Role
     * @return Data
     */
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    /**
     * @brief Sets the role data for the item at index to value
     * @param index Index
     * @param value Value
     * @param role Role
     * @return True if successful
     */
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    /**
     * @brief Returns the data for the given role and section in the header with the specified orientation.
     * @param section Section
     * @param orientation Orientation
     * @param role Role
     * @return Data
     */
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    /**
     * @brief Returns the item flags for the given index
     * @param index Index
     * @return Flags
     */
    Qt::ItemFlags flags (const QModelIndex &index) const;

    /**
     * @brief Initializes the model
     * @param gpxmw GPX_model wrapper
     * @param undoStack Undo stack
     */
    void init(GPX_wrapper *gpxmw, QUndoStack *undoStack);

    /**
     * @brief Builds the model
     */
    void build();

    /**
     * @brief Clears the model
     */
    void clear();

    /**
     * @brief Gets the number of rows
     * @return Number of rows
     */
    int getNumberRows() const;

    /**
     * @brief Gets the number of columns
     * @return Number of columns
     */
    int getNumberColumns() const;

private:

    struct column_t {
        GPX_wrapper::TrackPointProperty property;
        bool showByDefault;
        bool editable;
        int size;
    };
    static const column_t columns[];

    GPX_wrapper *gpxmw;
    QUndoStack *undoStack;
    int numberRows;
    int numberColumns; 
};

/**
 * @class QTableWidgetPoints
 *
 * @brief QTableWidget subclass
 *
 * Extends the QTableWidget class.
 *
 * @see QTableWidget
 */
class QTableWidgetPoints : public QTableView
{
    Q_OBJECT

public:

    /**
     * @brief Constructor
     * @param parent Parent
     */
    explicit QTableWidgetPoints(QWidget *parent = 0);

    /**
     * @brief Initializes the widget
     * @note Call this function prior to any other function
     * @param gpxmw GPX_model wrapper
     * @param undoStack Undo stack
     */
    void init(GPX_wrapper *gpxmw, QUndoStack *undoStack);

    /**
     * @brief Restores the state
     * @param state State
     * @return True on success
     */
    bool restoreState(const QByteArray & state);

    /**
     * @brief Saves the current state
     * @return State
     */
    QByteArray saveState() const;

    /**
     * @brief Generates table
     * @param selectedRow Row to select
     */
    void build(int selectedRow = 0);

    /**
     * @brief Clears the table
     */
    void clear();

    /**
     * @brief Updates the table
     */
    void update();

    /**
     * @brief Gets selected row
     * @return Selected row number
     */
    int getSelectedRow();

signals:

    /**
     * @brief Signal when the selection changed
     * @param rowNumber New selected row
     */
    void selectionChanged(int rowNumber);

private slots:

    /**
     * @brief Slot when settings are loaded or saved
     */
    void settingsChanged(bool loaded);

    void selectionChangedExt(const QItemSelection  &selected, const QItemSelection  &deselected);

    void on_actionRestore_triggered();

    void on_actionMenu_toggled(bool show);

private:
    QByteArray defaultState;
};

/** @} Widgets */

#endif // QTABLEWIDGETPOINTS_H
