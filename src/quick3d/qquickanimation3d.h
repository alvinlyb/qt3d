/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the Qt3D module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QQUICKANIMATION3D_H
#define QQUICKANIMATION3D_H

#include <Qt3DQuick/qt3dquickglobal.h>

#include <QtQml/qqml.h>
#include <QtCore/qobject.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

QT_MODULE(QtQuick3D)

class QQuickAnimation3DPrivate;
class QGLSceneAnimation;

class Q_QT3D_QUICK_EXPORT QQuickAnimation3D : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name NOTIFY nameChanged DESIGNABLE false )
public:
    explicit QQuickAnimation3D(QObject *parent = 0);
    explicit QQuickAnimation3D(QGLSceneAnimation* pAnim, QObject *parent = 0);
    ~QQuickAnimation3D();

    QString name() const;

public Q_SLOTS:

Q_SIGNALS:
    void nameChanged();

private:
    QQuickAnimation3DPrivate *d;
};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QQuickAnimation3D)

QT_END_HEADER

#endif // QQUICKANIMATION3D_H
