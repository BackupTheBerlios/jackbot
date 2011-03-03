/******************************\
 * project....: JacKBot       *
 * file.......: libjackbot.h  *
 * desrciption: prototypes... *
\******************************/
#ifndef _LIBJACKBOT_H_
#define _LIBJACKBOT_H_

#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <ctype.h>

#define APP_NAME "JacKBot"
#define APP_WWW  "http://jackbot.berlios.de/"
#define VERSION  "0.2-BETA"

#undef HOST_NAME_MAX
#define HOST_NAME_MAX  255
#define PORT_MAX       5
#define NICK_NAME_MAX  24
#define SERVER_CMD_MAX 8
#define MIDDLE_MAX     512
#define CMD_PREFIX     '!'

// defined by RFC 1459; 1.3 */
#define CHAN_NAME_MAX 200
// defined by RFC 1459; 2.3 */
#define MSG_MAX       512

// define mod consts
#define MOD_NAME_MAX 20
#define MOD_CMD_MAX  20
#define MOD_PATH     "mods"
#define MOD_PATH_MAX 255

// define macros
#define irc_cmd(...) snprintf(buffer_, MSG_MAX, __VA_ARGS__); send_irc(buffer_);

// these are for get_from_message():
#define GFM_NEW     0 // init get_from_message()
#define GFM_CMD     1 // the !cmd
#define GFM_WORD    2 // next word. not !cmd 
#define GFM_CHANNEL 3 // next channel. skips all words before
#define GFM_PARAMS  4 // everything after last calls position, if first call everything after !cmd

#if (DEBUG)
#define debug_out(...) fprintf(stderr, __VA_ARGS__);
#else
#define debug_out(...) //
#endif

// define structs
struct _Server_;
struct _Sender_;
struct _Mods_;
struct _Nfos_;

typedef struct _Server_
{
  //struct protoent *proto;
  int  socket;
  char hostname[HOST_NAME_MAX + 1];
  char ip[15 + 1];
  char port[PORT_MAX + 1];
  char nickname[NICK_NAME_MAX];
}_Server_t;

typedef struct _Sender_
{
  char nickname[NICK_NAME_MAX + 1];   // nick
  char user[NICK_NAME_MAX + 1];       // user
  char host[HOST_NAME_MAX + 1];       // host
  char servername[HOST_NAME_MAX + 1]; // servername; see RFC 952 for max length
  char command[SERVER_CMD_MAX + 1];   // command
  char request_nr[3 + 1];             // requestnumber (as a string!)
  char middle[MIDDLE_MAX + 1];        // a middle - see RFC 1459, section 2.3.1
  char message[MSG_MAX + 1];
}_Sender_t;

typedef struct _Mods_
{
  struct _Mods_ *next;                   // don't use it... init as NULL
  void (*mod_main)(struct _Nfos_ *nfos); // pointer to the mods main, init as &your_mods_main
  char name[MOD_NAME_MAX + 1];           // the name of your mod (20)
  char mod_cmd[MOD_CMD_MAX + 1];         // command in message, starts with ! (20), "0" is not allowed!
  char server_cmd[SERVER_CMD_MAX + 1];   // server message, as PRIVMSG or INVITE (8), "0" is not allowed!
  char requests[9];                      // server request numbers (3 x 3)
}_Mods_t;


typedef struct _Nfos_
{
  struct _Server_ *server;
  struct _Sender_ *sender;
  struct _Mods_   *mods;
  struct _Mods_   *first_mod;
}_Nfos_t;

// define prototypes
void send_irc(char *msg);
int is_priv(void);
int get_from_message(char dest[], int type);

// define globals
_Nfos_t *nfos;
char buffer_[1024];

#endif // _LIBJACKBOT_H_
