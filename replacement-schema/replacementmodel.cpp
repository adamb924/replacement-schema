#include "replacementmodel.h"

#include <QtDebug>
#include <QFont>
#include <QBrush>
#include <QMimeData>
#include <QFile>

#include "replacementschema.h"
#include "replacementpass.h"
#include "replacement.h"

ReplacementModel::ReplacementModel(ReplacementSchema *schema, QObject *parent)
    : QAbstractItemModel(parent), mSchema(schema)
{

}

QVariant ReplacementModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(orientation)
    Q_UNUSED(role)

    if (role != Qt::DisplayRole)
        return QVariant();

    switch(section)
    {
    case 0:
        return tr("Replace...");
    case 1:
        return tr("With...");
    case 2:
        return tr("Note");
    default:
        return tr("Error, unexpected value");
    }
}

QModelIndex ReplacementModel::index(int row, int column, const QModelIndex &parent) const
{
    if( parent == QModelIndex() ) // top level item, i.e., pass
    {
        if( row < mSchema->passes()->count() )
            return createIndex(row, column, mSchema->passes()->at(row) );
        else
            return QModelIndex();
    }
    else if (parent.parent() == QModelIndex() ) // second-level item, i.e., replacement group
    {
        if( row < mSchema->passes()->at( parent.row() )->replacementGroups()->count() )
            return createIndex( row, column, mSchema->passes()->at(parent.row())->replacementGroups()->at(row) );
    }
    else// replacement
    {
        if( row < mSchema->passes()->at(parent.parent().row())->replacementGroups()->at( parent.row() )->replacements()->count() )
            return createIndex(row, column, mSchema->passes()->at(parent.parent().row())->replacementGroups()->at( parent.row() )->replacements()->at(row) );
        else
            return QModelIndex();
    }
    return QModelIndex();
}

QModelIndex ReplacementModel::parent(const QModelIndex &index) const
{
    int pass, group;
    ReplacementGroup *g;
    Replacement *r;
    switch( mSchema->getLevelFromPointer(index.internalPointer()) )
    {
    case ReplacementSchema::PassObject:
        return QModelIndex(); // because Pass object have the top-level as a parent
    case ReplacementSchema::GroupObject:
        g = static_cast<ReplacementGroup*>(index.internalPointer());
        pass = mSchema->passFromReplacementGroup( g );
        return createIndex( pass, 0, mSchema->passes()->at(pass) );
//        return createIndex( index.row(), 0, mSchema->passes()->at(pass)->replacementGroups()->at(index.row()) );
    case ReplacementSchema::ReplacementObject:
        r = static_cast<Replacement*>(index.internalPointer());
        pass = mSchema->passFromReplacement(r);
        group = mSchema->passes()->at(pass)->groupFromReplacement(r);
        return createIndex( group, 0, mSchema->passes()->at(pass)->replacementGroups()->at(group) );
//        return createIndex( index.row(), 0, mSchema->passes()->at(pass)->replacementGroups()->at(group)->replacements()->at( index.row() ) );
    case ReplacementSchema::NoObject:
        return createIndex(0,0,nullptr);
    }
    return createIndex(0,0,nullptr);
}

int ReplacementModel::rowCount(const QModelIndex &parent) const
{
    if( parent == QModelIndex() ) // then it's top-level. the top-level contains the passes
    {
        return mSchema->passes()->count(); // and we're looking for the number of passes
    }
    else if( parent.parent() == QModelIndex() ) // then it's one down from top-level, a pass
    {
        return mSchema->passes()->at(parent.row())->replacementGroups()->count();
    }
    else if( parent.parent().parent() == QModelIndex() ) // then it's a third level, a replacement group
    {
        if( mSchema->passes()->at(parent.parent().row())->replacementGroups()->at( parent.row() )->replacements()->count() < 2 )
            return 0;
        else
            return mSchema->passes()->at(parent.parent().row())->replacementGroups()->at( parent.row() )->replacements()->count();
    }
    else // it's a replacement
    {
        return 0;
    }
}

int ReplacementModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    /// lazy, but don't want to deal with it now
    return 3;
