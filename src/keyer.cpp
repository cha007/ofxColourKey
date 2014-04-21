

//#include <string>
//#include <sstream>
//#include <iostream>
//#include <math.h>
//#include <Windows.h>
//#include "cv.h"
//#include "highgui.h"
//
//
////is required for cout
//using namespace std;
//
//
////std struct for a RGB pixel
//struct rgbValue
//{
//	float valueR;
//	float valueG;
//	float valueB;
//};
//
//
//////////////////////////////////////////////////////////////////////////////////
//
//// SET MAIN IMAGE OBJECT
//IplImage* myFG8;
//IplImage* myFG32;
//IplImage* myFG32_backup;
//IplImage* myBG8;
//IplImage* myBG32;
//IplImage* myMatte8;
//IplImage* myMatte32;
//IplImage* myComp8;
//IplImage* myComp32;
//CvCapture *sourceFile;
//
//// SET MAIN VARIABLES
//int findKeyTolerance = 7;				//tol to calculate the keying RGB
//float bubble1 = 0.15;					//tolerance area ( min = 0.0 / max: sqrt(3))
//int bubble1_GUI = 15; 					//tolerance area for slider ( min = 0 / max: sqrt(3)*100)
//float bubble2 = 0.35;					//softness area ( min = 0.0 / max: sqrt(3))
//int bubble2_GUI = 35; 					//softness area for slider ( min = 0 / max: sqrt(3)*100)
//float matteGamma = 1.0;					//gamma value for matte
//int matteGamma_GUI = 100;				//gamma value for matte (slider)
//rgbValue keyingColorRGB;				//keying color is autodetected later
//int keyColorR_GUI = 0;					//r value for slider (0..255)
//int keyColorG_GUI = 0;					//g value for slider (0..255)
//int keyColorB_GUI = 0;					//b value for slider (0..255)
//int dvOptimize = 0;						//enables the dvImprove method (0=false / 1 = true)
//int antiSpillOption = 0;				//auto antispill of the fg (0=false / 1 = true)
//int spillTolerance = 3;					//the bigger, the smaller the color area
//int erodeMatte = 0;						//erodes the matte (0=false / 1=true)
//int dilateMatte = 0;					//dilates the matte (0=false / 1=true)
//	
//
//
//
//////////////////////////////////////////////////////////////////////////////////
//
//
//
//bool dvImprove(IplImage* srcBGR32)
//{
//	//create new empty image for Y'CrCb representation
//	IplImage* srcYUV32 = cvCreateImage( cvGetSize(srcBGR32), IPL_DEPTH_32F, 3 );
//	cvCvtColor(srcBGR32, srcYUV32, CV_BGR2YCrCb);
//	
//	//three new grey images for each channel one
//	IplImage* srcY = cvCreateImage( cvGetSize(srcBGR32), IPL_DEPTH_32F, 1 );
//	IplImage* srcU = cvCreateImage( cvGetSize(srcBGR32), IPL_DEPTH_32F, 1 );
//	IplImage* srcV = cvCreateImage( cvGetSize(srcBGR32), IPL_DEPTH_32F, 1 );
//	
//	//split source into three grey images
//	cvCvtPixToPlane( srcYUV32, srcY, srcU, srcV, 0 );
//	
//	//smooth color channels
//	cvSmooth(srcU, srcU, CV_GAUSSIAN, 3, 3, 0);
//	cvSmooth(srcV, srcV, CV_GAUSSIAN, 3, 3, 0);
//	
//	//merge Y-U-V to one image
//	cvMerge(srcY, srcU, srcV, NULL, srcYUV32);
//	
//	//back convert Y'CrCb to BGR
//	cvCvtColor(srcYUV32, srcBGR32, CV_YCrCb2BGR);
//	
//	//empty memory
//	cvReleaseImage(&srcYUV32);
//	cvReleaseImage(&srcY);
//	cvReleaseImage(&srcU);
//	cvReleaseImage(&srcV);
//	
//	return true;
//}
//
//
//
//////////////////////////////////////////////////////////////////////////////////
//
//
//

