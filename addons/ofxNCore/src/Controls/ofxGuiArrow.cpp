//! ofxGuiArrow.cpp
/*!
*  
*
*  Created by Yishi Guo on 06/11/2011.
*  Copyright 2011 NUI Group. All rights reserved.
*
*/

// ----------------------------------------------

#include "ofxGuiArrow.h"

// ----------------------------------------------

ofxGuiArrow::ofxGuiArrow() {
	mParamType = kofxGui_Object_Arrow;
	
	//! Be not selected
	mValue = false;
}

// ----------------------------------------------

void ofxGuiArrow::init( int id, string name, int x, int y, int width, int height, int diretion, int offset ) {
	// TODO
	mParamId		= id;
	mParamName		= name;

	mObjX			= x;
	mObjY			= y;

	mObjWidth		= width;
	mObjHeight		= height;

	mOffset			= offset;

	setDirection( diretion );
	setControlRegion( 0, 0, width, height );

}

// ----------------------------------------------

void ofxGuiArrow::setDirection( int direction ) {
	mDirection = direction;
}

// ----------------------------------------------

void ofxGuiArrow::setValue( bool value ) {
	mValue = value;
}

// ----------------------------------------------

bool ofxGuiArrow::update( int id, int task, void* data, int length ) {
	// TODO
	bool handled = false;

	if ( id == mParamId && length == sizeof(bool) ) {
		setDirection( *(bool*)data );
		handled = true;
	}

	return handled;
}

// ----------------------------------------------

void ofxGuiArrow::draw() {
	// TODO
	drawArrow( mDirection );
}

// ----------------------------------------------

bool ofxGuiArrow::mouseDragged( int x, int y, int button ) {
	return mMouseIsDown;
}

// ----------------------------------------------

bool ofxGuiArrow::mousePressed( int x, int y, int button ) {
	mMouseIsDown = isPointInsideMe( mouseToLocal( x, y ) );

	if ( mMouseIsDown ) {
		setValue( true );	// Pressed

		mGlobals->mListener->handleGui( mParamId, kofxGui_Set_Bool, &mValue, sizeof(bool) );
	}

	return mMouseIsDown;
}

// ----------------------------------------------

bool ofxGuiArrow::mouseReleased( int x, int y, int button ) {
	bool handled = mMouseIsDown;

	if ( mMouseIsDown )
	{
		setValue( false );
		mGlobals->mListener->handleGui( mParamId, kofxGui_Set_Bool, &mValue, sizeof(bool) );

		mMouseIsDown = false;
	}

	return handled;

}

// ----------------------------------------------

void ofxGuiArrow::buildFromXml() {
	// TODO
	mGlobals->mListener->handleGui( mParamId, kofxGui_Set_Bool, &mValue, sizeof(bool) );
}

// ----------------------------------------------

void ofxGuiArrow::saveToXml() {
	// TODO
	saveObjectData();
}

// ----------------------------------------------

void ofxGuiArrow::drawArrow( int direction ) {
	// TODO
	glPushMatrix();
		glTranslatef( mObjX, mObjY, 0.0f );

		//! No param name output.

		//ofFill();

		////! background
		//glColor4f( mGlobals->mCoverColor.r, mGlobals->mCoverColor.g, mGlobals->mCoverColor.b, mGlobals->mCoverColor.a );
		//ofRect( mCtrX, mCtrY, mCtrWidth, mCtrHeight );

		if ( mValue == true ) {
			//! Handle
			ofFill();
			glColor4f( mGlobals->mButtonColor.r, mGlobals->mButtonColor.g, mGlobals->mButtonColor.b, mGlobals->mButtonColor.a );
		} else {
			ofNoFill();
			glColor4f( mGlobals->mFrameColor.r, mGlobals->mFrameColor.g, mGlobals->mFrameColor.b, mGlobals->mFrameColor.a );
		}

		switch( direction ) {
			case kofxGui_Arrow_Up:
				glBegin( GL_TRIANGLES );
					glVertex3f( mCtrX + mCtrWidth/2, mCtrY + mOffset, 0.0f );
					glVertex3f( mCtrX, mCtrY + mCtrHeight, 0.0f );
					glVertex3f( mCtrX + mCtrWidth, mCtrY + mCtrHeight, 0.0f );
				glEnd();
				break;

			case kofxGui_Arrow_Down:
				glBegin( GL_TRIANGLES );
					glVertex3f( mCtrX, mCtrY, 0.0f );
					glVertex3f( mCtrX + mCtrWidth, mCtrY, 0.0f );
					glVertex3f( mCtrX + mCtrWidth/2, mCtrY + mCtrHeight - mOffset, 0.0f );
				glEnd();
				break;

			case kofxGui_Arrow_Right:
				glBegin( GL_TRIANGLES );
					glVertex3f( mCtrX, mCtrY, 0.0f );
					glVertex3f( mCtrX, mCtrY + mCtrHeight, 0.0f );
					glVertex3f( mCtrX + mCtrWidth - mOffset, mCtrY + mCtrHeight/2, 0.0f );
				glEnd();
				break;
				
			case kofxGui_Arrow_Left:
				glBegin( GL_TRIANGLES );
					glVertex3f( mCtrX + mOffset, mCtrY + mCtrHeight/2, 0.0f );
					glVertex3f( mCtrX + mCtrWidth, mCtrY, 0.0f );
					glVertex3f( mCtrX + mCtrWidth, mCtrY + mCtrHeight, 0.0f );
				glEnd();
				break;

			default:
				ofRect( mCtrX, mCtrY, mCtrWidth, mCtrHeight );
				break;
		}
	glPopMatrix();
}

// ----------------------------------------------