//    qDebug() << parent << parent.parent();
//    if( parent == QModelIndex() )
//        return 1;
//    else
//        return 2;
}

bool ReplacementModel::hasChildren(const QModelIndex &parent) const
{
    if( parent == QModelIndex() ) // top-level
    {
        return (mSchema->passes()->count() > 0);
    }
    else
    {
        if( parent.parent() == QModelIndex() ) // then it's a pass
        {
            return (mSchema->passes()->at(parent.row())->replacementGroups()->count() > 0);
        }
        else if( parent.parent().parent() == QModelIndex() ) // then it's a replacement group
        {
            return (mSchema->passes()->at(parent.parent().row())->replacementGroups()->at(parent.row())->replacements()->count() > 1);
        }
        else // it's a replacement
        {
            return false;
        }
    }
}

QVariant ReplacementModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    /// For future reference: internalPointer gets created in the index() function

    ReplacementSchema::Level type = mSchema->getLevelFromPointer(index.internalPointer());

    if( role == Qt::DisplayRole || role == Qt::EditRole )
    {
        switch( type )
        {
        case ReplacementSchema::PassObject:
            if( index.column() == 0 )
                return mSchema->passes()->at( index.row() )->name();
            else /// other columns are empty
                return QVariant();
        case ReplacementSchema::GroupObject:
            if( mSchema->passes()->at( index.parent().row() )->replacementGroups()->at(index.row())->replacements()->count() == 1 )
            {
                return getReplacementData( mSchema->passes()->at( index.parent().row() )->replacementGroups()->at(index.row())->replacements()->first(), index );
            }
            else // replacement group with more than one child
            {
                if( index.column() == 0 )
                {
                    if( mSchema->passes()->at( index.parent().row() )->replacementGroups()->at(index.row())->replacements()->count() > 0 )
                    {
                        return mSchema->passes()->at( index.parent().row() )->replacementGroups()->at(index.row())->replacements()->first()->before().pattern();
                    }
                    else
                    {
                        return tr("<Empty>");
                    }
                }
                else /// other columns are empty
                {
                    return QVariant();
                }
            }
        case ReplacementSchema::ReplacementObject:
            return getReplacementData( index.internalPointer(), index );
        case ReplacementSchema::NoObject:
            return QVariant();
        }
    }
    else if ( role == Qt::FontRole )
    {
        switch( index.column() )
        {
        case 0:
            return QFont("Times New Roman", 14);
        case 1:
            return QFont("Adobe Arabic", 20);
        case 2:
            return QFont("Times New Roman", 14);
        default:
            return QVariant();
        }
    }
    else if ( role == Qt::TextAlignmentRole )
    {
        switch( index.column() )
        {
        case 1:
            return Qt::AlignRight;
        default:
            return Qt::AlignLeft;
        }
    }
    else if ( role == Qt::ForegroundRole )
    {
        switch( type )
        {
        case ReplacementSchema::GroupObject:
            return replacementGroupColor(index, static_cast<ReplacementGroup*>(index.internalPointer()) );
        case ReplacementSchema::ReplacementObject:
            return replacementColor(index, static_cast<Replacement*>(index.internalPointer()) );
        case ReplacementSchema::NoObject:
        case ReplacementSchema::PassObject:
            return QVariant();
        }
    }
    else if ( role == Qt::ToolTipRole )
    {
        switch( type )
        {
        case ReplacementSchema::GroupObject:
            return replacementGroupToolTip(index, static_cast<ReplacementGroup*>(index.internalPointer()) );
        case ReplacementSchema::ReplacementObject:
            return replacementToolTip(index, static_cast<Replacement*>(index.internalPointer()));
        case ReplacementSchema::PassObject:
        case ReplacementSchema::NoObject:
            return QVariant();
        }
    }
    else if ( role == ReplacementModel::Enabled )
    {
        Replacement * r = replacement(index);
        if( r == nullptr )
        {
            return QVariant();
        }
        else
        {
            return r->enabled();
        }
    }
    return QVariant();
}

