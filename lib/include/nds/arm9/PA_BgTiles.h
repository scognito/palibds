#ifndef _PA_BGTILES
#define _PA_BGTILES

//#include <PA9.h>
//#include <PA_FS.h>
#include <nds.h>
#include "PA_General.h"


#define BG_TILEDBG 	2
#define BG_ROTBG 	3
#define BG_LARGEMAP 4
#define BG_INFINITEMAP 5


void PA_EasyBgLoadEx(u8 screen, u8 bg_number, u32 *Infos, void *Tiles, u32 TileSize, void *Map, u32 MapSize, void *Palette);


// Extracts the size from PAGfx convertions
extern inline u8 PA_GetPAGfxBgSize(u16 width, u16 height)
{
	u8 bg_size = 0;
	if (height == 512)	bg_size = 2;
	if (width == 512)	bg_size ++;	
	return bg_size;
}

// Extracts the size from PAGfx convertions
extern inline u8 PA_GetPAGfxRotBgSize(u16 width)
{
	u8 bg_size = 1; //256x256 by default
	if (width <= 128)	bg_size = 0;	
	else if (width <= 256)	bg_size = 1;	
	else if (width <= 512)	bg_size = 2;		
	else if (width <= 1024) bg_size = 3;		
	
	return bg_size;
}


typedef u8 (*EasyBgPixels)(u8, u8, s32, s32);  

extern EasyBgPixels PA_EasyBgPixel[6];

/*! \file PA_BgTiles.h
    \brief Everything concerning the Bg Tile modes

    This file contains all the macros and variables regarding Tile modes, loading tiles and Bg, etc...
*/




//////////////////////////////////////////////////////////////////////
// Syst�me de Tiles
//////////////////////////////////////////////////////////////////////


//background memory offset macros
#define CharBaseBlock(screen, n) (((n)*0x4000) + 0x6000000 + (0x200000 *  screen))
#define ScreenBaseBlock(screen, n) (((n)*0x800) + 0x6000000 + (0x200000 *  screen))

// Pointers towards background arrays
typedef struct{
	void *Tiles;
	void *Map;
	void *Palette;
	u32 TileSize, MapSize;
	u32 Type, Width, Height; 
} PA_BgDefaultInfos;
	

typedef struct{
	// Memory management info...


	u32 tilesetsize; // Place utilis�e pour chaque tileset
	u16 mapsize; // Place utilis�e pour chaque map
//extern u8 tilesetchar[2][4];  // Emplacement m�moire de chaque tileset
	u8 mapchar;  // Emplacement m�moire de chaque map


	u32 Map; // Map pointer
	u8 TileSetChar;
	
	u32 NTiles;
	u32 *TilePos;
	void *Tiles;	
	
	PA_BgDefaultInfos Infos; // Pointers towards background arrays
	
	s32 ScrollX, ScrollY; // Scroll values
	
	u8 BgMode; // Background mode
} PA_BgInfos;
extern PA_BgInfos PA_BgInfo[2][4];

extern u8 charblocks[2][70];
//extern u32 PA_bgmap[2][4]; // Pointeur vers les maps, 4 maps par screen
//extern u8 tilesetchar[2][4];

// Quantit� de donn�es � charger en fonction de la taille de la map...

extern u16 *PA_DrawBg[2]; // Fond dessinable

extern u16 bg_sizes[4];
extern u8 bg_place[4];


extern u8 charsetstart[2];
extern s32 PA_parallaxX[2][4];
extern s32 PA_parallaxY[2][4];





typedef struct {
	u8 infscroll; // Passe � 1 si scrolling infini activ�...
	u16* bg_map; // Map
//	s32 x, y;
	s32 lx, ly;
	s32 scrollx, scrolly;
	s32 maxx; // 32 ou 64, en fonction de la largeur...
//	s32 startx, starty;
}scrollpositions;

extern scrollpositions scrollpos[2][4]; // Pour chaque �cran et pour chaque fond :)






// Mode de couleur
#define BG_COLOR16 0x00
#define BG_COLOR256 0x80


#define CHAR_SHIFT 2
#define SCREEN_SHIFT 8
#define WRAPAROUND 0x1

#define SCREEN_TILES 24576  // Taille � convertir pour avoir un �cran complet...


//scrolling registers for background 0


// Registre pour les controles de chaque �cran

#define REG_BGSCREEN0  0x04000000
#define REG_BGSCREEN1  0x04001000
#define REG_BGSCREEN(screen) (0x04000000 + (screen * 0x1000))

#define REG_BGCNT(screen, bg_number) (0x4000008 + (screen * 0x1000) + (bg_number << 1))

// Premier registre pour les scroll. On en d�duit les autres en ajoutant pour le screen et le num�ro du Bg
#define REG_BGSCROLLX 0x4000010
#define REG_BGSCROLLY 0x4000012


// Tailles pour les Bg :
#define BG_256X256 0
#define BG_512X256 1
#define BG_256X512 2
#define BG_512X512 3



//ALL_BUT() pour faire tout sauf...
#define TILE_N 1023
#define TILE_PAL 61440
#define TILE_HFLIP 1024
#define TILE_VFLIP 2048





/** @defgroup BgTiles Normal Tiled Background Modes
 *  Load a background, scroll it, etc...
 *  @{
 */

/*!
    \fn void PA_ResetBgSys(void)
    \brief
      \~english Reset the background system
      \~french Reinitialise le systeme de fonds
*/
void PA_ResetBgSys(void);


