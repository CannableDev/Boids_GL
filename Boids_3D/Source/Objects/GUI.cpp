#include "GUI.h"

GUI::GUI(const char* name, const char* display)
{
	gltInit();
	mode = true;
	sceneName = gltCreateText();
	displayDetails = gltCreateText();
	fps = gltCreateText();
	lightNum = gltCreateText();

	gltSetText(sceneName, name);
	gltSetText(displayDetails, display);
}

GUI::~GUI()
{
	gltDeleteText(sceneName);
	gltDeleteText(displayDetails);
	gltDeleteText(fps);
	gltDeleteText(lightNum);

	gltTerminate();
}

void GUI::Draw()
{
	gltBeginDraw();
	gltColor(1, 0.5f, 1, 1);

	int y = 1;
	
	if (mode) {
		gltDrawText2D(fps, 0, y, 1);
		gltDrawText2D(sceneName, 0, y += 20, 1);
		gltDrawText2D(displayDetails, 0, y += 20, 1);
		gltDrawText2D(lightNum, 0, y += 20, 1);
	}
	else {
		gltDrawText2D(fps, 0, y, 1);
	}

	gltEndDraw();
}

void GUI::SetName(std::string s)
{
	gltSetText(sceneName, s.c_str());
}

void GUI::SetDisplayDetails(std::string s)
{
	gltSetText(displayDetails, s.c_str());
}

void GUI::SetFPS(float f)
{
	std::string c = "FPS: " + std::to_string(f);
	gltSetText(fps, c.c_str());
}

void GUI::SetLightNumber(int l)
{
	std::string c = "Number of Lights: " + std::to_string(l);
	gltSetText(lightNum, c.c_str());
}

void GUI::ToggleMode()
{
	mode = !mode;
}
