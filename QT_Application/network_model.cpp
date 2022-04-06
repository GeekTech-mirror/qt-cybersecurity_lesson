#include <NetworkManagerQt/ConnectionSettings>
#include <NetworkManagerQt/Manager>
#include <NetworkManagerQt/Settings>

#include "network_model.h"
#include "network_model_item.h"

QNetworkModel::QNetworkModel(const QStringList &headers, const QString &data, QObject *parent)
    : QAbstractItemModel(parent)
{
    QVector<QVariant> rootData;
    for (const QString &header : headers) {
        rootData << header;
    }
    rootItem = new QNetworkItem(rootData);
    setupModelData(data.split('\n'), rootItem);
}

QNetworkModel::~QNetworkModel()
{
    delete rootItem;
}


QVariant QNetworkModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    if (role != Qt::DisplayRole && role != Qt::EditRole) {
        return QVariant();
    }

    QNetworkItem *item = static_cast<QNetworkItem*>(index.internalPointer());

    return item->data(index.column());
}

bool QNetworkModel::setData(const QModelIndex &index, const QVariant &value,
             int role)
{
    if (role != Qt::EditRole)
        return false;

    QNetworkItem *item = getItem(index);
    bool result = item->setData(index.column(), value);

    if (result)
        emit dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole});

    return result;
}

QVariant QNetworkModel::headerData(int section, Qt::Orientation orientation,
                    int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

bool QNetworkModel::setHeaderData(int section, Qt::Orientation orientation,
                   const QVariant &value, int role)
{
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    const bool result = rootItem->setData(section, value);

    if (result)
        emit headerDataChanged(orientation, section, section);

    return result;
}


QModelIndex QNetworkModel::index(int row, int column,
                  const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    QNetworkItem *parentItem = getItem(parent);

    if (!parentItem)
        return QModelIndex();

    QNetworkItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);

    return QModelIndex();

}

QModelIndex QNetworkModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    QNetworkItem *childItem = static_cast<QNetworkItem*>(index.internalPointer());
    QNetworkItem *parentItem = childItem->parent();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
}


int QNetworkModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() > 0)
        return 0;

    const QNetworkItem *parentItem = getItem(parent);

    return parentItem ? parentItem->childCount() : 0;
}

bool QNetworkModel::insertRows(int position, int rows,
                               const QModelIndex &parent)
{
    QNetworkItem *parentItem = getItem(parent);
    if(!parentItem)
        return false;

    beginInsertRows(parent, position, position + rows - 1);
    const bool success = parentItem->insertChildren(position,
                                                    rows,
                                                    rootItem->columnCount());
    endInsertRows();

    return success;
}

bool QNetworkModel::removeRows(int position, int rows,
                               const QModelIndex &parent)
{
    QNetworkItem *parentItem = getItem(parent);
    if (!parentItem)
        return false;

    beginRemoveRows(parent, position, position + rows - 1);
    const bool success = parentItem->removeChildren(position, rows);
    endRemoveRows();

    return success;
}


int QNetworkModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return rootItem->columnCount();
}

bool QNetworkModel::insertColumns(int position, int columns,
                   const QModelIndex &parent)
{
    beginInsertColumns(parent, position, position + columns - 1);
    const bool success = rootItem->insertColumns(position, columns);
    endInsertColumns();

    return success;
}

bool QNetworkModel::removeColumns(int position, int columns,
                   const QModelIndex &parent)
{
    beginRemoveColumns(parent, position, position + columns -1);
    const bool success = rootItem->removeColumns(position, columns);
    endRemoveColumns();

    if (rootItem->columnCount() == 0)
        removeRows(0, rowCount());

    return success;
}


QNetworkItem *QNetworkModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        QNetworkItem *item = static_cast<QNetworkItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return rootItem;
}

Qt::ItemFlags QNetworkModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}


void QNetworkModel::setupModelData(const QStringList &lines, QNetworkItem *parent)
{
    QList<QNetworkItem *> parents;
    QList<int> indentations;
    parents << parent;
    indentations << 0;

    int number = 0;

    while (number < lines.count()) {
        int position = 0;
        while (position < lines[number].length()) {
            if (lines[number].at(position) != ' ')
                break;
            ++position;
        }

        const QString lineData = lines[number].mid(position).trimmed();

        if (!lineData.isEmpty()) {
            // Read the column data from the rest of the line.
            const QStringList columnStrings =
                lineData.split(QLatin1Char('\t'), Qt::SkipEmptyParts);
            QList<QVariant> columnData;
            columnData.reserve(columnStrings.size());
            for (const QString &columnString : columnStrings)
                columnData << columnString;

            if (position > indentations.last()) {
                // The last child of the current parent is now the new parent
                // unless the current parent has no children.

                if (parents.last()->childCount() > 0) {
                    parents << parents.last()->child(parents.last()->childCount()-1);
                    indentations << position;
                }
            } else {
                while (position < indentations.last() && parents.count() > 0) {
                    parents.pop_back();
                    indentations.pop_back();
                }
            }

            // Append a new item to the current parent's list of children.
            QNetworkItem *parent = parents.last();
            parent->insertChildren(parent->childCount(), 1,
                                   rootItem->columnCount());
            for (int column = 0; column < columnData.size(); ++column)
                parent->child(parent->childCount() - 1)
                        ->setData(column, columnData[column]);
        }
        ++number;
    }
}
