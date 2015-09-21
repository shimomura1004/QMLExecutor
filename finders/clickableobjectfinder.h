#ifndef CLICKABLEOBJECTFINDER_H
#define CLICKABLEOBJECTFINDER_H

#include <QObject>
#include <QQuickItem>
#include <QQuickWindow>
#include <QSet>

class ClickableObjectFinder : public QObject
{
    Q_OBJECT
private:
    explicit ClickableObjectFinder(QObject *parent = 0);

protected:
    static bool contains(QQuickItem *container, QQuickItem *containee);

    static bool isVisible(QQuickItem *item);
    static bool isEnabled(QQuickItem *item);
    static bool hasMethod(QQuickItem *item, QString methodName);

public:
    static QList<QQuickItem*> find(QQuickItem* item);
    static QList<QQuickItem*> find(QQuickWindow* item);

};

#endif // CLICKABLEOBJECTFINDER_H
