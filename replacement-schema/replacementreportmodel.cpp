#include "replacementreportmodel.h"

#include "replacement.h"
#include "replacementreport.h"

#include <QFont>

ReplacementReportModel::ReplacementReportModel(ReplacementReport *report, QObject *parent)
    : QAbstractTableModel(parent),
      mReport(report)
{
}

ReplacementReportModel::~ReplacementReportModel()
{
    delete mReport;
}

QVariant ReplacementReportModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(orientation)
    if( orientation == Qt::Horizontal && role == Qt::DisplayRole )
    {
        switch(section)
        {
        case 0:
            return tr("Pass");
        case 1:
            return tr("Replace...");
        case 2:
            return tr("With...");
        case 3:
            return tr("Result");
        }
    }
    return QVariant();
}

int ReplacementReportModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return mReport->replacementEvents()->count();
}

int ReplacementReportModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return 4;
}

QVariant ReplacementReportModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if( role == Qt::DisplayRole )
    {
        int r = index.row();
        switch(index.column())
        {
        case 0:
            return mReport->replacementEvents()->at(r).passName();
        case 1:
            if( mReport->replacementEvents()->at(r).replacement() == nullptr )
                return tr("<initial>");
            else
                return mReport->replacementEvents()->at(r).replacement()->before().pattern();
        case 2:
            if( mReport->replacementEvents()->at(r).replacement() == nullptr )
                return tr("<initial>");
            else
                return mReport->replacementEvents()->at(r).replacement()->after();
        case 3:
            return mReport->replacementEvents()->at(r).result();
        }
    }
    else if ( role == Qt::FontRole )
    {
        return QFont("Times New Roman", 16);
    }

    return QVariant();
}
