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

//
//  W A R N I N G
//  -------------
//
// This file is not part of the QtCanvas3D API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.

#ifndef CONTEXT3D_P_H
#define CONTEXT3D_P_H

#include "canvas3dcommon_p.h"
#include "contextattributes_p.h"
#include "abstractobject3d_p.h"
#include "canvasglstatedump_p.h"

#include <QtGui/QOpenGLFunctions>
#include <QtCore/QString>
#include <QtCore/QRect>
#include <QtCore/QSize>
#include <QtCore/QSet>
#include <QtCore/QVariantList>
#include <QtQuick/QQuickItem>
#include <QtQml>
#include <QtQml/QJSEngine>
#include <QtQml/private/qqmlengine_p.h>

#ifdef NO_ERROR // may de defined in winerror.h
#undef NO_ERROR
#endif

QT_BEGIN_NAMESPACE
QT_CANVAS3D_BEGIN_NAMESPACE

#define ENUM_AS_PROPERTY(A) Q_PROPERTY(QtCanvas3D::CanvasContext::glEnums A READ A ## _read); inline QtCanvas3D::CanvasContext::glEnums A ## _read() { return CanvasContext::A; }

class Canvas;
class CanvasActiveInfo;
class CanvasTexture;
class CanvasShader;
class CanvasFrameBuffer;
class CanvasRenderBuffer;
class CanvasProgram;
class CanvasBuffer;
class CanvasUniformLocation;
class CanvasTextureImage;
class CanvasShaderPrecisionFormat;
class EnumToStringMap;


class QT_CANVAS3D_EXPORT CanvasContext : public CanvasAbstractObject, protected QOpenGLFunctions
{
    Q_OBJECT
    Q_DISABLE_COPY(CanvasContext)

    Q_ENUMS(glEnums)

    Q_PROPERTY(Canvas *canvas READ canvas NOTIFY canvasChanged)
    Q_PROPERTY(uint drawingBufferWidth READ drawingBufferWidth NOTIFY drawingBufferWidthChanged)
    Q_PROPERTY(uint drawingBufferHeight READ drawingBufferHeight NOTIFY drawingBufferHeightChanged)

public:
    enum glEnums {
        /* ClearBufferMask */
        DEPTH_BUFFER_BIT               = 0x00000100,
        STENCIL_BUFFER_BIT             = 0x00000400,
        COLOR_BUFFER_BIT               = 0x00004000,

        // EXTRA
        DEPTH_AND_COLOR_BUFFER_BIT     = 0x00004100,
        DEPTH_AND_STENCIL_AND_COLOR_BUFFER_BIT     = 0x00004500,

        /* BeginMode */
        POINTS                         = 0x0000,
        LINES                          = 0x0001,
        LINE_LOOP                      = 0x0002,
        LINE_STRIP                     = 0x0003,
        TRIANGLES                      = 0x0004,
        TRIANGLE_STRIP                 = 0x0005,
        TRIANGLE_FAN                   = 0x0006,

        /* AlphaFunction (not supported in ES20) */
        /*      NEVER */
        /*      LESS */
        /*      EQUAL */
        /*      LEQUAL */
        /*      GREATER */
        /*      NOTEQUAL */
        /*      GEQUAL */
        /*      ALWAYS */

        /* BlendingFactorDest */
        ZERO                           = 0,
        ONE                            = 1,
        SRC_COLOR                      = 0x0300,
        ONE_MINUS_SRC_COLOR            = 0x0301,
        SRC_ALPHA                      = 0x0302,
        ONE_MINUS_SRC_ALPHA            = 0x0303,
        DST_ALPHA                      = 0x0304,
        ONE_MINUS_DST_ALPHA            = 0x0305,

        /* BlendingFactorSrc */
        /*      ZERO */
        /*      ONE */
        DST_COLOR                      = 0x0306,
        ONE_MINUS_DST_COLOR            = 0x0307,
        SRC_ALPHA_SATURATE             = 0x0308,
        /*      SRC_ALPHA */
        /*      ONE_MINUS_SRC_ALPHA */
        /*      DST_ALPHA */
        /*      ONE_MINUS_DST_ALPHA */

        /* BlendEquationSeparate */
        FUNC_ADD                       = 0x8006,
        BLEND_EQUATION                 = 0x8009,
        BLEND_EQUATION_RGB             = 0x8009,   /* same as BLEND_EQUATION */
        BLEND_EQUATION_ALPHA           = 0x883D,

        /* BlendSubtract */
        FUNC_SUBTRACT                  = 0x800A,
        FUNC_REVERSE_SUBTRACT          = 0x800B,

        /* Separate Blend Functions */
        BLEND_DST_RGB                  = 0x80C8,
        BLEND_SRC_RGB                  = 0x80C9,
        BLEND_DST_ALPHA                = 0x80CA,
        BLEND_SRC_ALPHA                = 0x80CB,
        CONSTANT_COLOR                 = 0x8001,
        ONE_MINUS_CONSTANT_COLOR       = 0x8002,
        CONSTANT_ALPHA                 = 0x8003,
        ONE_MINUS_CONSTANT_ALPHA       = 0x8004,
        BLEND_COLOR                    = 0x8005,

        /* Buffer Objects */
        ARRAY_BUFFER                   = 0x8892,
        ELEMENT_ARRAY_BUFFER           = 0x8893,
        ARRAY_BUFFER_BINDING           = 0x8894,
        ELEMENT_ARRAY_BUFFER_BINDING   = 0x8895,

        STREAM_DRAW                    = 0x88E0,
        STATIC_DRAW                    = 0x88E4,
        DYNAMIC_DRAW                   = 0x88E8,

        BUFFER_SIZE                    = 0x8764,
        BUFFER_USAGE                   = 0x8765,

        CURRENT_VERTEX_ATTRIB          = 0x8626,

        /* CullFaceMode */
        FRONT                          = 0x0404,
        BACK                           = 0x0405,
        FRONT_AND_BACK                 = 0x0408,

        /* DepthFunction */
        /*      NEVER */
        /*      LESS */
        /*      EQUAL */
        /*      LEQUAL */
        /*      GREATER */
        /*      NOTEQUAL */
        /*      GEQUAL */
        /*      ALWAYS */

        /* EnableCap */
        /* TEXTURE_2D */
        CULL_FACE                      = 0x0B44,
        BLEND                          = 0x0BE2,
        DITHER                         = 0x0BD0,
        STENCIL_TEST                   = 0x0B90,
        DEPTH_TEST                     = 0x0B71,
        SCISSOR_TEST                   = 0x0C11,
        POLYGON_OFFSET_FILL            = 0x8037,
        SAMPLE_ALPHA_TO_COVERAGE       = 0x809E,
        SAMPLE_COVERAGE                = 0x80A0,

        /* Error Codes */
        NO_ERROR                       = 0,
        INVALID_ENUM                   = 0x0500,
        INVALID_VALUE                  = 0x0501,
        INVALID_OPERATION              = 0x0502,
        OUT_OF_MEMORY                  = 0x0505,

        /* FrontFaceDirection */
        CW                             = 0x0900,
        CCW                            = 0x0901,

