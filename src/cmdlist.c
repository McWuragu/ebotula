#include "cmdlist.h"

CmdHandleRef_t CmdHandleField[CMD_COUNT] = {
    {CMD_NONE },
    /* Events */
    {CMD_ONPING         ,hPong              ,"PING"             ,false},
    {CMD_ONQUIT         ,logoff             ,"QUIT"             ,false},
    {CMD_ONNICKCHG      ,hNickChange        ,"NICK"             ,false},
    {CMD_ONMODE         ,hResetModes        ,"MODE"             ,false},
    {CMD_ONKICK         ,hRejoinAfterKick   ,"KICK"             ,false},
    {CMD_ONJOIN         ,hSetModUser        ,"JOIN"             ,false},
    {CMD_ONNAMES        ,hBotNeedOp         ,"353"              ,false},
    {CMD_ONTOPIC        ,hResetTopic        ,"TOPIC"            ,false},
    {CMD_ONWHOIS        ,hCallback          ,"311"              ,false},
    {CMD_ONNONICK       ,hWhoisFailed       ,"401"              ,false},
    /* CTCP commands */
    {CMD_CTCPPING       ,ctcpping           ,"\001PING"         ,false},
    {CMD_CTCPVERSION    ,ctcpversion        ,"\001VERSION\001"  ,false},
    {CMD_CTCPTIME       ,ctcptime           ,"\001TIME\001"     ,false},
    /* any commands */
    {CMD_VIEWGREAT      ,greeting           ,"viewgreet"        ,true},
    {CMD_HELP           ,help               ,"help"             ,true},
    {CMD_VERSION        ,version            ,"version"          ,true},
    {CMD_HELLO          ,hello              ,"hello"            ,true},
    {CMD_IDENT          ,ident              ,"ident"            ,true},
    /* logged user commands */
    {CMD_LOGOFF         ,logoff             ,"logoff"           ,true},
    {CMD_PASS           ,password           ,"pass"             ,true},
    /* friend commands */
    {CMD_INVITE         ,inviteuser         ,"invite"           ,true},
    /* channel owner commands */
    {CMD_CHANMODE       ,chanmode           ,"chanmode"         ,true},
    {CMD_ACCOUNTLIST    ,accountlist        ,"accountlist"      ,true},
    {CMD_ACCOUNTMODE    ,accountmode        ,"accountmode"      ,true},
    {CMD_SAY            ,say                ,"say"              ,true},
    {CMD_KICK           ,kickuser           ,"kick"             ,true},
    {CMD_SET_TOPIC      ,setTopic           ,"topic"            ,true},
    {CMD_SET_GREATING   ,setGreeting        ,"greeting"         ,true},
    {CMD_BAN            ,banuser            ,"ban"              ,true},
    {CMD_DEBAN          ,debanuser          ,"deban"            ,true},
    /* master commands */
    {CMD_RESTART        ,restart            ,"restart"          ,true},
    {CMD_ALLSAY         ,allsay             ,"allsay"           ,true},
    {CMD_RMUSER         ,rmaccount          ,"rmaccount"        ,true},
    {CMD_NICK           ,setNick            ,"nick"             ,true},
    {CMD_DIE            ,die                ,"die"              ,true},
    {CMD_CHANNELS       ,chanlist           ,"chanlist"         ,true},
    {CMD_RMCHANNEL      ,rmChannel          ,"rmchannel"        ,true},
    {CMD_ADDCHANNEL     ,addChannel         ,"addchannel"       ,true},
    {CMD_JOIN           ,joinChannel        ,"join"             ,true},
    {CMD_PART           ,partChannel        ,"part"             ,true}
};

