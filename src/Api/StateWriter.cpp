#include <string>
#include <iostream>
#include <fstream>
#include <yaml-cpp/yaml.h>
#include "StateWriter.h"
#include "Game.h"
#include "State.h"
#include "SavedGame.h"
#include "SavedBattleGame.h"
#include "BattleUnit.h"
#include "Soldier.h"
#include "Armor.h"
#include "Language.h"
#include "Base.h"

using namespace std;

namespace OpenXcom
{
	Game* StateWriter::_game = 0;

	void StateWriter::setGame(Game *game)
	{
		_game = game;
	}

	void StateWriter::save()
	{
		if (_game) {
			SavedGame *savedGame = _game->getSavedGame();

			if (savedGame) {
				ofstream statefile;
				statefile.open("C:/Users/Micah/Desktop/xcom-stream-shite/state.yaml");

				YAML::Emitter out;

				out << YAML::BeginMap;

				// state

				out << YAML::Key << "state";

				SavedBattleGame *savedBattle = savedGame->getSavedBattle();

				if (savedBattle) {
					out << YAML::Value << "battle";

					BattleUnit *selectedUnit = savedBattle->getSelectedUnit();
					Soldier *selectedSoldier = selectedUnit->getGeoscapeSoldier();
					wstring selectedUnitName = selectedSoldier->getName();

					out << YAML::Key << "selectedUnit";
					out << YAML::Value << Language::wstrToUtf8(selectedUnitName);

					// units

					out << YAML::Key << "units";

					out << YAML::BeginSeq;

					vector<BattleUnit*> *units = savedBattle->getUnits();

					for (unsigned i = 0; i < units->size(); i++) {
						BattleUnit *unit = units->at(i);

						Soldier *soldier = unit->getGeoscapeSoldier();
						Armor *armor = unit->getArmor();

						if (soldier && armor) {
							int id = soldier->getId();
							wstring name = soldier->getName();
							string armorType = armor->getType();
							int timeUnits = unit->getTimeUnits();
							int energy = unit->getEnergy();
							int health = unit->getHealth();
							int baseHealth = soldier->getCurrentStats()->health;
							int morale = unit->getMorale();
							int fatalWounds = unit->getFatalWounds();
							int fire = unit->getFire();
							string rank = unit->getRankString();
							int killedBy = unit->killedBy();
							int rankInt = unit->getRankInt();

							out << YAML::BeginMap;

							out << YAML::Key << "name";
							out << YAML::Value << Language::wstrToUtf8(name);

							out << YAML::Key << "id";
							out << YAML::Value << id;

							out << YAML::Key << "armorType";
							out << YAML::Value << armorType;

							out << YAML::Key << "timeUnits";
							out << YAML::Value << timeUnits;

							out << YAML::Key << "energy";
							out << YAML::Value << energy;

							out << YAML::Key << "health";
							out << YAML::Value << health;

							out << YAML::Key << "baseHealth";
							out << YAML::Value << baseHealth;

							out << YAML::Key << "morale";
							out << YAML::Value << morale;

							out << YAML::Key << "fatalWounds";
							out << YAML::Value << fatalWounds;

							out << YAML::Key << "fire";
							out << YAML::Value << fire;

							out << YAML::Key << "rank";
							out << YAML::Value << rank;

							out << YAML::Key << "killedBy";
							out << YAML::Value << killedBy;

							out << YAML::Key << "rankInt";
							out << YAML::Value << rankInt;

							out << YAML::EndMap;
						}
					}

					out << YAML::EndSeq;

				} else {
					out << YAML::Value << "geo";
				}

				out << YAML::Key << "difficulty";
				out << YAML::Value << savedGame->getDifficulty();

				out << YAML::Key << "funds";
				out << YAML::Value << savedGame->getFunds();

				out << YAML::Key << "warned";
				out << YAML::Value << savedGame->getWarned();

				out << YAML::Key << "month";
				out << YAML::Value << savedGame->getMonthsPassed();

				vector<Soldier*> *soldiers = getSoldiers(savedGame);

				out << YAML::Key << "soldiers";

				out << YAML::BeginSeq;

				for (unsigned i = 0; i < soldiers->size(); i++) {
					Soldier *soldier = soldiers->at(i);

					int id = soldier->getId();
					wstring name = soldier->getName();
					int kills = soldier->getKills();
					int missions = soldier->getMissions();
					string rank = soldier->getRankString();
					bool dead = false;

					if (soldier->getDeath()) {
						dead = true;
					}

					out << YAML::BeginMap;

					out << YAML::Key << "name";
					out << YAML::Value << Language::wstrToUtf8(name);

					out << YAML::Key << "rank";
					out << YAML::Value << rank;

					out << YAML::Key << "kills";
					out << YAML::Value << kills;

					out << YAML::Key << "missions";
					out << YAML::Value << missions;

					out << YAML::Key << "dead";
					out << YAML::Value << dead;

					out << YAML::EndMap;
				}

				out << YAML::EndSeq;

				out << YAML::EndMap;

				statefile << out.c_str();

				statefile.close();
			}
		}
	}

	vector<Soldier*> *StateWriter::getSoldiers(SavedGame *savedGame) {
		vector<Soldier*> *soldiers = new vector<Soldier*>();
		vector<Soldier*> *deadSoldiers = savedGame->getDeadSoldiers();

		for (unsigned i = 0; i < deadSoldiers->size(); i++) {
			Soldier *deadSoldier = deadSoldiers->at(i);
			soldiers->push_back(deadSoldier);
		}

		vector<Base*> *bases = savedGame->getBases();

		for (unsigned i = 0; i < bases->size(); i++) {
			Base *base = bases->at(i);

			vector<Soldier*> *baseSoldiers = base->getSoldiers();

			for (unsigned j = 0; j < baseSoldiers->size(); j++) {
				Soldier *baseSoldier = baseSoldiers->at(j);
				soldiers->push_back(baseSoldier);
			}
		}

		return soldiers;
	}
}
