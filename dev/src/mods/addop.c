/*
   This mod is for adding nicknames to a channel in the file oplist.mod
   That file will be read by the mod op_by_list and will give OP prevs
   to the nicknames in the channel
*/

#include "../lib/libjackbot.h"

void addop(struct _Nfos_ *nfos);

struct _Mods_ _mod_info =
{
  NULL,
  &addop,
  "Add OP",
  "addop",
  "",
  ""
};

char *opfile = "mods/oplist.mod";

void addop(struct _Nfos_ *nfos)
{
  static FILE *fp_list;
  int ctr = 0;
  char channel[CHAN_NAME_MAX + 1];
  char list_channel[CHAN_NAME_MAX + 1];
  char nickname[NICK_NAME_MAX + 1];
  char *name_to_add;

  if(nfos->sender->message[0] == '#')
  {
    for(ctr = 0; nfos->sender->message[ctr] != ' '; ctr++)
      channel[ctr] = nfos->sender->message[ctr];             
    
    channel[ctr] = '\0';
  }

  while(nfos->sender->message[ctr] != ':')
    ctr++;
  ctr++;

  name_to_add = &nfos->sender->message[ctr + 7];

  if(!fp_list)
  {
    if(!(fp_list = fopen(opfile, "r+"))) // open filefile doesn't exist yet
      fopen(opfile, "w+"); // create file
  }else
    rewind(fp_list);
  
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

      if(!strcmp(channel, list_channel)) //I've found the searched channel
      {
        while(fgets(nickname, NICK_NAME_MAX + 1, fp_list))
        {
          if(nickname[0] == '\n')
            break; // nickname not in the list - has to be added
          
          if(nickname[strlen(nickname) - 1] == '\n')
            nickname[strlen(nickname) - 1] = '\0';

          if(!strcmp(name_to_add, nickname)) // nickname is already in the list...
          {
            irc_cmd("PRIVMSG %s :%s is already in OP list", channel, name_to_add);
            return;
          }
        }
        // break destination
        // write name_to_add in file
        fprintf(fp_list, "\b%s\n", name_to_add); 
        fflush(fp_list);
        irc_cmd("PRIVMSG %s :%s added to OPs of channel %s!", channel, name_to_add, channel);
        return;
      }
    }
  }
  //I didn't find the channel
  //channel and nickname have to be added
  fprintf(fp_list, "%s\n%s", channel, name_to_add);
  fflush(fp_list);
  irc_cmd("PRIVMSG %s :%s was proudly added as first OP of channel %s!", channel, name_to_add, channel);
}
