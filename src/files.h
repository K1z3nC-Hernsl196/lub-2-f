#ifndef FILES_H
#define FILES_H
#include "player.h"
#include "utils.h"
#include "backpack.h"

void search_item(const Player* p, const ItemCatalog* c);
void save_diary(const Player* p, const GameTime* t, const ItemCatalog* c);
void npc_dialogs(const Player* p, const GameTime* t, const ItemCatalog* c);

#endif