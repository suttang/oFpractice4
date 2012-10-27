#pragma once

#include "ofxOpenNI.h"
#include "ofMain.h"
#include "ofxBox2d.h"

class testApp : public ofBaseApp{
public:
	void setup();
	void update();
	void draw();
	
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	void fixEdge(ofxBox2dCircle circle);
	void drawUser(ofxTrackedUser* user);
	void drawLimb(struct ofxLimb limb);
	vector<ofVec2f> getHandPoint();
	void updateHandPoints();
	void drawBackground();
	
	int numberOfCircles = 60;
	float windowScale = 1;
	
	ofxHardwareDriver	hardware;
	ofxOpenNIContext niContext;
	ofxUserGenerator userGenerator;
	ofxDepthGenerator depthGenerator;
	ofxImageGenerator imageGenerator;
	ofImage userImage;
	
	ofxBox2dPolygon wall1;
    
	ofxBox2d box2d;
	vector<ofxBox2dCircle> circles;
	
	vector<ofVec2f> handPoints;
	vector< vector<float> > distancesFromHand;
	vector< vector<bool> > isDrawLines;
	vector< vector<ofVec2f> > motionVectors;
};
