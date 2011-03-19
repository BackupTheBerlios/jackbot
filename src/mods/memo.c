#include "../lib/libjackbot.h"

void memo(struct _Nfos_ *nfos);

struct _Mods_ _mod_info = 
{                        
  &memo,               
  "Memo",             
  "memo",           
  "JOIN",           
  ""              
};               

/*
 * This mod is for writing a message to someone who is not online atm.
 * You (your nickname might be 'sergiorey') just write
 *   !memo JacKMcRiDER The pope is a jew!? O_o
 * 
 * When 'JacKMcRiDER' is joining the channel, the bot will get
 *   :JacKMcRiDER JOIN #foobar
 *
 * The bot searches through the memo file and finds the saved message. So he'll send
 *   NOTICE sergiorey :MEMO by sergiorey from 18.12.2010, 17h47: The pope is a jew!? O_o
 */
void memo(struct _Nfos_ *nfos)
{
  if( strcmp( nfos->sender->command, nfos->mods->server_cmd ) ) // if someone wrote "!memo" in a channel (so you've to record one)
  {
    // record memo
      // get channel
      // get receiver nickname
      // get sender nickname
      // get memo message
      // get date and time
      // write to memo file
      // answer "memo saved" to sender
  }else // if someone joined a channel
  {
    // search and push memo
      // get channel
      // get nickname of joined user
      // open memo file
      // search for joined users nickname
      // parse date, time, nick and memo message to "NOTICE [joined_nick/receiver_nick] :MEMO by [sender_nick] from [date], [time]: [memo]
      // delete memo from memo file
  }
}
