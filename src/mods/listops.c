#include "../lib/libjackbot.h"

void listops(struct _Nfos_ *nfos);

struct _Mods_ _mod_info =
{
  &listops,
  "List OPs",
  "listops",
  "",
  ""
};

char *opfile = "mods/oplist.mod";

void listops(struct _Nfos_ *nfos)
{
  FILE *fp_list;
  int ctr, ctr2;
  char *channel = nfos->sender->middle;
  char list_channel[CHAN_NAME_MAX + 1];
  char nickname[NICK_NAME_MAX + 1];

  if(is_priv())
    return;

  fp_list = fopen(opfile, "r");
  
  if(fp_list == NULL)
  {
    printf("Error on opening %s\n", opfile);
    return;
  }
    
  while(fgets(list_channel, CHAN_NAME_MAX + 1, fp_list))
  {
    if(list_channel[0] == '#')
    {
      
//FIXXME - boeser hack! evtl. 
      if(list_channel[strlen(list_channel) - 1] == '\n')
	      list_channel[strlen(list_channel) - 1] = '\0';
//FIXXME

      if(!strcmp(channel, list_channel)) // found channel
      {
	      while(fgets(nickname, NICK_NAME_MAX + 1, fp_list))
	      {
	        if(nickname[0] == '\n')
	          return; 
	  
	        if(nickname[strlen(nickname) - 1] == '\n')
	          nickname[strlen(nickname) - 1] = '\0';

	        irc_cmd("NOTICE %s :%s", nfos->sender->nickname, nickname);
	      }
      }else // no list yet | issue: whats about empty channel lists? perhaps deleting them in delop.c
      {
        irc_cmd("NOTICE %s :No OPs for channel %s!", nfos->sender->nickname, channel);
      }
    }
  }

  fclose(fp_list);
}
