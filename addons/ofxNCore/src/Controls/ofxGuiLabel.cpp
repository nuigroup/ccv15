//! ofxGuiLabel.cpp
/*!
*  
*
*  Created by Yishi Guo on 06/02/2011.
*  Copyright 2011 NUI Group. All rights reserved.
*
*/
//	----------------------------------------------------------------------------------------------------

#include "ofxGuiLabel.h"
//	----------------------------------------------------------------------------------------------------

ofxGuiLabel::ofxGuiLabel() {
	mParamType = kofxGui_Object_Text;
}

//	----------------------------------------------------------------------------------------------------

void ofxGuiLabel::init( int id, string name, int x, int y, int width, int height, string text, string fontName, int fontSize, ofRGBA fontColor ) {
	mParamId	= id;
	mParamName	= name;

	mObjX		= x;
	mObjY		= y;

	mObjWidth	= width;
	mObjHeight	= height;

	setText( text );
	setColor( fontColor );

	this->fontName = fontName;
	this->fontSize = fontSize;

	textFont = new ofTrueTypeFont();
	textFont->loadFont( this->fontName, this->fontSize, true, true );

}

//	----------------------------------------------------------------------------------------------------

void ofxGuiLabel::init( int id, string name, int x, int y, int width, int height, string text, ofTrueTypeFont* font, ofRGBA color ) {
	mParamId	= id;
	mParamName	= name;

	mObjX		= x;
	mObjY		= y;

	mObjWidth	= width;
	mObjHeight	= height;

	setText( text );
	setFont( font );
	setColor( color );
}


//	----------------------------------------------------------------------------------------------------

void ofxGuiLabel::setText( string text ) {
	this->text = text;
}
//	----------------------------------------------------------------------------------------------------

void ofxGuiLabel::setColor( ofRGBA color ) {
	this->fontColor = color;
}
//	----------------------------------------------------------------------------------------------------

void ofxGuiLabel::drawString( float x, float y, string text, bool center ) {
	glColor4f( fontColor.r, fontColor.g, fontColor.b, fontColor.a );

	if ( center ) {
		x -= roundInt( textFont->stringWidth( text ) /2.0f );
	} else {
		x += mGlobals->mParamFontXOffset;
	}

	y += mGlobals->mParamFontYOffset;
	textFont->drawString( text, x, y );
}
//	----------------------------------------------------------------------------------------------------


bool ofxGuiLabel::update( int id, int task, void* data, int length ) {
	bool handled = false;

	if ( id == mParamId ) {
		// TODO
		handled = true;
	}

	return true;
}
//	----------------------------------------------------------------------------------------------------

void ofxGuiLabel::draw() {
	glPushMatrix();
		glTranslatef( mObjX, mObjY, 0.0f );

		glColor4f( fontColor.r, fontColor.g, fontColor.b, fontColor.a );

		//if( mParamName != "" ) {
		//	drawString( 0, 0, mParamName, false );
		//}

		//fontHeight = textFont.stringHeight( mParamName );

		if ( text != "" ) {
			drawString( 0, 0, text, false );
		}

		ofFill();

		//! background
		glColor4f( mGlobals->mCoverColor.r, mGlobals->mCoverColor.g, mGlobals->mCoverColor.b, mGlobals->mCoverColor.a );
		ofRect( mCtrX, mCtrY, mCtrWidth, mCtrHeight );

	glPopMatrix();

}
//	----------------------------------------------------------------------------------------------------

bool ofxGuiLabel::mouseDragged( int x, int y, int button ) {
	// TODO
	return mMouseIsDown;
}
//	----------------------------------------------------------------------------------------------------

bool ofxGuiLabel::mousePressed( int x, int y, int button ) {
	// TODO
	return mMouseIsDown;
}
//	----------------------------------------------------------------------------------------------------

bool ofxGuiLabel::mouseReleased( int x, int y, int button ) {
	// TODO
	return mMouseIsDown;
}
//	----------------------------------------------------------------------------------------------------

void ofxGuiLabel::buildFromXml() {
	mGlobals->mListener->handleGui( mParamId, kofxGui_Set_String, &text, sizeof(string));
}
//	----------------------------------------------------------------------------------------------------

void ofxGuiLabel::saveToXml() {
	int id = saveObjectData();

	mGlobals->mXml.setValue( "OBJECT:TEXT", text, id );
}

//	----------------------------------------------------------------------------------------------------

void ofxGuiLabel::setFont( ofTrueTypeFont* font ) {
	this->textFont = font;
}

//	----------------------------------------------------------------------------------------------------
