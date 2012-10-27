#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
	ofSetFrameRate(40);
	ofEnableSmoothing();
	ofEnableAlphaBlending();
	ofBackground(0x000000);
	
	//
	// OpenNI
	//
	hardware.setup();
	hardware.setLedOption(LED_OFF);
	niContext.setup();
	imageGenerator.setup(&niContext);
	depthGenerator.setup(&niContext);
	userGenerator.setup(&niContext);
	userGenerator.setSmoothing(0.1f);
	userGenerator.setUseMaskPixels(false);
	userGenerator.setUseCloudPoints(false);
	userGenerator.setMaxNumberOfUsers(1);
	niContext.toggleMirror();
	
	//
	// Box2d
	//
	box2d.init();
	box2d.setGravity(0, 0);
	box2d.setFPS(40.0);
	wall1.addVertex(0, 0);
	wall1.addVertex(0, ofGetHeight());
	wall1.addVertex(ofGetWidth(), ofGetHeight());
	wall1.addVertex(ofGetWidth(), 0);
	wall1.addVertex(0, 0);
	wall1.setPhysics(0.0, 1, 0.25);
	wall1.create(box2d.getWorld());

	// Circles
	numberOfCircles = 260;
	float radius;
	
	handPoints.push_back(ofVec2f(0, 0));
	handPoints.push_back(ofVec2f(0, 0));
	
	for (int i = 0; i < handPoints.size(); i++) {
		vector<float> distanceFromHandPoint = vector<float>(numberOfCircles);
		vector<bool> isDrawLine = vector<bool>(numberOfCircles);
		vector<ofVec2f> motionVector = vector<ofVec2f>(numberOfCircles);
		for (int j = 0; j < numberOfCircles; j++) {
			distanceFromHandPoint.push_back(40000);
			isDrawLine.push_back(false);
			motionVector.push_back(ofVec2f(0, 0));
		}
		distancesFromHand.push_back(distanceFromHandPoint);
		isDrawLines.push_back(isDrawLine);
		motionVectors.push_back(motionVector);
	}
	
	for (int i = 0; i < numberOfCircles; i++) {
		radius = ofRandom(4, 20);
		ofxBox2dCircle circle;
		
//		circle.setPhysics(3.0, 0.53, 0.1);
		circle.setPhysics(3.0, 1, 0.1);
		circle.setup(box2d.getWorld(), ofRandom(0, ofGetWidth()), ofRandom(0, ofGetHeight()), radius);
		circles.push_back(circle);
	}
}

//--------------------------------------------------------------
void testApp::update() {
	float range = 40000;
	//
	// OpenNI
	//
	hardware.update();
	niContext.update();
	imageGenerator.update();
	depthGenerator.update();
	userGenerator.update();
	updateHandPoints();
	
	//
	// Box2D
	//
	box2d.update();
	
	for (int i = 0; i < circles.size(); i++) {
		ofxBox2dCircle circle = circles[i];
		ofVec2f position = circle.getPosition();
		ofVec2f velocity = circle.getVelocity();
		float vx = 0;
		float vy = 0;;
		for (int j = 0; j < handPoints.size(); j++) {
			float pointX = handPoints[j].x;
			float pointY = handPoints[j].y;
			float distance = ofDistSquared(position.x, position.y, pointX, pointY);
			float theta = atan2(pointX - position.x, pointY - position.y) - (PI / 2);
			float angle = ofRadToDeg(theta);
			if (distance < range) {
				ofVec2f v = motionVectors[j][i];
				isDrawLines[j][i] = true;
				v.x = (range - distance) / 6000;
				v.y = 0;
				v.rotate(angle);
				v.x = -v.x;
				vx += v.x;
				vy += v.y;
				distancesFromHand[j][i] = distance;
			} else {
				isDrawLines[j][i] = false;
			}
		}
//		circle.setVelocity(vx, vy);
		circle.setVelocity(vx + velocity.x / 1.16, vy + velocity.y / 1.16);
	}
	
	
//	for (int i = 0; i < circles.size(); i++) {
//		ofxBox2dCircle circle = circles[i];
//		ofVec2f position = circle.getPosition();
//		ofVec2f velocity = circle.getVelocity();
//		distance = ofDistSquared(position.x, position.y, mouseX, mouseY);
//		theta = atan2(mouseX - position.x, mouseY - position.y) - (PI/2);
//		angle = ofRadToDeg(theta);
//		if (distance < range) {
//			ofVec2f v = motionVectors[i];
//			isDrawLines[i] = true;
//			v.x = (range - distance) / 6000;
//			v.y = 0;
//			v.rotate(angle);
//			v.x = -v.x;
//			circle.setVelocity(v.x, v.y);
//			distances[i] = distance;
//		} else {
//			isDrawLines[i] = false;
//		}
//	}
}

