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

#include "warholelement.h"

WarholElement::WarholElement(): QbElement()
{
    this->m_nFrames = 3;

    this->m_colorTable << 0x000080 << 0x008000 << 0x800000
                       << 0x00e000 << 0x808000 << 0x800080
                       << 0x808080 << 0x008080 << 0xe0e000;
}

QObject *WarholElement::controlInterface(QQmlEngine *engine, const QString &controlId) const
{
    Q_UNUSED(controlId)

    if (!engine)
        return NULL;

    // Load the UI from the plugin.
    QQmlComponent component(engine, QUrl(QStringLiteral("qrc:/Warhol/share/qml/main.qml")));

    // Create a context for the plugin.
    QQmlContext *context = new QQmlContext(engine->rootContext());
    context->setContextProperty("Warhol", (QObject *) this);
    context->setContextProperty("controlId", this->objectName());

    // Create an item with the plugin context.
    QObject *item = component.create(context);
    context->setParent(item);

    return item;
}

int WarholElement::nFrames() const
{
    return this->m_nFrames;
}

void WarholElement::setNFrames(int nFrames)
{
    if (this->m_nFrames == nFrames)
        return;

    this->m_nFrames = nFrames;
    emit this->nFramesChanged(nFrames);
}

void WarholElement::resetNFrames()
{
    this->setNFrames(3);
}

QbPacket WarholElement::iStream(const QbPacket &packet)
{
    QImage src = QbUtils::packetToImage(packet);

    if (src.isNull())
        return QbPacket();

    src = src.convertToFormat(QImage::Format_ARGB32);
    QImage oFrame = QImage(src.size(), src.format());
    int nFrames = this->m_nFrames;

    for (int y = 0; y < src.height(); y++) {
        QRgb *oLine = (QRgb *) oFrame.scanLine(y);

        for (int x = 0; x < src.width(); x++) {
            int p = (x * nFrames) % src.width();
            int q = (y * nFrames) % src.height();
            int i = ((y * nFrames) / src.height()) * nFrames +
                    ((x * nFrames) / src.width());

            i = qBound(0, i, this->m_colorTable.size() - 1);
            const QRgb *iLine = (const QRgb *) src.constScanLine(q);
            oLine[x] = (iLine[p] ^ this->m_colorTable[i]) | 0xff000000;
        }
    }

    QbPacket oPacket = QbUtils::imageToPacket(oFrame, packet);
    qbSend(oPacket)
}
