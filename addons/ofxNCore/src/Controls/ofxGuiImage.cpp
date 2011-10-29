//! ofxGuiImage.cpp
/*!
*  
*
*  Created by Yishi Guo on 06/10/2011.
*  Refactored by Anatoly
*  Copyright 2011 NUI Group. All rights reserved.
*
*/

#include "ofxGuiImage.h"

ofxGuiImage::ofxGuiImage() 
{
	bImageSet = false;
	pCam = NULL;
	pCvImage = new ofxCvGrayscaleImage();
	bDrawInfo =false;
	bCanDrawInfo = false;
	bCamSelected = false;
	strSelectedText = "Active";
	pImage = NULL;
}

ofxGuiImage::~ofxGuiImage() 
{
	if (pCvImage!=NULL)
	{
		delete pCvImage;
		pCvImage = NULL;
	}
	if (pImage != NULL)
	{
		free(pImage);
		pImage = NULL;
	}
}

void ofxGuiImage::init( int id, string name, int x, int y, int width, int height, unsigned char* pImage ) {

	mParamId			= id;
	mParamName			= name;
	mObjX				= x;
	mObjY				= y;
	mObjWidth			= width;
	mObjHeight			= height;
	setControlRegion( 0, 0, width, height );
}

void ofxGuiImage::setImage( unsigned char* pImage, int width, int height ) 
{
	if ( pImage != NULL ) 
	{
		this->pImage = pImage;
		bImageSet = true;
		mCamWidth = width;
		mCamHeight = height;
		if ( !pCvImage->bAllocated ) 
			pCvImage->allocate( mCamWidth, mCamHeight );
		pCvImage->setFromPixels( this->pImage, this->mCamWidth, this->mCamHeight );
		pCvImage->setUseTexture( false );
	}
}

void ofxGuiImage::setBlank( bool bBlank ) 
{
	if ( bBlank ) 
	{
		bImageSet = false;
		setCanDrawInfo( false );
	}
}

void ofxGuiImage::setCamera( ofxCameraBase *cam ) 
{
	if ( cam != NULL ) 
	{
		this->pCam = cam;
		unsigned char pixelMode;
		pCam->getCameraSize( &mCamWidth, &mCamHeight, &mCamDepth,&pixelMode );
		if (pImage!=NULL)
			free(pImage);
		pImage = (unsigned char*)malloc(mCamWidth*mCamHeight*sizeof(unsigned char));
		pCam->getCameraFrame(pImage);
		setImage( pImage, mCamWidth, mCamHeight );
		setCanDrawInfo( true );
	} 
	else 
	{
		setBlank( true );
	}
}

void ofxGuiImage::setTitle( string name ) 
{
	mParamName = name;
}

void ofxGuiImage::setXYPos( int x, int y ) 
{
	this->mObjX = x;
	this->mObjY = y;
}

void ofxGuiImage::setDrawInfo( bool draw ) 
{
	bDrawInfo = draw;
}

void ofxGuiImage::setCamSelected( bool selected ) 
{
	bCamSelected = selected;
}

ofxCameraBase* ofxGuiImage::getCamera() 
{
	return pCam;
}

bool ofxGuiImage::update( int id, int task, void* data, int length ) 
{
	bool handled = false;
	return ( id == mParamId );
}

void ofxGuiImage::draw() 
{
	glPushMatrix();
	glTranslatef( mObjX, mObjY, 0.0f );
	ofFill();
	if (bImageSet) 
		drawImage();
	else 
		drawBlank();
	if ( mParamName != "" ) 
		drawHighlightParamString( PARAM_TEXT_OFFSET_X, .0f, mParamName, false );
	if ( bDrawInfo && bCanDrawInfo && pCam != NULL )
		drawInfo();
	if (bCamSelected)
		drawSelectedText();
	glPopMatrix();
}

bool ofxGuiImage::mouseDragged( int x, int y, int button ) 
{
	return mMouseIsDown;
}

bool ofxGuiImage::mousePressed( int x, int y, int button ) 
{
	return mMouseIsDown;
}

bool ofxGuiImage::mouseReleased( int x, int y, int button ) 
{
	return mMouseIsDown;
}

void ofxGuiImage::drawBlank() {
	ofFill();
	glColor4f( 0.5f, 0.5f, 0.5f, 1 );
	ofRect( mCtrX, mCtrY, mCtrWidth, mCtrHeight );
}

void ofxGuiImage::drawImage() 
{
	if ( bImageSet ) 
	{
		if ( pCam != NULL ) 
		{
			pCam->getCameraFrame(pImage);
			setImage( pImage, mCamWidth, mCamHeight );
		}
		if ( pCvImage != NULL ) 
		{
			pCvImage->draw( mCtrX, mCtrY, mCtrWidth, mCtrHeight );
		}
	}
}

void ofxGuiImage::drawString( int x, int y, string str, bool center, ofRGBA color ) 
{
	glColor4f( color.r, color.g, color.b, color.a );
	if (center) 
		x -= roundInt( mGlobals->mParamFont.stringWidth( str ) / 2.0f );
	else 
		x += mGlobals->mParamFontXOffset;
	y += mGlobals->mParamFontYOffset;
	mGlobals->mParamFont.drawString( str, x, y );
}

void ofxGuiImage::drawInfo() 
{
	int i = 0;
	int textHeight = mGlobals->mParamFont.stringHeight( "foo" );
	glColor4f( 0.9f, 0.9f, 0.9f, 1.0f );
	drawHighlightParamString( PARAM_TEXT_OFFSET_X, mCtrHeight - PARAM_TEXT_OFFSET_X - ++i * textHeight, "GUID: " + GUIDToString( pCam->getCameraGUID()), false );
	drawHighlightParamString( PARAM_TEXT_OFFSET_X, mCtrHeight - PARAM_TEXT_OFFSET_X - ++i * textHeight, "Camera resolution: " + ofToString( (int)mCamWidth ) + "x" + ofToString( (int)mCamHeight ), false );
	drawHighlightParamString( PARAM_TEXT_OFFSET_X, mCtrHeight - PARAM_TEXT_OFFSET_X - ++i * textHeight, "Type: " + pCam->getBaseCameraTypeName(), false );
}

void ofxGuiImage::drawSelectedText() 
{
	int textWidth = mGlobals->mParamFont.stringWidth( strSelectedText );
	int textHeight = mGlobals->mParamFont.stringHeight( strSelectedText );
	color.r = .0f;	color.g = .0f;	color.b = .0f;	color.a = 1.0f;
	drawString( (mCtrWidth - textWidth) / 2 + 1, (mCtrHeight - textHeight) / 2 + 1, strSelectedText, false, color );
	color.r = 1.0f;	color.g = .0f;	color.b = .0f;	color.a = 1.0f;
	drawString( (mCtrWidth - textWidth) / 2, (mCtrHeight - textHeight) / 2, strSelectedText, false, color );
}

void ofxGuiImage::setCanDrawInfo( bool draw ) 
{
	bCanDrawInfo = draw;
}

