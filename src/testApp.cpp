
#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	
	// load image file
	sourceImg.loadImage("images/bluescreen.jpg");
	if(sourceImg.bpp / 8 != 3) {
		ofSystemAlertDialog("Input Image is no rgb colour image with 24 bit!");
	}
	
	sourceImg.resize(400 , 300);
	
	// weite und hoehe merken
	int w = sourceImg.getWidth();
	int h = sourceImg.getHeight();
	
	// pointer auf bildanfang (1. pixel) setzen
	unsigned char* pixels = sourceImg.getPixels();
	
	//find key colour
	findKeyColour(pixels, w, h);
	
	// call invert
	invertImage(pixels, w, h);
	
	// Bildaenderung registrieren
	sourceImg.update();
}

//--------------------------------------------------------------
void testApp::update(){
	
	ofBackground(255);	
}

//--------------------------------------------------------------
void testApp::draw(){	

	// key box
	ofFill();
	ofSetColor(keyColor);
	ofRect(400, 400, 30, 30);

	ofSetColor(255);
	sourceImg.draw(10, 10);

}

//--------------------------------------------------------------
// Determin key colour
void testApp::findKeyColour(unsigned char* pixels, int w, int h) {
	
	int size = w * h;
	int r, g, b;
	r=g=b=0;
	
	//loop through all pixels and sum up rgb values
	for(int y = 0; y < h; y++) {
		for(int x = 0; x < w; x++) {
			r = r + pixels[ y*w*3 + x*3 ];
			g = g + pixels[ y*w*3 + x*3 + 1];
			b = b + pixels[ y*w*3 + x*3 + 2];
		}
	}
	
	//divide rgb sums through pixel amount to determin average background
	r = r/size;
	g = g/size;
	b = b/size;
	keyColor.set(r, g, b);
	update();
}


//--------------------------------------------------------------
void testApp::invertImage(unsigned char* pixels, int w, int h)
{
	// schleife, die alle pixel durchlaeuft und den rgb-wert invertiert
	for(int y = 0; y < h; y++) {
		for(int x = 0; x < w; x++) {
			
			//read in old values
			int red     = pixels[ y*w*3 + x*3 ];
			int green   = pixels[ y*w*3 + x*3 + 1 ];
			int blue    = pixels[ y*w*3 + x*3 + 2 ];
			
			//calculate invert
			red = 255-red;
			green = 255-green;
			blue = 255-blue;
			
			//write new pixels
			pixels[ y*w*3 + x*3 ] = red;
			pixels[ y*w*3 + x*3 + 1 ] = green;
			pixels[ y*w*3 + x*3 + 2 ] = blue;
		}
	}
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){ 
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	
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

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}