/*!
    \fn void PA_ResetBgSysScreen(u8 screen)
    \brief
      \~english Reset the background system on 1 screen
      \~french Reinitialise le systeme de fonds pour 1 �cran
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1)
*/
void PA_ResetBgSysScreen(u8 screen);


/*!
    \fn void PA_InitBg(u8 screen, u8 bg_select, u8 bg_size, u8 wraparound, u8 color_mode)
    \brief
      \~english Initialise a given background. Do this only after having loaded a tileset and a map.
      \~french Initialise un fond. A faire uniquement apr�s avoir charg� un tileset et une map.
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1)
    \~\param bg_select
      \~english Background number to load (from 0 to 3)
      \~french Num�ro du fond que l'on veut charger (de 0 � 3 en mode 0, uniquement 2 et 3 en mode 2)
    \~\param bg_size
      \~english Background size. This is important, because it also determines whether the Bg is rotatable or not. To use a normal background, use the macros BG_256X256, BG_256X512, etc... For a rotatable Bg, use the macros BG_ROT_128X128...
      \~french Taille du fond. Ceci est tr�s important, car �a d�termine aussi si le Bg est rotatif ou non. Pour un fond normal, on utilise les macros BG_256X256, BG_256X512, etc... Pour un fond rotatif, il suffit d'utiliser BG_ROT_128X128...
    \~\param wraparound
      \~english If the background wraps around or not. More important for rotating backgrounds.
      \~french Si le fond boucle ou non. C'est plus important pour les fonds rotatifs...
    \~\param color_mode
      \~english Color mode : 0 for 16 color mode, 1 for 256...
      \~french Nombre de couleurs : 0 pour 16 couleurs, 1 pour 256

*/
void PA_InitBg(u8 screen, u8 bg_select, u8 bg_size, u8 wraparound, u8 color_mode);
//#define PA_InitBg(screen, bg_select, bg_size, wraparound, color_mode) {	PA_bgmap[screen][bg_select] = ScreenBaseBlock(screen, ((bg_select + 1) << 3) - bg_place[bg_size]);_REG16(REG_BGSCREEN(screen)) |= (0x100 << (bg_select)); _REG16(REG_BGCNT(screen, bg_select)) = ((bg_select) | (bg_size << 14) |((((bg_select + 1) << 3) - bg_place[bg_size]) << SCREEN_SHIFT) | (wraparound << 13) | ((bg_select) << 2)) + (color_mode << 7);}


/*!
    \def PA_HideBg(screen, bg_select)
    \brief
      \~english Hide a screen's background.
      \~french Cacher un fond.
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1)
    \~\param bg_select
      \~english Background number to load (from 0 to 3)
      \~french Num�ro du fond que l'on veut charger (de 0 � 3 en mode 0, uniquement 2 et 3 en mode 2)
*/
#define PA_HideBg(screen, bg_select) _REG16(REG_BGSCREEN(screen)) &= ~(0x100 << (bg_select))


/*!
    \def PA_ShowBg(screen, bg_select)
    \brief
      \~english Show a hidden background.
      \~french Afficher un fond auparavant cach�.
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1)
    \~\param bg_select
      \~english Background number to load (from 0 to 3)
      \~french Num�ro du fond que l'on veut charger (de 0 � 3 en mode 0, uniquement 2 et 3 en mode 2)
*/
#define PA_ShowBg(screen, bg_select) _REG16(REG_BGSCREEN(screen)) |= (0x100 << (bg_select))


/*!
    \def PA_ResetBg(screen)
    \brief
      \~english Reinitialize de Bg system of a screen. It only hides all the backgrounds in reality...
      \~french Reinitialiser les fonds d'un �cran. En fait ca ne fait que cacher tous les fonds
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1)
*/
#define PA_ResetBg(screen) _REG16(REG_BGSCREEN(screen)) &= ~(0xF00)



/*!
    \def PA_LoadBgTiles(screen, bg_select, bg_tiles)
    \brief
      \~english Load a tileset into memory
      \~french Charger un tileset en m�moire
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1)
    \~\param bg_select
      \~english Background number to load (from 0 to 3)
      \~french Num�ro du fond que l'on veut charger (de 0 � 3 en mode 0, uniquement 2 et 3 en mode 2)
    \~\param bg_tiles
      \~english Name of the tiles' info (example: ship_Tiles)
      \~french Nom du tableau contenant les tiles (exemple: ship_Tiles)
*/
#define PA_LoadBgTiles(screen, bg_select, bg_tiles) PA_LoadBgTilesEx(screen, bg_select, (void*)bg_tiles, SIZEOF_16BIT(bg_tiles))
//#define PA_LoadBgTiles(screen, bg_select, bg_tiles) {DMA_Copy((void*)(bg_tiles), (void*)CharBaseBlock(screen, bg_select),SIZEOF_16BIT(bg_tiles), DMA_16NOW);}


/*!
    \fn void PA_LoadBgTilesEx(u8 screen, u8 bg_select, void* bg_tiles, u32 size)
    \brief
      \~english Load a tileset into memory with a given size
      \~french Charger un tileset en m�moire avec une taille donn�e
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1)
    \~\param bg_select
      \~english Background number to load (from 0 to 3)
      \~french Num�ro du fond que l'on veut charger (de 0 � 3 en mode 0, uniquement 2 et 3 en mode 2)
    \~\param bg_tiles
      \~english Name of the tiles' info (example: ship_Tiles)
      \~french Nom du tableau contenant les tiles (exemple: ship_Tiles)
    \~\param size
      \~english 16 bit size...
      \~french Taille en 16 bits...

*/
void PA_LoadBgTilesEx(u8 screen, u8 bg_select, void* bg_tiles, u32 size);


