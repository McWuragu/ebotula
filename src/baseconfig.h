/* #############################################################
*
* This is a part of ebotula.
* It is distributed under the GNU General Public License
* See the file COPYING for details.
*
* (c)2003-2004 Steffen Laube <Laube.Steffen@gmx.de>
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
