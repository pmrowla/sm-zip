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
 * @brief Zip extension configuration
 */

#ifndef _INCLUDE_SOURCEMOD_EXTENSION_CONFIG_H_
#define _INCLUDE_SOURCEMOD_EXTENSION_CONFIG_H_

/* Basic information exposed publicly */
#define SMEXT_CONF_NAME			"Zip"
#define SMEXT_CONF_DESCRIPTION	"Zip file compression extension"
#define SMEXT_CONF_VERSION	    SMZIP_VERSION	
#define SMEXT_CONF_AUTHOR		"Peter \"astroman\" Rowlands"
#define SMEXT_CONF_URL			"https://github.com/pmrowla/sm-zip"
#define SMEXT_CONF_LOGTAG		"ZIP"
#define SMEXT_CONF_LICENSE		"GPL"
#define SMEXT_CONF_DATESTRING	__DATE__

/** 
 * @brief Exposes plugin's main interface.
 */
#define SMEXT_LINK(name) SDKExtension *g_pExtensionIface = name;

/** Enable interfaces you want to use here by uncommenting lines */
//#define SMEXT_ENABLE_FORWARDSYS
#define SMEXT_ENABLE_HANDLESYS
//#define SMEXT_ENABLE_PLAYERHELPERS
//#define SMEXT_ENABLE_DBMANAGER
//#define SMEXT_ENABLE_GAMECONF
//#define SMEXT_ENABLE_MEMUTILS
//#define SMEXT_ENABLE_GAMEHELPERS
//#define SMEXT_ENABLE_TIMERSYS
//#define SMEXT_ENABLE_THREADER
//#define SMEXT_ENABLE_LIBSYS
//#define SMEXT_ENABLE_MENUS
//#define SMEXT_ENABLE_ADTFACTORY
//#define SMEXT_ENABLE_PLUGINSYS
//#define SMEXT_ENABLE_ADMINSYS
//#define SMEXT_ENABLE_TEXTPARSERS
//#define SMEXT_ENABLE_USERMSGS
//#define SMEXT_ENABLE_TRANSLATOR
//#define SMEXT_ENABLE_NINVOKE
//#define SMEXT_ENABLE_ROOTCONSOLEMENU

#endif // _INCLUDE_SOURCEMOD_EXTENSION_CONFIG_H_