//--------------------------------------------------------------
void testApp::draw() {
	drawBackground();
//	ofSetHexColor(0xFFFFFF);
//	if (userGenerator.getNumberOfTrackedUsers() > 0) {
//		for (int i = 1; i <= userGenerator.getNumberOfTrackedUsers(); i++) {
//			drawUser(userGenerator.getTrackedUser(i));
//		}
//	}
	
	int l = circles.size();
	float range = 40000;
	for (int i = 0; i < l; i++) {
		ofFill();
		ofSetColor(255 - 235 / (range / distancesFromHand[0][i]), 14, 255 - 214 / (range / distancesFromHand[1][i]));

//		fixEdge(circles[i]);
		circles[i].draw();
	}
	for (int i = 0; i < l; i++) {
		for (int j = 0; j < handPoints.size(); j++) {
			if (isDrawLines[j][i]) {
				ofSetColor(255, 255, 255, 80);
				ofVec2f position = circles[i].getPosition();
				ofSetLineWidth(0.2);
				ofLine(position.x, position.y, handPoints[j].x, handPoints[j].y);
				ofDrawBitmapStringHighlight(ofToString(floor(distancesFromHand[j][i])), position.x + 10, position.y - 10);
			}
		}
	}
	
	ofSetHexColor(0xFF00000);
	wall1.draw();
	
	
//	int l = circles.size();
//	float range = 40000;
//	for(int i = 0; i < l; i++) {
//		ofFill();
//		ofSetColor(144, 212, 243 / (range / distances[i]));
//		
//		fixEdge(circles[i]);
//		circles[i].draw();
//	}
//	for(int i = 0; i < l; i++) {
//		if (isDrawLines[i]) {
//			ofSetColor(255, 255, 255, 120);
//			ofVec2f position = circles[i].getPosition();
//			ofSetLineWidth(0.2);
//			ofLine(position.x, position.y, mouseX, mouseY);
//			ofDrawBitmapStringHighlight(ofToString(floor(distances[i])), position.x + 10, position.y - 10);
//		}
//	}
//	
//	//
//	vector<ofVec2f> handPoints = getHandPoint();
//	ofSetHexColor(0xFF0000);
//	for (int i = 0; i < handPoints.size(); i++) {
//		ofCircle(handPoints[i].x, handPoints[i].y, 5);
//	}
//	delete &handPoints;
	
//	ofSetHexColor(0xFFFFFF);
//	ofDrawBitmapString(ofToString(ofGetFrameRate()), 0, 20);
}

