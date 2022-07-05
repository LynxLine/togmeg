//
// Copyright Petrostreamz 2008
//

#include <QtGui>
#include "NavigationStack.h"
#include "NavigationCommand.h"

class NavigationCommand::Private {
public:
    bool saved;
    const QObject * location;    
};

NavigationCommand::NavigationCommand(const QObject * l)
{
    d = new Private;
    d->location = l;
    d->saved = false;
}

NavigationCommand::~NavigationCommand()
{
    delete d;
}

const QObject * NavigationCommand::location() const
{
    return d->location;
}

/*!
 When reimplementing this call remember to cast NavigationCommand::saveState()
 */
void NavigationCommand::saveState()
{
    d->saved = true;
}

bool NavigationCommand::hasState() const
{
    return d->saved;
}
