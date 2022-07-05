//
// Copyright Petrostreamz 2008
//

#ifndef NAVIGATIONGROUP_H
#define NAVIGATIONGROUP_H

#include <QObject>

class QAction;
class NavigationStack;

/*!
 \ingroup Navigation
 \brief The class handles all available navigation stacks.
 Also the object has functionality to operate with active stack
 and bind functionality of active stack to back/forward actions
 which are application wide.
 */
class NavigationGroup : public QObject {
Q_OBJECT
public:
    NavigationGroup(QObject * parent);
    virtual ~NavigationGroup();
    
    NavigationStack * activeStack() const;
    void setActiveStack(NavigationStack *);
    
    void addStack(NavigationStack * stack);
    void removeStack(NavigationStack * stack);
    QList<NavigationStack *> stacks() const;
    
    QAction * createForwardAction(QObject * parent) const;
    QAction * createBackAction(QObject * parent) const;
    
signals:
    void activeStackChanged(NavigationStack *);
    void indexChanged(int idx);
    
private:
    class Private;
    Private * d;
};

#endif // NAVIGATIONGROUP_H
