// license [
// This file is part of the Phantom project. Copyright 2011-2019 Vivien Millet.
// Distributed under the MIT license. Text available here at
// http://www.wiwila.com/tools/phantom/license/
// ]

#pragma once

/* ****************** Includes ******************* */
#include "SourceStream.h"

#include <phantom/String.h>
/* **************** Declarations ***************** */

/* *********************************************** */
namespace phantom
{
class Phantom;
}

namespace phantom
{
namespace reflection
{
/// \brief  Represents an user repository source file for Source compilation.
class PHANTOM_EXPORT_PHANTOM SourceFile : public SourceStream
{
    friend class Application;
    friend class Source;

public:
    static SourceFile* CreateOnDisk(StringView a_strPath, bool a_bOverwrite /*= false*/);
    static SourceFile* CreateOnDisk(Source* a_pSource, StringView a_Path, bool a_bOverwrite /*= false*/);

public:
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Constructs a source file from a path.
    ///
    /// \param  a_strPath The path of the represented file.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    SourceFile(StringView a_strAbsolutePath);

    /// \brief  Destructor.
    ~SourceFile();

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Deletes the represented file.
    ///
    /// \return true if success
    ////////////////////////////////////////////////////////////////////////////////////////////////////
    bool deleteFile();

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Reads the whole file into the given String argument.
    ///
    /// \param [in,out] a_Out   The String receiving the file content.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void read(String& a_Out);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// \brief  Writes the given String argument to the file (overwriting existing content).
    ///
    /// \param [in,out] a_In   The String to write.
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    void write(StringView a_In);

    ////////////////////////////////////////////////////////////////////////////////////////////////////
    /// Reimplemented from SourceStream
    ////////////////////////////////////////////////////////////////////////////////////////////////////

    time_t getLastChangeTime() const override;

    std::basic_istream<char>* createInputStream() override;

    void destroyInputStream(std::basic_istream<char>*) override;

    SourceFile* clone() const override;
};

} // namespace reflection
} // namespace phantom
