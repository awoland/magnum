#ifndef Magnum_Text_AbstractFont_h
#define Magnum_Text_AbstractFont_h
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
 * @brief Class Magnum::Text::AbstractFont, Magnum::Text::AbstractLayouter
 */

#include <memory>
#include <string>
#include <tuple>
#include <PluginManager/AbstractPlugin.h>

#include "Magnum.h"
#include "Texture.h"
#include "Text/Text.h"
#include "Text/magnumTextVisibility.h"

namespace Magnum { namespace Text {

/**
@brief Base for font plugins

@section AbstractFont-usage Usage

First step is to open the font using @ref open(), next step is to prerender all
the glyphs which will be used in text rendering later, see @ref GlyphCache for
more information. See @ref Renderer for information about text rendering.

@section AbstractFont-subclassing Subclassing

Plugin implements @ref doFeatures(), @ref doClose(), @ref doCreateGlyphCache(),
@ref doLayout() and one or more of `doOpen*()` functions.

You don't need to do most of the redundant sanity checks, these things are
checked by the implementation:

-   Functions @ref doOpenData(), @ref doOpenSingleData() and @ref doOpenFile()
    are called after the previous file was closed, function @ref doClose() is
    called only if there is any file opened.
-   Functions @ref doOpenData() and @ref doOpenSingleData() are called only if
    @ref Feature::OpenData is supported.
-   All `do*()` implementations working on opened file are called only if
    there is any file opened.
*/
class MAGNUM_TEXT_EXPORT AbstractFont: public PluginManager::AbstractPlugin {
    CORRADE_PLUGIN_INTERFACE("cz.mosra.magnum.Text.AbstractFont/0.2.1")

    public:
        /**
         * @brief Features supported by this importer
         *
         * @see Features, features()
         */
        enum class Feature: UnsignedByte {
            /** Opening fonts from raw data using openData() */
            OpenData = 1 << 0,

            /**
             * The format is multi-file, thus openSingleData() convenience
             * function cannot be used.
             */
            MultiFile = 1 << 1,

            /**
             * The font contains prepared glyph cache.
             *
             * @see fillGlyphCache(), createGlyphCache()
             */
            PreparedGlyphCache = 1 << 2
        };

        /** @brief Set of features supported by this importer */
        typedef Containers::EnumSet<Feature, UnsignedByte> Features;

        /** @brief Default constructor */
        explicit AbstractFont();

        /** @brief Plugin manager constructor */
        explicit AbstractFont(PluginManager::AbstractManager* manager, std::string plugin);

        /** @brief Features supported by this font */
        Features features() const { return doFeatures(); }

        /** @brief Whether any file is opened */
        bool isOpened() const { return doIsOpened(); }

        /**
         * @brief Open font from raw data
         * @param data          Pairs of filename and file data
         * @param size          Font size
         *
         * Closes previous file, if it was opened, and tries to open given
         * file. Available only if @ref Feature::OpenData is supported. Returns
         * `true` on success, `false` otherwise.
         */
        bool openData(const std::vector<std::pair<std::string, Containers::ArrayReference<const unsigned char>>>& data, Float size);

        /**
         * @brief Open font from single data
         * @param data          File data
         * @param size          Font size
         *
         * Closes previous file, if it was opened, and tries to open given
         * file. Available only if @ref Feature::OpenData is supported and the
         * plugin doesn't have @ref Feature::MultiFile. Returns `true` on
         * success, `false` otherwise.
         */
        bool openSingleData(Containers::ArrayReference<const unsigned char> data, Float size);

        /**
         * @brief Open font from file
         * @param filename      Font file
         * @param size          Font size
         *
         * Closes previous file, if it was opened, and tries to open given
         * file. If the plugin has @ref Feature::MultiFile, the function will
         * use additional files in given path, all sharing common basename
         * derived from @p filename. Returns `true` on success, `false`
         * otherwise.
         */
        bool openFile(const std::string& filename, Float size);

        /** @brief Close font */
        void close();

        /** @brief Font size */
        Float size() const { return _size; }

        /**
         * @brief Glyph ID for given character
         *
         * @note This function is not meant to be used in performance-critical
         *      code, only for font observations and conversions.
         */
        UnsignedInt glyphId(char32_t character);

        /**
         * @brief Glyph advance
         * @param glyph     Glyph ID
         *
         * @note This function is not meant to be used in performance-critical
         *      code, only for font observations and conversions.
         * @see glyphId()
         */
        Vector2 glyphAdvance(UnsignedInt glyph);

