#pragma once

#include "ofMain.h"


class distanceKey : public ofBaseApp{
	
	public:
		
		void setup();
		void update();
		void draw();
		
		void findKeyColour(unsigned char* pixels, int w, int h); 
		void createMatte(unsigned char* pixels, int w, int h); 
	
		ofImage sourceImg;
		ofImage matteImg;
	
		ofColor keyColor;
		
		int bubble1;
		int bubble2;
		
};

