#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxMidi.h"
#include "ofxVectorGraphics.h"

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	ofxMidiOut midiOut;
	ofxVectorGraphics output;

	// Weisse Tasten
	void searchC();
	void searchD();
	void searchE();
	void searchF();
	void searchG();
	void searchA();
	void searchH();
	void searchC2();
	void searchD2();
	void searchE2();

	bool Cgefunden;
	bool Dgefunden;
	bool Egefunden;
	bool Fgefunden;
	bool Ggefunden;
	bool Agefunden;
	bool Hgefunden;
	bool C2gefunden;
	bool D2gefunden;
	bool E2gefunden;

	bool playC;
	bool playD;
	bool playE;
	bool playF;
	bool playG;
	bool playA;
	bool playH;
	bool playC2;
	bool playD2;
	bool playE2;

	// Schwarze Tasten
	void searchCis();
	void searchDis();
	void searchFis();
	void searchGis();
	void searchAis();
	void searchCis2();
	void searchDis2();

	bool Cisgefunden;
	bool Disgefunden;
	bool Fisgefunden;
	bool Gisgefunden;
	bool Aisgefunden;
	bool Cis2gefunden;
	bool Dis2gefunden;

	bool playCis;
	bool playDis;
	bool playFis;
	bool playGis;
	bool playAis;
	bool playCis2;
	bool playDis2;

	// Parameter
	int aufloesungX = 320;
	int aufloesungY = 240;

	int whiteKeyWidth = 32;
	int blackKeyWidth = 16;
	int whiteKeyLength = 160;
	int blackKeyLength = 100;
	int startX = 360;
	int startY = 20;

	// Pixelwert für Empfindlichkeit der Hintergrundsubtraktion
	int pixel = 50;

	// toter Bereich, in dem der Finger erkannt wird, aber keinen Ton spielt
	int deadAreaWhite = 80;
	int deadAreaBlack = 140;

	// int werte um Farbe der Tasten bei berührung zu verändern
	int farbe = 255;
	int farbDiff = 200;
	int transparency;
	bool transpBool;

	// int Startwerte für das Piano
	int xred = 360;
	int xred2 = 384;

	// int Werte zur berechnung der Position der einzelnen Tasten
	int reihenfolge = 0;
	int reihenfolge2 = 0;

	// Methoden für das Piano
	void drawRectPiano();
	void drawRectRedPiano();
	void drawRectRedPiano2();
	void noteAusgeben();

	ofVideoGrabber 			cam;
	ofxCvColorImage			colorImg;
	ofxCvGrayscaleImage 	grayImage;
	ofxCvGrayscaleImage 	grayBg;
	ofxCvGrayscaleImage 	grayDiff;
	ofxCvContourFinder 		contourFinder;

	int 					threshold;
	bool					bLearnBackground;
};
