/*
   This mod is for adding nicknames to a channel in the file oplist.mod
   That file will be read by the mod op_by_list and will give OP prevs
   to the nicknames in the channel
*/

#include "../lib/libjackbot.h"

void delop(struct _Nfos_ *nfos);
void finish_opfile(void);

struct _Mods_ _mod_info =
{
  NULL,
  &delop,
  "Delete OP",
  "delop",
  "",
  ""
};

FILE *fp_oplist;
FILE *fp_tmplist;
char *opfile_name  = "mods/oplist.mod";
char *tmpfile_name = "mods/tmpfile.mod";

void delop(struct _Nfos_ *nfos)
{
  int ctr = 0;
  char channel[CHAN_NAME_MAX + 1];
  char list_channel[CHAN_NAME_MAX + 1];
  char nickname[NICK_NAME_MAX + 1];
  char *name_to_del;

  if(nfos->sender->message[0] == '#')
  {
    for(ctr = 0; nfos->sender->message[ctr] != ' '; ctr++)
      channel[ctr] = nfos->sender->message[ctr];             
    
    channel[ctr] = '\0';
  }

  while(nfos->sender->message[ctr] != ':')
    ctr++;
  ctr++;

  name_to_del = &nfos->sender->message[ctr + 7];

  if(!(fp_oplist = fopen(opfile_name, "r+"))) // if file doesn't exist yet
  {
    irc_cmd("PRIVMSG %s :%s is not in the OP list of %s!", channel, name_to_del, channel);
    return;
  }
  
  fp_tmplist = fopen(tmpfile_name, "w");
  
  if(fp_oplist == NULL || fp_tmplist == NULL)
  {
    printf("Error on opening a file\n");
    return;
  }
  
  while(fgets(list_channel, CHAN_NAME_MAX + 1, fp_oplist) && fputs(list_channel, fp_tmplist))
  {
    if(list_channel[0] == '#')
    {
      
//FIXXME - boeser hack! evtl. 
      if(list_channel[strlen(list_channel) - 1] == '\n')
        list_channel[strlen(list_channel) - 1] = '\0';
//FIXXME

      if(!strcmp(channel, list_channel)) //I've found the searched channel
      {
        while(fgets(nickname, NICK_NAME_MAX + 1, fp_oplist))
        {
          if(nickname[0] == '\n')
          {
            // nickname not in the list
            fputs(nickname, fp_tmplist);
            irc_cmd("PRIVMSG %s :%s is not in the OP list of %s!", channel, name_to_del, channel);
            finish_opfile();
            return;
          }
          
          if(nickname[strlen(nickname) - 1] == '\n') // the 'if' is because at the end of the file there is no '\n'
            nickname[strlen(nickname) - 1] = '\0';

          if(strcmp(name_to_del, nickname)) // cpy nick to file if the nickname is *not* name_do_del
            fprintf(fp_tmplist, "%s\n", nickname);
          else
          {
            irc_cmd("PRIVMSG %s :Deleted %s from the OP list of %s", channel, name_to_del, channel); // 've find nickname - don't cpy to file -> deleted
            finish_opfile();
            return;
          }
        }
      }
    }
  }
  // nickname not in the list
  irc_cmd("PRIVMSG %s :%s is not in the OP list of this channel!", channel, name_to_del);
  finish_opfile();
}

void finish_opfile(void)
{
  char line[CHAN_NAME_MAX + 1];

  while(fgets(line, CHAN_NAME_MAX + 1, fp_oplist) && fputs(line, fp_tmplist));

  if(!rename(tmpfile_name, opfile_name))
    remove(tmpfile_name);
  else
    printf("Error on renaming %s", tmpfile_name);

  fclose(fp_oplist);
  fclose(fp_tmplist);
}