        /* GetPName */
        LINE_WIDTH                     = 0x0B21,
        ALIASED_POINT_SIZE_RANGE       = 0x846D,
        ALIASED_LINE_WIDTH_RANGE       = 0x846E,
        CULL_FACE_MODE                 = 0x0B45,
        FRONT_FACE                     = 0x0B46,
        DEPTH_RANGE                    = 0x0B70,
        DEPTH_WRITEMASK                = 0x0B72,
        DEPTH_CLEAR_VALUE              = 0x0B73,
        DEPTH_FUNC                     = 0x0B74,
        STENCIL_CLEAR_VALUE            = 0x0B91,
        STENCIL_FUNC                   = 0x0B92,
        STENCIL_FAIL                   = 0x0B94,
        STENCIL_PASS_DEPTH_FAIL        = 0x0B95,
        STENCIL_PASS_DEPTH_PASS        = 0x0B96,
        STENCIL_REF                    = 0x0B97,
        STENCIL_VALUE_MASK             = 0x0B93,
        STENCIL_WRITEMASK              = 0x0B98,
        STENCIL_BACK_FUNC              = 0x8800,
        STENCIL_BACK_FAIL              = 0x8801,
        STENCIL_BACK_PASS_DEPTH_FAIL   = 0x8802,
        STENCIL_BACK_PASS_DEPTH_PASS   = 0x8803,
        STENCIL_BACK_REF               = 0x8CA3,
        STENCIL_BACK_VALUE_MASK        = 0x8CA4,
        STENCIL_BACK_WRITEMASK         = 0x8CA5,
        VIEWPORT                       = 0x0BA2,
        SCISSOR_BOX                    = 0x0C10,
        /*      SCISSOR_TEST */
        COLOR_CLEAR_VALUE              = 0x0C22,
        COLOR_WRITEMASK                = 0x0C23,
        UNPACK_ALIGNMENT               = 0x0CF5,
        PACK_ALIGNMENT                 = 0x0D05,
        MAX_TEXTURE_SIZE               = 0x0D33,
        MAX_VIEWPORT_DIMS              = 0x0D3A,
        SUBPIXEL_BITS                  = 0x0D50,
        RED_BITS                       = 0x0D52,
        GREEN_BITS                     = 0x0D53,
        BLUE_BITS                      = 0x0D54,
        ALPHA_BITS                     = 0x0D55,
        DEPTH_BITS                     = 0x0D56,
        STENCIL_BITS                   = 0x0D57,
        POLYGON_OFFSET_UNITS           = 0x2A00,
        /*      POLYGON_OFFSET_FILL */
        POLYGON_OFFSET_FACTOR          = 0x8038,
        TEXTURE_BINDING_2D             = 0x8069,
        SAMPLE_BUFFERS                 = 0x80A8,
        SAMPLES                        = 0x80A9,
        SAMPLE_COVERAGE_VALUE          = 0x80AA,
        SAMPLE_COVERAGE_INVERT         = 0x80AB,

        /* GetTextureParameter */
        /*      TEXTURE_MAG_FILTER */
        /*      TEXTURE_MIN_FILTER */
        /*      TEXTURE_WRAP_S */
        /*      TEXTURE_WRAP_T */

        COMPRESSED_TEXTURE_FORMATS     = 0x86A3,

        /* HintMode */
        DONT_CARE                      = 0x1100,
        FASTEST                        = 0x1101,
        NICEST                         = 0x1102,

        /* HintTarget */
        GENERATE_MIPMAP_HINT            = 0x8192,

        /* DataType */
        BYTE                           = 0x1400,
        UNSIGNED_BYTE                  = 0x1401,
        SHORT                          = 0x1402,
        UNSIGNED_SHORT                 = 0x1403,
        INT                            = 0x1404,
        UNSIGNED_INT                   = 0x1405,
        FLOAT                          = 0x1406,

        /* PixelFormat */
        DEPTH_COMPONENT                = 0x1902,
        ALPHA                          = 0x1906,
        RGB                            = 0x1907,
        RGBA                           = 0x1908,
        LUMINANCE                      = 0x1909,
        LUMINANCE_ALPHA                = 0x190A,

        /* PixelType */
        /*      UNSIGNED_BYTE */
        UNSIGNED_SHORT_4_4_4_4         = 0x8033,
        UNSIGNED_SHORT_5_5_5_1         = 0x8034,
        UNSIGNED_SHORT_5_6_5           = 0x8363,

        /* Shaders */
        FRAGMENT_SHADER                  = 0x8B30,
        VERTEX_SHADER                    = 0x8B31,
        MAX_VERTEX_ATTRIBS               = 0x8869,
        MAX_VERTEX_UNIFORM_VECTORS       = 0x8DFB,
        MAX_VARYING_VECTORS              = 0x8DFC,
        MAX_COMBINED_TEXTURE_IMAGE_UNITS = 0x8B4D,
        MAX_VERTEX_TEXTURE_IMAGE_UNITS   = 0x8B4C,
        MAX_TEXTURE_IMAGE_UNITS          = 0x8872,
        MAX_FRAGMENT_UNIFORM_VECTORS     = 0x8DFD,
        SHADER_TYPE                      = 0x8B4F,
        DELETE_STATUS                    = 0x8B80,
        LINK_STATUS                      = 0x8B82,
        VALIDATE_STATUS                  = 0x8B83,
        ATTACHED_SHADERS                 = 0x8B85,
        ACTIVE_UNIFORMS                  = 0x8B86,
        ACTIVE_ATTRIBUTES                = 0x8B89,
        SHADING_LANGUAGE_VERSION         = 0x8B8C,
        CURRENT_PROGRAM                  = 0x8B8D,

        /* StencilFunction */
        NEVER                          = 0x0200,
        LESS                           = 0x0201,
        EQUAL                          = 0x0202,
        LEQUAL                         = 0x0203,
        GREATER                        = 0x0204,
        NOTEQUAL                       = 0x0205,
        GEQUAL                         = 0x0206,
        ALWAYS                         = 0x0207,

        /* StencilOp */
        /*      ZERO */
        KEEP                           = 0x1E00,
        REPLACE                        = 0x1E01,
        INCR                           = 0x1E02,
        DECR                           = 0x1E03,
        INVERT                         = 0x150A,
        INCR_WRAP                      = 0x8507,
        DECR_WRAP                      = 0x8508,

        /* StringName */
        VENDOR                         = 0x1F00,
        RENDERER                       = 0x1F01,
        VERSION                        = 0x1F02,

        /* TextureMagFilter */
        NEAREST                        = 0x2600,
        LINEAR                         = 0x2601,

        /* TextureMinFilter */
        /*      NEAREST */
        /*      LINEAR */
        NEAREST_MIPMAP_NEAREST         = 0x2700,
        LINEAR_MIPMAP_NEAREST          = 0x2701,
        NEAREST_MIPMAP_LINEAR          = 0x2702,
        LINEAR_MIPMAP_LINEAR           = 0x2703,

        /* TextureParameterName */
        TEXTURE_MAG_FILTER             = 0x2800,
        TEXTURE_MIN_FILTER             = 0x2801,
        TEXTURE_WRAP_S                 = 0x2802,
        TEXTURE_WRAP_T                 = 0x2803,

        /* TextureTarget */
        TEXTURE_2D                     = 0x0DE1,
        TEXTURE                        = 0x1702,

        TEXTURE_CUBE_MAP               = 0x8513,
        TEXTURE_BINDING_CUBE_MAP       = 0x8514,
        TEXTURE_CUBE_MAP_POSITIVE_X    = 0x8515,
        TEXTURE_CUBE_MAP_NEGATIVE_X    = 0x8516,
        TEXTURE_CUBE_MAP_POSITIVE_Y    = 0x8517,
        TEXTURE_CUBE_MAP_NEGATIVE_Y    = 0x8518,
        TEXTURE_CUBE_MAP_POSITIVE_Z    = 0x8519,
        TEXTURE_CUBE_MAP_NEGATIVE_Z    = 0x851A,
        MAX_CUBE_MAP_TEXTURE_SIZE      = 0x851C,

        /* TextureUnit */
        TEXTURE0                       = 0x84C0,
        TEXTURE1                       = 0x84C1,
        TEXTURE2                       = 0x84C2,
        TEXTURE3                       = 0x84C3,
        TEXTURE4                       = 0x84C4,
        TEXTURE5                       = 0x84C5,
        TEXTURE6                       = 0x84C6,
        TEXTURE7                       = 0x84C7,
        TEXTURE8                       = 0x84C8,
        TEXTURE9                       = 0x84C9,
        TEXTURE10                      = 0x84CA,
        TEXTURE11                      = 0x84CB,
        TEXTURE12                      = 0x84CC,
        TEXTURE13                      = 0x84CD,
        TEXTURE14                      = 0x84CE,
        TEXTURE15                      = 0x84CF,
        TEXTURE16                      = 0x84D0,
        TEXTURE17                      = 0x84D1,
        TEXTURE18                      = 0x84D2,
        TEXTURE19                      = 0x84D3,
        TEXTURE20                      = 0x84D4,
        TEXTURE21                      = 0x84D5,
        TEXTURE22                      = 0x84D6,
        TEXTURE23                      = 0x84D7,
        TEXTURE24                      = 0x84D8,
        TEXTURE25                      = 0x84D9,
        TEXTURE26                      = 0x84DA,
        TEXTURE27                      = 0x84DB,
        TEXTURE28                      = 0x84DC,
        TEXTURE29                      = 0x84DD,
        TEXTURE30                      = 0x84DE,
        TEXTURE31                      = 0x84DF,
        ACTIVE_TEXTURE                 = 0x84E0,

