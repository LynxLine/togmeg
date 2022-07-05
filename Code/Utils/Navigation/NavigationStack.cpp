//
// Copyright Petrostreamz 2008
//

#include <QtGui>
#include "NavigationStack.h"
#include "NavigationCommand.h"

class NavigationStack::Private {
public:
    int index;
    QList<NavigationCommand *> stack;
    static QList<NavigationStack *> registeredStacks;
};

QList<NavigationStack *> NavigationStack::Private::registeredStacks = QList<NavigationStack *>();

NavigationStack::NavigationStack(QObject * parent)
:QObject(parent)
{
    d = new Private;
    d->index = -1;
    Private::registeredStacks << this;
}

NavigationStack::~NavigationStack()
{
    Private::registeredStacks.removeAll(this);
    delete d;
}

bool NavigationStack::canBack() const
{
    return d->stack.count() && d->index >0;
}

bool NavigationStack::canForward() const
{
    return d->stack.count() && d->index < d->stack.count()-1;
}

void NavigationStack::push(NavigationCommand * cmd)
{
    if (d->index >= 0) {
        d->stack[d->index]->saveState();
    
        QList<NavigationCommand *> toRemove;
        for (int i=d->index+1; i<d->stack.count(); i++) {
            toRemove << d->stack[i];
        }
        
        while(d->index+1 <= d->stack.count()-1)
            d->stack.removeAt(d->index+1);
        
        foreach(NavigationCommand * c, toRemove) {
            delete c;
        }
    }
    
    d->stack << cmd;
    d->index = d->stack.count()-1;
    
    emit canBackChanged(canBack());
    emit canForwardChanged(canForward());
    emit indexChanged(d->index);
}

void NavigationStack::take(NavigationCommand * cmd)
{
    if (d->stack.count() <1) return;
    
    int indexToRemove = -1;
    for (int i=0; i<d->stack.count(); i++) {
        if (d->stack[i] == cmd) {
            indexToRemove = i;
            break;
        }
    }
    
    if (indexToRemove <0) return;
    
    d->stack.removeAt(indexToRemove);
    if (d->index >= indexToRemove)
        d->index--;
    
    int prevIndex = indexToRemove-1;
    int nextIndex = indexToRemove; //shifted
    
    //check if prev != next, else compress into one
    if (prevIndex >=0 && nextIndex < d->stack.count()) {
        const QObject * p1 = d->stack[prevIndex]->location();
        const QObject * p2 = d->stack[nextIndex]->location();
        if (p1 == p2) {
            NavigationCommand * cmd = d->stack[nextIndex];
            d->stack.removeAt(nextIndex);
            delete cmd;
            
            if (d->index >= nextIndex)
                d->index--;
        }
    }
    
    emit canBackChanged(canBack());
    emit canForwardChanged(canForward());
    emit indexChanged(d->index);    
}

int NavigationStack::index() const
{
    return d->index;
}

QList<NavigationCommand *> NavigationStack::commands() const
{
    return d->stack;
}

NavigationCommand * NavigationStack::command(QObject * location) const
{
    int from = d->index;
    int to = d->stack.count()-1;
    
    for (int i=from;i<=to;i++)
        if (d->stack[i]->location() == location && d->stack[i]->hasState())
            return d->stack[i];
            
    from = d->stack.count()-1;
    to = 0;
    
    for (int i=from;i>=to;i--)
        if (d->stack[i]->location() == location && d->stack[i]->hasState())
            return d->stack[i];
            
    return 0L;
}

void NavigationStack::back()
{
    if (d->index <=0 ) return;
    if (d->index >= 0) {
        d->stack[d->index]->saveState();
    }
    
    d->index--;
    d->stack[d->index]->activate();

    emit canBackChanged(canBack());
    emit canForwardChanged(canForward());
    emit indexChanged(d->index);
}

void NavigationStack::forward()
{
    if (d->index >= d->stack.count()-1) return;
    if (d->index >= 0) {
        d->stack[d->index]->saveState();
    }
    
    d->index++;
    d->stack[d->index]->activate();

    emit canBackChanged(canBack());
    emit canForwardChanged(canForward());
    emit indexChanged(d->index);
}

void NavigationStack::activate(int idx)
{
    if (d->index >= 0 && d->index < d->stack.count()) {
        d->stack[d->index]->saveState();
    }
    
    d->index = idx;
    d->stack[d->index]->activate();
    
    emit canBackChanged(canBack());
    emit canForwardChanged(canForward());
    emit indexChanged(d->index);
}

QList<NavigationStack *> NavigationStack::registeredStacks()
{
    return Private::registeredStacks;
}