Replacement *ReplacementModel::replacement(const QModelIndex &index)
{
    if( index.parent() == QModelIndex() ) // top-level, so it's a pass
    {
        return nullptr;
    }
    else if( index.parent().parent() == QModelIndex() ) // first-level, so it's a replacement group
    {
        /// return a pointer only if this index is displayed as a replacement
        if( mSchema->passes()->at( index.parent().row() )->replacementGroups()->at( index.row() )->replacements()->count() == 1 )
            return mSchema->passes()->at( index.parent().row() )->replacementGroups()->at( index.row() )->replacements()->first();
        else
            return nullptr;
    }
    else // replacement
    {
        return mSchema->passes()->at( index.parent().parent().row() )->replacementGroups()->at(index.parent().row())->replacements()->at( index.row() );
    }
}

Replacement *ReplacementModel::replacement(const QModelIndex &index) const
{
    if( index.parent() == QModelIndex() ) // top-level, so it's a pass
    {
        return nullptr;
    }
    else if( index.parent().parent() == QModelIndex() ) // first-level, so it's a replacement group
    {
        /// return a pointer only if this index is displayed as a replacement
        if( mSchema->passes()->at( index.parent().row() )->replacementGroups()->at( index.row() )->replacements()->count() == 1 )
            return mSchema->passes()->at( index.parent().row() )->replacementGroups()->at( index.row() )->replacements()->first();
        else
            return nullptr;
    }
    else // replacement
    {
        return mSchema->passes()->at( index.parent().parent().row() )->replacementGroups()->at(index.parent().row())->replacements()->at( index.row() );
    }
}

