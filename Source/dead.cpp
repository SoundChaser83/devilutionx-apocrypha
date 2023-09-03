/**
 * @file dead.cpp
 *
 * Implementation of functions for placing dead monsters.
 */
#include "dead.h"

#include "gendung.h"
#include "lighting.h"
#include "misdat.h"
#include "monster.h"

namespace devilution {

Corpse Corpses[MaxCorpses];
int8_t stonendx;

namespace {
void InitDeadAnimationFromMonster(Corpse &corpse, const CMonster &mon)
{
	int i = 0;
	const auto &animData = mon.GetAnimData(MonsterGraphic::Death);
	for (const auto &celSprite : animData.CelSpritesForDirections)
		corpse.data[i++] = celSprite->Data();
	corpse.frame = animData.Frames;
	corpse.width = animData.CelSpritesForDirections[0]->Width();
}
} // namespace

void InitCorpses()
{
	int8_t mtypes[MAXMONSTERS] = {};

	int8_t nd = 0;

	for (int i = 0; i < LevelMonsterTypeCount; i++) {
		if (mtypes[LevelMonsterTypes[i].mtype] != 0)
			continue;

		InitDeadAnimationFromMonster(Corpses[nd], LevelMonsterTypes[i]);
		Corpses[nd].translationPaletteIndex = 0;
		nd++;

		LevelMonsterTypes[i].mdeadval = nd;
		mtypes[LevelMonsterTypes[i].mtype] = nd;
	}

	nd++; // Unused blood spatter

	for (auto &corpse : Corpses[nd].data)
		corpse = MissileSpriteData[MFILE_SHATTER1].animData[0].get();

	Corpses[nd].frame = 12;
	Corpses[nd].width = 128;
	Corpses[nd].translationPaletteIndex = 0;
	nd++;

	stonendx = nd;

	for (int i = 0; i < ActiveMonsterCount; i++) {
		auto &monster = Monsters[ActiveMonsters[i]];
		if (monster._uniqtype != 0) {
			InitDeadAnimationFromMonster(Corpses[nd], *monster.MType);
			Corpses[nd].translationPaletteIndex = ActiveMonsters[i] + 1;
			nd++;

			monster._udeadval = nd;
		}
	}

	assert(static_cast<unsigned>(nd) <= MaxCorpses);
}

void AddCorpse(Point tilePosition, int8_t dv, Direction ddir)
{
	dCorpse[tilePosition.x][tilePosition.y] = (dv & 0x1F) + (static_cast<int>(ddir) << 5);
}

void SyncUniqDead()
{
	for (int i = 0; i < ActiveMonsterCount; i++) {
		auto &monster = Monsters[ActiveMonsters[i]];
		if (monster._uniqtype == 0)
			continue;
		for (int dx = 0; dx < MAXDUNX; dx++) {
			for (int dy = 0; dy < MAXDUNY; dy++) {
				if ((dCorpse[dx][dy] & 0x1F) == monster._udeadval)
					ChangeLightXY(monster.mlid, { dx, dy });
			}
		}
	}
}

} // namespace devilution