        /**
         * @brief Fill glyph cache with given character set
         * @param cache         Glyph cache instance
         * @param characters    UTF-8 characters to render
         *
         * Fills the cache with given characters. Fonts having
         * @ref Feature::PreparedGlyphCache do not support partial glyph cache
         * filling, use @ref createGlyphCache() instead.
         */
        void fillGlyphCache(GlyphCache& cache, const std::string& characters);

        /**
         * @brief Create glyph cache
         *
         * Configures and fills glyph cache with the contents of whole font.
         * Available only if @ref Feature::PreparedGlyphCache is supported.
         * Other fonts support only partial glyph cache filling, see
         * @ref fillGlyphCache().
         */
        std::unique_ptr<GlyphCache> createGlyphCache();

        /**
         * @brief Layout the text using font's own layouter
         * @param cache     Glyph cache
         * @param size      Font size
         * @param text      %Text to layout
         *
         * @see fillGlyphCache(), createGlyphCache()
         */
        std::unique_ptr<AbstractLayouter> layout(const GlyphCache& cache, Float size, const std::string& text);

    #ifdef DOXYGEN_GENERATING_OUTPUT
    private:
    #else
    protected:
    #endif
        Float _size;

    #ifdef DOXYGEN_GENERATING_OUTPUT
    protected:
    #else
    private:
    #endif
        /** @brief Implementation for features() */
        virtual Features doFeatures() const = 0;

        /** @brief Implementation for isOpened() */
        virtual bool doIsOpened() const = 0;

        /**
         * @brief Implementation for openData()
         *
         * If the plugin doesn't have @ref Feature::MultiFile, default
         * implementation calls @ref doOpenSingleData().
         */
        virtual void doOpenData(const std::vector<std::pair<std::string, Containers::ArrayReference<const unsigned char>>>& data, Float size);

        /** @brief Implementation for openSingleData() */
        virtual void doOpenSingleData(Containers::ArrayReference<const unsigned char> data, Float size);

        /**
         * @brief Implementation for openFile()
         *
         * If @ref Feature::OpenData is supported and the plugin doesn't have
         * @ref Feature::MultiFile, default implementation opens the file and
         * calls @ref doOpenSingleData() with its contents.
         */
        virtual void doOpenFile(const std::string& filename, Float size);

        /** @brief Implementation for close() */
        virtual void doClose() = 0;

        /** @brief Implementation for glyphId() */
        virtual UnsignedInt doGlyphId(char32_t character) = 0;

        /** @brief Implementation for glyphAdvance() */
        virtual Vector2 doGlyphAdvance(UnsignedInt glyph) = 0;

        /**
         * @brief Implementation for createGlyphCache()
         *
         * The string is converted from UTF-8 to UTF-32, unique characters are
         * *not* removed.
         * @note On Windows uses `std::vector<char32_t>` instead of
         *      `std::u32string`. See @ref Corrade::Utility::Unicode::utf32()
         *      for more information.
         */
        #ifndef _WIN32
        virtual void doFillGlyphCache(GlyphCache& cache, const std::u32string& characters);
        #else
        virtual void doFillGlyphCache(GlyphCache& cache, const std::vector<char32_t>& characters);
        #endif

        /**
         * @brief Implementation for createGlyphCache()
         */
        virtual std::unique_ptr<GlyphCache> doCreateGlyphCache();

        /** @brief Implementation for layout() */
        virtual std::unique_ptr<AbstractLayouter> doLayout(const GlyphCache& cache, Float size, const std::string& text) = 0;
};

CORRADE_ENUMSET_OPERATORS(AbstractFont::Features)

/**
@brief Base for text layouters

Returned by AbstractFont::layout().
*/
class MAGNUM_TEXT_EXPORT AbstractLayouter {
    AbstractLayouter(const AbstractLayouter&) = delete;
    AbstractLayouter(AbstractLayouter&&) = delete;
    AbstractLayouter& operator=(const AbstractLayouter&) = delete;
    AbstractLayouter& operator=(const AbstractLayouter&&) = delete;

    public:
        explicit AbstractLayouter();
        virtual ~AbstractLayouter() = 0;

        /** @brief Count of glyphs in laid out text */
        UnsignedInt glyphCount() const {
            return _glyphCount;
        }

        /**
         * @brief Render glyph
         * @param i                 Glyph index
         *
         * Returns quad position, texture coordinates and advance to next
         * glyph.
         */
        virtual std::tuple<Rectangle, Rectangle, Vector2> renderGlyph(UnsignedInt i) = 0;

    #ifdef DOXYGEN_GENERATING_OUTPUT
    private:
    #else
    protected:
    #endif
        UnsignedInt _glyphCount;
};

}}

#endif
