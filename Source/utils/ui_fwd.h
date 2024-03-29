#pragma once

#include <SDL.h>

namespace devilution {

extern Uint16 gnScreenWidth;
extern Uint16 gnScreenHeight;
extern Uint16 gnViewportHeight;

bool SpawnWindow(const char *lpWindowName);
void UiErrorOkDialog(const char *caption, const char *text, bool error = true);

} // namespace devilution