        /* TextureWrapMode */
        REPEAT                         = 0x2901,
        CLAMP_TO_EDGE                  = 0x812F,
        MIRRORED_REPEAT                = 0x8370,

        /* Uniform Types */
        FLOAT_VEC2                     = 0x8B50,
        FLOAT_VEC3                     = 0x8B51,
        FLOAT_VEC4                     = 0x8B52,
        INT_VEC2                       = 0x8B53,
        INT_VEC3                       = 0x8B54,
        INT_VEC4                       = 0x8B55,
        BOOL                           = 0x8B56,
        BOOL_VEC2                      = 0x8B57,
        BOOL_VEC3                      = 0x8B58,
        BOOL_VEC4                      = 0x8B59,
        FLOAT_MAT2                     = 0x8B5A,
        FLOAT_MAT3                     = 0x8B5B,
        FLOAT_MAT4                     = 0x8B5C,
        SAMPLER_2D                     = 0x8B5E,
        SAMPLER_CUBE                   = 0x8B60,

        /* Vertex Arrays */
        VERTEX_ATTRIB_ARRAY_ENABLED        = 0x8622,
        VERTEX_ATTRIB_ARRAY_SIZE           = 0x8623,
        VERTEX_ATTRIB_ARRAY_STRIDE         = 0x8624,
        VERTEX_ATTRIB_ARRAY_TYPE           = 0x8625,
        VERTEX_ATTRIB_ARRAY_NORMALIZED     = 0x886A,
        VERTEX_ATTRIB_ARRAY_POINTER        = 0x8645,
        VERTEX_ATTRIB_ARRAY_BUFFER_BINDING = 0x889F,

        /* Read Format */
        IMPLEMENTATION_COLOR_READ_TYPE   = 0x8B9A,
        IMPLEMENTATION_COLOR_READ_FORMAT = 0x8B9B,

        /* Shader Source */
        COMPILE_STATUS                 = 0x8B81,

        /* OES_standard_derivatives */
        FRAGMENT_SHADER_DERIVATIVE_HINT_OES = 0x8B8B,

        /* Shader Precision-Specified Types */
        LOW_FLOAT                      = 0x8DF0,
        MEDIUM_FLOAT                   = 0x8DF1,
        HIGH_FLOAT                     = 0x8DF2,
        LOW_INT                        = 0x8DF3,
        MEDIUM_INT                     = 0x8DF4,
        HIGH_INT                       = 0x8DF5,

        /* Framebuffer Object. */
        FRAMEBUFFER                    = 0x8D40,
        RENDERBUFFER                   = 0x8D41,

        RGBA4                          = 0x8056,
        RGB5_A1                        = 0x8057,
        RGB565                         = 0x8D62,
        DEPTH_COMPONENT16              = 0x81A5,
        STENCIL_INDEX                  = 0x1901,
        STENCIL_INDEX8                 = 0x8D48,
        DEPTH_STENCIL                  = 0x84F9,

        RENDERBUFFER_WIDTH             = 0x8D42,
        RENDERBUFFER_HEIGHT            = 0x8D43,
        RENDERBUFFER_INTERNAL_FORMAT   = 0x8D44,
        RENDERBUFFER_RED_SIZE          = 0x8D50,
        RENDERBUFFER_GREEN_SIZE        = 0x8D51,
        RENDERBUFFER_BLUE_SIZE         = 0x8D52,
        RENDERBUFFER_ALPHA_SIZE        = 0x8D53,
        RENDERBUFFER_DEPTH_SIZE        = 0x8D54,
        RENDERBUFFER_STENCIL_SIZE      = 0x8D55,

        FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE           = 0x8CD0,
        FRAMEBUFFER_ATTACHMENT_OBJECT_NAME           = 0x8CD1,
        FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL         = 0x8CD2,
        FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE = 0x8CD3,

        COLOR_ATTACHMENT0              = 0x8CE0,
        DEPTH_ATTACHMENT               = 0x8D00,
        STENCIL_ATTACHMENT             = 0x8D20,
        DEPTH_STENCIL_ATTACHMENT       = 0x821A,

        NONE                           = 0,

        FRAMEBUFFER_COMPLETE                      = 0x8CD5,
        FRAMEBUFFER_INCOMPLETE_ATTACHMENT         = 0x8CD6,
        FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT = 0x8CD7,
        FRAMEBUFFER_INCOMPLETE_DIMENSIONS         = 0x8CD9,
        FRAMEBUFFER_UNSUPPORTED                   = 0x8CDD,

        FRAMEBUFFER_BINDING            = 0x8CA6,
        RENDERBUFFER_BINDING           = 0x8CA7,
        MAX_RENDERBUFFER_SIZE          = 0x84E8,

        INVALID_FRAMEBUFFER_OPERATION  = 0x0506,

        /* WebGL-specific enums */
        UNPACK_FLIP_Y_WEBGL            = 0x9240,
        UNPACK_PREMULTIPLY_ALPHA_WEBGL = 0x9241,
        CONTEXT_LOST_WEBGL             = 0x9242,
        UNPACK_COLORSPACE_CONVERSION_WEBGL = 0x9243,
        BROWSER_DEFAULT_WEBGL          = 0x9244,

        /* WEBGL_debug_renderer_info */
        UNMASKED_VENDOR_WEBGL            = 0x9245,
        UNMASKED_RENDERER_WEBGL          = 0x9246
    };

    /* ClearBufferMask */
    ENUM_AS_PROPERTY(DEPTH_BUFFER_BIT)
    ENUM_AS_PROPERTY(STENCIL_BUFFER_BIT)
    ENUM_AS_PROPERTY(COLOR_BUFFER_BIT)

    /* BeginMode */
    ENUM_AS_PROPERTY(POINTS)
    ENUM_AS_PROPERTY(LINES)
    ENUM_AS_PROPERTY(LINE_LOOP)
    ENUM_AS_PROPERTY(LINE_STRIP)
    ENUM_AS_PROPERTY(TRIANGLES)
    ENUM_AS_PROPERTY(TRIANGLE_STRIP)
    ENUM_AS_PROPERTY(TRIANGLE_FAN)

    /* AlphaFunction (not supported in ES20) */
    /*      NEVER */
    /*      LESS */
    /*      EQUAL */
    /*      LEQUAL */
    /*      GREATER */
    /*      NOTEQUAL */
    /*      GEQUAL */
    /*      ALWAYS */

    /* BlendingFactorDest */
    ENUM_AS_PROPERTY(ZERO)
    ENUM_AS_PROPERTY(ONE)
    ENUM_AS_PROPERTY(SRC_COLOR)
    ENUM_AS_PROPERTY(ONE_MINUS_SRC_COLOR)
    ENUM_AS_PROPERTY(SRC_ALPHA)
    ENUM_AS_PROPERTY(ONE_MINUS_SRC_ALPHA)
    ENUM_AS_PROPERTY(DST_ALPHA)
    ENUM_AS_PROPERTY(ONE_MINUS_DST_ALPHA)

    /* BlendingFactorSrc */
    /*      ZERO */
    /*      ONE */
    ENUM_AS_PROPERTY(DST_COLOR)
    ENUM_AS_PROPERTY(ONE_MINUS_DST_COLOR)
    ENUM_AS_PROPERTY(SRC_ALPHA_SATURATE)
    /*      SRC_ALPHA */
    /*      ONE_MINUS_SRC_ALPHA */
    /*      DST_ALPHA */
    /*      ONE_MINUS_DST_ALPHA */

