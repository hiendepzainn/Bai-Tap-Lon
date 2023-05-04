#include "pipe.h"
#include "stdio.h"
#include "lib.h"

vector<position> posPipe;
bool increase[4] = {false, true, false, true};

bool pipe::init()
{
    posPipe.clear();
    for (signed char i = 0; i < TOTAL_PIPE; i++)
    {
        position temp;
        temp.getPos(SCREEN_WIDTH + i * PIPE_DISTANCE + 350, (rand() % (randMax - randMin + 1)) + randMin);
        posPipe.push_back(temp);
    }
    if (isNULL())
    {
        if (Load( "res/image/pipe2.png", 1 ))
        {
            return true;
        }
    }
    return false;
}

void pipe::Free()
{
    free();
}

void pipe::render()
{
    for (signed char i = 0; i < TOTAL_PIPE; i++)
    {
        if (posPipe[i].x <= SCREEN_WIDTH && posPipe[i].x > -getWidth())
        {
            Render(posPipe[i].x, posPipe[i].y);
            Render(posPipe[i].x, posPipe[i].y + getHeight() + PIPE_SPACE, 180);
        }
        //Render(posPipe[i].x, posPipe[i].y + getHeight() + PIPE_SPACE, 180);
    }
}

void pipe::update()
{
    if (!die)
    {
        for (signed char i = 0; i < TOTAL_PIPE; i++)
        {
            if (posPipe[i].x < - getWidth())
            {
                posPipe[i].y = (rand() % (randMax - randMin + 1)) + randMin;
                posPipe[i].x = posPipe[(i + TOTAL_PIPE - 1) % TOTAL_PIPE].x + PIPE_DISTANCE;
            }
            else
            {
                if (score <= 6) posPipe[i].x -= 3;
                if (score > 6)
                {
                    if (i % 2 == 0) posPipe[i].x -= 4;
                    else posPipe[i].x -= 3;
                }
                if (score > 5)
                {
                    if (increase[i] == false) posPipe[i].y -= 1;
                    if (posPipe[i].y == randMin) increase[i] = true;
                    if (increase[i] == true) posPipe[i].y += 1;
                    if (posPipe[i].y == randMax) increase[i] = false;
                }
            }
        }
    }
}
