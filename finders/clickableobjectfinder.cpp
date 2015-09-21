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

QSet<QQuickItem*> ClickableObjectFinder::find(QObject *object)
{
    QObjectList visibleChildren;
    foreach (auto child, object->children()) {
        auto visible = child->property("visible");

        // push visible items in reverse order
        // todo: consider z-order
        if (!visible.isValid() || visible.toBool()) {
            visibleChildren.push_front(child);
        }
    }

    // check from upper items
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

//    QPoint topleft(object->x(), object->y());
//    QPoint bottomright(object->x() + object->width(),
//                       object->y() + object->height());
//    foreach (auto clickableItem, clickableItems) {

//    }

//    foreach (auto clickableItem, clickableItems) {
//        find(clickableItem);
//    }

    qDebug() << clickableItems;
    return clickableItems.toSet();
}
