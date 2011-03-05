#ifndef __SND_H
#define __SND_H

//TODO: Play small sound clips for sound effects

void SND_init(void);
void SND_shutdown(void);

void SND_play_music(const char *f);
//void SND_play_sound();
void SND_stop();
void SND_volume(int v);

#endif /* !__SND_H */
