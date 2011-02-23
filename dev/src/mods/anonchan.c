#include "../lib/libjackbot.h"

void modmain(struct _Nfos_ *nfos); 

struct _Mods_ _mod_info = 
{                         
  NULL,                   
  &modmain,                  
  "anonchan", 
  "NOTICE",                    
  "anonchan",                
  "" 
};

typedef struct _Nicks_
{
  char nick[NICK_NAME_MAX];
  struct _Nicks_ *next;
  struct _Nicks_ *prev;
}Nicks;

Nicks *firstnick, *nicklist, *tmpnick;

/*
 * description
 */
void modmain(struct _Nfos_ *nfos)
{
  char nickname[NICK_NAME_MAX + 1];
  char cmd[MOD_CMD_MAX + 1];
  char param = [4 + 1]; // join/part + \0

  get_from_message(cmd, GFM_CMD);

  if(!strcmp(cmd, nfos->mods->mod_cmd))
  {
    get_from_message(param, GFM_PARAM);

    // siehe Kalender...
  }
}
