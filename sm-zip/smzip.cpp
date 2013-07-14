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
 * @brief sm-zip extension code
 *
 * @author Peter "astroman" Rowlands <peter@pmrowla.com>
 */

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <zlib.h>
#include <ioapi.h>
#include <zip.h>

#include "smzip.h"

HandleType_t g_ZipFileType = 0;
ZipFileHandler g_ZipFileHandler;

bool SmZip::SDK_OnLoad(char *error, size_t maxlength, bool late)
{
    HandleError err;
    g_ZipFileType = handlesys->CreateType("ZipFile", &g_ZipFileHandler, 0,
            NULL, NULL, myself->GetIdentity(), &err);
    if (0 == g_ZipFileType)
    {
        handlesys->RemoveType(g_ZipFileType, myself->GetIdentity());
        snprintf(error, maxlength, "Could not create ZipFile type (err: %d)", err);
        return false;
    }

    sharesys->RegisterLibrary(myself, "zip");

    return true;
}

void SmZip::SDK_OnUnload()
{
    if (0 != g_ZipFileType)
    {
        handlesys->RemoveType(g_ZipFileType, myself->GetIdentity());
        g_ZipFileType = 0;
    }
}

void SmZip::SDK_OnAllLoaded()
{
    sharesys->AddNatives(myself, smzip_natives);
}

bool SmZip::QueryRunning(char *error, size_t maxlength)
{
    return true;
}

void ZipFileHandler::OnHandleDestroy(HandleType_t, void *object)
{
    if (NULL != object)
    {
        zipClose((zipFile)object, NULL);
    }
}

int isLargeFile(const char* filename)
{
    int largeFile = 0;
    ZPOS64_T pos = 0;
    FILE* pFile = fopen64(filename, "rb");

    if (pFile != NULL)
    {
        int n = fseeko64(pFile, 0, SEEK_END);
        pos = ftello64(pFile);

        if (pos >= 0xffffffff)
            largeFile = 1;

        fclose(pFile);
    }

    return largeFile;
}

static cell_t Zip_Open(IPluginContext *pCtx, const cell_t *params)
{
    char path[PLATFORM_MAX_PATH];
    char *filename;
    pCtx->LocalToString(params[1], &filename);
    g_pSM->BuildPath(Path_Game, path, sizeof(path), "%s", filename);
    cell_t append = params[2];

    zipFile zf = zipOpen64(path, append);
    if (NULL == zf)
    {
        return BAD_HANDLE;
    }

    return handlesys->CreateHandle(g_ZipFileType, zf, pCtx->GetIdentity(),
            myself->GetIdentity(), NULL);
}

static cell_t Zip_AddFile(IPluginContext *pCtx, const cell_t *params)
{
    Handle_t handle = static_cast<Handle_t>(params[1]);
    HandleError err;
    HandleSecurity sec;
    cell_t ret = true;

    sec.pOwner = NULL;
    sec.pIdentity = myself->GetIdentity();

    zipFile zf;
    err = handlesys->ReadHandle(handle, g_ZipFileType, &sec, (void **)&zf);
    if (HandleError_None != err)
    {
        return pCtx->ThrowNativeError("Invalid Zip file handle %x (error %d)", handle, err);
    }

    char path[PLATFORM_MAX_PATH];
    char *filename;
    pCtx->LocalToString(params[2], &filename);
    g_pSM->BuildPath(Path_Game, path, sizeof(path), "%s", filename);

    zip_fileinfo zi;
    memset(&zi, 0, sizeof(zi));

    int zipErr = zipOpenNewFileInZip64(zf, filename, &zi, NULL, 0, NULL, 0, NULL,
            Z_DEFLATED, Z_DEFAULT_COMPRESSION, isLargeFile(path));
    if (ZIP_OK != zipErr)
    {
        g_pSM->LogError(myself, "Could not open new file %s in zip (%d)", filename, err);
        return false;
    }

    FILE *fp = fopen64(path, "rb");
    if (NULL == fp)
    {
        g_pSM->LogError(myself, "fopen64(%s) failed", path);
        ret = false;
    }

    if (ret)
    {
        char buf[4096];
        size_t bytesRead = 0;

        do
        {
            zipErr = ZIP_OK;
            bytesRead = fread(buf, 1, sizeof(buf), fp);
            if (bytesRead < sizeof(buf) && feof(fp) != 0)
            {
                zipErr = ZIP_ERRNO;
            }

            if (bytesRead > 0)
            {
                zipErr = zipWriteInFileInZip(zf, buf, bytesRead);
            }

            if (ZIP_ERRNO == zipErr && EAGAIN == errno)
                zipErr = ZIP_OK;

        } while (ZIP_OK == zipErr && bytesRead > 0);

        if (ZIP_OK != zipErr)
        {
            if (ZIP_ERRNO == zipErr)
                g_pSM->LogError(myself, "Failed to write to zip archive (%s)", strerror(errno));
            else
                g_pSM->LogError(myself, "Failed to write to zip archive (%d)", zipErr);
            ret = false;
        }
    }

    if (fp)
        fclose(fp);

    zipCloseFileInZip(zf);
    return ret;
}

const sp_nativeinfo_t smzip_natives[] =
{
    {"Zip_Open",    Zip_Open},
    {"Zip_AddFile", Zip_AddFile},
    {NULL,          NULL},
};

SmZip g_SmZip;	    /**< Global singleton for extension's main interface */
SMEXT_LINK(&g_SmZip);