//--------------------------------------------------------------
void testApp::drawBackground() {
	float w = ofGetWidth();
	float h = ofGetHeight();
	int numCols = w / 10;
	int numRows = h / 10;
	ofSetColor(255, 255, 255, 55);
	for (int i = 0; i < numRows; i++) {
		ofLine(0, i * 10, w, i * 10);
	}
	for (int i = 0; i < numCols; i++) {
		ofLine(i * 10, 0, i * 10, h);
	}
}
//--------------------------------------------------------------
void testApp::updateHandPoints() {
	ofxTrackedUser* user = userGenerator.getTrackedUser(1);
	ofxLimb leftHand = user->left_lower_arm;
	ofxLimb rightHand = user->right_lower_arm;
	handPoints[0].x = leftHand.position[1].X * windowScale;
	handPoints[0].y = leftHand.position[1].Y * windowScale;
	handPoints[1].x = rightHand.position[1].X * windowScale;
	handPoints[1].y = rightHand.position[1].Y * windowScale;
}

//--------------------------------------------------------------
vector<ofVec2f> testApp::getHandPoint() {
	ofxTrackedUser* user = userGenerator.getTrackedUser(1);
	ofxLimb leftHand = user->left_lower_arm;
	ofxLimb rightHand = user->right_lower_arm;
	vector<ofVec2f> v = vector<ofVec2f>(2);
	v.push_back(ofVec2f(leftHand.position[1].X, leftHand.position[1].Y));
	v.push_back(ofVec2f(rightHand.position[1].X, rightHand.position[1].Y));
	cout << leftHand.position[0].X << endl;
	return v;
}

//--------------------------------------------------------------
void testApp::drawLimb(struct ofxLimb limb) {
	if (limb.found) {
		ofLine(limb.position[0].X, limb.position[0].Y, limb.position[1].X, limb.position[1].Y);
		ofCircle(limb.position[0].X, limb.position[0].Y, 5);
	}
}

//--------------------------------------------------------------
void testApp::drawUser(ofxTrackedUser* user) {
	ofSetHexColor(0xFFFFFF);
	ofNoFill();
	drawLimb(user->neck);
	drawLimb(user->left_shoulder);
	drawLimb(user->left_upper_arm);
	drawLimb(user->left_lower_arm);
	drawLimb(user->right_shoulder);
	drawLimb(user->right_upper_arm);
	drawLimb(user->right_lower_arm);
	drawLimb(user->left_upper_torso);
	drawLimb(user->right_upper_torso);
	drawLimb(user->left_lower_torso);
	drawLimb(user->left_upper_leg);
	drawLimb(user->left_lower_leg);
	drawLimb(user->right_lower_torso);
	drawLimb(user->right_upper_leg);
	drawLimb(user->right_lower_leg);
	ofFill();
	ofDrawBitmapStringHighlight(ofToString(user->id), user->neck.position[0].X, user->neck.position[0].Y);
}

//--------------------------------------------------------------
void testApp::fixEdge(ofxBox2dCircle circle) {
	float radius = circle.getRadius();
	ofVec2f p = circle.getPosition();
	ofVec2f v = circle.getVelocity();
	if (p.x < radius) {
		circle.setPosition(radius, p.y);
		circle.setVelocity(0, v.y);
	} else if (p.x > ofGetWidth() - radius) {
		circle.setPosition(ofGetWidth() - radius, p.y);
		circle.setVelocity(0, v.y);
	}
	if (p.y < radius) {
		circle.setPosition(p.x, radius);
		circle.setVelocity(v.x, 0);
	} else if (p.y > ofGetHeight() - radius) {
		circle.setPosition(p.x, ofGetHeight() - radius);
		circle.setVelocity(v.x, 0);
	}
	
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	if (key == 'f') {
		ofToggleFullscreen();
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	float ow = 640;
	float oh = 480;
	if (w / ow < h / oh) {
		windowScale = h / oh;
	} else {
		windowScale = w / ow;
	}
	
	
	wall1.destroy();
	wall1 = ofxBox2dPolygon();
	wall1.addVertex(0, 0);
	wall1.addVertex(0, ofGetHeight());
	wall1.addVertex(ofGetWidth(), ofGetHeight());
	wall1.addVertex(ofGetWidth(), 0);
	wall1.addVertex(0, 0);
	wall1.setPhysics(0.0, 1, 0.25);
	wall1.create(box2d.getWorld());
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}