/*!
    \fn void PA_ReLoadBgTiles(u8 screen, u8 bg_select, void* bg_tiles)
    \brief
      \~english ReLoad a tileset into memory
      \~french ReCharger un tileset en m�moire
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1)
    \~\param bg_select
      \~english Background number to load (from 0 to 3)
      \~french Num�ro du fond que l'on veut charger (de 0 � 3 en mode 0, uniquement 2 et 3 en mode 2)
    \~\param bg_tiles
      \~english Name of the tiles' info (example: ship_Tiles)
      \~french Nom du tableau contenant les tiles (exemple: ship_Tiles)
*/
void PA_ReLoadBgTiles(u8 screen, u8 bg_select, void* bg_tiles);


/*!
    \fn void PA_DeleteTiles(u8 screen, u8 bg_select)
    \brief
      \~english Delete a tilest in memory. Note that loading a tileset automatically deletes the preceding one, so you won't need to use this function often
      \~french Effacer un tileset en m�moire. A noter que charger un tileset efface automatiquement le tileset pr�c�dent, donc on n'aura pas souvent besoin de cette fonction...
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1)
    \~\param bg_select
      \~english Background number to load (from 0 to 3)
      \~french Num�ro du fond que l'on veut charger (de 0 � 3 en mode 0, uniquement 2 et 3 en mode 2)
*/
void PA_DeleteTiles(u8 screen, u8 bg_select);


/*!
    \fn void PA_DeleteMap(u8 screen, u8 bg_select)
    \brief
      \~english Delete a map in memory. Note that loading a map automatically deletes the preceding one, so you won't need to use this function often
      \~french Effacer une map en m�moire. A noter que charger une map efface automatiquement la map pr�c�dent, donc on n'aura pas souvent besoin de cette fonction...
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1)
    \~\param bg_select
      \~english Background number to load (from 0 to 3)
      \~french Num�ro du fond que l'on veut charger (de 0 � 3 en mode 0, uniquement 2 et 3 en mode 2)
*/
void PA_DeleteMap(u8 screen, u8 bg_select);


/*!
    \fn extern inline void PA_DeleteBg(u8 screen, u8 bg_select)
    \brief
      \~english Delete and reset a complete background
      \~french Effacer et reinitialiser un fond compl�tement
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1)
    \~\param bg_select
      \~english Background number to load (from 0 to 3)
      \~french Num�ro du fond que l'on veut charger (de 0 � 3 en mode 0, uniquement 2 et 3 en mode 2)
*/
extern inline void PA_DeleteBg(u8 screen, u8 bg_select) {
  // By Maple, June 2008
  // This is a small piece of PA_ResetBgSysScreen()
  
	PA_HideBg(screen, bg_select);
	PA_DeleteTiles(screen, bg_select);
	PA_DeleteMap(screen, bg_select);

  PA_BgInfo[screen][bg_select].tilesetsize = 0;
  PA_BgInfo[screen][bg_select].mapsize = 0;
  PA_BgInfo[screen][bg_select].TileSetChar = 0;
  PA_BgInfo[screen][bg_select].mapchar = 0;
  PA_parallaxX[screen][bg_select] = 0;
  PA_parallaxY[screen][bg_select] = 0;
  scrollpos[screen][bg_select].infscroll = 0;
}


/*!
    \fn void PA_LoadBgMap(u8 screen, u8 bg_select, void* bg_map, u8 bg_size)
    \brief
      \~english Load a background's map info
      \~french Charge la carte d'un fond
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1)
    \~\param bg_select
      \~english Background number to load (from 0 to 3)
      \~french Num�ro du fond que l'on veut charger (de 0 � 3 en mode 0, uniquement 2 et 3 en mode 2)
    \~\param bg_map
      \~english Name of the map's info (example : (void*)ship_Map) Don't forget the void...
      \~french Nom du tableau contenant les infos sur la map (exemple : (void*)ship_Map) n'oublie pas le void...
    \~\param bg_size
      \~english Background size. This is important, because it also determines whether the Bg is rotatable or not. To use a normal background, use the macros BG_256X256, BG_256X512, etc... 
      \~french Taille du fond. Ceci est tr�s important, car �a d�termine aussi si le Bg est rotatif ou non. Pour un fond normal, on utilise les macros BG_256X256, BG_256X512, etc...
*/

void PA_LoadBgMap(u8 screen, u8 bg_select, void* bg_map, u8 bg_size);





/*!
    \def PA_LoadTiledBg(screen, bg_number, bg_name)
    \brief
      \~english This will never get easier... Loads a background TiledBg converted with PAGfx, with it's tiles, map, and palette. Only 256 color mode available. 
      \~french On ne pourra jamais rendre ca plus simple... Charge un fond de type TiledBg converti avec PAGfx, en mettant les tiles, la map, et meme la palette ! Seulement en mode 256 couleurs
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1)
    \~\param bg_number
      \~english Background number to load (from 0 to 3)
      \~french Num�ro du fond que l'on veut charger (de 0 � 3 en mode 0, uniquement 2 et 3 en mode 2)
    \~\param bg_name
      \~english Background name, like bg0
      \~french Nom du fond, comme bg0
*/

