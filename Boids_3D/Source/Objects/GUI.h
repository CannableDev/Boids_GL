#ifndef GUI_H
#define GUI_H

#define GLT_IMPLEMENTATION
#include <glad/glad.h>
#include <sdl/SDL.h>
#include <string>
#include <gltext/gltext.h>

class GUI {
public:
	GUI(const char* name, const char* display);
	~GUI();

	void Draw();

	void SetName(std::string s);
	void SetDisplayDetails(std::string s);
	void SetFPS(float f);
	void SetLightNumber(int l);

	void ToggleMode();

private:
	bool mode;

	GLTtext* sceneName, 
		* displayDetails, 
		* fps, 
		* lightNum;

};


#endif