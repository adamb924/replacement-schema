#ifndef REPLACEMENTREPORTMODEL_H
#define REPLACEMENTREPORTMODEL_H

#include <QAbstractTableModel>

class ReplacementReport;

class ReplacementReportModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit ReplacementReportModel(ReplacementReport * report, QObject *parent = nullptr);
    ~ReplacementReportModel();

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    ReplacementReport * mReport;
};

#endif // REPLACEMENTREPORTMODEL_H
