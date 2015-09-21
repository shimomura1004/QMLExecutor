#include "clickableobjectfinder.h"
#include <QQmlProperty>
#include <QDebug>

ClickableObjectFinder::ClickableObjectFinder(QObject *parent) : QObject(parent)
{
}

bool ClickableObjectFinder::contains(QQuickItem *container, QQuickItem *containee)
{
    QPoint topleft(containee->x(), containee->y());
    QPoint bottomright(containee->x() + containee->width(),
                       containee->y() + containee->height());

    return container->contains(topleft) && container->contains(bottomright);
}


bool ClickableObjectFinder::isVisible(QQuickItem *item) {
    auto visible = item->property("visible");
    return !visible.isValid() || visible.toBool();
}

bool ClickableObjectFinder::isEnabled(QQuickItem *item) {
    auto enabled = item->property("enabled");
    return !enabled.isValid() || enabled.toBool();
}

bool ClickableObjectFinder::hasMethod(QQuickItem *item, QString methodName) {
     return item->metaObject()->indexOfMethod("clicked(QQuickMouseEvent*)") >= 0;
}

QList<QQuickItem*> ClickableObjectFinder::find(QQuickItem* item)
{
    if (!item) {
        return QList<QQuickItem*>();
    }

    // check if item is clickable:
    // visible, enabled, covered by child items and has clicked method
    QList<QQuickItem*> result;
    if (isVisible(item) && isEnabled(item) && hasMethod(item, "clicked")) {
        bool isCoveredByChild = false;
        foreach (auto child, item->children()) {
            // todo: consider object that is covered by multiple object
            if (contains(qobject_cast<QQuickItem*>(child), item)) {
                isCoveredByChild = true;
                break;
            }
        }

        if (!isCoveredByChild) {
            result.push_back(item);
        }
    }

    // recursively check for item's children
    QObjectList visibleChildren;
    foreach (auto child, item->children()) {
        // push visible items in reverse order
        // todo: consider z-order
        // todo: QQmlComponent is not a QQuickWindow nor QQuickItem
        if (qobject_cast<QQuickItem*>(child) && isVisible(qobject_cast<QQuickItem*>(child))) {
            visibleChildren.push_front(child);
        }
    }

    // remove covered items
    QList<QQuickItem*> clickableItems;
    foreach (auto child, visibleChildren) {
        auto childItem = qobject_cast<QQuickItem*>(child);

        bool covered = false;
        foreach (auto clickableItem, clickableItems) {
            // todo: consider object that is covered by multiple object
            if (contains(clickableItem, childItem)) {
                covered = true;
                break;
            }
        }

        if (!covered) {
            clickableItems.push_back(childItem);
        }
    }

    foreach (auto clickableItem, clickableItems) {
        result.append(find(clickableItem));
    }

    return result;
}

QList<QQuickItem *> ClickableObjectFinder::find(QQuickWindow *item)
{
    QList<QQuickItem*> result;

    foreach (auto child, item->children()) {
        auto quickItem = qobject_cast<QQuickItem*>(child);
        result.append(find(quickItem));
    }

    return result;
}