bool ReplacementModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value)
    {
        ReplacementSchema::Level type = mSchema->getLevelFromPointer(index.internalPointer());
        switch( type )
        {
        case ReplacementSchema::PassObject:
            mSchema->passes()->at( index.row() )->setName( value.toString() );
            break;
        case ReplacementSchema::GroupObject:
            /// groups should only be modified if there is one
            if( mSchema->passes()->at(index.parent().row())->replacementGroups()->at(index.row())->replacements()->count() == 1)
            {
                setReplacementData(mSchema->passes()->at(index.parent().row())->replacementGroups()->at(index.row())->replacements()->first(), index, value, role );
            }
            break;
        case ReplacementSchema::ReplacementObject:
            setReplacementData(mSchema->passes()->at(index.parent().parent().row())->replacementGroups()->at(index.parent().row())->replacements()->at(index.row()), index, value, role );
            break;
        case ReplacementSchema::NoObject:
            break;
        }

        mSchema->setChanged(true);

        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags ReplacementModel::flags(const QModelIndex &index) const
{
    /// the root item must be drop-enabled
    if (!index.isValid())
        return Qt::ItemIsDropEnabled;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;
}

Qt::DropActions ReplacementModel::supportedDragActions() const
{
    return Qt::MoveAction;
}

bool ReplacementModel::insertPass(const QString &name)
{
    beginInsertRows( QModelIndex(), mSchema->passes()->count(), mSchema->passes()->count());
    mSchema->passes()->append( new ReplacementPass(name) );
    endInsertRows();

    mSchema->setChanged(true);

    return true;
}

bool ReplacementModel::insertPassAfter(int afterPass, ReplacementPass *pass)
{
    beginInsertRows( QModelIndex(), afterPass, afterPass );
    mSchema->passes()->insert( afterPass + 1, pass );
    endInsertRows();
    return true;
}

bool ReplacementModel::insertReplacementGroup(int toPass, ReplacementGroup *group)
{
    beginInsertRows( index(toPass,0), mSchema->passes()->at(toPass)->replacementGroups()->count(), mSchema->passes()->at(toPass)->replacementGroups()->count() );
    mSchema->passes()->at(toPass)->replacementGroups()->insert(-1, group);
    endInsertRows();

    mSchema->setChanged(true);

    return true;
}

bool ReplacementModel::insertReplacementGroupAfter(int toPass, int afterReplacement, ReplacementGroup *group)
{
    beginInsertRows( index(toPass,0), afterReplacement, afterReplacement );
    mSchema->passes()->at(toPass)->replacementGroups()->insert( afterReplacement + 1, group );
    endInsertRows();

    mSchema->setChanged(true);

    return true;
}

bool ReplacementModel::insertReplacementGroup(int toPass, Replacement * replacement)
{
    ReplacementGroup * group = new ReplacementGroup;
    group->replacements()->append( replacement );
    return insertReplacementGroup(toPass, group);
}

bool ReplacementModel::insertReplacementGroupAfter(int toPass, int afterReplacement, Replacement *replacement)
{
    ReplacementGroup * group = new ReplacementGroup;
    group->replacements()->append( replacement );
    return insertReplacementGroupAfter(toPass, afterReplacement, group);
}

void ReplacementModel::removePass(int pass)
{
    beginRemoveRows( QModelIndex(), pass, pass);
    mSchema->passes()->removeAt(pass);
    endRemoveRows();

    mSchema->setChanged(true);
}

void ReplacementModel::removeReplacementGroup(int pass, int replacementGroup)
{
    beginRemoveRows( index(pass,0), replacementGroup, replacementGroup);
    mSchema->passes()->at(pass)->replacementGroups()->removeAt(replacementGroup);
    endRemoveRows();

    mSchema->setChanged(true);
}

Qt::DropActions ReplacementModel::supportedDropActions() const
{
    return Qt::MoveAction;
}

QStringList ReplacementModel::mimeTypes() const
{
    QStringList types;
    types << "text/plain";
    return types;
}

QMimeData *ReplacementModel::mimeData(const QModelIndexList &indexes) const
{
    QMimeData *mimeData = new QMimeData();
    QString encodedData;

    QModelIndex index = indexes.first(); /// it happens that only one row can ever be selected
    if (index.isValid())
    {
        if( index.parent().isValid() ) /// replacement
        {
            encodedData.append( QString("R\t%0\t%1").arg(index.parent().row()).arg(index.row()) );
        }
        else /// pass
        {
            encodedData.append( QString("P\t%0").arg(index.row()) );
        }
    }

    mimeData->setText(encodedData);

    return mimeData;
}

bool ReplacementModel::canDropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(action)
    Q_UNUSED(row)
    Q_UNUSED(parent)

    if( action != Qt::MoveAction || row == -1 || column == -1 )
        return false;

    if (!data->hasText())
    {
        qDebug() << "Item does not have text";
        return false;
    }

    QStringList fields = data->text().split("\t");

    if( fields.at(0) == "P" && !parent.isValid() ) /// if the item is a pass and the parent is null (i.e., the root element)
    {
        return true;
    }
    else if ( fields.at(0) == "R" && parent.isValid() ) /// if the item is a replacement and the parent is a pass
    {
        return true;
    }

    return true;
}

bool ReplacementModel::dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    if (!canDropMimeData(data, action, row, column, parent))
        return false;

    if (action != Qt::MoveAction)
        return true;

    QStringList fields = data->text().split("\t");

    QString type = fields.at(0);

    if( type == "R" && parent.isValid() ) /// it's replacement reordering
    {
        int targetPass = parent.row();
        int placeAfter = row - 1;
        int sourcePass = fields.at(1).toInt();
        int sourceReplacement = fields.at(2).toInt();

        ReplacementGroup * r = mSchema->passes()->at( sourcePass )->replacementGroups()->at( sourceReplacement );
        removeReplacementGroup( sourcePass, sourceReplacement );
        insertReplacementGroupAfter( targetPass, placeAfter, r );

        return true;
    }

    if( type == "P" )
    {
        int sourcePass = fields.at(1).toInt();
        int placeAfter = row - 1;

        ReplacementPass * p = mSchema->passes()->at( sourcePass );
        removePass( sourcePass );
        insertPassAfter( placeAfter, p );

        return true;
    }

    return false;
}

