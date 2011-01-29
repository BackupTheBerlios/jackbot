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
  int pos;

  // find '#' as channelname indicator
  for(pos = 0; nfos->sender->message[pos] != '#'; pos++)
  {
    if(nfos->sender->message[pos] == '\0')
      return;
  }

  irc_cmd("JOIN %s", &nfos->sender->message[pos]);

  debug_out("  sent: JOIN %s\n", &nfos->sender->message[pos]);
}