    /* BlendEquationSeparate */
    ENUM_AS_PROPERTY(FUNC_ADD)
    ENUM_AS_PROPERTY(BLEND_EQUATION)
    ENUM_AS_PROPERTY(BLEND_EQUATION_RGB)   /* same as BLEND_EQUATION */
    ENUM_AS_PROPERTY(BLEND_EQUATION_ALPHA)

    /* BlendSubtract */
    ENUM_AS_PROPERTY(FUNC_SUBTRACT)
    ENUM_AS_PROPERTY(FUNC_REVERSE_SUBTRACT)

    /* Separate Blend Functions */
    ENUM_AS_PROPERTY(BLEND_DST_RGB)
    ENUM_AS_PROPERTY(BLEND_SRC_RGB)
    ENUM_AS_PROPERTY(BLEND_DST_ALPHA)
    ENUM_AS_PROPERTY(BLEND_SRC_ALPHA)
    ENUM_AS_PROPERTY(CONSTANT_COLOR)
    ENUM_AS_PROPERTY(ONE_MINUS_CONSTANT_COLOR)
    ENUM_AS_PROPERTY(CONSTANT_ALPHA)
    ENUM_AS_PROPERTY(ONE_MINUS_CONSTANT_ALPHA)
    ENUM_AS_PROPERTY(BLEND_COLOR)

    /* Buffer Objects */
    ENUM_AS_PROPERTY(ARRAY_BUFFER)
    ENUM_AS_PROPERTY(ELEMENT_ARRAY_BUFFER)
    ENUM_AS_PROPERTY(ARRAY_BUFFER_BINDING)
    ENUM_AS_PROPERTY(ELEMENT_ARRAY_BUFFER_BINDING)

    ENUM_AS_PROPERTY(STREAM_DRAW)
    ENUM_AS_PROPERTY(STATIC_DRAW)
    ENUM_AS_PROPERTY(DYNAMIC_DRAW)

    ENUM_AS_PROPERTY(BUFFER_SIZE)
    ENUM_AS_PROPERTY(BUFFER_USAGE)

    ENUM_AS_PROPERTY(CURRENT_VERTEX_ATTRIB)

    /* CullFaceMode */
    ENUM_AS_PROPERTY(FRONT)
    ENUM_AS_PROPERTY(BACK)
    ENUM_AS_PROPERTY(FRONT_AND_BACK)

    /* DepthFunction */
    /*      NEVER */
    /*      LESS */
    /*      EQUAL */
    /*      LEQUAL */
    /*      GREATER */
    /*      NOTEQUAL */
    /*      GEQUAL */
    /*      ALWAYS */

    /* EnableCap */
    /* TEXTURE_2D */
    ENUM_AS_PROPERTY(CULL_FACE)
    ENUM_AS_PROPERTY(BLEND)
    ENUM_AS_PROPERTY(DITHER)
    ENUM_AS_PROPERTY(STENCIL_TEST)
    ENUM_AS_PROPERTY(DEPTH_TEST)
    ENUM_AS_PROPERTY(SCISSOR_TEST)
    ENUM_AS_PROPERTY(POLYGON_OFFSET_FILL)
    ENUM_AS_PROPERTY(SAMPLE_ALPHA_TO_COVERAGE)
    ENUM_AS_PROPERTY(SAMPLE_COVERAGE)

    /* ErrorCode */
    ENUM_AS_PROPERTY(NO_ERROR)
    ENUM_AS_PROPERTY(INVALID_ENUM)
    ENUM_AS_PROPERTY(INVALID_VALUE)
    ENUM_AS_PROPERTY(INVALID_OPERATION)
    ENUM_AS_PROPERTY(OUT_OF_MEMORY)

    /* FrontFaceDirection */
    ENUM_AS_PROPERTY(CW)
    ENUM_AS_PROPERTY(CCW)

    /* GetPName */
    ENUM_AS_PROPERTY(LINE_WIDTH)
    ENUM_AS_PROPERTY(ALIASED_POINT_SIZE_RANGE)
    ENUM_AS_PROPERTY(ALIASED_LINE_WIDTH_RANGE)
    ENUM_AS_PROPERTY(CULL_FACE_MODE)
    ENUM_AS_PROPERTY(FRONT_FACE)
    ENUM_AS_PROPERTY(DEPTH_RANGE)
    ENUM_AS_PROPERTY(DEPTH_WRITEMASK)
    ENUM_AS_PROPERTY(DEPTH_CLEAR_VALUE)
    ENUM_AS_PROPERTY(DEPTH_FUNC)
    ENUM_AS_PROPERTY(STENCIL_CLEAR_VALUE)
    ENUM_AS_PROPERTY(STENCIL_FUNC)
    ENUM_AS_PROPERTY(STENCIL_FAIL)
    ENUM_AS_PROPERTY(STENCIL_PASS_DEPTH_FAIL)
    ENUM_AS_PROPERTY(STENCIL_PASS_DEPTH_PASS)
    ENUM_AS_PROPERTY(STENCIL_REF)
    ENUM_AS_PROPERTY(STENCIL_VALUE_MASK)
    ENUM_AS_PROPERTY(STENCIL_WRITEMASK)
    ENUM_AS_PROPERTY(STENCIL_BACK_FUNC)
    ENUM_AS_PROPERTY(STENCIL_BACK_FAIL)
    ENUM_AS_PROPERTY(STENCIL_BACK_PASS_DEPTH_FAIL)
    ENUM_AS_PROPERTY(STENCIL_BACK_PASS_DEPTH_PASS)
    ENUM_AS_PROPERTY(STENCIL_BACK_REF)
    ENUM_AS_PROPERTY(STENCIL_BACK_VALUE_MASK)
    ENUM_AS_PROPERTY(STENCIL_BACK_WRITEMASK)
    ENUM_AS_PROPERTY(VIEWPORT)
    ENUM_AS_PROPERTY(SCISSOR_BOX)
    /*      SCISSOR_TEST */
    ENUM_AS_PROPERTY(COLOR_CLEAR_VALUE)
    ENUM_AS_PROPERTY(COLOR_WRITEMASK)
    ENUM_AS_PROPERTY(UNPACK_ALIGNMENT)
    ENUM_AS_PROPERTY(PACK_ALIGNMENT)
    ENUM_AS_PROPERTY(MAX_TEXTURE_SIZE)
    ENUM_AS_PROPERTY(MAX_VIEWPORT_DIMS)
    ENUM_AS_PROPERTY(SUBPIXEL_BITS)
    ENUM_AS_PROPERTY(RED_BITS)
    ENUM_AS_PROPERTY(GREEN_BITS)
    ENUM_AS_PROPERTY(BLUE_BITS)
    ENUM_AS_PROPERTY(ALPHA_BITS)
    ENUM_AS_PROPERTY(DEPTH_BITS)
    ENUM_AS_PROPERTY(STENCIL_BITS)
    ENUM_AS_PROPERTY(POLYGON_OFFSET_UNITS)
    /*      POLYGON_OFFSET_FILL */
    ENUM_AS_PROPERTY(POLYGON_OFFSET_FACTOR)
    ENUM_AS_PROPERTY(TEXTURE_BINDING_2D)
    ENUM_AS_PROPERTY(SAMPLE_BUFFERS)
    ENUM_AS_PROPERTY(SAMPLES)
    ENUM_AS_PROPERTY(SAMPLE_COVERAGE_VALUE)
    ENUM_AS_PROPERTY(SAMPLE_COVERAGE_INVERT)

    /* GetTextureParameter */
    /*      TEXTURE_MAG_FILTER */
    /*      TEXTURE_MIN_FILTER */
    /*      TEXTURE_WRAP_S */
    /*      TEXTURE_WRAP_T */

    ENUM_AS_PROPERTY(COMPRESSED_TEXTURE_FORMATS)

