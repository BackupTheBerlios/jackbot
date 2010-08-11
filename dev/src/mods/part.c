#include "../lib/libjackbot.h"

void part(struct _Nfos_ *nfos);

struct _Mods_ _mod_info =
{
  NULL,
  &part,
  "Part",
  "part",
  "",
  ""
};

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
    
  irc_cmd("PART %s :Tschuess, %s will mich loswerden!", channel, nfos->sender->nickname);
}
