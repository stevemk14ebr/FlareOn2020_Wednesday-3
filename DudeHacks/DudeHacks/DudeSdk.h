#pragma once
#include <assert.h>
#include <stdint.h>

// Created with ReClass.NET 1.2 by KN4CK3R
enum class MoveState : int8_t
{
	Duck = 0,
	Jump = 1,
	Walk = 2
};

class Entity
{
public:
	class NimInt* m_Type; //0x0000
	char pad_0004[16]; //0x0004
	class SeqNimString* m_Tags; //0x0014
	char pad_0018[44]; //0x0018
	class SeqEntity* m_collidingEntities; //0x0044
	double m_posX; //0x0048
	double m_posY; //0x0050
	double m_posZ; //0x0058
	char pad_0060[152]; //0x0060
	MoveState m_MoveState; //0x00F8
	bool isDead; //0x00F9
	bool m_Unk; //0x00FA
	bool m_Unk2; //0x00FB
	class NimInt* m_pScore; //0x00FC
	uint32_t Timer; //0x0100
	char pad_0104[292]; //0x0104
	uint32_t PassingObstacle; //0x0228
	char pad_022C[1540]; //0x022C
}; //Size: 0x0830

class Scene
{
public:
	NimInt* m_Type;
	class SeqEntity* fList; //0x0004
	class SeqEntity* fAddList; //0x0008
	char pad_000C[28]; //0x000C
	class Entity* m_PlayerEntity; //0x0028
	class Entity* m_GroundEntity; //0x002C
	class Entity* m_Ground2Entity; //0x0030
	class TextGraphic* m_ScoreEntity; //0x0034
	class TextGraphic* m_HighScoreEntity; //0x0038
	class Entity* m_ObstacleEntity; //0x003C
	class Entity* m_ObstacleEntity2; //0x0040
	char pad_0044[4]; //0x0044
}; //Size: 0x0048

class ObstacleRelated
{
public:
	char pad_0000[72]; //0x0000
	double N0000033F; //0x0048
	double N00000340; //0x0050
	double N00000341; //0x0058
	char pad_0060[32]; //0x0060
}; //Size: 0x0080

class TextGraphic
{
public:
	char pad_0000[20]; //0x0000
	class SeqNimString* m_Lines; //0x0014
}; //Size: 0x0018

class NimString
{
public:
	uint32_t Length; //0x0000
	uint32_t Reserved; //0x0004
	char Data[20]; //0x0008
}; //Size: 0x001C

class SeqNimString
{
public:
	uint32_t Length; //0x0000
	uint32_t Reserved; //0x0004
	class NimString* m_strings[1]; //0x0008
}; //Size: 0x000C

class NimInt
{
public:
	uint32_t Value; //0x0000
}; //Size: 0x0004

class Dim
{
public:
	uint32_t m_Width; //0x0000
	uint32_t m_Height; //0x0004
}; //Size: 0x0008

class Game
{
public:
	class SdlWindow* m_sdlWindow; //0x0000
	class Dim m_fsize; //0x0004 icon size
	char pad_000C[4]; //0x000C
	class Scene* m_fscene; //0x0010 cur scene
}; //Size: 0x0014

class SdlWindow
{
public:
	char pad_0000[8]; //0x0000
	char* m_WindowName; //0x0008
	char pad_000C[4]; //0x000C
	uint32_t m_WindowWidth; //0x0010
	uint32_t m_WindowHeight; //0x0014
	uint32_t m_ContentWidth; //0x0018
	uint32_t m_ContentHeight; //0x001C
}; //Size: 0x0020

class SeqEntity
{
public:
	uint32_t m_Length; //0x0000
	uint32_t m_Reserved; //0x0004
	class Entity* m_entities[9]; //0x0008
}; //Size: 0x002C

class N00000312
{
public:
	char pad_0000[4]; //0x0000
	class N00000326* N00000314; //0x0004
	char pad_0008[60]; //0x0008
}; //Size: 0x0044

class N00000326
{
public:
	char pad_0000[68]; //0x0000
}; //Size: 0x0044

Game** p_GAME = (Game**)0x44DC98;
const uint32_t MAIN_SCENE_TYPE = 80;