    /* HintMode */
    ENUM_AS_PROPERTY(DONT_CARE)
    ENUM_AS_PROPERTY(FASTEST)
    ENUM_AS_PROPERTY(NICEST)

    /* HintTarget */
    ENUM_AS_PROPERTY(GENERATE_MIPMAP_HINT)

    /* DataType */
    ENUM_AS_PROPERTY(BYTE)
    ENUM_AS_PROPERTY(UNSIGNED_BYTE)
    ENUM_AS_PROPERTY(SHORT)
    ENUM_AS_PROPERTY(UNSIGNED_SHORT)
    ENUM_AS_PROPERTY(INT)
    ENUM_AS_PROPERTY(UNSIGNED_INT)
    ENUM_AS_PROPERTY(FLOAT)

    /* PixelFormat */
    ENUM_AS_PROPERTY(DEPTH_COMPONENT)
    ENUM_AS_PROPERTY(ALPHA)
    ENUM_AS_PROPERTY(RGB)
    ENUM_AS_PROPERTY(RGBA)
    ENUM_AS_PROPERTY(LUMINANCE)
    ENUM_AS_PROPERTY(LUMINANCE_ALPHA)

    /* PixelType */
    /*      UNSIGNED_BYTE */
    ENUM_AS_PROPERTY(UNSIGNED_SHORT_4_4_4_4)
    ENUM_AS_PROPERTY(UNSIGNED_SHORT_5_5_5_1)
    ENUM_AS_PROPERTY(UNSIGNED_SHORT_5_6_5)

    /* Shaders */
    ENUM_AS_PROPERTY(FRAGMENT_SHADER)
    ENUM_AS_PROPERTY(VERTEX_SHADER)
    ENUM_AS_PROPERTY(MAX_VERTEX_ATTRIBS)
    ENUM_AS_PROPERTY(MAX_VERTEX_UNIFORM_VECTORS)
    ENUM_AS_PROPERTY(MAX_VARYING_VECTORS)
    ENUM_AS_PROPERTY(MAX_COMBINED_TEXTURE_IMAGE_UNITS)
    ENUM_AS_PROPERTY(MAX_VERTEX_TEXTURE_IMAGE_UNITS)
    ENUM_AS_PROPERTY(MAX_TEXTURE_IMAGE_UNITS)
    ENUM_AS_PROPERTY(MAX_FRAGMENT_UNIFORM_VECTORS)
    ENUM_AS_PROPERTY(SHADER_TYPE)
    ENUM_AS_PROPERTY(DELETE_STATUS)
    ENUM_AS_PROPERTY(LINK_STATUS)
    ENUM_AS_PROPERTY(VALIDATE_STATUS)
    ENUM_AS_PROPERTY(ATTACHED_SHADERS)
    ENUM_AS_PROPERTY(ACTIVE_UNIFORMS)
    ENUM_AS_PROPERTY(ACTIVE_ATTRIBUTES)
    ENUM_AS_PROPERTY(SHADING_LANGUAGE_VERSION)
    ENUM_AS_PROPERTY(CURRENT_PROGRAM)

    /* StencilFunction */
    ENUM_AS_PROPERTY(NEVER)
    ENUM_AS_PROPERTY(LESS)
    ENUM_AS_PROPERTY(EQUAL)
    ENUM_AS_PROPERTY(LEQUAL)
    ENUM_AS_PROPERTY(GREATER)
    ENUM_AS_PROPERTY(NOTEQUAL)
    ENUM_AS_PROPERTY(GEQUAL)
    ENUM_AS_PROPERTY(ALWAYS)

    /* StencilOp */
    /*      ZERO */
    ENUM_AS_PROPERTY(KEEP)
    ENUM_AS_PROPERTY(REPLACE)
    ENUM_AS_PROPERTY(INCR)
    ENUM_AS_PROPERTY(DECR)
    ENUM_AS_PROPERTY(INVERT)
    ENUM_AS_PROPERTY(INCR_WRAP)
    ENUM_AS_PROPERTY(DECR_WRAP)

    /* StringName */
    ENUM_AS_PROPERTY(VENDOR)
    ENUM_AS_PROPERTY(RENDERER)
    ENUM_AS_PROPERTY(VERSION)

    /* TextureMagFilter */
    ENUM_AS_PROPERTY(NEAREST)
    ENUM_AS_PROPERTY(LINEAR)

    /* TextureMinFilter */
    /*      NEAREST */
    /*      LINEAR */
    ENUM_AS_PROPERTY(NEAREST_MIPMAP_NEAREST)
    ENUM_AS_PROPERTY(LINEAR_MIPMAP_NEAREST)
    ENUM_AS_PROPERTY(NEAREST_MIPMAP_LINEAR)
    ENUM_AS_PROPERTY(LINEAR_MIPMAP_LINEAR)

    /* TextureParameterName */
    ENUM_AS_PROPERTY(TEXTURE_MAG_FILTER)
    ENUM_AS_PROPERTY(TEXTURE_MIN_FILTER)
    ENUM_AS_PROPERTY(TEXTURE_WRAP_S)
    ENUM_AS_PROPERTY(TEXTURE_WRAP_T)

    /* TextureTarget */
    ENUM_AS_PROPERTY(TEXTURE_2D)
    ENUM_AS_PROPERTY(TEXTURE)

    ENUM_AS_PROPERTY(TEXTURE_CUBE_MAP)
    ENUM_AS_PROPERTY(TEXTURE_BINDING_CUBE_MAP)
    ENUM_AS_PROPERTY(TEXTURE_CUBE_MAP_POSITIVE_X)
    ENUM_AS_PROPERTY(TEXTURE_CUBE_MAP_NEGATIVE_X)
    ENUM_AS_PROPERTY(TEXTURE_CUBE_MAP_POSITIVE_Y)
    ENUM_AS_PROPERTY(TEXTURE_CUBE_MAP_NEGATIVE_Y)
    ENUM_AS_PROPERTY(TEXTURE_CUBE_MAP_POSITIVE_Z)
    ENUM_AS_PROPERTY(TEXTURE_CUBE_MAP_NEGATIVE_Z)
    ENUM_AS_PROPERTY(MAX_CUBE_MAP_TEXTURE_SIZE)

    /* TextureUnit */
    ENUM_AS_PROPERTY(TEXTURE0)
    ENUM_AS_PROPERTY(TEXTURE1)
    ENUM_AS_PROPERTY(TEXTURE2)
    ENUM_AS_PROPERTY(TEXTURE3)
    ENUM_AS_PROPERTY(TEXTURE4)
    ENUM_AS_PROPERTY(TEXTURE5)
    ENUM_AS_PROPERTY(TEXTURE6)
    ENUM_AS_PROPERTY(TEXTURE7)
    ENUM_AS_PROPERTY(TEXTURE8)
    ENUM_AS_PROPERTY(TEXTURE9)
    ENUM_AS_PROPERTY(TEXTURE10)
    ENUM_AS_PROPERTY(TEXTURE11)
    ENUM_AS_PROPERTY(TEXTURE12)
    ENUM_AS_PROPERTY(TEXTURE13)
    ENUM_AS_PROPERTY(TEXTURE14)
    ENUM_AS_PROPERTY(TEXTURE15)
    ENUM_AS_PROPERTY(TEXTURE16)
    ENUM_AS_PROPERTY(TEXTURE17)
    ENUM_AS_PROPERTY(TEXTURE18)
    ENUM_AS_PROPERTY(TEXTURE19)
    ENUM_AS_PROPERTY(TEXTURE20)
    ENUM_AS_PROPERTY(TEXTURE21)
    ENUM_AS_PROPERTY(TEXTURE22)
    ENUM_AS_PROPERTY(TEXTURE23)
    ENUM_AS_PROPERTY(TEXTURE24)
    ENUM_AS_PROPERTY(TEXTURE25)
    ENUM_AS_PROPERTY(TEXTURE26)
    ENUM_AS_PROPERTY(TEXTURE27)
    ENUM_AS_PROPERTY(TEXTURE28)
    ENUM_AS_PROPERTY(TEXTURE29)
    ENUM_AS_PROPERTY(TEXTURE30)
    ENUM_AS_PROPERTY(TEXTURE31)
    ENUM_AS_PROPERTY(ACTIVE_TEXTURE)

