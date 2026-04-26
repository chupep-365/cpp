// g++ main.cpp -o main.out -lncurses --no-warnings && ./main.out

#include <iostream>
#include <cstdint>
#include <math.h>
#include <ncurses.h>

#include "vec.hpp"

bool circle(float x, float y, float r = 0.5) { return x * x + y * y < r;}

float dist(float x, float y) { return sqrt(x * x + y * y); }

float clamp(float value, float min, float max) { return fmax(fmin(value, max), min); }



int main(){
    initscr();            // Запускаем режим ncurses

    int w = 125, h = 50;
    float asp = 1.0 * w / h;
    float pxlAsp = 11.0f / 24.0f;
    char gradient[] = " .:!/r(l1Z4H9W8$@";
    int gradientSize = std::size(gradient) - 2;

    char* sc = new char[w * h + 1];
    sc[w * h] = '\0';

    for (int t = 0; t < 9999999; ++t){

        //vec3 light(0.1, sin(t * 0.01), cos(t * 0.02));
        vec3 light(sin(t * 0.005), cos(t * -0.005), -0.7);
        // vec3 light(-0.8, -0.5, -0.7);

        light = light.norm();

        vec3 ro(-1.5, 0, 0);

        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j){

                vec2 uv = vec2(j, i) / vec2(w, h) * 2.0f - 1.0f;
                uv.x *= asp * pxlAsp;

                vec3 rd(1, uv);
                rd = rd.norm();

                vec2 intersect = sphere(ro, rd, 1);

                int color = 0;
                if (intersect.x > 0) {dssd
                    vec3 n = (ro + rd * intersect.x).norm();
                    float deg = light.dot(n);
                    color = deg * gradientSize;
                }

                color = clamp(color, 0, gradientSize - 1);
                char pixel = gradient[color];
                sc[i * w + j] = pixel;
            }
        }
        mvprintw(0, 0, sc);
        refresh();
    }

    getch();
    endwin();

    delete[] sc;

    return 0;
}