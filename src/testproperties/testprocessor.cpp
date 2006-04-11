// vim:tabstop=4:shiftwidth=4:foldmethod=marker:expandtab:cinoptions=(s,U1,m1
// Copyright (C) 2006

#include <QtGui>
#include "testprocessor.h"

class TestProcessor::Private {
public:
};

/*!
 * Creates the object.
 */
TestProcessor::TestProcessor(QObject * parent)
:QObject(parent)
{
    d = new Private;
}

/*!
 * Deletes it.
 */
TestProcessor::~TestProcessor()
{
    delete d;
}

void TestProcessor::start()
{
}

void TestProcessor::stop()
{
}