#define PA_LoadTiledBg(screen, bg_number, bg_name){\
	PA_LoadBgPal(screen, bg_number, (void*)bg_name##_Pal); \
	PA_LoadSimpleBg(screen, bg_number, bg_name##_Tiles, bg_name##_Map, PA_GetPAGfxBgSize(bg_name##_Info[1], bg_name##_Info[2]), 0, 1);}




/*!
    \def PA_LoadSimpleBg(screen, bg_select, bg_tiles, bg_map, bg_size, wraparound, color_mode)
    \brief
      \~english Simple way to load a Background. Combines PA_InitBg, PA_LoadBgTiles, and PA_LoadBgMap
      \~french Facon simple de cahrger un fond. Combine PA_InitBg, PA_LoadBgTiles, et PA_LoadBgMap
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1)
    \~\param bg_select
      \~english Background number to load (from 0 to 3)
      \~french Num�ro du fond que l'on veut charger (de 0 � 3 en mode 0, uniquement 2 et 3 en mode 2)
    \~\param bg_tiles
      \~english Name of the tiles' info (example: ship_Tiles)
      \~french Nom du tableau contenant les tiles (exemple: ship_Tiles)
    \~\param bg_map
      \~english Name of the map's info (example : ship_Map)
      \~french Nom du tableau contenant les infos sur la map (exemple : ship_Map)
    \~\param bg_size
      \~english Background size. To use a normal background, use the macros BG_256X256, BG_256X512, etc...
      \~french Taille du fond. Pour un fond normal, on utilise les macros BG_256X256, BG_256X512, etc... 
    \~\param wraparound
      \~english If the background wraps around or not. More important for rotating backgrounds. 
      \~french Si le fond boucle ou non. C'est plus important pour les fonds rotatifs... 
    \~\param color_mode
      \~english Color mode : 0 for 16 color mode, 1 for 256...
      \~french Nombre de couleurs : 0 pour 16 couleurs, 1 pour 256
*/

#define PA_LoadSimpleBg(screen, bg_select, bg_tiles, bg_map, bg_size, wraparound, color_mode) {\
PA_DeleteBg(screen, bg_select);\
PA_LoadBgTiles(screen, bg_select, bg_tiles); \
PA_LoadBgMap(screen, bg_select, (void*)bg_map, bg_size); \
PA_InitBg(screen, bg_select, bg_size, 0, color_mode);\
PA_BGScrollXY(screen, bg_select, 0, 0);}




/*!
    \def PA_LoadBg(screen, bg_select, bg_tiles, tile_size, bg_map, bg_size, wraparound, color_mode)
    \brief
      \~english Simplest way to load a Background. Combines PA_InitBg, PA_LoadBgTiles, and PA_LoadBgMap
      \~french Facon la plus simple de cahrger un fond. Combine PA_InitBg, PA_LoadBgTiles, et PA_LoadBgMap
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1)
    \~\param bg_select
      \~english Background number to load (from 0 to 3)
      \~french Num�ro du fond que l'on veut charger (de 0 � 3 en mode 0, uniquement 2 et 3 en mode 2)
    \~\param bg_tiles
      \~english Name of the tiles' info (example: ship_Tiles)
      \~french Nom du tableau contenant les tiles (exemple: ship_Tiles)
    \~\param tile_size
      \~english Size of your tileset
      \~french Taille du tilset
    \~\param bg_map
      \~english Name of the map's info (example : ship_Map)
      \~french Nom du tableau contenant les infos sur la map (exemple : ship_Map)
    \~\param bg_size
      \~english Background size. This is important, because it also determines whether the Bg is rotatable or not. To use a normal background, use the macros BG_256X256, BG_256X512, etc... For a rotatable Bg, use the macros BG_ROT_128X128...
      \~french Taille du fond. Ceci est tr�s important, car �a d�termine aussi si le Bg est rotatif ou non. Pour un fond normal, on utilise les macros BG_256X256, BG_256X512, etc... Por un fond rotatif, il suffit d'utiliser BG_ROT_128X128...
    \~\param wraparound
      \~english If the background wraps around or not. More important for rotating backgrounds. 
      \~french Si le fond boucle ou non. C'est plus important pour les fonds rotatifs... 
    \~\param color_mode
      \~english Color mode : 0 for 16 color mode, 1 for 256...
      \~french Nombre de couleurs : 0 pour 16 couleurs, 1 pour 256
*/

#define PA_LoadBg(screen, bg_select, bg_tiles, tile_size, bg_map, bg_size, wraparound, color_mode) {\
PA_LoadBgTilesEx(screen, bg_select, (void*)bg_tiles, tile_size); \
PA_LoadBgMap(screen, bg_select, (void*)bg_map, bg_size); \
PA_InitBg(screen, bg_select, bg_size, 0, color_mode);\
PA_BGScrollXY(screen, bg_select, 0, 0);}



/*!
    \fn extern inline void PA_BGScrollX(u8 screen, u8 bg_number, s32 x)
    \brief
      \~english Scroll horizontaly a Tiled background
      \~french Scroll horizontal d'un fond de type Tiled
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1)
    \~\param bg_number
      \~english Background number (0-3)
      \~french Num�ro du fond que l'on veut tourner (0-3)
    \~\param x
      \~english X value to scroll
      \~french Valeur X � d�placer, horizontalement...
*/
extern inline void PA_BGScrollX(u8 screen, u8 bg_number, s32 x) {
	PA_BgInfo[screen][bg_number].ScrollX = (x)&1023;
	_REG16(REG_BGSCROLLX + ((screen) * 0x1000) + ((bg_number) << 2)) = (x)&1023;
}	

