// ***************************************************************** -*- C++ -*-
/*
 * Copyright (C) 2004-2008 Andreas Huggel <ahuggel@gmx.net>
 *
 * This program is part of the Exiv2 distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, 5th Floor, Boston, MA 02110-1301 USA.
 */
/*!
  @file    pngchunk.hpp
  @brief   Class PngChunk to parse PNG chunk data implemented using the following references:<br>
           <a href="http://www.vias.org/pngguide/chapter11_05.html">PNG iTXt chunk structure</a> from PNG definitive guide,<br>
           <a href="http://www.vias.org/pngguide/chapter11_04.html">PNG tTXt and zTXt chunks structures</a> from PNG definitive guide,<br>
           <a href="http://www.sno.phy.queensu.ca/~phil/exiftool/TagNames/PNG.html">PNG tags list</a> by Phil Harvey<br>
           Email communication with <a href="mailto:caulier dot gilles at gmail dot com">caulier dot gilles at gmail dot com</a><br>
  @version $Rev: 823 $
  @author  Andreas Huggel (ahu)
           <a href="mailto:ahuggel@gmx.net">ahuggel@gmx.net</a>
  @author  Gilles Caulier (cgilles)
           <a href="mailto:caulier dot gilles at gmail dot com">caulier dot gilles at gmail dot com</a>
  @date    12-Jun-06, gc: submitted
 */
#ifndef PNGCHUNK_HPP_
#define PNGCHUNK_HPP_

// *****************************************************************************
// included header files
#include "types.hpp"

// + standard includes
#include <iosfwd>
#include <cassert>

// *****************************************************************************
// namespace extensions
namespace Exiv2 
{

// *****************************************************************************
// class declarations

    class Image;

// *****************************************************************************
// class definitions

    /*!
      @brief Stateless parser class for data in PNG chunk format. Images use this
             class to decode and encode PNG-based data.
     */
    class PngChunk 
    {
    public:

    /*!
      @brief Text Chunk types.
     */
    enum TxtChunkType
    {
        tEXt_Chunk = 0,
        zTXt_Chunk = 1,
        iTXt_Chunk = 2
    };

    /*!
      @brief Metadata Chunk types.
     */
    enum MetadataType
    {
        exif_Data    = 0,
        iptc_Data    = 1,
        xmp_Data     = 2,
        comment_Data = 3
    };

    public:

        /*!
          @brief Decode PNG IHDR chunk data from a data buffer
                 \em data and return image size to \em outWidth and \em outHeight.

          @param data      PNG Chunk data buffer.
          @param outWidth  Integer pointer to be set to the width of the image.
          @param outHeight Integer pointer to be set to the height of the image.
        */
        static void decodeIHDRChunk(const DataBuf& data,
                                    int*           outWidth,
                                    int*           outHeight);

        /*!
          @brief Decode PNG tEXt, zTXt, or iTXt chunk data from \em pImage passed by data buffer
                 \em data and extract Comment, Exif, Iptc, Xmp metadata accordingly.

          @param pImage    Pointer to the image to hold the metadata
          @param data      PNG Chunk data buffer.
          @param type      PNG Chunk TXT type.
        */
        static void decodeTXTChunk(Image*         pImage,
                                   const DataBuf& data,
                                   TxtChunkType   type);

        /*!
          @brief Return PNG TXT chunk key as data buffer.

          @param data        PNG Chunk data buffer.
          @param stripHeader Set true if chunk data start with header bytes, else false (default).
        */
        static DataBuf keyTXTChunk(const DataBuf& data, bool stripHeader=false);

        /*!
          @brief Return a complete PNG chunk data compressed or not as buffer. Data returned is formated 
                 accordingly with metadata \em type to host passed by \em metadata.

          @param metadata    metadata buffer.
          @param type        metadata type.
          @param compress    compress or not metadata.
        */
        static DataBuf makeMetadataChunk(const DataBuf& metadata, MetadataType type, bool compress);

    private:

        //! @name Accessors
        //@{

        /*!
          @brief Parse PNG Text chunk to determine type and extract content. 
                 Supported Chunk types are tTXt, zTXt, and iTXt.
         */
        static DataBuf parseTXTChunk(const DataBuf& data,
                                     int            keysize,
                                     TxtChunkType   type);

        /*!
          @brief Parse PNG chunk contents to extract metadata container and assign it to image. 
                 Supported contents are:
                    Exif raw text profile generated by ImageMagick ==> Image Exif metadata.
                    Iptc raw text profile generated by ImageMagick ==> Image Iptc metadata.
                    Xmp  raw text profile generated by ImageMagick ==> Image Xmp metadata.
                    Xmp  packet generated by Adobe                 ==> Image Xmp metadata.
                    Description string                             ==> Image Comments.
         */
        static void parseChunkContent(Image*        pImage, 
                                      const byte*   key, 
                                      const DataBuf arr);

        /*!
          @brief Return a compressed (zTXt) or uncompressed (tEXt) PNG ASCII text chunk 
                 (header + key + flags + data + CRC) as data buffer.

          @param key         PNG Chunk key.
          @param data        PNG Chunk raw data.
          @param compress    Compress or not PNG Chunk data.
        */
        static DataBuf makeAsciiTxtChunk(const DataBuf& key, const DataBuf& data, bool compress);

        /*!
          @brief Return a compressed or uncompressed (iTXt) PNG UTF8 text chunk 
                 (header + key + flags + data + CRC) as data buffer.

          @param key         PNG Chunk key.
          @param data        PNG Chunk raw data.
          @param compress    Compress or not PNG Chunk data.
        */
        static DataBuf makeUtf8TxtChunk(const DataBuf& key, const DataBuf& data, bool compress);

        /*!
          @brief Wrapper around zlib to uncompress a PNG chunk content.
         */
        static void zlibUncompress(const byte*  compressedText, 
                                   unsigned int compressedTextSize, 
                                   DataBuf&     arr);

        /*!
          @brief Wrapper around zlib to compress a PNG chunk content.
         */
        static void zlibCompress(const byte*  text, 
                                 unsigned int textSize, 
                                 DataBuf&     arr);

        /*!
          @brief Decode from ImageMagick raw text profile which host encoded Exif/Iptc/Xmp metadata byte array.
         */
        static DataBuf readRawProfile(const DataBuf& text);

        /*!
          @brief Encode to ImageMagick raw text profile which host encoded Exif/Iptc/Xmp metadata byte array.
         */
        static DataBuf writeRawProfile(const DataBuf& profile_data, const DataBuf& profile_type);

        static size_t copyString(char* destination, 
                                 const char* source, 
                                 const size_t length);

        static long formatString(char*        string, 
                                 const size_t length, 
                                 const char*  format, 
                                 ...);

        static long formatStringList(char *string, 
                                     const size_t length, 
                                     const char *format, 
                                     va_list operands);
        //@}

    }; // class PngChunk

}                                       // namespace Exiv2

#endif                                  // #ifndef PNGCHUNK_HPP_
