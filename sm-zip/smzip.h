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
 * @brief sm-zip extension header
 *
 * @author Peter "astroman" Rowlands <peter@pmrowla.com>
 */

#ifndef _SMZIP_H_
#define _SMZIP_H_

#include "smsdk_ext.h"


/**
 * @brief sm-zip implementation of the SDK Extension.
 * Note: Uncomment one of the pre-defined virtual functions in order to use it.
 */
class SmZip : public SDKExtension
{
public:
	virtual bool SDK_OnLoad(char *error, size_t maxlength, bool late);
	virtual void SDK_OnUnload();
	virtual void SDK_OnAllLoaded();
	virtual bool QueryRunning(char *error, size_t maxlength);
};

class ZipFileHandler : public IHandleTypeDispatch
{
public:
    void OnHandleDestroy(HandleType_t, void *object);
};

extern HandleType_t g_ZipFileType;
extern const sp_nativeinfo_t smzip_natives[];

#endif // ! _SMZIP_H_
