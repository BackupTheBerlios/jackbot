/*************************************\
 * project....: JacKBot              *
 * file.......: libjackbot.c         *
 * description: lib for mods and bot *
\*************************************/
#include "libjackbot.h"

void send_irc(char *msg)
{
  char full_msg[MSG_MAX + 1], cmd[MSG_MAX + 1];
  char *line, *cmd_end;

  if(strchr(msg, '\n'))
  {
    if((cmd_end = strchr(msg, ':')))
    {
      strncpy(cmd, msg, (cmd_end - msg));
      cmd[cmd_end - msg] = '\0'; // cmd[0] -> != cmd_end - msg + 1
      msg = cmd_end + 1;
      for(line = strtok(msg, "\n"); line; line = strtok(NULL, "\n"))
      {
	
#ifdef __linux__
        snprintf(full_msg, MSG_MAX, "%.*s%.*s\r\n", MSG_MAX - 2, cmd, (int)(MSG_MAX - 2 - strlen(cmd)), line);
#else
        // was working on FreeBSD
	      snprintf(full_msg, MSG_MAX, "%.*s%.*s\r\n", MSG_MAX - 2, cmd, MSG_MAX - 2 - strlen(cmd), line);
#endif // __linux__

	send(nfos->server->socket, full_msg, strlen(full_msg), 0);
	
	debug_out("sent: %s", full_msg);

      }
    }
  }else
  {
    snprintf(full_msg, MSG_MAX, "%.*s\r\n", MSG_MAX - 2, msg);
    send(nfos->server->socket, full_msg, strlen(full_msg), 0);
    
	debug_out("sent: %s", full_msg);

  }

  return;
}

int is_priv(void)
{
  if(nfos->sender->middle[0] == '#')
    return 0;
  else
    return 1;
}

int get_from_message(char dest[], int type)
{
  static int pos = 0;
  int tmp_pos = 0;

  switch(type)
  {
    case GFM_NEW:
      pos = 0;
      break;
    case GFM_WORD:
      if(pos == 0 && nfos->sender->message[0] == '!')
      {
        while(nfos->sender->message[pos] != ' ')
          pos++;
      }

      while(nfos->sender->message[pos] == ' ') pos++; // skip spaces
      tmp_pos = pos;
      while(nfos->sender->message[pos] != ' ' && nfos->sender->message[pos] != '\0') pos++; // now we have the word! write it to dest!

      strncpy(dest, &nfos->sender->message[tmp_pos], pos - tmp_pos);
      dest[pos - tmp_pos] = '\0';
      debug_out("WORD: %s\n", dest);
      break;
    case GFM_CHANNEL:
      while(nfos->sender->message[pos] != '\0')
      {
        while(nfos->sender->message[pos] == ' ') pos++;

        if(nfos->sender->message[pos] != '#') // no, it's not a channel!
        {
          while(nfos->sender->message[pos] != ' ') pos++;
          continue;
        }else
          break;
      }

      if(nfos->sender->message[pos] == '\0')
        return 0;

      tmp_pos = pos;
      while(nfos->sender->message[pos] != ' ' && nfos->sender->message[pos] != '\0') pos++;
      dest[pos - tmp_pos] = '\0';
      strncpy(dest, &nfos->sender->message[tmp_pos], pos - tmp_pos);

      debug_out("CHANNEL: %s\n", dest);
      break;
    case GFM_PARAMS:
      if(pos == 0 && nfos->sender->message[0] == '!')
      {
        while(nfos->sender->message[pos] != ' ')
          pos++;
      }

      while(nfos->sender->message[pos] == ' ') pos++;

      tmp_pos = pos;
      while(nfos->sender->message[pos] != '\0') pos++;

      strncpy(dest, &nfos->sender->message[tmp_pos], pos - tmp_pos + 1);
      debug_out("PARAMS: %s\n", dest);
      break;
    default:
      return 0;
      break;
  }

  return 1;
}

