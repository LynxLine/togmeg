//
// Copyright Petrostreamz 2008
//

#include <QtGui>
#include "NavigationGroup.h"
#include "NavigationStack.h"

class NavigationGroup::Private {
public:
    QList<NavigationStack *> stacks;
    NavigationStack * activeStack;
    QAction * a_forward;
    QAction * a_back;
};

NavigationGroup::NavigationGroup(QObject * parent)
:QObject(parent)
{
    d = new Private;
    d->activeStack = 0L;
}

NavigationGroup::~NavigationGroup()
{
    delete d;
}

NavigationStack * NavigationGroup::activeStack() const
{
    return d->activeStack;
}

void NavigationGroup::setActiveStack(NavigationStack * stack)
{
    if (d->activeStack) {
        disconnect(d->activeStack, SIGNAL(canBackChanged(bool)), d->a_back, 0);
        disconnect(d->activeStack, SIGNAL(canForwardChanged(bool)), d->a_forward, 0);
        disconnect(d->activeStack, SIGNAL(indexChanged(int)), this, SIGNAL(indexChanged(int)));
        //disconnect(d->a_forward,   SIGNAL(triggered()), d->activeStack, SLOT(forward()));
        //disconnect(d->a_back,      SIGNAL(triggered()), d->activeStack, SLOT(back()));
    }
    
    d->activeStack = stack;
    d->a_back->setEnabled( stack && stack->canBack() );
    d->a_forward->setEnabled( stack && stack->canForward() );
    
    if (stack) {
        connect(stack, SIGNAL(canBackChanged(bool)), d->a_back, SLOT(setEnabled(bool)));
        connect(stack, SIGNAL(canForwardChanged(bool)), d->a_forward, SLOT(setEnabled(bool)));
        connect(stack, SIGNAL(indexChanged(int)), this, SIGNAL(indexChanged(int)));
        //connect(d->a_forward, SIGNAL(triggered()), stack, SLOT(forward()));
        //connect(d->a_back,    SIGNAL(triggered()), stack, SLOT(back()));
    }
    
    emit activeStackChanged(stack);
}

QAction * NavigationGroup::createForwardAction(QObject * parent) const
{
    d->a_forward = new QAction(tr("Forward"), parent);
    return d->a_forward;
}

QAction * NavigationGroup::createBackAction(QObject * parent) const
{
    d->a_back = new QAction(tr("Back"), parent);
    return d->a_back;
}

void NavigationGroup::addStack(NavigationStack * stack)
{
    d->stacks << stack;
}

void NavigationGroup::removeStack(NavigationStack * stack)
{
    d->stacks.removeAll(stack);
}

QList<NavigationStack *> NavigationGroup::stacks() const
{
    return d->stacks;
}
