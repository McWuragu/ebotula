/* #############################################################
*
* This is a part of ebotula.
* It is distributed under the GNU General Public License
* See the file COPYING for details.
*
* (c)2004 Steffen Laube <Laube.Steffen@gmx.de>
* ############################################################# 
*/
#ifndef __H_BASECONFIG
#define __H_BASECONFIG
/* 
 * modiefier for param list
 */
typedef enum{PARAM_CHAR,
	PARAM_INT,
	PARAM_LONG,
	PARAM_HEX,
	PARAM_FLOAT,
	PARAM_DOUBLE,
	PARAM_STRING} Output_t;
typedef enum {
	MOD_NONE=0,
	MOD_QUOTED
	} Modefier_t;
/*
 * Value Structure
 */
enum Values_e{
	VAL_BOTNAME=0,
	VAL_REALNAME,
	VAL_EXEUSER,
	VAL_EXEGROUP,
	VAL_THREADLIMIT,
	VAL_SERVER,
	VAL_PORT,
	VAL_DATABASEPATH,
	VAL_FASTSENDDELAY,
	VAL_SLOWSENDDELAY,
	VAL_FASTSENDLIMIT,
	VAL_INITDELAY,
	VAL_ALT,
	VAL_AUTOLOGOFF,
	VAL_PINGTIMEOUT,
	VAL_LOGLEVEL,
	VAL_COUNT
};
#define MAX_COMMENT_LINES 25
typedef struct tag_ValueStruct
{
	char (sDescription)[MAX_COMMENT_LINES][255];
	char sParamName[255];
	Output_t iParamType;
	Modefier_t iModefier;
	void *pParam;
}ValueStruct_t;
/**
 * generates baseconfig string for baseconfigfile and writes it 
 *
 */
void write_baseconfig();
#endif 
