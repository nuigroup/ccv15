#ifndef _TEST_APP
#define _TEST_APP

//#define AUTOTRACKER

#define OF_ADDON_USING_OFXNCORE

#include "ofMain.h"
#include "ofAddons.h"


class testApp : public TouchListener, public ofSimpleApp
{
public:
	testApp() 
	{
		TouchEvents.addListener(this);
	}
	ofxNCoreVision tbeta;

	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased();

	//Touch Events
	void TouchDown(ofxTBetaCvBlob b);
	void TouchMoved(ofxTBetaCvBlob b);
	void TouchUp(ofxTBetaCvBlob b);
};

#endif

