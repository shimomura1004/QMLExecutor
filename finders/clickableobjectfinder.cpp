#include "clickableobjectfinder.h"
#include <QQmlProperty>

ClickableObjectFinder::ClickableObjectFinder(QObject *parent) : QObject(parent)
{
}

bool ClickableObjectFinder::contains(QQuickItem *container, QQuickItem *containee)
{
    const int x1 = containee->x() - container->x();
    const int y1 = containee->y() - container->y();
    const int x2 = x1 + containee->width();
    const int y2 = y1 + containee->height();

    return containee->contains(QPointF(x1, y1)) && containee->contains(QPointF(x2, y2));
}

bool ClickableObjectFinder::isVisible(QQuickItem *item) {
    auto visible = item->property("visible");
    return !visible.isValid() || visible.toBool();
}

bool ClickableObjectFinder::isEnabled(QQuickItem *item) {
    auto enabled = item->property("enabled");
    return !enabled.isValid() || enabled.toBool();
}

bool ClickableObjectFinder::hasMethod(QQuickItem *item, const char *methodName) {
     return item->metaObject()->indexOfMethod(methodName) >= 0;
}

// todo: equivalence partitioning of eventhandlers
QList<QQuickItem*> ClickableObjectFinder::find(QQuickItem* item)
{
    if (!item) {
        return QList<QQuickItem*>();
    }

    // check if item is clickable:
    // visible, enabled, covered by child items and has clicked method
    QList<QQuickItem*> result;
    if (isVisible(item) && isEnabled(item) && hasMethod(item, "clicked(QQuickMouseEvent*)")) {
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
        result.append(find(qobject_cast<QQuickItem*>(child)));
    }

    return result;
}
