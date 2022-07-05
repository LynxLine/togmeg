//
// Copyright Petrostreamz 2008
//

#ifndef NAVIGATIONSTACK_H
#define NAVIGATIONSTACK_H

/*!
 \defgroup Navigation
 */

#include <QObject>
class NavigationCommand;

/*!
 \ingroup Navigation
 \brief The stack is collection of pushed states of one browser window.
 Api provides functionality for moving though this stack, provides
 commands available in this stack and signas about state changes.
 */
class NavigationStack : public QObject {
Q_OBJECT
public:
    NavigationStack(QObject * parent);
    virtual ~NavigationStack();
    
    bool canBack() const;
    bool canForward() const;
    void push(NavigationCommand *);
    void take(NavigationCommand *);

    int index() const;
    QList<NavigationCommand *> commands() const;
    NavigationCommand * command(QObject * location) const;
    
    static QList<NavigationStack *> registeredStacks();
    
public slots:
    void back();
    void forward();
    void activate(int idx);
    
signals:
    void canBackChanged(bool canBack);
    void canForwardChanged(bool canForward);
    void indexChanged(int idx);
    
private:
    class Private;
    Private * d;
};

#endif // NAVIGATIONSTACK_H
