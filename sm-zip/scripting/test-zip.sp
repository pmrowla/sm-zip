/*
 * Copyright (c) 2013 Peter Rowlands
 *
 * This file is a part of sm-zip.
 *
 * sm-zip is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * sm-zip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with sm-zip.  If not, see <http://www.gnu.org/licenses/>.
 *
 * vim: set ts=4 et :
 */
/**
 * @file
 * @brief sm-zip test plugin
 *
 * @author Peter "astroman" Rowlands <peter@pmrowla.com>
 */

#pragma semicolon 1
#include <sourcemod>
#include <test>
#include <zip>

#define VERSION "0.0.1"

public Plugin:myinfo =
{
    name = "test-smzip",
    author = "Peter \"astroman\" Rowlands",
    description = "Test smzip extension",
    version = VERSION,
};

public OnPluginStart()
{
    CreateConVar("sm_smzip_test_version", VERSION, "Tests all sm-zip natives.",
        FCVAR_PLUGIN | FCVAR_SPONLY | FCVAR_REPLICATED | FCVAR_NOTIFY | FCVAR_DONTRECORD);

    new bool:bStepResult = true;

    new Handle:hTest = Test_New(3);
    Test_Ok(hTest, LibraryExists("zip"), "Library is loaded");

    new Handle:hZip = Zip_Open("test_archive.zip", ZIP_APPEND_STATUS_CREATE);
    Test_IsNot(hTest, hZip, INVALID_HANDLE, "Creating zip archive");

    new Handle:hFile = OpenFile("test_file.txt", "w");
    WriteFileLine(hFile, "1234567890");
    FlushFile(hFile);
    CloseHandle(hFile);

    bStepResult = Zip_AddFile(hZip, "test_file.txt");
    Test_Ok(hTest, bStepResult, "Adding file to archive");

    DeleteFile("test_file.txt");

    CloseHandle(hZip);
}
