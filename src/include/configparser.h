/* #############################################################
 *
 *  This file is a part of ebotula. 
 *
 *  Coypright (C)2003-2023 Steffen Laube <Laube.Steffen@gmx.de>
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

#ifndef __H_CONFIG_FILE_PARSER
#define __H_CONFIG_FILE_PARSER
#define MAX_COMMENT_LINES 25
typedef struct 
{
	char *sParamName;
	int iParamType;
	void *pParam;
	int isFound;
	int (*check_param)(const void *param,const void *range1,const void *range2,const char *param_name);
	void *pRange1;
	void *pRange2;
}InputValueStruct_t;
enum ErrCodes{
	  No_Error=0,
	  EndOfFile=1,
	  ErrUnkownParam=-1,
	  ErrInvalidParam=-2,
	  ErrNoParam=-3,
        ErrOnMalloc=-4

};
/**
 * read configfile
 *
 */
int ConfigFileParser(void);
#endif
