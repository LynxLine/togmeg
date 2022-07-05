/****************************************************************************
**
** Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info.com)
**
** This file is part of a Qt Solutions component.
**
** Commercial Usage
** Licensees holding valid Qt Solutions licenses may use this file in
** accordance with the Qt Solutions Commercial License Agreement provided
** with the Software or, alternatively, in accordance with the terms
** contained in a written agreement between you and Nokia.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales.com.
**
****************************************************************************/

#ifndef QTLOCKEDFILE_H
#define QTLOCKEDFILE_H

#include <QFile>

#if defined(Q_WS_WIN)
#  if !defined(QT_QTLOCKEDFILE_EXPORT) && !defined(QT_QTLOCKEDFILE_IMPORT)
#    define QT_QTLOCKEDFILE_EXPORT
#  elif defined(QT_QTLOCKEDFILE_IMPORT)
#    if defined(QT_QTLOCKEDFILE_EXPORT)
#      undef QT_QTLOCKEDFILE_EXPORT
#    endif
#    define QT_QTLOCKEDFILE_EXPORT __declspec(dllimport)
#  elif defined(QT_QTLOCKEDFILE_EXPORT)
#    undef QT_QTLOCKEDFILE_EXPORT
#    define QT_QTLOCKEDFILE_EXPORT __declspec(dllexport)
#  endif
#else
#  define QT_QTLOCKEDFILE_EXPORT
#endif

class QT_QTLOCKEDFILE_EXPORT QtLockedFile : public QFile
{
public:    
    enum LockMode { NoLock = 0, ReadLock, WriteLock };

    QtLockedFile();
    QtLockedFile(const QString &name);
    ~QtLockedFile();
    
    bool lock(LockMode mode, bool block = true);
    bool unlock();
    bool isLocked() const;
    LockMode lockMode() const;
    
private:
#ifdef Q_OS_WIN
    Qt::HANDLE m_semaphore_hnd;
    Qt::HANDLE m_mutex_hnd;
#endif
    LockMode m_lock_mode;
};

#endif
