//
// Copyright Petrostreamz 2008
//

#ifndef NAVIGATIONCOMMAND_H
#define NAVIGATIONCOMMAND_H

/*!
 \ingroup Navigation
 \brief The class handles state and action of one view state.
 */
class NavigationCommand {
public:
    NavigationCommand(const QObject * location);
    virtual ~NavigationCommand();

    bool hasState() const;
    virtual void saveState();
    virtual void activate() = 0;

    const QObject * location() const;
    
public:
    class Private;
    Private * d;
    
};

#endif // NAVIGATIONCOMMAND_H