/*!
    \fn extern inline void PA_BGScrollY(u8 screen, u8 bg_number, s32 y) 
    \brief
      \~english Scroll vertically a Tiled background
      \~french Scroll vertical d'un fond de type Tiled
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1)
    \~\param bg_number
      \~english Background number (0-3)
      \~french Num�ro du fond que l'on veut tourner (0-3)
    \~\param y
      \~english Y value to scroll
      \~french Valeur Y � d�placer, verticalement...
*/
extern inline void PA_BGScrollY(u8 screen, u8 bg_number, s32 y) {
	PA_BgInfo[screen][bg_number].ScrollY = (y)&1023;
	_REG16(REG_BGSCROLLY + ((screen) * 0x1000) + ((bg_number) << 2)) = (y)&1023;
}	



/*!
    \def PA_BGScrollXY(screen, bg_number, x, y)
    \brief
      \~english Scroll horizontaly and vertically a Tiled background
      \~french Scroll horizontal et vertical d'un fond de type Tiled
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1)
    \~\param bg_number
      \~english Background number (0-3)
      \~french Num�ro du fond que l'on veut tourner (0-3)
    \~\param x
      \~english X value to scroll
      \~french Valeur X � d�placer, horizontalement...
    \~\param y
      \~english Y value to scroll
      \~french Valeur Y � d�placer, verticalement...
*/
extern inline void PA_BGScrollXY(u8 screen, u8 bg_number, s32 x, s32 y) {
	PA_BGScrollX(screen, bg_number, x); 
	PA_BGScrollY(screen, bg_number, y);
}




/*!
    \fn extern inline void PA_SetMapTile(u8 screen, u8 bg_select, s16 x, s16 y, s16 tile_number)
    \brief
      \~english Change the tile gfx used by a given tile in the map
      \~french Change la tile gfx utilis�e pour une tile donn�e dans la map
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1)
    \~\param bg_select
      \~english Background number (0-3)
      \~french Num�ro du fond que l'on veut tourner (0-3)
    \~\param x
      \~english X value of the tile to change
      \~french Valeur X de la tile � changer
    \~\param y
      \~english Y value of the map tile to change
      \~french Valeur Y de la tile � changer dans la carte
    \~\param tile_number
      \~english New tile number to put
      \~french Nouveau num�ro de tile que l'on veut mettre
*/
extern inline void PA_SetMapTile(u8 screen, u8 bg_select, s16 x, s16 y, s16 tile_number) {
*(u16*)(PA_BgInfo[screen][bg_select].Map + ((x) << 1) + ((y) << 6)) &= ~(TILE_N); 
*(u16*)(PA_BgInfo[screen][bg_select].Map + ((x) << 1) + ((y) << 6)) |= ((tile_number)&TILE_N);
}


/*!
    \def PA_SetMapTileAll(screen, bg_select, x, y, tile_info)
    \brief
      \~english Change the tile info used by a given tile in the map
      \~french Change les infos tiles utilis�e pour une tile donn�e dans la map
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1)
    \~\param bg_select
      \~english Background number (0-3)
      \~french Num�ro du fond que l'on veut tourner (0-3)
    \~\param x
      \~english X value of the tile to change
      \~french Valeur X de la tile � changer
    \~\param y
      \~english Y value of the map tile to change
      \~french Valeur Y de la tile � changer dans la carte
    \~\param tile_info
      \~english New tile to put (tile + palette + flips...)
      \~french Nouveau num�ro de tile que l'on veut mettre (tile + palette + flips...)
*/
#define PA_SetMapTileAll(screen, bg_select, x, y, tile_info) *(u16*)(PA_BgInfo[screen][bg_select].Map + ((x) << 1) + ((y) << 6)) = (tile_info)





/*!
    \fn extern inline void PA_SetLargeMapTile(u8 screen, u8 bg_select, s32 x, s32 y, u32 tile_info)
    \brief
      \~english Change the tile info used by a given tile in the map, only for big background (512 large or wide)
      \~french Change les infos tiles utilis�e pour une tile donn�e dans la map, seulement pour les grands fonds (512 de large ou haut)
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1)
    \~\param bg_select
      \~english Background number (0-3)
      \~french Num�ro du fond que l'on veut tourner (0-3)
    \~\param x
      \~english X value of the tile to change
      \~french Valeur X de la tile � changer
    \~\param y
      \~english Y value of the map tile to change
      \~french Valeur Y de la tile � changer dans la carte
    \~\param tile_info
      \~english New tile to put (tile + palette + flips...)
      \~french Nouveau num�ro de tile que l'on veut mettre (tile + palette + flips...)
*/
extern inline void PA_SetLargeMapTile(u8 screen, u8 bg_select, s32 x, s32 y, u32 tile_info) {
	u32 truex;
	u32 mapblock = 0;
	truex = x&31;
	mapblock = ((x >> 5) << 11) + ((y >> 5) << 12); // Permet d'avoir le bon block...
	
	*(u16*)(PA_BgInfo[screen][bg_select].Map + ((truex) << 1) + ((y&31) << 6) + mapblock) = tile_info;
}



/*!
    \fn void PA_SetMapTileHflip(u8 screen, u8 bg_select, u8 x, u8 y, u8 hflip) 
    \brief
      \~english Flip a given tile horizontaly
      \~french Flipper une tile de la carte, horizontalement
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1)
    \~\param bg_select
      \~english Background number (0-3)
      \~french Num�ro du fond que l'on veut tourner (0-3)
    \~\param x
      \~english X value of the tile to change
      \~french Valeur X de la tile � changer
    \~\param y
      \~english Y value of the map tile to change
      \~french Valeur Y de la tile � changer dans la carte
    \~\param hflip
      \~english Set the map tile to horizontal flip
      \~french Mettre la tile de la carte en flip horizontal
*/
extern inline void PA_SetMapTileHflip(u8 screen, u8 bg_select, u8 x, u8 y, u8 hflip) {
	u16 *pointer = (u16*)(PA_BgInfo[screen][bg_select].Map + ((x) << 1) + ((y) << 6));	
	*pointer &= ~TILE_HFLIP; 
	*pointer |= ((hflip) << 10);
}


