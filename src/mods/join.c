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
  char channel[CHAN_NAME_MAX + 1];

  get_from_message(channel, GFM_CHANNEL);

  irc_cmd("JOIN %s", channel);
}
