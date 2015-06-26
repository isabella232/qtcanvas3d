/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtCanvas3D module of the Qt Toolkit.
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

#include "canvasrendernode_p.h"
#include "canvas3d_p.h"
#include "glcommandqueue_p.h"

#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLFramebufferObject>

QT_BEGIN_NAMESPACE
QT_CANVAS3D_BEGIN_NAMESPACE

CanvasRenderNode::CanvasRenderNode(QQuickWindow *window) :
    QObject(),
    QSGSimpleTextureNode(),
    m_texture(0),
    m_window(window)
{
    qCDebug(canvas3drendering).nospace() << "CanvasRenderNode::" << __FUNCTION__;

    // Our texture node must have a texture, so use the default 0 texture.
    m_texture = m_window->createTextureFromId(0, QSize(1, 1));
    setTexture(m_texture);
    setFiltering(QSGTexture::Linear);
    setTextureCoordinatesTransform(QSGSimpleTextureNode::MirrorVertically);
}

CanvasRenderNode::~CanvasRenderNode()
{
    delete m_texture;
}

// Called in render thread as a response to CanvasRenderer::textureReady signal.
void CanvasRenderNode::newTexture(int id, const QSize &size)
{
    qCDebug(canvas3drendering).nospace() << "CanvasRenderNode::" << __FUNCTION__
                                         << "(" << id << ", " << size << ")";
    if (id) {
        qCDebug(canvas3drendering).nospace() << "CanvasRenderNode::" << __FUNCTION__
                                             << " showing new texture:" << id
                                             << " size:" << size
                                             << " targetRect:" << rect();
        delete m_texture;
        m_texture = m_window->createTextureFromId(id, size);
        setTexture(m_texture);
        qCDebug(canvas3drendering).nospace() << "CanvasRenderNode::" << __FUNCTION__
                                             << " SGTexture size:" << m_texture->textureSize()
                                             << " normalizedTextureSubRect:"
                                             << m_texture->normalizedTextureSubRect();
    } else {
        qCDebug(canvas3drendering).nospace() << "CanvasRenderNode::" << __FUNCTION__
                                             << " showing previous texture";
    }
}

QT_CANVAS3D_END_NAMESPACE
QT_END_NAMESPACE