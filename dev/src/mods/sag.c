#include "../lib/libjackbot.h"

void sag(struct _Nfos_ *nfos);

struct _Mods_ _mod_info =
{
  NULL,
  &sag,
  "Sag",
  "sag",
  "",
  ""
};

void sag(struct _Nfos_ *nfos)
{
  int ctr = 0;
  char channel[CHAN_NAME_MAX + 1];
  
  if(nfos->sender->message[0] == '#')
  {
    for(ctr = 0; nfos->sender->message[ctr] != ' '; ctr++)
      channel[ctr] = nfos->sender->message[ctr];
    
    channel[ctr] = '\0';
  }else
  {
    strcpy(channel, nfos->sender->nickname);
  }
  
  while(nfos->sender->message[ctr] != ':')
    ctr++;
  ctr++;
    
  irc_cmd("PRIVMSG %s :%s", channel, &nfos->sender->message[ctr + 5]);
}
