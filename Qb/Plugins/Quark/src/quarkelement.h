/* Webcamoid, webcam capture application.
 * Copyright (C) 2011-2015  Gonzalo Exequiel Pedone
 *
 * Webcamoid is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Webcamoid is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Webcamoid. If not, see <http://www.gnu.org/licenses/>.
 *
 * Email   : hipersayan DOT x AT gmail DOT com
 * Web-Site: http://github.com/hipersayanX/webcamoid
 */

#ifndef QUARKELEMENT_H
#define QUARKELEMENT_H

#include <QQmlComponent>
#include <QQmlContext>
#include <qb.h>
#include <qbutils.h>

class QuarkElement: public QbElement
{
    Q_OBJECT
    Q_PROPERTY(int nFrames
               READ nFrames
               WRITE setNFrames
               RESET resetNFrames
               NOTIFY nFramesChanged)

    public:
        explicit QuarkElement();

        Q_INVOKABLE QObject *controlInterface(QQmlEngine *engine,
                                              const QString &controlId) const;

        Q_INVOKABLE int nFrames() const;

    private:
        int m_nFrames;

        QVector<QImage> m_frames;
        QSize m_frameSize;

    signals:
        void nFramesChanged(int nFrames);

    public slots:
        void setNFrames(int nFrames);
        void resetNFrames();

        QbPacket iStream(const QbPacket &packet);
};

#endif // QUARKELEMENT_H
