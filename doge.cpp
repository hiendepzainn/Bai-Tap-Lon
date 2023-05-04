#include "doge.h"
#include <stdio.h>
#include <iostream>
#include "SDL_mixer.h"

bool doge::init(bool isDark)
{
    string shiba_path = "D:/codeblock/19-04/res/image/cat.png";
    if (isDark) shiba_path = "D:/codeblock/19-04/res/image/cat-dark.png";
    if (saved_path == shiba_path)
    {
        posDoge.getPos(75, SCREEN_HEIGHT / 2 - 10);
        ahead = 0;
        angle = 0;
    }
    if (isNULL() || saved_path != shiba_path)
    {
        saved_path = shiba_path;
        if ( Load(shiba_path.c_str() , 1) )
        {
            return true;
        }
        else
        {
            return false;
        }

    }
    return false;
}

void doge::Free()
{
    free();
}

void doge::render()
{
    Render(posDoge.x, posDoge.y, angle);
}

void doge::fall()
{
    if (die && posDoge.y < SCREEN_HEIGHT - LAND_HEIGHT - SHIBA_HEIGHT - 5)
    {
        if (time == 0)
        {
            x0 = posDoge.y;
            angle = -25;
        }
        else if (angle < 70 && time > 30)
        {
            angle += 3;
        }

        if (time >= 0)
        {
            posDoge.y = x0 + time * time * 0.18 - 7.3 * time;
            time++;
        }
    }
    else return;
}

void doge::update(short int pipeWidth, short int pipeHeight)
{
    if (!die)
    {
        if (time == 0)
        {
            x0 = posDoge.y;
            angle = -25;
        }
        else if ( time > 30)
        {
            angle += 3;
        }

        if (time >= 0)
        {
            posDoge.y = x0 + time * time * 0.18 - 7.3 * time;
            time++;
        }

        if ( (posDoge.x + getWidth() > posPipe[ahead].x + 5) && (posDoge.x + 5 < posPipe[ahead].x + pipeWidth) &&
             (posDoge.y + 5 < posPipe[ahead].y + pipeHeight || posDoge.y  + getHeight() > posPipe[ahead].y + pipeHeight + PIPE_SPACE + 5) )
        {
            die = true;
        }
        else if (posDoge.x > posPipe[ahead].x + pipeWidth )
        {
            ahead = ( ahead + 1 ) % TOTAL_PIPE;
            score++;
            Mix_Chunk *sound_qua_cot = NULL;
            Mix_Chunk *sound_qua_cot_10 = NULL;
            sound_qua_cot = Mix_LoadWAV("D:/codeblock/19-04/res/sound/pass.wav");
            sound_qua_cot_10 = Mix_LoadWAV("D:/codeblock/19-04/res/sound/pass-10.wav");
            if (score % 10 != 0) Mix_PlayChannel(0, sound_qua_cot, 0);
            else Mix_PlayChannel(1, sound_qua_cot_10, 0);
        }

        if (posDoge.y > SCREEN_HEIGHT - LAND_HEIGHT -  SHIBA_HEIGHT - 5 || posDoge.y < - 10 )
        {
            die = true;
        }
    }
}