void ReplacementModel::importReplacements(int toPass, const QString &filename)
{
    if( toPass < 0 || toPass >= mSchema->passes()->count() ) return;

    QFile data(filename);
    if (data.open(QFile::ReadOnly))
    {
        /// first store them temporarily so we know how many there are
        QList<Replacement*> replacements;

        QTextStream in(&data);

#if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
        in.setEncoding( QStringConverter::Utf8 );
#else
        in.setCodec("UTF-8");
#endif

        while( !in.atEnd() )
        {
            QString line = in.readLine();
            QStringList fields = line.split("\t");
            if( fields.count() == 3 )
            {
                replacements << new Replacement( QRegularExpression(fields.at(0), QRegularExpression::UseUnicodePropertiesOption), fields.at(1), fields.at(2) );
            }
            else if( fields.count() == 2 )
            {
                replacements << new Replacement( QRegularExpression(fields.at(0), QRegularExpression::UseUnicodePropertiesOption), fields.at(1) );
            }
            else
            {
                qDebug() << "Invalid format. Only this many fields: " << fields.count();
            }
        }

        /// now add them to the model
        beginInsertRows( index(toPass,0), mSchema->passes()->at(toPass)->replacementGroups()->count(), mSchema->passes()->at(toPass)->replacementGroups()->count() + replacements.count() - 1 );
        ReplacementGroup * g = new ReplacementGroup;
        g->replacements()->append(replacements);
        mSchema->passes()->at(toPass)->replacementGroups()->append(g);
        endInsertRows();
    }
    else
    {
        qDebug() << "Cannot open file: " << filename;
    }

    mSchema->setChanged(true);
}

void ReplacementModel::setEnabled(const QModelIndex &index, bool enabled)
{
    setData( index, enabled, ReplacementModel::Enabled );
}

QVariant ReplacementModel::replacementColor(const QModelIndex &index, const Replacement *r) const
{
    if( index.column() == 0 )
    {
        if( r->before().isValid() )
        {
            if( r->enabled() )
                return QBrush( Qt::black );
            else
                return QBrush( Qt::gray );
        }
        else
        {
            return QBrush( Qt::red );
        }
    }
    else /// not the regular expression column
    {
        if( r->enabled() )
            return QBrush( Qt::black );
        else
            return QBrush( Qt::gray );
    }
}

QVariant ReplacementModel::replacementGroupColor(const QModelIndex &index, const ReplacementGroup *g) const
{
    if( g->replacements()->count() == 1 )
    {
        return replacementColor(index, g->replacements()->first() );
    }
    else
    {
        return QVariant();
    }
}

QVariant ReplacementModel::replacementToolTip(const QModelIndex &index, const Replacement *r) const
{
    if( index.column() == 0
            && ! r->before().isValid()  )
    {
        return r->before().errorString();
    }
    else
    {
        return QVariant();
    }
}

QVariant ReplacementModel::replacementGroupToolTip(const QModelIndex &index, const ReplacementGroup *g) const
{
    if( index.column() == 0
            && g->replacements()->count() == 1
            && ! g->replacements()->first()->before().isValid() )
    {
        return g->replacements()->first()->before().errorString();
    }
    else
    {
        return QVariant();
    }
}

void ReplacementModel::setReplacementData(Replacement *r, const QModelIndex &index, const QVariant &value, int role)
{
    if( role == ReplacementModel::Enabled )
    {
        r->setEnabled( value.toBool() );
    }
    else
    {
        switch(index.column())
        {
        case 0:
            r->setBefore( QRegularExpression(value.toString(), QRegularExpression::UseUnicodePropertiesOption) );
            break;
        case 1:
            r->setAfter( value.toString() );
            break;
        case 2:
            r->setNote( value.toString() );
            break;
        default:
            qDebug() << "Unexpected value: ReplacementModel::setReplacementData";
        }
    }
}

QVariant ReplacementModel::getReplacementData(Replacement *r, const QModelIndex &index) const
{
    switch( index.column() )
    {
    case 0:
        return r->before().pattern();
    case 1:
        return r->after();
    case 2:
        return r->note();
    default:
        return tr("Error");
    }
}

QVariant ReplacementModel::getReplacementData(void *r, const QModelIndex &index) const
{
    return getReplacementData( static_cast<Replacement*>(r), index );
}