/*!
    \def extern inline void PA_SetMapTileVflip(u8 screen, u8 bg_select, u8 x, u8 y, u8 vflip)
    \brief
      \~english Flip a given tile verticaly
      \~french Flipper une tile de la carte, verticalement
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1)
    \~\param bg_select
      \~english Background number (0-3)
      \~french Num�ro du fond que l'on veut tourner (0-3)
    \~\param x
      \~english X value of the tile to change
      \~french Valeur X de la tile � changer
    \~\param y
      \~english Y value of the map tile to change
      \~french Valeur Y de la tile � changer dans la carte
    \~\param vflip
      \~english Set the map tile to vertical flip
      \~french Mettre la tile de la carte en flip vertical
*/
extern inline void PA_SetMapTileVflip(u8 screen, u8 bg_select, u8 x, u8 y, u8 vflip) {
	u16 *pointer = (u16*)(PA_BgInfo[screen][bg_select].Map + ((x) << 1) + ((y) << 6));	
	*pointer &= ~TILE_VFLIP; 
	*pointer |= ((vflip) << 10);
}

/*!
    \fn extern inline void PA_SetMapTilePal(u8 screen, u8 bg_select, u8 x, u8 y, u8 palette_number)
    \brief
      \~english Change the 16 color palette used by a tile. Works only if the Bg is in 16 colors...
      \~french Changer la palette de 16 couleurs utilis�e par une tile de la carte. Marche uniquement en mode 16 couleurs pour le Bg.
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1)
    \~\param bg_select
      \~english Background number (0-3)
      \~french Num�ro du fond que l'on veut tourner (0-3)
    \~\param x
      \~english X value of the tile to change
      \~french Valeur X de la tile � changer
    \~\param y
      \~english Y value of the map tile to change
      \~french Valeur Y de la tile � changer dans la carte
    \~\param palette_number
      \~english Palette number (0-15)
      \~french Num�ro de la palette (0-15)
*/
extern inline void PA_SetMapTilePal(u8 screen, u8 bg_select, u8 x, u8 y, u8 palette_number) {
	u16 *pointer = (u16*)(PA_BgInfo[screen][bg_select].Map + ((x) << 1) + ((y) << 6));	
	*pointer &= ~TILE_PAL; 
	*pointer |= ((palette_number) << 12);
}

/*!
    \def PA_SetMapTileEx(screen, bg_select, x, y, tile_number, hflip, vflip, palette_number)
    \brief
      \~english Change every aspect of a given map tile
      \~french Changer tous les aspect d'une tile donn�e dans la map.
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1)
    \~\param bg_select
      \~english Background number (0-3)
      \~french Num�ro du fond que l'on veut tourner (0-3)
    \~\param x
      \~english X value of the tile to change
      \~french Valeur X de la tile � changer
    \~\param y
      \~english Y value of the map tile to change
      \~french Valeur Y de la tile � changer dans la carte
    \~\param tile_number
      \~english New tile number to put
      \~french Nouveau num�ro de tile que l'on veut mettre
    \~\param hflip
      \~english Set the map tile to horizontal flip
      \~french Mettre la tile de la carte en flip horizontal
    \~\param vflip
      \~english Set the map tile to vertical flip
      \~french Mettre la tile de la carte en flip vertical
    \~\param palette_number
      \~english Palette number (0-15)
      \~french Num�ro de la palette (0-15)
*/
extern inline void PA_SetMapTileEx(u8 screen, u8 bg_select, s16 x, s16 y, u16 tile_number, u8 hflip, u8 vflip, u8 palette_number) {
	*(u16*)(PA_BgInfo[screen][bg_select].Map + ((x) << 1) + ((y) << 6)) = (tile_number) + ((hflip) << 10) + ((vflip) << 11) + ((palette_number) << 12);
}










/*!
    \fn extern inline void PA_SetBgPrio(u8 screen, u8 bg, u8 prio)
    \brief
      \~english Change a backgrounds priority
      \~french Changer la priorit� d'un fond
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1)
    \~\param bg
      \~english Background...
      \~french Num�ro du fond...
    \~\param prio
      \~english Priority level (0-3, 0 being the highest)
      \~french Niveau de priorit�, de 0 � 3, 0 �tant priorit� la plus �lev�e
*/
extern inline void PA_SetBgPrio(u8 screen, u8 bg, u8 prio) {
	_REG16(REG_BGCNT(screen, bg)) &= ~3;
	_REG16(REG_BGCNT(screen, bg)) |= prio;
}

extern inline void PA_CreateBgFromTiles(u8 screen, u8 bg_select, u8 bg_tiles, void *bg_map, u8 bg_size){
	PA_LoadBgMap(screen, bg_select, bg_map, bg_size);
	scrollpos[screen][bg_select].infscroll = 0; // Par d�faut pas de scrolling infini...
	PA_BgInfo[screen][bg_select].Map = ScreenBaseBlock(screen, PA_BgInfo[screen][bg_select].mapchar);
	PA_BgInfo[screen][bg_select].TileSetChar = PA_BgInfo[screen][bg_tiles].TileSetChar;
	PA_BgInfo[screen][bg_select].tilesetsize = PA_BgInfo[screen][bg_tiles].tilesetsize;
	
	_REG16(REG_BGSCREEN(screen)) |= (0x100 << (bg_select));
	_REG16(REG_BGCNT(screen, bg_select)) = bg_select | (bg_size << 14) |(PA_BgInfo[screen][bg_select].mapchar << SCREEN_SHIFT) | (1 << 13) | (PA_BgInfo[screen][bg_select].TileSetChar << 2) | (1 << 7);
	PA_BGScrollXY(screen, bg_select, 0, 0);	
}



