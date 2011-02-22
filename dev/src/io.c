/*******************************************\
 * project....: JacKBot                    *
 * file.......: io.c                       *
 * description: functions for input/output *
\*******************************************/
#include "io.h"

void parse_prefix(char *prefix);

void parse_msg(char *msg)
{
  // see RFC 1459, 2.3
  int msg_pos = 0, cur_pos; // msg_pos: position in 'msg'; cur_pos: position in current string to parse (such as nickname etc.)
  char tmp_str[512];

  memset(nfos->sender, 0, sizeof(_Sender_t));
  
  if(msg[0] == ':') // not a protocol command?
  { 
    // get the prefix
    for(cur_pos = 0; msg[msg_pos] != ' '; msg_pos++, cur_pos++)
      tmp_str[cur_pos] = msg[msg_pos];
    tmp_str[cur_pos] = '\0';

    parse_prefix(tmp_str + 1); // not sending the colon (:)
    
    // because RFC 1459 define <SPACE> as one ' ' _or more_
    while(msg[msg_pos] == ' ')
      msg_pos++;
  }

  //get the command
  if(isalpha(msg[msg_pos])) // if it as a command
  {
    for(cur_pos = 0; msg[msg_pos] != ' '; msg_pos++, cur_pos++)
      tmp_str[cur_pos] = msg[msg_pos];
    tmp_str[cur_pos] = '\0';
    strncpy(nfos->sender->command, tmp_str, SERVER_CMD_MAX + 1);
  }else // if we have a request number
  {
    strcpy(nfos->sender->command, "0");
    //sprintf(nfos->sender->request_nr, "%c%c%c", msg[msg_pos], msg[++msg_pos], msg[++msg_pos]); // is that fine code???
    nfos->sender->request_nr[0] = msg[msg_pos];
    nfos->sender->request_nr[1] = msg[++msg_pos];
    nfos->sender->request_nr[2] = msg[++msg_pos];
    nfos->sender->request_nr[3] = '\0';

    msg_pos++;
  }

  while(msg[msg_pos] == ' ')
    msg_pos++;

  if(msg[msg_pos] != ':') // we got a middle!
  {
    for(cur_pos = 0; msg[msg_pos] != ':'; msg_pos++, cur_pos++)
      tmp_str[cur_pos] = msg[msg_pos];
    // there might be spaces before colon (:)
    for(; msg[msg_pos] == ' '; msg_pos--, cur_pos--);
    tmp_str[--cur_pos] = '\0';
    strncpy(nfos->sender->middle, tmp_str, MIDDLE_MAX + 1);
  }

  while(msg[msg_pos] != ':') 
    msg_pos++;
  msg_pos++; //because noone needs the colon (:)...

  // now the rest is called "message", so all of these <params> are 'ignored'
  //strncpy(nfos->sender->message, &msg[msg_pos], MSG_MAX + 1);
  snprintf(nfos->sender->message, MSG_MAX + 1, "%s", &msg[msg_pos]);

  return;
}

void parse_prefix(char *prefix)
{
  char *chr_return;
  if((chr_return = strchr(prefix, '!'))) // if we have a "nick-prefix"
  {
    // hey, a nick + user + host
    *chr_return = '\0';
    strncpy(nfos->sender->nickname, prefix, NICK_NAME_MAX); // get nick

    prefix = chr_return + 1;
    if(!(chr_return = strchr(prefix, '@')))
    {
      printf("ERROR: bad message!\n");
      exit(EXIT_FAILURE);
    }
    *chr_return = '\0';
    strncpy(nfos->sender->user, prefix, NICK_NAME_MAX); // get user

    prefix = chr_return + 1;
    strncpy(nfos->sender->host, prefix, HOST_NAME_MAX); // get host
  }else // hmm... in seems to be a "server-prefix"
    strncpy(nfos->sender->servername, prefix, HOST_NAME_MAX);
}
