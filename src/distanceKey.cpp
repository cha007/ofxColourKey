
#include "distanceKey.h"

//--------------------------------------------------------------
void distanceKey::setup(){
	
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
	
	//threshold size (=bubble1) and tolerance (=bubble2) of keying value, each 0-255, bubble1 must be smaller than bubble2
	bubble1 = 25;
	bubble2 = 50;
	
	//find key colour
	findKeyColour(pixels, w, h);
	
	// call invert
	//invertImage(pixels, w, h);
	
	//calclulate matte
	createMatte(pixels, w, h);
	
	// Bildaenderung registrieren
	sourceImg.update();
}

//--------------------------------------------------------------
void distanceKey::update(){
	
	ofBackground(255);	
}

//--------------------------------------------------------------
void distanceKey::draw(){	

	// key box
	ofFill();
	ofSetColor(keyColor);
	ofRect(500, 300, 30, 30);

	ofSetColor(255);
	sourceImg.draw(10, 10);
	matteImg.draw(10, 300);

}

//--------------------------------------------------------------
// Determin key colour
void distanceKey::findKeyColour(unsigned char* pixels, int w, int h) {
	
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
	cout << "Keying color set to: R=" << (int)keyColor.r << " / G=" << (int)keyColor.g << " / B=" << (int)keyColor.b << endl;
	
	update();
}


//--------------------------------------------------------------
void distanceKey::createMatte(unsigned char* pixels, int w, int h)
{
	//create greyscale images for matte of source image
	matteImg.allocate(w, h, OF_IMAGE_GRAYSCALE);
	
	//counter for matte position
	int i = 0;
	
	// schleife, die alle pixel durchlaeuft und den rgb-wert invertiert
	for(int y = 0; y < h; y++) {   //y = zeile
		for(int x = 0; x < w; x++) {    //x = pixel
			
			//read in source pixel values
			int red     = pixels[ y*w*3 + x*3 ];
			int green   = pixels[ y*w*3 + x*3 + 1 ];
			int blue    = pixels[ y*w*3 + x*3 + 2 ];
			
			//calculate Euklidian distance in float space for RGB each min = 0.0 / max = 1.0
			//overall distance, min=0.0 and max=sqrt(3*1.0)
			float distR = powf( float(red) / 255.0 - float(keyColor.r) / 255.0  ,  2 );
			float distG = powf( float(green) / 255.0 - float(keyColor.g) / 255.0  , 2 );
			float distB = powf( float(blue) / 255.0 - float(keyColor.b) / 255.0  , 2 );
			float myDistance = sqrt( distR + distG + distB );
			
			//grey value for matte, 0-255
			int myGreyValue = 255 * myDistance / sqrt(3.0) ;
			
			//map distances to greyvalues (three field softkey) and set to matte
			if( myGreyValue <= bubble1 ){
				matteImg.getPixelsRef()[i] = 0;
			} else if ( myGreyValue >= bubble2 ){
				matteImg.getPixelsRef()[i] = 255;
			} else {
				myGreyValue = 255 * (myGreyValue-bubble1) / (bubble2-bubble1);
    			matteImg.getPixelsRef()[i] = myGreyValue;
			}
			i++;
		}
	}
	
	// update texture
	matteImg.reloadTexture();
	
}