/*!
    \fn extern inline void PA_SetBgPrioSeq(u8 screen, u8 priority0, u8 priority1, u8 priority2, u8 priority3)
    \brief
      \~english Change all the background priorities to a given background order
      \~french Changer la priorit� des fonds pour qu'ils soient dans un ordre donn�
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1)
    \~\param priority0      
	  \~english Background to show on top
      \~french Fond � mettre en premier
    \~\param priority1
      \~english Next one...
      \~french Suivant...
    \~\param priority2
      \~english Next one...
      \~french Suivant...	  
    \~\param priority2
      \~english Last one...
      \~french Dernier...	  
	  
*/
extern inline void PA_SetBgPrioSeq(u8 screen, u8  priority0, u8  priority1, u8  priority2, u8  priority3)
{
   PA_SetBgPrio(screen, priority0, 0);
   PA_SetBgPrio(screen, priority1, 1);
   PA_SetBgPrio(screen, priority2, 2);
   PA_SetBgPrio(screen, priority3, 3);
} 




/*! \fn extern inline void PA_ClearBg(u8 screen, u8 bg_select)
    \brief
         \~english Erase a given background (just the tilemap)
         \~french Effacer un fond donn� (juste la map)
    \~\param screen
         \~english Choose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1)
    \~\param bg_select
         \~english Background...
         \~french Fond...	 
*/
extern inline void PA_ClearBg(u8 screen, u8 bg_select){
u8 i, j;
for (i = 0; i < 32; i++) for (j = 0; j < 32; j++) PA_SetMapTileAll(screen, bg_select, i, j, 0);
}





/*! \fn #define PA_EasyBgLoad(u8 screen, u8 bg_number, PAGfx_struct bg_name)
    \brief
         \~english Easiest way to load a background converted with PAGfx...
         \~french Moyen le plus simple de charger un fond cr�� avec PAGfx
    \~\param screen
         \~english Choose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1)
    \~\param bg_number
         \~english Background number... (0-3)
         \~french Num�ro du fond...	 (0-3)
    \~\param bg_name
         \~english Background name
         \~french Nom du fond	 
*/
#define PA_EasyBgLoad(screen, bg_number, bg_name)	{PA_BgInfo[screen][bg_number].BgMode = bg_name##_Info[0];\
	PA_StoreEasyBgInfos(screen, bg_number, bg_name##_Info[0], bg_name##_Info[1], bg_name##_Info[2], (void*)bg_name##_Tiles, SIZEOF_16BIT(bg_name##_Tiles), (void*)bg_name##_Map, SIZEOF_16BIT(bg_name##_Map), (void*)bg_name##_Pal);\
	if(PA_BgInfo[screen][bg_number].BgMode == BG_TILEDBG){	PA_LoadTiledBg(screen, bg_number, bg_name);}\
	else{PA_LoadPAGfxLargeBg(screen, bg_number, bg_name);}}
	
/*! \fn #define PA_EasyBgLoadPtr(screen, bg_number, bg_name)
    \brief
         \~english Easiest way to load a background converted with PAGfx... Can take pointers !
         \~french Moyen le plus simple de charger un fond cr�� avec PAGfx... Peut prendre des pointeurs !
    \~\param screen
         \~english Choose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1)
    \~\param bg_number
         \~english Background number... (0-3)
         \~french Num�ro du fond...	 (0-3)
    \~\param bg_name
         \~english Background, like &bg0
         \~french Fond, par exemple &bg0
*/	
#define PA_EasyBgLoadPtr(screen, bg_number, bg_name) PA_EasyBgLoadEx(screen, bg_number, (u32*)bg_name->Info, bg_name->Tiles, bg_name->TileSize, bg_name->Map, bg_name->MapSize, bg_name->Palette)

/*
extern inline void PA_EasyBgLoadPtr(u8 screen, u8 bg_number, PAGfx_struct *bg_name){
   PA_EasyBgLoadEx(screen, bg_number, (u32*)bg_name->Info, bg_name->Tiles, bg_name->TileSize, bg_name->Map, bg_name->MapSize, bg_name->Palette);
}
*/

//#define PA_EasyBgLoad(screen, bg_number, bg_name)   PA_EasyBgLoadEx(screen, bg_number, (u32*)bg_name.Info, (void*)bg_name.Tiles, (u32)bg_name.TileSize, (void*)bg_name.Map, (u32)bg_name.MapSize, (void*)bg_name.Palette)

/*PA_EasyBgLoadEx(screen, bg_number, (u32*)bg_name##_Info, (void*)bg_name##_Tiles, SIZEOF_16BIT(bg_name##_Tiles), (void*)bg_name##_Map, SIZEOF_16BIT(bg_name##_Map), (void*)bg_name##_Pal)*/



/*!
    \fn void PA_EasyBgScrollX(u8 screen, u8 bg_number, s32 x)
    \brief
      \~english Scroll horizontaly any background
      \~french Scroll horizontal de n'importe quel fond
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1)
    \~\param bg_number
      \~english Background number (0-3)
      \~french Num�ro du fond que l'on veut tourner (0-3)
    \~\param x
      \~english X value to scroll
      \~french Valeur X � d�placer, horizontalement...
*/
void PA_EasyBgScrollX(u8 screen, u8 bg_number, s32 x);



