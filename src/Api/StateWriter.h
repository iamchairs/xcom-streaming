#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <yaml-cpp/yaml.h>
#include "State.h"
#include "Game.h"
#include "SavedGame.h"
#include "Soldier.h"
#include "Base.h"

using namespace std;

namespace OpenXcom
{
	class Game;

	class StateWriter
	{
		protected:
			static Game *_game;
		public:
			static void setGame(Game *game);
			static void save();
			static string ws2s(wstring &s);
			static vector<Soldier*> *getSoldiers(SavedGame *savedGame);
	};

}
