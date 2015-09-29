#ifndef EXECUTION_H
#define EXECUTION_H

#include <QObject>
#include "executionbase.h"

class Execution : public ExecutionBase
{
    Q_OBJECT

protected:
    Execution *clone() Q_DECL_OVERRIDE;
    QString getState() Q_DECL_OVERRIDE;
    QList<ID> getInvokableEventHandlers() Q_DECL_OVERRIDE;

public:
    explicit Execution(QObject *parent = 0);

};

#endif // EXECUTION_H