    /* TextureWrapMode */
    ENUM_AS_PROPERTY(REPEAT)
    ENUM_AS_PROPERTY(CLAMP_TO_EDGE)
    ENUM_AS_PROPERTY(MIRRORED_REPEAT)

    /* Uniform Types */
    ENUM_AS_PROPERTY(FLOAT_VEC2)
    ENUM_AS_PROPERTY(FLOAT_VEC3)
    ENUM_AS_PROPERTY(FLOAT_VEC4)
    ENUM_AS_PROPERTY(INT_VEC2)
    ENUM_AS_PROPERTY(INT_VEC3)
    ENUM_AS_PROPERTY(INT_VEC4)
    ENUM_AS_PROPERTY(BOOL)
    ENUM_AS_PROPERTY(BOOL_VEC2)
    ENUM_AS_PROPERTY(BOOL_VEC3)
    ENUM_AS_PROPERTY(BOOL_VEC4)
    ENUM_AS_PROPERTY(FLOAT_MAT2)
    ENUM_AS_PROPERTY(FLOAT_MAT3)
    ENUM_AS_PROPERTY(FLOAT_MAT4)
    ENUM_AS_PROPERTY(SAMPLER_2D)
    ENUM_AS_PROPERTY(SAMPLER_CUBE)

    /* Vertex Arrays */
    ENUM_AS_PROPERTY(VERTEX_ATTRIB_ARRAY_ENABLED)
    ENUM_AS_PROPERTY(VERTEX_ATTRIB_ARRAY_SIZE)
    ENUM_AS_PROPERTY(VERTEX_ATTRIB_ARRAY_STRIDE)
    ENUM_AS_PROPERTY(VERTEX_ATTRIB_ARRAY_TYPE)
    ENUM_AS_PROPERTY(VERTEX_ATTRIB_ARRAY_NORMALIZED)
    ENUM_AS_PROPERTY(VERTEX_ATTRIB_ARRAY_POINTER)
    ENUM_AS_PROPERTY(VERTEX_ATTRIB_ARRAY_BUFFER_BINDING)

    /* Read Format */
    ENUM_AS_PROPERTY(IMPLEMENTATION_COLOR_READ_TYPE)
    ENUM_AS_PROPERTY(IMPLEMENTATION_COLOR_READ_FORMAT)

    /* Shader Source */
    ENUM_AS_PROPERTY(COMPILE_STATUS)

    /* Shader Precision-Specified Types */
    ENUM_AS_PROPERTY(LOW_FLOAT)
    ENUM_AS_PROPERTY(MEDIUM_FLOAT)
    ENUM_AS_PROPERTY(HIGH_FLOAT)
    ENUM_AS_PROPERTY(LOW_INT)
    ENUM_AS_PROPERTY(MEDIUM_INT)
    ENUM_AS_PROPERTY(HIGH_INT)

    /* Framebuffer Object. */
    ENUM_AS_PROPERTY(FRAMEBUFFER)
    ENUM_AS_PROPERTY(RENDERBUFFER)

    ENUM_AS_PROPERTY(RGBA4)
    ENUM_AS_PROPERTY(RGB5_A1)
    ENUM_AS_PROPERTY(RGB565)
    ENUM_AS_PROPERTY(DEPTH_COMPONENT16)
    ENUM_AS_PROPERTY(STENCIL_INDEX)
    ENUM_AS_PROPERTY(STENCIL_INDEX8)
    ENUM_AS_PROPERTY(DEPTH_STENCIL)

    ENUM_AS_PROPERTY(RENDERBUFFER_WIDTH)
    ENUM_AS_PROPERTY(RENDERBUFFER_HEIGHT)
    ENUM_AS_PROPERTY(RENDERBUFFER_INTERNAL_FORMAT)
    ENUM_AS_PROPERTY(RENDERBUFFER_RED_SIZE)
    ENUM_AS_PROPERTY(RENDERBUFFER_GREEN_SIZE)
    ENUM_AS_PROPERTY(RENDERBUFFER_BLUE_SIZE)
    ENUM_AS_PROPERTY(RENDERBUFFER_ALPHA_SIZE)
    ENUM_AS_PROPERTY(RENDERBUFFER_DEPTH_SIZE)
    ENUM_AS_PROPERTY(RENDERBUFFER_STENCIL_SIZE)

    ENUM_AS_PROPERTY(FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE)
    ENUM_AS_PROPERTY(FRAMEBUFFER_ATTACHMENT_OBJECT_NAME)
    ENUM_AS_PROPERTY(FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL)
    ENUM_AS_PROPERTY(FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE)

    ENUM_AS_PROPERTY(COLOR_ATTACHMENT0)
    ENUM_AS_PROPERTY(DEPTH_ATTACHMENT)
    ENUM_AS_PROPERTY(STENCIL_ATTACHMENT)
    ENUM_AS_PROPERTY(DEPTH_STENCIL_ATTACHMENT)

    ENUM_AS_PROPERTY(NONE)

    ENUM_AS_PROPERTY(FRAMEBUFFER_COMPLETE)
    ENUM_AS_PROPERTY(FRAMEBUFFER_INCOMPLETE_ATTACHMENT)
    ENUM_AS_PROPERTY(FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT)
    ENUM_AS_PROPERTY(FRAMEBUFFER_INCOMPLETE_DIMENSIONS)
    ENUM_AS_PROPERTY(FRAMEBUFFER_UNSUPPORTED)

    ENUM_AS_PROPERTY(FRAMEBUFFER_BINDING)
    ENUM_AS_PROPERTY(RENDERBUFFER_BINDING)
    ENUM_AS_PROPERTY(MAX_RENDERBUFFER_SIZE)

    ENUM_AS_PROPERTY(INVALID_FRAMEBUFFER_OPERATION)

    /* WebGL-specific enums */
    ENUM_AS_PROPERTY(UNPACK_FLIP_Y_WEBGL)
    ENUM_AS_PROPERTY(UNPACK_PREMULTIPLY_ALPHA_WEBGL)
    ENUM_AS_PROPERTY(CONTEXT_LOST_WEBGL)
    ENUM_AS_PROPERTY(UNPACK_COLORSPACE_CONVERSION_WEBGL)
    ENUM_AS_PROPERTY(BROWSER_DEFAULT_WEBGL)

    CanvasContext(QOpenGLContext *context, QSurface *surface, QQmlEngine *engine,
                  int width, int height, bool isES2, QObject *parent = 0);
    ~CanvasContext();

    void setCanvas(Canvas *canvas);
    Canvas *canvas();

    uint drawingBufferWidth();
    uint drawingBufferHeight();

    Q_INVOKABLE QVariantList getSupportedExtensions();
    Q_INVOKABLE QVariant getExtension(const QString &name);

    Q_INVOKABLE bool isContextLost();
    Q_INVOKABLE QJSValue getContextAttributes();
    void setContextAttributes(const CanvasContextAttributes &attribs);

    Q_INVOKABLE QJSValue getShaderPrecisionFormat(glEnums shadertype, glEnums precisiontype);

    Q_INVOKABLE void flush();
    Q_INVOKABLE void finish();

    Q_INVOKABLE QJSValue createTexture();
    Q_INVOKABLE void deleteTexture(QJSValue texture);
    Q_INVOKABLE void activeTexture(glEnums texture);
    Q_INVOKABLE void bindTexture(glEnums target, QJSValue texture);
    Q_INVOKABLE void generateMipmap(glEnums target);
    Q_INVOKABLE bool isTexture(QJSValue anyObject);
    Q_INVOKABLE void compressedTexImage2D(glEnums target, int level, glEnums internalformat,
                                          int width, int height, int border,
                                          QJSValue pixels);
    Q_INVOKABLE void compressedTexSubImage2D(glEnums target, int level,
                                             int xoffset, int yoffset,
                                             int width, int height,
                                             glEnums format,
                                             QJSValue pixels);

