//! ofxGuiLabel.h
/*!
*  
*
*  Created by Yishi Guo on 06/02/2011.
*  Copyright 2011 NUI Group. All rights reserved.
*
*/

#ifndef OFX_GUI_TEXT
#define OFX_GUI_TEXT

#include "ofxGuiTypes.h"
#include "ofxGuiObject.h"

class ofxGuiLabel : public ofxGuiObject {
public:
	ofxGuiLabel();

	void init( int id, string name, int x, int y, int width, int height, string text, string fontName, int fontSize, ofRGBA  fontColor );
	void init( int id, string name, int x, int y, int width, int height, string text, ofTrueTypeFont* font, ofRGBA color );

	void setText( string text );
	void setColor( ofRGBA color );

	void drawString( float x, float y, string text, bool center );

	bool update( int parameterId, int task, void* data, int length );
	void draw();

	bool mouseDragged( int x, int y, int button );
	bool mousePressed( int x, int y, int button );
	bool mouseReleased( int x, int y, int button );

	void buildFromXml();
	void saveToXml();

	string text, fontName;
	int fontSize, fontHeight;
	ofRGBA fontColor;
	ofTrueTypeFont* textFont;

private:
	void setFont( ofTrueTypeFont* font );
};

#endif