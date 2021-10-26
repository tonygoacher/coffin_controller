
#include <stdio.h>
#include <stdlib.h>
#include "SDL.h"
#include "SDL_mixer.h"

#include "SDLWrapper.h"
#include "SDLBlockedSample.h"
#include "SDLAudio.h"

#include "PinMonitor.h"

#include "TimedOutput.h"

#include "Doorbell.h"
#include "SoundList.h"
#include "MarkSpaceOutput.h"
#include "Coffin.h"
#include <unistd.h>




/* Mix_Chunk is like Mix_Music, only it's for ordinary sounds. */
Mix_Chunk *phaser = NULL;

void Finished(int channel)
{
	int a=1;
}

/* Every sound that gets played is assigned to a channel.  Note that
   this is different from the number of channels you request when you
   open the audio device; a channel in SDL_mixer holds information
   about a sound sample that is playing, while the number of channels
   you request when opening the device is dependant on what sort of
   sound you want (1 channel = mono, 2 = stereo, etc) */
int phaserChannel = -1;

void handleKey(SDL_KeyboardEvent key);

int main(int argc, char *argv[]) {
// 
 	CSDLWrapper::GetInstance()->Init(CSDLWrapper::eAudioOnly);

	CSoundList::GetInstance()->Go();		
	CDoorBell::GetInstance();
	CCoffin::GetInstance();
	CMarkSpaceOutput* pMarkSpace = new CMarkSpaceOutput( 15, false);
	while(1) 
	{
		sleep(10);

	}
	delete pMarkSpace;
	delete CCoffin::GetInstance();
	delete CSoundList::GetInstance();
	return 0;

}



void handleKey(SDL_KeyboardEvent key) {
  switch(key.keysym.sym) {
  case SDLK_p:

    /* We're going to have the phaser continually fire as long as
       the user is holding the button down */
    if(key.type == SDL_KEYDOWN) {

      if(phaserChannel < 0) {

	/* Mix_PlayChannel takes, as its arguments, the channel that
	   the given sound should be played on, the sound itself, and
	   the number of times it should be looped.  If you don't care
	   what channel the sound plays on, just pass in -1.  Looping
	   works like Mix_PlayMusic. This function returns the channel
	   that the sound was assigned to, which you'll need later. */
	phaserChannel = Mix_PlayChannel(-1, phaser, -1);
	
      }
    } else {
      /* Mix_HaltChannel stops a certain channel from playing - this
	 is one of the reasons we kept track of which channel the
	 phaser had been assigned to */
      Mix_HaltChannel(phaserChannel);
      
      phaserChannel = -1;
    }
    break;
  }
}
