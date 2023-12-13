/* #############################################################
 *
 *  This file is a part of ebotula. 
 *
 *  Coypright (C)2003-2005 Steffen Laube <Laube.Steffen@gmx.de>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the Free
 *  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA. 
 *
 * ############################################################# 
 */

#ifndef __H_BASECONFIG
#define __H_BASECONFIG
#define MAX_COMMENT_LINES 25
typedef struct tag_ValueStruct
{
	char *sDescription[MAX_COMMENT_LINES];
	char *sParamName;
	Output_t iParamType;
	Modefier_t iModefier;
	void *pParam;
}OutputValueStruct_t;
/**
 * generates baseconfig string for baseconfigfile and writes it 
 *
 */
int write_baseconfig();
#endif 
