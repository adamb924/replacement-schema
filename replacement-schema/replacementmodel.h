#ifndef REPLACEMENTMODEL_H
#define REPLACEMENTMODEL_H

#include <QAbstractItemModel>

class Replacement;
class ReplacementSchema;
class ReplacementPass;
class ReplacementGroup;

#include "replacement-schema_global.h"

class REPLACEMENTSCHEMA_EXPORT ReplacementModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    enum Roles { Enabled = Qt::UserRole + 1};

    ReplacementModel(ReplacementSchema * schema, QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    // Fetch data dynamically:
    bool hasChildren(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;


    Replacement *replacement( const QModelIndex & index );
    Replacement *replacement( const QModelIndex & index ) const;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;
    Qt::DropActions supportedDragActions() const override;

    bool insertPass(const QString & name);
    bool insertPassAfter(int afterPass, ReplacementPass * pass);
    bool insertReplacementGroup(int toPass, ReplacementGroup * group);
    bool insertReplacementGroupAfter(int toPass, int afterReplacement, ReplacementGroup * group);
    bool insertReplacementGroup(int toPass, Replacement * replacement);
    bool insertReplacementGroupAfter(int toPass, int afterReplacement, Replacement * replacement);

    void removePass(int pass);
    void removeReplacementGroup(int pass, int replacementGroup);

    /// Drag/Drop:
    Qt::DropActions supportedDropActions() const override;
    QStringList mimeTypes() const override;
    QMimeData *mimeData(const QModelIndexList &indexes) const override;
    bool canDropMimeData(const QMimeData *data,
        Qt::DropAction action, int row, int column, const QModelIndex &parent) const override;
    bool dropMimeData(const QMimeData *data,
        Qt::DropAction action, int row, int column, const QModelIndex &parent) override;

public slots:
    void importReplacements(int toPass, const QString & filename);
    void setEnabled(const QModelIndex & index, bool enabled);
    QVariant replacementColor(const QModelIndex &index, const Replacement * r) const;
    QVariant replacementGroupColor(const QModelIndex &index, const ReplacementGroup * g) const;

    QVariant replacementToolTip(const QModelIndex &index, const Replacement * r) const;
    QVariant replacementGroupToolTip(const QModelIndex &index, const ReplacementGroup * g) const;

private:
    void setReplacementData(Replacement * r, const QModelIndex &index, const QVariant &value, int role);
    QVariant getReplacementData(Replacement * r, const QModelIndex &index) const;
    QVariant getReplacementData(void * r, const QModelIndex &index) const;


    ReplacementSchema * mSchema;
};

#endif // REPLACEMENTMODEL_H