//bool calculateMatte(IplImage* matte,
//					IplImage* srcBGR32,
//			  		rgbValue keyValue,
//			  		float softMin,
//			  		float softMax)
//{
//	//three new grey images for each channel one
//	IplImage* srcB = cvCreateImage( cvGetSize(srcBGR32), IPL_DEPTH_32F, 1 );
//	IplImage* srcG = cvCreateImage( cvGetSize(srcBGR32), IPL_DEPTH_32F, 1 );
//	IplImage* srcR = cvCreateImage( cvGetSize(srcBGR32), IPL_DEPTH_32F, 1 );
//	
//	//split source into three grey images
//	cvCvtPixToPlane( srcBGR32, srcB, srcG, srcR, 0 );
//
//	//create pixel pointer for each channel
//	float* pixelB = (float*) srcB->imageData;
//	float* pixelG = (float*) srcG->imageData;
//	float* pixelR = (float*) srcR->imageData;
//	float* pixelMatte = (float*) matte->imageData;
//
//	//for each pixel: calculate the euclid distance from refenerce value
//	//to pixel value and set the distance value to the matte
//	for(unsigned int j=0 ; j < (srcR->imageSize / sizeof(float)) ; j++) {
//		
//		//calculate distances ( min = 0.0 / max = sqrt(3) )
//		float myB = pow( *pixelB - keyValue.valueB , 2 );
//		float myG = pow( *pixelG - keyValue.valueG , 2 );
//		float myR = pow( *pixelR - keyValue.valueR , 2 );
//		float myDistance = sqrt( myB + myG + myR );
//
//		//map distances to greyvalues (three field softkey) and set to matte
//		//distance calculated by euclidian meassure
//		if( myDistance <= softMin ){
//			*pixelMatte = 0.0;
//		} else if ( myDistance >= softMax ){
//			*pixelMatte = 1.0;
//		} else {
//			float myGreyValue = pow( (myDistance-softMin) / (softMax-softMin) , 2.0 );
//			*pixelMatte = myGreyValue;
//		}
//		
//		//increase pointers
//		pixelB++; pixelG++; pixelR++; pixelMatte++;
//	}
//
//	//delete image objects to save memory
//	cvReleaseImage(&srcR);
//	cvReleaseImage(&srcG);
//	cvReleaseImage(&srcB);
//
//	return true;
//}
//
//
//
//////////////////////////////////////////////////////////////////////////////////
//
//
//
//bool editMatte(IplImage* myMatte32, bool dilateMatte, bool erodeMatte, float gamma) {
//	
//	//possible erosion of the matte
//	if(erodeMatte == 1) {
//		cvErode(myMatte32, myMatte32, NULL, 1 );
//	}
//	
//	//possible dilatation of the matte
//	if(dilateMatte == 1) {
//		cvDilate(myMatte32, myMatte32, NULL, 1 );
//	}
//	
//	//possible gamma correction
//	if(gamma != 1.0) {
//		cvPow(myMatte32, myMatte32, gamma);
//	}
//	
//	return true;
//}
//
//
//
//////////////////////////////////////////////////////////////////////////////////
//
//
//
//
//bool antiSpill(IplImage* srcBGR, rgbValue myKeyValueHLS, unsigned int spillTol)
//{
//	//create empty 32 bit image for HLS representation
//	IplImage* srcHLS = cvCreateImage( cvGetSize(srcBGR), IPL_DEPTH_32F, 3 );
//	
//	//convert color space from RGB into HLS
//	cvCvtColor(srcBGR , srcHLS , CV_BGR2HLS);	
//	
//	//create empty image for H , L (will not be changed) and S
//	IplImage* channelH = cvCreateImage( cvGetSize(srcHLS), IPL_DEPTH_32F, 1 );
//	IplImage* channelL = cvCreateImage( cvGetSize(srcHLS), IPL_DEPTH_32F, 1 );
//	IplImage* channelS = cvCreateImage( cvGetSize(srcHLS), IPL_DEPTH_32F, 1 );
//	
//	//split source empty images
//    cvCvtPixToPlane( srcHLS, channelH, channelL, channelS, 0 );
//    
//    //create pixel pointers for H and S
//    float* pixelH = (float*) channelH->imageData;
//	float* pixelS = (float*) channelS->imageData;
//	
//	//get keying H value to be surpressed
//	float keyH = myKeyValueHLS.valueR / 360.0;
//	
//	//loop through pixels and set the saturation according to the h value
//	for(unsigned int j=0 ; j < (channelH->imageSize / sizeof(float)) ; j++) {
//		
//		//scale h value down to 0..1
//		float myH = *pixelH / 360;
//		
//		//calculate factor of how much S should be reduced (range: -7 .. +1)
//		float pi = 3.14159;
//		float myFactor = (float)spillTol * sin( 2.0*pi*(keyH + (0.25-myH)) ) - 
//														(float)((float)spillTol - 1.5);
//		
//		//limit myFactor to range 0.0 - 1.0
//		if(myFactor < 0.0) {
//			myFactor = 0.0;
//		}
//		
//		//reduce S according to the factor
//		*pixelS = *pixelS - (*pixelS * myFactor);
//		
//		//increase pixel pointers to next pixel
//		pixelH++;
//		pixelS++;
//	}
//	
//	//recombine HLS
//	cvMerge(channelH, channelL, channelS, NULL, srcHLS);
//	
//	//convert colors from RGB into HLS and save image
//	cvCvtColor(srcHLS , srcBGR , CV_HLS2BGR);
//	
//	//delete image objects to save memory
//	cvReleaseImage(&srcHLS);
//	cvReleaseImage(&channelH);
//	cvReleaseImage(&channelL);
//	cvReleaseImage(&channelS);
//	
//	return true;
//}
//
//
//
//////////////////////////////////////////////////////////////////////////////////
//
//
////Composition uses the formula C = x*FG + (1-x)BG
////where x is a value from myMatte (0.0 - 1.0)
//
//bool makeComposit(IplImage* myFG,
//				  IplImage* myBG,
//				  IplImage* myMatte,
//				  IplImage* composition)
//{
//	//flip fg and matte
//	cvFlip(myFG, NULL, 0);
//	cvFlip(myMatte, NULL, 0);
//		
//	//create empty image for foreground and background (will be summed later)
//	IplImage* cFG = cvCreateImage( cvGetSize(myFG), IPL_DEPTH_32F, 3 );
//	IplImage* cBG = cvCreateImage( cvGetSize(myBG), IPL_DEPTH_32F, 3 );
//	
//	//create a matte consiting of 3x myMatte for easy composition (openCV spacific)
//	IplImage* newMatte = cvCreateImage( cvGetSize(myMatte), IPL_DEPTH_32F, 3 );
//	cvMerge(myMatte, myMatte, myMatte, NULL, newMatte);
//	
//	//create the matte difference image (1-x)
//	IplImage* negMatte = cvCreateImage( cvGetSize(myMatte), IPL_DEPTH_32F, 3 );
//	CvScalar myScalar = cvScalarAll(1.0);
//	cvAbsDiffS(newMatte, negMatte, myScalar);
//	
//	//create composition FG and BG element
//	cvMul( myFG , newMatte , cFG );
//	cvMul( myBG, negMatte, cBG );
//	
//	//add FG and BG
//	cvAdd(cFG , cBG, composition, NULL);
//	
//	//delete image objects to save memory
//	cvReleaseImage(&cFG);
//	cvReleaseImage(&cBG);
//	cvReleaseImage(&newMatte);
//	cvReleaseImage(&negMatte);
//	
//	return true;
//}
//
//
//////////////////////////////////////////////////////////////////////////////////
//
//
//rgbValue convertRGB2HLS(rgbValue myRGB)
//{
//	//empty images for new HLS values
//	IplImage* myH = cvCreateImage( cvSize(1,1), IPL_DEPTH_32F, 1 );
//	IplImage* myL = cvCreateImage( cvSize(1,1), IPL_DEPTH_32F, 1 );
//	IplImage* myS = cvCreateImage( cvSize(1,1), IPL_DEPTH_32F, 1 );
//	float* pixelH = (float*) myH->imageData;
//	float* pixelL = (float*) myL->imageData;
//	float* pixelS = (float*) myS->imageData;
//	
//	//convert RGB to 0..1 range and copy to h/l/s images
//	*pixelH = myRGB.valueB / 255.0;
//	*pixelL = myRGB.valueG / 255.0;
//	*pixelS = myRGB.valueR / 255.0;
//	
//	//merge h/l/s -> HLS
//	IplImage* myHLS = cvCreateImage( cvSize(1,1), IPL_DEPTH_32F, 3 );
//	cvMerge(myH, myL, myS, NULL, myHLS);
//	
//	//convert to HLS
//	cvCvtColor(myHLS, myHLS, CV_BGR2HLS);
//	
//	//slip into single images and copy values into struct
//	cvSplit(myHLS, myH, myL, myS, NULL);
//	myRGB.valueR = *pixelH;
//	myRGB.valueG = *pixelL;
//	myRGB.valueB = *pixelS;
//	
//	//delete image objects to save memory
//	cvReleaseImage(&myH);
//	cvReleaseImage(&myL);
//	cvReleaseImage(&myS);
//	cvReleaseImage(&myHLS);
//	
//	return(myRGB);
//}
//
//
//
//////////////////////////////////////////////////////////////////////////////////
//
//
//
//rgbValue findKeyValue (IplImage* srcBGR, unsigned short int tolerance)
//{
//	//create empty 32 bit image for HLS representation
//	IplImage* srcHLS = cvCreateImage( cvGetSize(srcBGR), IPL_DEPTH_32F, 3 );
//	
//	//convert color space from RGB into HLS
//	cvCvtColor(srcBGR , srcHLS , CV_BGR2HLS);	
//	
//	//create empty image for the histogram H value (8 bit, greyscale image)
//	IplImage* channelH = cvCreateImage( cvGetSize(srcHLS), IPL_DEPTH_32F, 1 );
//	
//	//create empty images for the later RGB value key
//	IplImage* channelR = cvCreateImage( cvGetSize(srcBGR), IPL_DEPTH_32F, 1 );
//	IplImage* channelG = cvCreateImage( cvGetSize(srcBGR), IPL_DEPTH_32F, 1 );
//	IplImage* channelB = cvCreateImage( cvGetSize(srcBGR), IPL_DEPTH_32F, 1 );
//	
//	//split source video frame into HLS channels, copy H to empty image
//    cvCvtPixToPlane( srcHLS, channelH, NULL, NULL, 0 );
//    
//    //split source video frame into RGB channels, copy R/G/B to empty image
//    cvCvtPixToPlane( srcBGR, channelB, channelG, channelR, 0 );
//    
//   	//delete IPLimage object scrHLS to save memory
//	cvReleaseImage(&srcHLS);
//
//	//rounding the values from H (i.e. 122.176 -> 122.0) for a more exact histo
//	float* myPointer = (float*)channelH->imageData;
//	for(unsigned int j=0 ; j < (channelH->imageSize / sizeof(float)) ; j++) {
//		*myPointer = (float)cvRound(*myPointer);
//		myPointer++;
//	}
//	
//	//bins determines the number of levels the colors are divided into
//    //Since the H value (color tone) has a range from 0..359 we define 360 bins
//    int bins = 360;
//    int hist_size[] = {bins};
//    
//    //hue varies from 0 (~0°red) to 359 (359° red again)
//    float range[] = { 0, 360 };
//    float* ranges[] = { range };
//
//    //init variables
//    CvHistogram *histoH;
//    float maxValueH = 0.0;
//    int key_h_value = 0;
//           
//    //create and calculate the histogram of H value
//    histoH = cvCreateHist( 1, hist_size, CV_HIST_ARRAY, ranges, 1 );
//    cvCalcHist( &channelH, histoH, 0, 0 );
//    
//    //the max values of the calculated histograms
//    cvGetMinMaxHistValue( histoH, 0, &maxValueH, 0, 0 );
//    
//    //create the histogram image background (gradient from h=0 to h=360)
//    IplImage* histoImg = cvLoadImage( "histo.bmp" , 1 );
//    
//    //loop through histo values and draw them as the histogram bars                   
//    for( int h=0 ; h < bins; h++ )
//    {
//		//get bin value from the histogram array
//    	float bin_val = cvQueryHistValue_1D( histoH, h );
//    	
//		//scale value for display (scale into histo image size)
//        int intensity = cvRound(bin_val * (histoImg->height - 10) / maxValueH);
//        
//        //draw the bar
//        cvRectangle( histoImg, cvPoint( h, histoImg->height - intensity ),
//                             	cvPoint( h , histoImg->height),
//                             	CV_RGB(0,0,0), 
//                             	CV_FILLED );
//                             
//        //remember highest histo value and corresponding H value
//        if(bin_val == maxValueH){
//        	key_h_value = h;
//        }
//    }
//
//    //show images
//    cvShowImage ("colorHistogram", histoImg);
//
//    //calculate RGB keyvalue: run through hls image and whenever h-value is
//    //within h-key range, read the r/g/b value and average them
//    float* pixelH = (float*) channelH->imageData;
//    float* pixelR = (float*) channelR->imageData;
//    float* pixelG = (float*) channelG->imageData;
//    float* pixelB = (float*) channelB->imageData;
//    float sumR = 0;		//0.0 - 1.0
//    float sumG = 0;		//0.0 - 1.0
//    float sumB = 0;		//0.0 - 1.0
//    unsigned long int sumCounter = 0;
//
//	//loop through pixels
//    for(unsigned int j=0; j < (channelH->imageSize / sizeof(float)) ; j++){
//    	
//    	//is the current pixel H within the key tolerance
//    	if( (*pixelH) > (key_h_value - tolerance) &&
//    		(*pixelH) < (key_h_value + tolerance) ){
//    			//add the rgb value to a sum and increase element counter
//    			sumR = sumR + *pixelR;
//    			sumG = sumG + *pixelG;
//				sumB = sumB + *pixelB;
//				sumCounter++;
//    	}
//
//    	//increase pixel pointer
//    	pixelH++; pixelR++; pixelG++; pixelB++;
//    }
//  			
//    //calculate average and save key value in struct
//    rgbValue myKeyRGB;
//    if( sumCounter == 0 ){
//    	myKeyRGB.valueB = 0.0;
//	   	myKeyRGB.valueG = 0.0;
//    	myKeyRGB.valueR = 0.0;
//    }else{
//    	myKeyRGB.valueB = sumB / sumCounter;
//	    myKeyRGB.valueG = sumG / sumCounter;
//    	myKeyRGB.valueR = sumR / sumCounter;
//    }
//    
//    //delete image objects to save memory
//    cvReleaseImage(&srcHLS);
//    cvReleaseImage(&channelH);
//    cvReleaseImage(&channelR);
//    cvReleaseImage(&channelG);
//    cvReleaseImage(&channelB);
//    cvReleaseImage(&histoImg);
//    
//    //return key value
//    cout << "Key H-value = " << key_h_value << endl;
//    cout << "RGB key color is: " << (int)(myKeyRGB.valueR * 255) << "/"
//    							 << (int)(myKeyRGB.valueG * 255) << "/"
//    							 << (int)(myKeyRGB.valueB * 255) << endl;
//    	 
//    return(myKeyRGB);
//}
//
//
//
//////////////////////////////////////////////////////////////////////////////////
//
//
//
//void updateScreen(int id)
//{
//	//create copy of fg image to be used for changes - like this the original
//	//fg image (backup) is not edited
//	myFG32 = cvCloneImage(myFG32_backup);
//	
//	//update float values (calculate them from GUI int values)
//	matteGamma = ((float) cvGetTrackbarPos("Gamma" , "matte"))  / 100.0;
//	bubble1 = ((float) cvGetTrackbarPos("Bubble 1" , "composit"))  / 100.0;
//	bubble2 = ((float) cvGetTrackbarPos("Bubble 2" , "composit"))  / 100.0;
//	keyingColorRGB.valueR = cvGetTrackbarPos("Key R" , "colorHistogram") / 255.0;
//	keyingColorRGB.valueG = cvGetTrackbarPos("Key G" , "colorHistogram") / 255.0;
//	keyingColorRGB.valueB = cvGetTrackbarPos("Key B" , "colorHistogram") / 255.0;
//	
//	//blur UV channel of source image (DV video option)
//	if(dvOptimize == 1){
//		dvImprove(myFG32);
//	}
//	
//	//calculate matte
//	calculateMatte(myMatte32, myFG32, keyingColorRGB, bubble1, bubble2);
//	
//	//post-process matte
//	editMatte(myMatte32, erodeMatte, dilateMatte, matteGamma);
//	
//	//prepare FG
//	if(antiSpillOption == 1){
//		antiSpill(myFG32, convertRGB2HLS(keyingColorRGB), spillTolerance);
//	}
//	
//	//calculate composition
//	makeComposit(myFG32, myBG32, myMatte32, myComp32);
//	
//	//show images
//	cvShowImage ("foreground", myFG32);
//	cvShowImage ("background", myBG8);
//	cvShowImage ("matte", myMatte32);
//	cvShowImage ("composit", myComp32);
//	
//}
//
//
//////////////////////////////////////////////////////////////////////////////////
//
//
//void renderKey()
//{
//	//update float values (calculate them from GUI int values)
//	matteGamma = ((float) cvGetTrackbarPos("Gamma" , "matte"))  / 100.0;
//	bubble1 = ((float) cvGetTrackbarPos("Bubble 1" , "composit"))  / 100.0;
//	bubble2 = ((float) cvGetTrackbarPos("Bubble 2" , "composit"))  / 100.0;
//	keyingColorRGB.valueR = cvGetTrackbarPos("Key R" , "colorHistogram") / 255.0;
//	keyingColorRGB.valueG = cvGetTrackbarPos("Key G" , "colorHistogram") / 255.0;
//	keyingColorRGB.valueB = cvGetTrackbarPos("Key B" , "colorHistogram") / 255.0;
//	
//	//create output folder
//    SYSTEMTIME st;
//    GetSystemTime(&st);
//    ostringstream myFolderStream;
//    myFolderStream << "keying_" << st.wYear << "-" << st.wMonth << "-" << st.wDay
//    			   << "_" << st.wHour << "-" << st.wMinute;
//    string renderFolderName = myFolderStream.str();
//    LPSECURITY_ATTRIBUTES attr;
//	attr = NULL;
//	CreateDirectory(renderFolderName.c_str(), attr);
//	
//	//create 2 video writer object
//	string myCompPath = renderFolderName + "/composit.avi";
//	string myMattePath = renderFolderName + "/matte.avi";
//	CvVideoWriter *myCompWriter = cvCreateVideoWriter( myCompPath.c_str(), -1, 25, cvGetSize(myFG32), 1);
//	CvVideoWriter *myMatteWriter = cvCreateVideoWriter( myMattePath.c_str(), -1, 25, cvGetSize(myFG32), 1);
//	
//	//loop through movie
//	double currentFrame = 0.0;
//	while( currentFrame < (cvGetCaptureProperty(sourceFile , CV_CAP_PROP_FRAME_COUNT) - 1) ){
//		
//		//get new image
//		myFG8 = cvQueryFrame(sourceFile);
//		cvConvertScale( myFG8 , myFG32 , 1.0/255.0 , 0 );
//		
//		//blur UV channel of source image (DV video option)
//		if(dvOptimize == 1){
//			dvImprove(myFG32);
//		}
//		
//		//calculate matte
//		calculateMatte(myMatte32, myFG32, keyingColorRGB, bubble1, bubble2);
//		
//		//post-process matte
//		editMatte(myMatte32, erodeMatte, dilateMatte, matteGamma);
//		
//		//prepare FG
//		if(antiSpillOption == 1){
//			antiSpill(myFG32, convertRGB2HLS(keyingColorRGB), spillTolerance);
//		}
//		
//		//calculate composition and scale it down to 8 bit
//		makeComposit(myFG32, myBG32, myMatte32, myComp32);
//		cvConvertScale( myComp32 , myComp8 , 255 , 0 );
//		cvConvertScale( myMatte32 , myMatte8 , 255 , 0 );
//		
//		//write composit and matte image to output movie
//		cvWriteFrame(myCompWriter, myComp8);
//		cvWriteFrame(myMatteWriter, myMatte8);
//		
//		//increase frame counter
//		currentFrame++;
//	}
//	
//	//release video writer
//	cvReleaseVideoWriter(&myCompWriter);
//	cvReleaseVideoWriter(&myMatteWriter);
//}
//
//
//
//////////////////////////////////////////////////////////////////////////////////
//
//
//
//int main(int argc, char *argv[])
//{
//	
//	//checking if both parameters are supplied (foreground video and background image)
//    if( argc != 3 ) {
//        cout << "ERROR: WRONG PARAMETER USAGE!" << endl 
//             << "please use this format: " << argv[0] << " <keying_video_file> <background_image_file>" << endl;
//             exit(0);
//    }
//     
//	
//	////////////////////////////////////////////////////////////////////////////
//	// SET IMAGE FILES
//	////////////////////////////////////////////////////////////////////////////
//	
//	//set foreground source avi file (read in 8 bit, resize and convert to 32 bit)
//	sourceFile = cvCaptureFromFile( argv[1] );
//	IplImage* myFG8 = cvQueryFrame(sourceFile);
//	myFG32_backup = cvCreateImage( cvGetSize(myFG8), IPL_DEPTH_32F, 3 );
//	cvConvertScale( myFG8 , myFG32_backup , 1.0/255.0 , 0 );
//	myFG32 = cvCloneImage(myFG32_backup);
//	
//	//set background image file (read in 8 bit, resize and convert to 32 bit)
//	myBG8 = cvCreateImage( cvGetSize(myFG8), 8, 3 );
//	cvResize(cvLoadImage( argv[2] ) , myBG8 , CV_INTER_CUBIC);
//	myBG32 = cvCreateImage( cvGetSize(myFG32), IPL_DEPTH_32F, 3 );
//	cvConvertScale( myBG8 , myBG32 , 1.0/255.0 , 0 );
//	
//	//create empty image for the image matte (32 bit for calculation, 8 bit for output)
//	myMatte32 = cvCreateImage( cvGetSize(myFG32), IPL_DEPTH_32F, 1 );
//	myMatte8 = cvCreateImage( cvGetSize(myFG32), 8, 1 );
//	
//	//create empty image for final composit (32 bit for calculation, 8 bit for output)
//	myComp32 = cvCreateImage( cvGetSize(myFG32), IPL_DEPTH_32F, 3 );
//	myComp8 = cvCreateImage( cvGetSize(myFG32), 8, 3 );
//
//	////////////////////////////////////////////////////////////////////////////
//	// CREATE GUI
//	////////////////////////////////////////////////////////////////////////////
//	
//	//create windows for images
//	cvNamedWindow("foreground", CV_WINDOW_AUTOSIZE);
//	cvNamedWindow("background", CV_WINDOW_AUTOSIZE);
//	cvNamedWindow("matte", CV_WINDOW_AUTOSIZE);
//	cvNamedWindow("composit", CV_WINDOW_AUTOSIZE);
//	cvNamedWindow("colorHistogram", CV_WINDOW_AUTOSIZE);
//	
//	//set key color
//	cout << "Retrieve key color automatically..." << endl;
//	keyingColorRGB = findKeyValue(myFG32, findKeyTolerance);
//	keyColorR_GUI = cvRound(keyingColorRGB.valueR * 255);
//	keyColorG_GUI = cvRound(keyingColorRGB.valueG * 255);
//	keyColorB_GUI = cvRound(keyingColorRGB.valueB * 255);
//	
//	//create sliders in histo window
//	cvCreateTrackbar( "Key R", "colorHistogram", &keyColorR_GUI, 255, updateScreen );
//	cvCreateTrackbar( "Key G", "colorHistogram", &keyColorG_GUI, 255, updateScreen );
//	cvCreateTrackbar( "Key B", "colorHistogram", &keyColorB_GUI, 255, updateScreen );
//	
//	//create sliders in control window
//	cvCreateTrackbar( "Bubble 1", "composit", &bubble1_GUI, 173, updateScreen );
//	cvCreateTrackbar( "Bubble 2", "composit", &bubble2_GUI, 173, updateScreen );
//	cvCreateTrackbar( "AntiSpill", "composit", &antiSpillOption, 1, updateScreen );
//	cvCreateTrackbar( "Spill Tol", "composit", &spillTolerance, 5, updateScreen );
//	cvCreateTrackbar( "DV Improve", "composit", &dvOptimize, 1, updateScreen );
//	
//	//create sliders in matte window
//	cvCreateTrackbar( "Erode", "matte", &erodeMatte, 1, updateScreen );
//	cvCreateTrackbar( "Dilate", "matte", &dilateMatte, 1, updateScreen );
//	cvCreateTrackbar( "Gamma", "matte", &matteGamma_GUI, 200, updateScreen );
//	
//	//process key for first frame
//	updateScreen(0);
//
//	////////////////////////////////////////////////////////////////////////////
//	// WAIT FOR RENDER COMMAND
//	////////////////////////////////////////////////////////////////////////////
//	
//	//if "r" pressed, render movie and quit  // if "q" pressed simply quit
//	char lastKey = cvWaitKey(0);
//	while ( lastKey != 'q' ){
//		if(lastKey == 'r'){
//			cout << "Rendering video..." << endl;
//			renderKey();
//			lastKey = 'a';
//		} else {
//			lastKey = cvWaitKey(0);
//		}
//	}
//	
//	//end main function
//	return true;
//}
//
//
//
///////////////////////////////////////////////////////////////////////////////
//
//
//
