/*Copyright (C) 2015 Wyatt Huckaby
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * /along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <string>
#include <iostream>

#ifdef __WIN32
	#include <windows.h>
#else
	#include <sys/stat.h>
 	#include <sys/types.h>
#endif


int FontRenderStyle = 0;
std::string texpath;
int size;
bool string_ends_with(const std::string& s, const std::string& suffix){
    return s.rfind(suffix) == (s.size()-suffix.size());
}

enum textquality {solid, shaded, blended};

 SDL_Surface *drawtext(TTF_Font *fonttodraw, char fgR, char fgG, char fgB, char fgA,
 char bgR, char bgG, char bgB, char bgA, char text[], textquality quality)
 {
   SDL_Color tmpfontcolor = {fgR,fgG,fgB,fgA};
   SDL_Color tmpfontbgcolor = {bgR, bgG, bgB, bgA};
   SDL_Surface *resulting_text;

   if (quality == solid) resulting_text = TTF_RenderText_Solid(fonttodraw, text, tmpfontcolor);
   else if (quality == shaded) resulting_text = TTF_RenderText_Shaded(fonttodraw, text, tmpfontcolor, tmpfontbgcolor);
   else if (quality == blended) resulting_text = TTF_RenderText_Blended(fonttodraw, text, tmpfontcolor);

   return resulting_text;
 }

void createDirectory(const char *folder){
	#ifdef __WIN32
	CreateDirectory(folder, NULL);
	#else
	struct stat st = {0};
    if (stat(folder, &st) == -1){
		mkdir (folder, 0777);
	}

	#endif
}

int main (int argc, char *argv[]){
	if (SDL_Init(SDL_INIT_EVERYTHING)< 0){
		printf("Failed to init SDL Version 1.2 \n");
		SDL_Quit(); //clean up whatever is left
		return 1;
	}

    if (TTF_Init() == -1)
    {
        printf("Unable to initialize SDL_ttf: %s \n", TTF_GetError());
        SDL_Quit();
        return 1;
    }
	//check arguments
    for (unsigned i = 1; i < argc; i++){
		std::string argument(argv[i]);
		if (argument.find ("-s") == 0){
			argument.erase(0,2);
			char *argument_info = const_cast <char *> (argument.c_str());
			if (argument_info == "solid"){
				FontRenderStyle = 0;
			} else if (argument_info == "shaded"){
				FontRenderStyle = 1;
			} else if (argument_info == "blended"){
				FontRenderStyle = 2;
			} else {
				printf ("Invalid argument after '-s' token \n");
				SDL_Quit();
				return 1;
			}
		}
		else if (argument.find ("-p") == 0){
			//extract path
            if (!string_ends_with(argument, std::string (".ttf"))){
				printf ("This converter only accepts .ttf file types \n");
				SDL_Quit();
				return 1;
			}
			argument.erase (0,2);
			texpath = argument;
        } else if (argument.find ("-i") == 0){
            argument.erase(0,2);
            char *argument_info = const_cast <char *> (argument.c_str());
            size = atoi (argument_info);
        }
		else {
			printf("Invalid argument, %s \n", argument.c_str());
            SDL_Quit();
            return 1;
		}
	}


    createDirectory("Font-Coverted");
    //now is where the conversion starts.
    char toPrint;
    TTF_Font *font;
    font = TTF_OpenFont(texpath.c_str(), size);
    /*
    SDL_Surface *character = drawtext(font, 1,1,1,1,0,0,0,1, "a", solid);
    SDL_SaveBMP(character, "a.bmp");
    */

    char *print;

    for  (int i =33; i <  123; i++){

        /*
        if ((char)i == '#') continue;
        if ((char)i == '%') continue;
        if ((char)i == '*') continue;
        if ((char)i == '+') continue;
        if ((char)i == '-') continue;
        if ((char)i == '/') continue;
        if ((char)i == '<') continue;
        if ((char)i == '>') continue;
        if ((char)i == '=') continue;
        if ((char)i == '@') continue;
        if ((char)i == '[') continue;
        if ((char)i == ']') continue;
        if ((char)i == '\/') continue;
        */
        char pre_print [2] = {(char) i , 0};
        //char *printx = print;
        //std::cout << printx << std::endl;
        print = pre_print;
        SDL_Surface *character = drawtext(font, 1,1,1,1,0,0,0,1, print, solid);

        if (!character) continue;
        std::cout <<print << std::endl;
        /*
        std::stringstream ss;
        ss << "Font/";
        ss << print;
        ss << ".bmp";
        std::string filesavepath = ss.str();
        */
        std::string filesavepath;
        filesavepath += std::string( "Font/");
        filesavepath += std::string (print);
        filesavepath += std::string (".bmp");
        std::cout << "Saving:" << filesavepath << std::endl;
        SDL_SaveBMP(character, filesavepath.c_str());


    }
    SDL_Quit();
    return 0;
}


