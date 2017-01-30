/****************************************************************************
**
** Copyright (C) 2017 Klaralvdalens Datakonsult AB (KDAB).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the Qt3D module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL3$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPLv3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or later as published by the Free
** Software Foundation and appearing in the file LICENSE.GPL included in
** the packaging of this file. Please review the following information to
** ensure the GNU General Public License version 2.0 requirements will be
** met: http://www.gnu.org/licenses/gpl-2.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "fcurve_p.h"
#include <private/bezierevaluator_p.h>

#include <QtCore/qjsonarray.h>
#include <QtCore/qjsonobject.h>
#include <QtCore/QLatin1String>

QT_BEGIN_NAMESPACE

namespace Qt3DAnimation {
namespace Animation {

FCurve::FCurve()
    : m_rangeFinder(m_localTimes)
{
}

float FCurve::evaluateAtTime(float localTime) const
{
    // TODO: Implement extrapolation beyond first/last keyframes
    if (localTime < m_localTimes.first()) {
        return m_keyframes.first().value;
    } else if (localTime > m_localTimes.last()) {
        return m_keyframes.last().value;
    } else {
        // Find keyframes that sandwich the requested localTime
        int keyframe0 = m_rangeFinder.findLowerBound(localTime);

        BezierEvaluator evaluator(m_localTimes[keyframe0], m_keyframes[keyframe0],
                                  m_localTimes[keyframe0 + 1], m_keyframes[keyframe0 + 1]);
        return evaluator.valueForTime(localTime);
    }
}

float FCurve::startTime() const
{
    if (!m_localTimes.isEmpty())
        return m_localTimes.first();
    return 0.0f;
}

float FCurve::endTime() const
{
    if (!m_localTimes.isEmpty())
        return m_localTimes.last();
    return 0.0f;
}

void FCurve::appendKeyframe(float localTime, const Keyframe &keyframe)
{
    m_localTimes.append(localTime);
    m_keyframes.append(keyframe);
}

void FCurve::read(const QJsonObject &json)
{
    clearKeyframes();

    const QJsonArray keyframeArray = json[QLatin1String("keyframes")].toArray();
    const int keyframeCount = keyframeArray.size();

    for (int i = 0; i < keyframeCount; ++i) {
        const QJsonObject keyframeData = keyframeArray.at(i).toObject();

        // Extract the keyframe local time and value
        const QJsonArray keyframeCoords = keyframeData[QLatin1String("co")].toArray();
        float localTime = keyframeCoords.at(0).toDouble();

        Keyframe keyframe;
        keyframe.interpolation = Keyframe::Bezier;
        keyframe.value = keyframeCoords.at(1).toDouble();

        const QJsonArray leftHandle = keyframeData[QLatin1String("handle_left")].toArray();
        keyframe.leftControlPoint[0] = leftHandle.at(0).toDouble();
        keyframe.leftControlPoint[1] = leftHandle.at(1).toDouble();

        const QJsonArray rightHandle = keyframeData[QLatin1String("handle_right")].toArray();
        keyframe.rightControlPoint[0] = rightHandle.at(0).toDouble();
        keyframe.rightControlPoint[1] = rightHandle.at(1).toDouble();

        appendKeyframe(localTime, keyframe);
    }

    // TODO: Ensure beziers have no loops or cusps by scaling the control points
    // back so they do not interset.
}

void Channel::read(const QJsonObject &json)
{
    name = json[QLatin1String("name")].toString();
    fcurve.read(json);
}

void ChannelGroup::read(const QJsonObject &json)
{
    name = json[QLatin1String("group")].toString();
    const QJsonArray channelsArray = json[QLatin1String("channels")].toArray();
    const int channelCount = channelsArray.size();
    channels.resize(channelCount);

    for (int i = 0; i < channelCount; ++i) {
        const QJsonObject channel = channelsArray.at(i).toObject();
        channels[i].read(channel);
    }
}

} // namespace Animation
} // namespace Qt3DAnimation

QT_END_NAMESPACE
