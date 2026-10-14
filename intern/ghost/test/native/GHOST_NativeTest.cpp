/* Native adapter regression checks at the existing GHOST public interface. */
#include "GHOST_ISystem.h"
#include "GHOST_IWindow.h"
#include "GHOST_Rect.h"
#include "STR_String.h"
#include <OpenGL/gl.h>
#include <cstdio>
#include <cstdlib>

#define CHECK(condition) do { if (!(condition)) { \
	std::fprintf(stderr, "FAIL line %d: %s\n", __LINE__, #condition); \
	std::exit(1); } } while (0)

int main()
{
	CHECK(GHOST_ISystem::createSystem() == GHOST_kSuccess);
	GHOST_ISystem *system = GHOST_ISystem::getSystem();
	GHOST_IWindow *window = system->createWindow(STR_String("Blender native adapter check"),
		100, 100, 640, 480, GHOST_kWindowStateNormal, GHOST_kDrawingContextTypeOpenGL, false);
	CHECK(window && window->getValid());
	CHECK(window->getDrawingContextType() == GHOST_kDrawingContextTypeOpenGL);
	CHECK(window->activateDrawingContext() == GHOST_kSuccess);
	CHECK(glGetString(GL_VERSION) != NULL);
	GHOST_Rect bounds;
	window->getClientBounds(bounds);
	CHECK(bounds.getWidth() == 640 && bounds.getHeight() == 480);
	GHOST_TInt32 screenX, screenY, clientX, clientY;
	window->clientToScreen(37, 51, screenX, screenY);
	CHECK(screenX == bounds.m_l + 37 && screenY == bounds.m_t + 51);
	window->screenToClient(screenX, screenY, clientX, clientY);
	CHECK(clientX == 37 && clientY == 51);
	glClearColor(17.0f / 255, 34.0f / 255, 51.0f / 255, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	unsigned char pixel[4];
	glReadPixels(10, 10, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixel);
	CHECK(pixel[0] == 17 && pixel[1] == 34 && pixel[2] == 51 && pixel[3] == 255);
	CHECK(glGetError() == GL_NO_ERROR);
	CHECK(window->swapBuffers() == GHOST_kSuccess);
	CHECK(window->setClientSize(800, 600) == GHOST_kSuccess);
	window->getClientBounds(bounds);
	CHECK(bounds.getWidth() == 800 && bounds.getHeight() == 600);
	CHECK(window->setDrawingContextType(GHOST_kDrawingContextTypeNone) == GHOST_kSuccess);
	CHECK(window->setDrawingContextType(GHOST_kDrawingContextTypeOpenGL) == GHOST_kSuccess);
	CHECK(window->activateDrawingContext() == GHOST_kSuccess);
	CHECK(system->disposeWindow(window) == GHOST_kSuccess);
	GHOST_DisplaySetting setting;
	system->getMainDisplayDimensions(setting.xPixels, setting.yPixels);
	setting.bpp = 32;
	setting.frequency = 0;
	GHOST_IWindow *fullscreen = NULL;
	CHECK(system->beginFullScreen(setting, &fullscreen, false) == GHOST_kSuccess);
	CHECK(fullscreen && fullscreen->getValid() && system->getFullScreen());
	CHECK(system->endFullScreen() == GHOST_kSuccess);
	CHECK(!system->getFullScreen() && !system->validWindow(fullscreen));
	CHECK(GHOST_ISystem::disposeSystem() == GHOST_kSuccess);
	std::puts("PASS: native GHOST context, pixels, coordinates, resize, recreation, fullscreen, disposal");
	return 0;
}
