#pragma once

#include "ITAG.h"
#include "chunk_hashmap.h"
#include "chunk.h"
#include <sys/stat.h>
#include <sstream>
extern "C"
{
	#include "zlib.h"
}
#define MAX_INVENTORY 4+4+36

struct Level
{
	__int64 time;
	__int64 mapSeed;
	struct
	{
		double position[3];
		struct
		{
			short itemID;
			short damage;
			char count;
			char slot;
		} Inventory[MAX_INVENTORY];
		float rotation[2];
		char onGround;
	} Player;
	int spawnX;
	int spawnY;
	int spawnZ;
	ChunkHashMap chunks;

	void Save()
	{
		cTAG_Compound *compound = new cTAG_Compound("Data");
		compound->add("SnowCovered",new cTAG_Byte(0));
		compound->add("Time",new cTAG_Long(time));
		compound->add("LastPlayed",new cTAG_Long(time));
		
		///cTAG_Compound *player = new cTAG_Compound("Player");
		///player->add("Inventory"
		compound->add("SpawnX",new cTAG_Int(spawnX));
		compound->add("SpawnY",new cTAG_Int(spawnY));
		compound->add("SpawnZ",new cTAG_Int(spawnZ));
		compound->add("SizeOnDisk",new cTAG_Long(chunks.numChunks * 2048)); // ??
		compound->add("RandomSeed", new cTAG_Long(mapSeed));

		_mkdir(worldfolder);
		
		std::stringstream ss(std::stringstream::in | std::stringstream::out);
		unsigned int c = 0x0A00000A;
		ss.write((char*)&c,4);
		compound->WriteStream(ss);
		char cend = 0;
		ss.write((char*)&cend,1);

		std::string a = ss.str();
		char filename[128];
		sprintf_s(filename,127,"%s/level.dat",worldfolder);
		gzFile fp = gzopen(filename,"w");
		if(fp == NULL)
		{
			printf("Failed to save level.dat\n");
			delete compound;
		}
		gzwrite(fp, a.c_str(),a.length());
		gzclose(fp);


		delete compound;

	}
};

Level g_level;