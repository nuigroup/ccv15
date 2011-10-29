#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){

}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){

}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){

}

//--------------------------------------------------------------
void testApp::keyReleased  (int key){

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
void testApp::mouseReleased(){

}

/*****************************************************************************
 *	TOUCH EVENTS
 *****************************************************************************/
void testApp::TouchDown( ofxTBetaCvBlob b)
{
    ofSetColor(0xFF0000);
    ofFill();
	ofEllipse( b.centroid.x * ofGetWidth(), b.centroid.y * ofGetHeight(), 10, 10);
}

void testApp::TouchUp( ofxTBetaCvBlob b)
{

    ofSetColor(0x0FFFFF);
    ofFill();
	ofEllipse( b.centroid.x * ofGetWidth(), b.centroid.y * ofGetHeight(), 10, 10);
}

void testApp::TouchMoved( ofxTBetaCvBlob b)
{
	ofSetColor(0xFFFFFF);
    ofFill();
	ofEllipse( b.centroid.x * ofGetWidth(), b.centroid.y * ofGetHeight(), 10, 10);

}