    Q_INVOKABLE void copyTexImage2D(glEnums target, int level,
                                    glEnums internalformat,
                                    int x, int y, int width, int height,
                                    int border);
    Q_INVOKABLE void copyTexSubImage2D(glEnums target, int level,
                                       int xoffset, int yoffset,
                                       int x, int y,
                                       int width, int height);

    Q_INVOKABLE void texImage2D(glEnums target, int level, glEnums internalformat,
                                int width, int height, int border,
                                glEnums format, glEnums type,
                                QJSValue pixels);
    Q_INVOKABLE void texSubImage2D(glEnums target, int level,
                                   int xoffset, int yoffset,
                                   int width, int height,
                                   glEnums format, glEnums type,
                                   QJSValue pixels);
    Q_INVOKABLE void texImage2D(glEnums target, int level, glEnums internalformat,
                                glEnums format, glEnums type, QJSValue image);
    Q_INVOKABLE void texSubImage2D(glEnums target, int level,
                                   int xoffset, int yoffset,
                                   glEnums format, glEnums type, QJSValue image);
    Q_INVOKABLE void texParameterf(glEnums target, glEnums pname, float param);
    Q_INVOKABLE void texParameteri(glEnums target, glEnums pname, int param);

    Q_INVOKABLE QJSValue createFramebuffer();
    Q_INVOKABLE void deleteFramebuffer(QJSValue buffer);
    Q_INVOKABLE void bindFramebuffer(glEnums target, QJSValue framebuffer);
    Q_INVOKABLE glEnums checkFramebufferStatus(glEnums target);
    Q_INVOKABLE void framebufferRenderbuffer(glEnums target, glEnums attachment,
                                             glEnums renderbuffertarget,
                                             QJSValue renderbuffer);
    Q_INVOKABLE void framebufferTexture2D(glEnums target, glEnums attachment, glEnums textarget,
                                          QJSValue texture, int level);
    Q_INVOKABLE bool isFramebuffer(QJSValue anyObject);

    Q_INVOKABLE QJSValue createRenderbuffer();
    Q_INVOKABLE void bindRenderbuffer(glEnums target, QJSValue renderbuffer);
    Q_INVOKABLE void renderbufferStorage(glEnums target, glEnums internalformat, int width,
                                         int height);
    Q_INVOKABLE bool isRenderbuffer(QJSValue anyObject);
    Q_INVOKABLE void deleteRenderbuffer(QJSValue renderbuffer);

    Q_INVOKABLE void sampleCoverage(float value, bool invert);

    Q_INVOKABLE QJSValue createShader(glEnums type);
    Q_INVOKABLE bool isShader(QJSValue shader3D);
    Q_INVOKABLE void deleteShader(QJSValue shader3D);
    Q_INVOKABLE void shaderSource(QJSValue shader3D, const QString &shaderSource);
    Q_INVOKABLE QJSValue getShaderSource(QJSValue shader3D);
    Q_INVOKABLE void compileShader(QJSValue shader3D);

    Q_INVOKABLE void uniform1f(QJSValue location, float x);
    Q_INVOKABLE void uniform1i(QJSValue location, int x);
    Q_INVOKABLE void uniform2f(QJSValue location, float x, float y);
    Q_INVOKABLE void uniform2i(QJSValue location, int x, int y);
    Q_INVOKABLE void uniform3f(QJSValue location, float x, float y, float z);
    Q_INVOKABLE void uniform3i(QJSValue location, int x, int y, int z);
    Q_INVOKABLE void uniform4f(QJSValue location, float x, float y, float z, float w);
    Q_INVOKABLE void uniform4i(QJSValue location, int x, int y, int z, int w);

    Q_INVOKABLE void uniform1fv(QJSValue location, QJSValue array);
    Q_INVOKABLE void uniform1iv(QJSValue location, QJSValue array);
    Q_INVOKABLE void uniform2fv(QJSValue location, QJSValue array);
    Q_INVOKABLE void uniform2iv(QJSValue location, QJSValue array);
    Q_INVOKABLE void uniform3fv(QJSValue location, QJSValue array);
    Q_INVOKABLE void uniform3iv(QJSValue location, QJSValue array);
    Q_INVOKABLE void uniform4fv(QJSValue location, QJSValue array);
    Q_INVOKABLE void uniform4iv(QJSValue location, QJSValue array);

    Q_INVOKABLE void uniformMatrix2fv(QJSValue location, bool transpose, QJSValue value);
    Q_INVOKABLE void uniformMatrix3fv(QJSValue location, bool transpose, QJSValue value);
    Q_INVOKABLE void uniformMatrix4fv(QJSValue location, bool transpose, QJSValue value);

    Q_INVOKABLE QJSValue createProgram();
    Q_INVOKABLE void attachShader(QJSValue program, QJSValue shader3D);
    Q_INVOKABLE void detachShader(QJSValue program, QJSValue shader3D);
    Q_INVOKABLE void linkProgram(QJSValue program);
    Q_INVOKABLE void useProgram(QJSValue program);
    Q_INVOKABLE void validateProgram(QJSValue program);
    Q_INVOKABLE QJSValue getUniformLocation(QJSValue program,
                                                          const QString &name);
    Q_INVOKABLE int getAttribLocation(QJSValue program, const QString &name);
    Q_INVOKABLE void bindAttribLocation(QJSValue program, int index, const QString &name);
    Q_INVOKABLE QJSValue getProgramInfoLog(QJSValue program) const;
    Q_INVOKABLE bool isProgram(QJSValue anyObject) const;
    Q_INVOKABLE void deleteProgram(QJSValue program);

    Q_INVOKABLE void disableVertexAttribArray(int index);
    Q_INVOKABLE void enableVertexAttribArray(int index);
    Q_INVOKABLE void vertexAttribPointer(int indx, int size, glEnums type,
                                         bool normalized, int stride, long offset);
    Q_INVOKABLE void vertexAttrib1f(unsigned int indx, float x);
    Q_INVOKABLE void vertexAttrib2f(unsigned int indx, float x, float y);
    Q_INVOKABLE void vertexAttrib3f(unsigned int indx, float x, float y, float z);
    Q_INVOKABLE void vertexAttrib4f(unsigned int indx, float x, float y, float z, float w);

    Q_INVOKABLE void vertexAttrib1fv(unsigned int indx, QJSValue array);
    Q_INVOKABLE void vertexAttrib2fv(unsigned int indx, QJSValue array);
    Q_INVOKABLE void vertexAttrib3fv(unsigned int indx, QJSValue array);
    Q_INVOKABLE void vertexAttrib4fv(unsigned int indx, QJSValue array);

    Q_INVOKABLE int getShaderParameter(QJSValue shader3D, glEnums paramName);
    Q_INVOKABLE QVariant getProgramParameter(QJSValue program, glEnums paramName);
    Q_INVOKABLE QJSValue getShaderInfoLog(QJSValue shader3D) const;

    /* Buffer object methods */
    Q_INVOKABLE QJSValue createBuffer();
    Q_INVOKABLE void bindBuffer(glEnums target, QJSValue buffer);
    Q_INVOKABLE void bufferData(glEnums target, long size, glEnums hint);
    Q_INVOKABLE void bufferData(glEnums target, QJSValue data, glEnums hint);
    Q_INVOKABLE void bufferSubData(glEnums target, int offset, QJSValue data);

    Q_INVOKABLE QJSValue getBufferParameter(glEnums target, glEnums pname);
    Q_INVOKABLE bool isBuffer(QJSValue value);
    Q_INVOKABLE void deleteBuffer(QJSValue buffer);

    Q_INVOKABLE glEnums getError();

    Q_INVOKABLE void cullFace(glEnums mode);
    Q_INVOKABLE void frontFace(glEnums mode);

    Q_INVOKABLE void lineWidth(float width);
    Q_INVOKABLE void pixelStorei(glEnums pname, int param);
    Q_INVOKABLE void polygonOffset(float factor, float units);
    Q_INVOKABLE void hint(glEnums target, glEnums mode);
    Q_INVOKABLE void enable(glEnums cap);
    Q_INVOKABLE bool isEnabled(glEnums cap);
    Q_INVOKABLE void disable(glEnums cap);

