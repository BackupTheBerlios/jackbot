#include "../lib/libjackbot.h"

void listops(struct _Nfos_ *nfos);

struct _Mods_ _mod_info =
{
  NULL,
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
  char channel[CHAN_NAME_MAX + 1];;
  char list_channel[CHAN_NAME_MAX + 1];
  char nickname[NICK_NAME_MAX + 1];

  fp_list = fopen(opfile, "r");
  
  if(fp_list == NULL)
  {
    printf("Error on opening %s\n", opfile);
    return;
  }
  
  for(ctr = 0; nfos->sender->message[ctr] != '#'; ctr++);
  for(ctr2 = 0; nfos->sender->message[ctr + ctr2] != ' ' && ctr2 <= CHAN_NAME_MAX; ctr2++)
    channel[ctr2] = nfos->sender->message[ctr + ctr2];
  channel[ctr2] = '\0';
  
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
      }
    }
  }

  fclose(fp_list);
}
