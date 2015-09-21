#ifndef CLICKABLEOBJECTFINDER_H
#define CLICKABLEOBJECTFINDER_H

#include <QObject>
#include <QQuickItem>
#include <QSet>

class ClickableObjectFinder : public QObject
{
    Q_OBJECT
private:
    explicit ClickableObjectFinder(QObject *parent = 0);

protected:
    static bool contains(QQuickItem *container, QQuickItem *containee);

public:
    static QSet<QQuickItem *> find(QObject * object);
};

#endif // CLICKABLEOBJECTFINDER_H
