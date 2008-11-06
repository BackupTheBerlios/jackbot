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
        snprintf(full_msg, MSG_MAX, "%.*s%.*s\r\n", MSG_MAX - 2, cmd, MSG_MAX - 2 - strlen(cmd), line);
        send(nfos->server->socket, full_msg, strlen(full_msg), 0);
      }
    }
  }else
  {
    snprintf(full_msg, MSG_MAX, "%.*s\r\n", MSG_MAX - 2, msg);
    send(nfos->server->socket, full_msg, strlen(full_msg), 0);
  }

  return;
}
