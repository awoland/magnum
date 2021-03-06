#ifndef Magnum_Text_Text_h
#define Magnum_Text_Text_h
/*
    This file is part of Magnum.

    Copyright © 2010, 2011, 2012, 2013 Vladimír Vondruš <mosra@centrum.cz>

    Permission is hereby granted, free of charge, to any person obtaining a
    copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.
*/

/** @file
 * @brief Forward declarations for Magnum::Text namespace
 */

#include "Types.h"

#include "magnumConfigure.h"

namespace Magnum { namespace Text {

class AbstractFont;
class AbstractFontConverter;
class AbstractLayouter;
class DistanceFieldGlyphCache;
class GlyphCache;

#ifndef MAGNUM_GCC46_COMPATIBILITY
enum class Alignment: UnsignedByte;
#endif

class AbstractRenderer;
template<UnsignedInt> class Renderer;
typedef Renderer<2> Renderer2D;
typedef Renderer<3> Renderer3D;

#ifdef MAGNUM_BUILD_DEPRECATED
typedef Renderer<2> TextRenderer2D;
typedef Renderer<3> TextRenderer3D;
#endif

}}

#endif