    Q_INVOKABLE QJSValue getParameter(glEnums pname);

    Q_INVOKABLE void clear(glEnums flags);
    Q_INVOKABLE void depthMask(bool flag);
    Q_INVOKABLE void depthFunc(glEnums func);
    Q_INVOKABLE void depthRange(float zNear, float zFar);
    Q_INVOKABLE void clearDepth(float depth);
    Q_INVOKABLE void clearColor(float red, float green, float blue, float alpha);
    Q_INVOKABLE void clearStencil(int s);
    Q_INVOKABLE void colorMask(bool red, bool green, bool blue, bool alpha);

    Q_INVOKABLE void blendColor(float red, float green, float blue, float alpha);
    Q_INVOKABLE void blendEquation(glEnums mode);
    Q_INVOKABLE void blendEquationSeparate(glEnums modeRGB, glEnums modeAlpha);
    Q_INVOKABLE void blendFunc(glEnums sfactor, glEnums dfactor);
    Q_INVOKABLE void blendFuncSeparate(glEnums srcRGB, glEnums dstRGB, glEnums srcAlpha,
                                       glEnums dstAlpha);

    Q_INVOKABLE void drawArrays(glEnums type, int first, int count);
    Q_INVOKABLE void drawElements(glEnums mode, int count, glEnums type, long offset);

    Q_INVOKABLE void viewport(int x, int y, int width, int height);

    Q_INVOKABLE void scissor(int x, int y, int width, int height);

    Q_INVOKABLE void readPixels(int x, int y, long width, long height, glEnums format,
                                glEnums type, QJSValue pixels);

    Q_INVOKABLE CanvasActiveInfo *getActiveAttrib(QJSValue program, uint index);
    Q_INVOKABLE CanvasActiveInfo *getActiveUniform(QJSValue program, uint index);
    Q_INVOKABLE QVariantList getAttachedShaders(QJSValue program);

    Q_INVOKABLE void stencilFunc(glEnums func, int ref, uint mask);
    Q_INVOKABLE void stencilFuncSeparate(glEnums face, glEnums func, int ref, uint mask);
    Q_INVOKABLE void stencilMask(uint mask);
    Q_INVOKABLE void stencilMaskSeparate(glEnums face, uint mask);
    Q_INVOKABLE void stencilOp(glEnums fail, glEnums zfail, glEnums zpass);
    Q_INVOKABLE void stencilOpSeparate(glEnums face, glEnums fail, glEnums zfail, glEnums zpass);

    Q_INVOKABLE int getFramebufferAttachmentParameter(glEnums target,
                                                      glEnums attachment,
                                                      glEnums pname);
    Q_INVOKABLE int getRenderbufferParameter(glEnums target, glEnums pname);
    Q_INVOKABLE QVariant getTexParameter(glEnums target, glEnums pname);
    Q_INVOKABLE QVariant getUniform(QJSValue program, QJSValue location);
    Q_INVOKABLE uint getVertexAttribOffset(uint index, glEnums pname);
    Q_INVOKABLE QJSValue getVertexAttrib(uint index, glEnums pname);

signals:
    void canvasChanged(Canvas *canvas);
    void drawingBufferWidthChanged();
    void drawingBufferHeightChanged();

private:
    uchar *getAsUint8ArrayRawPtr(QJSValue value);
    uchar *getAsUint16ArrayRawPtr(QJSValue value);
    CanvasTexture *getAsTexture3D(QJSValue anyObject);
    CanvasTextureImage* getAsTextureImage(QJSValue image);
    CanvasFrameBuffer *getAsFramebuffer(QJSValue anyObject);
    CanvasRenderBuffer *getAsRenderbuffer3D(QJSValue anyObject) const;
    CanvasShader *getAsShader3D(QJSValue shader3D) const;
    CanvasUniformLocation *getAsUniformLocation3D(QJSValue anyObject) const;
    CanvasProgram *getAsProgram3D(QJSValue anyObject) const;
    CanvasBuffer *getAsBuffer3D(QJSValue value) const;

    void uniform1fva(CanvasUniformLocation *location, QVariantList array);
    void uniform2fva(CanvasUniformLocation *location, QVariantList array);
    void uniform3fva(CanvasUniformLocation *location, QVariantList array);
    void uniform4fva(CanvasUniformLocation *location, QVariantList array);
    void uniform1iva(CanvasUniformLocation *location, QVariantList array);
    void uniform2iva(CanvasUniformLocation *location, QVariantList array);
    void uniform3iva(CanvasUniformLocation *location, QVariantList array);
    void uniform4iva(CanvasUniformLocation *location, QVariantList array);

    void uniformMatrix2fva(CanvasUniformLocation *location, bool transpose, QVariantList value);
    void uniformMatrix3fva(CanvasUniformLocation *location, bool transpose, QVariantList value);
    void uniformMatrix4fva(CanvasUniformLocation *location, bool transpose, QVariantList value);

    void vertexAttrib1fva(uint indx, QVariantList values);
    void vertexAttrib2fva(uint indx, QVariantList values);
    void vertexAttrib3fva(uint indx, QVariantList values);
    void vertexAttrib4fva(uint indx, QVariantList values);

    QString glEnumToString(glEnums value) const;
    float devicePixelRatio();
    void setDevicePixelRatio(float ratio);
    int getSufficientSize(glEnums internalFormat, int width, int height);

    QRect glViewportRect() const;
    GLuint currentFramebuffer();
    void logAllGLErrors(const QString &funcName);

    uchar *unpackPixels(uchar *srcData, bool useSrcDataAsDst,
                        int bytesPerPixel, int width, int height);

    bool isOfType(const QJSValue &value, const QString &classname) const;

    typedef enum {
        CANVAS_NO_ERRORS = 0,
        CANVAS_INVALID_ENUM = 1 << 0,
        CANVAS_INVALID_VALUE = 1 << 1,
        CANVAS_INVALID_OPERATION = 1 << 2,
        CANVAS_OUT_OF_MEMORY = 1 << 3,
        CANVAS_INVALID_FRAMEBUFFER_OPERATION = 1 << 4
    } errorBits;

private:
    QQmlEngine *m_engine;
    QV4::ExecutionEngine *m_v4engine;
    bool m_unpackFlipYEnabled;
    bool m_unpackPremultiplyAlphaEnabled;
    QRect m_glViewportRect;
    qreal m_devicePixelRatio;
    CanvasProgram *m_currentProgram;
    CanvasBuffer *m_currentArrayBuffer;
    CanvasBuffer *m_currentElementArrayBuffer;
    CanvasTexture *m_currentTexture2D;
    CanvasTexture *m_currentTextureCubeMap;
    CanvasFrameBuffer *m_currentFramebuffer;
    CanvasRenderBuffer *m_currentRenderbuffer;
    QOpenGLContext *m_context;
    QSet<QByteArray> m_extensions;
    QSurface *m_surface;
    CanvasContextAttributes m_contextAttributes;
    QMap<int, CanvasBuffer*> m_idToCanvasBufferMap;
    friend class Canvas;
    friend class QFBOCanvas3D;
    QString m_emptyString;
    int m_error;
    EnumToStringMap *m_map;
    Canvas *m_canvas;
    uint m_maxVertexAttribs;
    float **m_vertexAttribPointers;
    bool m_isOpenGLES2;

    bool invalidEnumFlag;
    bool invalidValueFlag;
    bool invalidOperationFlag;
    bool invalidStackOverflowFlag;
    bool invalidStackUnderflowFlag;
    bool invalidOutOfMemoryFlag;
    bool invalidFramebufferFlag;
    bool invalidContextLostFlag;

    // EXTENSIONS
    CanvasGLStateDump *m_stateDumpExt;
    QObject *m_standardDerivatives;
};

QT_CANVAS3D_END_NAMESPACE
QT_END_NAMESPACE

#endif // CONTEXT3D_P_H