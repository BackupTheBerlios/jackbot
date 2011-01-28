#include "../lib/libjackbot.h"

void join(struct _Nfos_ *nfos); 

struct _Mods_ _mod_info = 
{                         
  NULL,                   
  &join,                  
  "Join", 
  "join",                    
  "",                
  "" 
};

/*
 * This is the join mod. It forces the bot to join a channel.
 * You have to write
 *   !join #foobar
 * into a channel or as NOTICE to the JacKBot. It will join the channel #foobar.
 *   JOIN #foobar
 */
void join(struct _Nfos_ *nfos)
{
  irc_cmd("JOIN %s", &nfos->sender->message[strlen(nfos->mods->mod_cmd) + 1]);

#if (DEBUG)
  fprintf(stderr, "  sent: JOIN %s", &nfos->sender->message[strlen(nfos->mods->mod_cmd) + 1]);
#endif
}
