#include "testApp.h"
#include "ofAppGlutWindow.h"

//--------------------------------------------------------------
int main(){
	ofAppGlutWindow window; // create a window
	// set width, height, mode (OF_WINDOW or OF_FULLSCREEN)
	ofSetupOpenGL(&window, 640, 480, OF_WINDOW);
//	ofSetupOpenGL(&window, 640, 480, OF_WINDOW);
	ofRunApp(new testApp()); // start the app
}
