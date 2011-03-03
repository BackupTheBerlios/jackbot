#include "../lib/libjackbot.h"

void ping(struct _Nfos_ *nfos); // prototype of the 'main' of the mod (because it's needed in the following struct)

struct _Mods_ _mod_info = // This is the mod struct given in libjackbot.h,
{                         //   it gives the 'core' everything it needs for starting the mod
  NULL,                   // struct _Mods_ *next; -> will be set as pointer to the next mod in a chain list
  &ping,                  // (*mod_main)(struct _Nfos_ *nfos);  -> pointer to the 'main' of the mod, for the 'core' for starting it
  "Ping",                 // char name[];         -> the name of the mod as a string
  "",                     // char mod_cmd[];      -> the command for starting mod from channel with a leading '!'
  "PING",                 // char server_cmd[];   -> server messages for starting the mod, like 'PING'
  ""                      // char requests[][];   -> request numbers sent from irc for starting mod, can be more then one, as a string
};                        // ... not a typedef, so no name in this line

/*
 * This mod responses 
 *   PONG foobar
 * to a ping from the irc server, which looks like this:
 *   PING foobar
 *
 * irc_cmd() is a macro from libjackbot.h using snprintf() for string literal and sends
 * the string to irc with automatically added \r\n at the end (see send_irc() in libjackbot.c)
 */
void ping(struct _Nfos_ *nfos)
{
  if(nfos->sender->middle[0] == '\0')
  {
    irc_cmd("PONG %s", nfos->sender->message);
  }else
  {
    irc_cmd("PONG %s", nfos->sender->middle);
  }
}
