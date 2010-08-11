#include "../lib/libjackbot.h"

void op_by_list(struct _Nfos_ *nfos);

struct _Mods_ _mod_info =
{
  NULL,
  &op_by_list,
  "OP by list",
  "",
  "JOIN",
  ""
};

char *opfile = "mods/oplist.mod";

void op_by_list(struct _Nfos_ *nfos)
{
  static FILE *fp_list;
  int ctr, ctr2;
  char channel[CHAN_NAME_MAX + 1];;
  char list_channel[CHAN_NAME_MAX + 1];
  char nickname[NICK_NAME_MAX + 1];

  if(!fp_list)
    fp_list = fopen(opfile, "r");
  else
    rewind(fp_list);
  
  if(fp_list == NULL)
  {
    printf("Error on opening %s\n", opfile);
    return;
  }
  
  for(ctr = 0; nfos->sender->message[ctr] != '#'; ctr++);
  for(ctr2 = 0; nfos->sender->message[ctr] != ' ' && ctr2 <= CHAN_NAME_MAX; ctr2++)
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

      if(!strcmp(channel, list_channel))
      {
	while(fgets(nickname, NICK_NAME_MAX + 1, fp_list))
	{
	  if(nickname[0] == '\n')
	    break;
	  
	  if(nickname[strlen(nickname) - 1] == '\n')
	    nickname[strlen(nickname) - 1] = '\0';

	  if(!strcmp(nfos->sender->nickname, nickname))
	  {
	    irc_cmd("MODE %s +o %s", channel, nickname);
	    return;
	  }
	}
      }
    }
  }
}
