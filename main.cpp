#include "game.h"
#include "lib.h"
#include <time.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

const short int FPS = 60;
const short int frameDelay = 1000 / FPS;

using namespace std;

int main(int argc, char** argv)
{
    Uint32 frameStart;
    short int frameTime;
    game g;
    bool isMenu = 0;
    bool isPause = 0;
    bool isDark = 0;

    Mix_Music *music = NULL;
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
        {
            printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        }

    srand(time(NULL));
    string a[7] = {"D:/codeblock/19-04/res/sound/1.mp3",
                   "D:/codeblock/19-04/res/sound/2.mp3",
                   "D:/codeblock/19-04/res/sound/3.mp3",
                   "D:/codeblock/19-04/res/sound/4.mp3",
                   "D:/codeblock/19-04/res/sound/5.mp3",
                   "D:/codeblock/19-04/res/sound/6.mp3",
                   "D:/codeblock/19-04/res/sound/7.mp3"};
    int randomIndex = rand() % 7;
    music = Mix_LoadMUS(a[randomIndex].c_str());

    if (music == NULL) printf("Failed to load music! SDL_mixer Error: %s\n", Mix_GetError());
    //Mix_PlayMusic(music, -1);

    int dem = 0;

    while(!g.isQuit())
    {
        dem++;
        //if (dem > 1) Mix_ResumeMusic();
        frameStart = SDL_GetTicks();

        if (g.isDie())
        {
            if (isMenu)
            {
                g.sound.playHit();
                //Mix_PauseMusic();
            }
            g.userInput.Type = game::input::NONE;

            while(g.isDie() && !g.isQuit())
            {
                g.takeInput();
                if (isMenu == 1 && g.userInput.Type == game::input::PLAY)
                {
                    if (g.checkReplay())
                    {
                        isMenu = 0;
                    }
                    g.userInput.Type = game::input::NONE;
                }

                if (!isDark) g.renderBackground();
                else g.renderBackgroundNight();
                g.pipe.render();
                g.land.render();

                if (isMenu)
                {
                    g.shiba.render();
                    g.shiba.fall();
                    g.renderGameOver();
                    g.renderMedal();
                    g.renderScoreSmall();
                    g.renderBestScore();
                    g.replay();
                }
                else
                {
                    g.pipe.init();
                    g.shiba.init(isDark);
                    g.shiba.render();
                    g.renderMessage();
                    if (g.userInput.Type == game::input::PLAY)
                    {
                        g.Restart();
                        isMenu = 1;
                        g.userInput.Type = game::input::NONE;
                    }
                    g.land.update();
                }
                g.display();
            }
            //g.pipe.init();
        }
        else
        {
            g.takeInput();
            if (g.userInput.Type == game::input::PAUSE)
            {
                isPause = abs(1 - isPause);
                g.userInput.Type = game::input::NONE;
            }

            if (isPause == 0 && g.userInput.Type == game::input::PLAY)
            {
                g.sound.playBreath();
                g.shiba.resetTime();
                g.userInput.Type = game::input::NONE;
            }

            if (!isDark) g.renderBackground();
            else g.renderBackgroundNight();
            g.pipe.render();
            g.land.render();
            g.shiba.render();
            g.renderScoreLarge();

            if (!isPause)
            {
                g.shiba.update(g.getPipeWidth(), g.getPipeHeight());
                g.pipe.update();
                g.land.update();
                g.pause();
            }
            else
            {
                g.resume();
                g.renderPauseTab();
                g.renderScoreSmall();
                g.renderBestScore();
                g.replay();
                if (!isDark) g.lightTheme(); else g.darkTheme();
                g.nextButton();
                if (g.userInput.Type == game::input::PLAY)
                {
                    if (g.checkReplay())
                    {
                        isPause = 0;
                    }
                    else if (g.changeTheme())
                    {
                        isDark = abs(1 - isDark);
                        g.shiba.init(isDark);
                    }
                    g.userInput.Type = game::input::NONE;
                }
            }
            g.display();
        }

        frameTime = SDL_GetTicks() - frameStart;
        if (frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
    }
    Mix_FreeMusic(music);
    music = NULL;
    return 0;
}