/*!
    \fn void PA_EasyBgScrollY(u8 screen, u8 bg_number, s32 y)
    \brief
      \~english Scroll vertically any background
      \~french Scroll vertical de n'importe quel fond
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1)
    \~\param bg_number
      \~english Background number (0-3)
      \~french Num�ro du fond que l'on veut tourner (0-3)
    \~\param y
      \~english Y value to scroll
      \~french Valeur Y � d�placer, verticalement...
*/
void PA_EasyBgScrollY(u8 screen, u8 bg_number, s32 y);


/*!
    \fn extern inline void PA_EasyBgScrollXY(u8 screen, u8 bg_number, s32 x, s32 y)
    \brief
      \~english Scroll horizontaly and vertically any background
      \~french Scroll horizontal et vertical de n'importe quel fond
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1)
    \~\param bg_number
      \~english Background number (0-3)
      \~french Num�ro du fond que l'on veut tourner (0-3)
    \~\param x
      \~english X value to scroll
      \~french Valeur X � d�placer, horizontalement...
    \~\param y
      \~english Y value to scroll
      \~french Valeur Y � d�placer, verticalement...
*/
extern inline void PA_EasyBgScrollXY(u8 screen, u8 bg_number, s32 x, s32 y){
	PA_EasyBgScrollX(screen, bg_number, x);
	PA_EasyBgScrollY(screen, bg_number, y);
}


/*!
    \fn extern inline u8 PA_EasyBgGetPixel(u8 screen, u8 bg_number, s32 x, s32 y)
    \brief
      \~english Returns the color (number in the palette) of the screen pixel...
      \~french Renvoie le num�ro dans la palette du pixel � l'�cran...
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1)
    \~\param bg_number
      \~english Background number (0-3)
      \~french Num�ro du fond que l'on veut tourner (0-3)
    \~\param x
      \~english X screen pixel position
      \~french Valeur X du pixel � l'�cran
    \~\param y
      \~english Y screen pixel position
      \~french Valeur Y du pixel � l'�cran
*/
extern inline u8 PA_EasyBgGetPixel(u8 screen, u8 bg_number, s32 x, s32 y){
	return PA_EasyBgPixel[PA_BgInfo[screen][bg_number].Infos.Type](screen, bg_number, x, y);
}


/*!
    \fn extern inline u16 PA_EasyBgGetPixelCol(u8 screen, u8 bg_number, s32 x, s32 y)
    \brief
      \~english Returns the color (u16 value) of the screen pixel...
      \~french Renvoie la couleur (valeur u16) du pixel � l'�cran...
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1)
    \~\param bg_number
      \~english Background number (0-3)
      \~french Num�ro du fond que l'on veut tourner (0-3)
    \~\param x
      \~english X screen pixel position
      \~french Valeur X du pixel � l'�cran
    \~\param y
      \~english Y screen pixel position
      \~french Valeur Y du pixel � l'�cran
*/
extern inline u16 PA_EasyBgGetPixelCol(u8 screen, u8 bg_number, s32 x, s32 y){
	return ((u16*)(PA_BgInfo[screen][bg_number].Infos.Palette))[PA_EasyBgGetPixel(screen, bg_number, x, y)];
}


/*!
    \fn extern inline void PA_SetBgWrap(u8 screen, u8 bg, u8 wrap)
    \brief
      \~english Set on/off the background wrapping (for rotating, 8bit, and 16bit backgrounds)
      \~french Active ou non le wrapping des fonds (rotatifs, 8bit, et 16bit)
    \~\param screen
         \~english Chose de screen (0 or 1)
         \~french Choix de l'�cran (0 ou 1)
    \~\param bg
      \~english Background number (0-3)
      \~french Num�ro du fond que l'on veut tourner (0-3)
    \~\param wrap
      \~english Wrap around on or off...
      \~french Wrap activ� ou d�sactiv�...
*/
extern inline void PA_SetBgWrap(u8 screen, u8 bg, u8 wrap){
   _REG16(REG_BGCNT(screen, bg)) &= ~(1<<13); // Remove
	_REG16(REG_BGCNT(screen, bg)) |= (wrap<<13);
}



/** @} */ // end of BgTiles


/*

void PA_StoreEasyBgInfos(u8 screen, u8 bg_number, u32 *Infos, void *Tiles, u32 TileSize, void *Map, u32 MapSize, void *Palette);
	
*/


// Get pixel functions

u8 PAEasyBgGetPixelTiled(u8 screen, u8 bg_number, s32 x, s32 y);

u8 PAEasyBgGetPixelLarge(u8 screen, u8 bg_number, s32 x, s32 y);

u8 PAEasyBgGetPixelInf(u8 screen, u8 bg_number, s32 x, s32 y);

u8 PANoPixel(u8 screen, u8 bg_number, s32 x, s32 y);

extern inline void PA_UpdateBgTile(u8 screen, u8 bg_select, u16 tilepos, void *tile){
tilepos = tilepos << 4;
u32 *tilecopy = (u32*)tile;
u8 i;
	for (i = 0; i < 16; i++)
		PA_BgInfo[screen][bg_select].TilePos[tilepos+i] = tilecopy[i];
}

void PA_StoreEasyBgInfos(u8 screen, u8 bg_number, u32 Type, u32 Width, u32 Height, void *Tiles, u32 TileSize, void *Map, u32 MapSize, void *Palette);


#endif

