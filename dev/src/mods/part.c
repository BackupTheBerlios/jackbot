#include "../lib/libjackbot.h"

void part(struct _Nfos_ *nfos);

struct _Mods_ _mod_info = // for description see pong.c
{
  NULL,
  &part,
  "Part",
  "part",
  "",
  ""
};

/*
 * This is the opposite of the invite mod. If someone sends a !part in the channel, the bot will get
 *   :JacK_McRiDER PRIVMSG #foo :!part
 * This mod reads the channel (#foo) and leaves it (with a little exit message)...
 */
void part(struct _Nfos_ *nfos)
{
  int ctr = 0;
  char channel[CHAN_NAME_MAX + 1];

  for(ctr = 0; nfos->sender->message[ctr] != ' '; ctr++)
    channel[ctr] = nfos->sender->message[ctr];
    
  channel[ctr] = '\0';
  
  while(nfos->sender->message[ctr] != ':')
    ctr++;
  ctr++;
    
  irc_cmd("PART %s :see ya'll, %s wants me to leave!", channel, nfos->sender->nickname);
}
