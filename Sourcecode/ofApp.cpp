#include "ofMain.h"
#include "ofApp.h"
#include "ofxMidi.h"

//--------------------------------------------------------------

void ofApp::setup() {
	cam.listDevices();
	cam.setDeviceID(0);
	cam.setVerbose(true);
	cam.setup(aufloesungX, aufloesungY);

	bLearnBackground = true;
	threshold = 25;
	midiOut.openPort(0);
}

//--------------------------------------------------------------

void ofApp::update() {
	cam.update();
	ofBackground(255, 255, 255);

	bool bNewFrame = false;
	bNewFrame = cam.isFrameNew();

	if (bNewFrame) {
		colorImg.setFromPixels(cam.getPixels());
		grayImage = colorImg;

		if (bLearnBackground == true) {
			grayBg = grayImage;
			bLearnBackground = false;
		}

		grayDiff.absDiff(grayBg, grayImage);
		grayDiff.threshold(threshold);

		// Sucht nach Veränderung in den einzelnen Pixel Bereichen
		searchC();
		searchD();
		searchE();
		searchF();
		searchG();
		searchA();
		searchH();
		searchC2();
		searchD2();
		searchE2();

		searchCis();
		searchDis();
		searchFis();
		searchGis();
		searchAis();
		searchCis2();
		searchDis2();

		grayDiff.mirror(true, true);
		colorImg.mirror(true, true);
	}
}

//--------------------------------------------------------------

void ofApp::draw() {

	// Farbbild, Graubild, Fotobild, Differenzbild mit Position mit Koordinaten
	//colorImg.draw(20, 20);
	grayDiff.draw(360, 20);
	//grayImage.draw(360, 20);
	//grayBg.draw(20, 280);


	// Schrift
	stringstream reportStr;
	reportStr << "Hintergrundsubtraktion" << endl
		<< "Leerzeichen um Bild aufzunehmen" << endl
		<< "threshold " << threshold << " (press: +/-)" << endl;
	ofDrawBitmapString(reportStr.str(), 360, 280);

	drawRectPiano();
	drawRectRedPiano();
	drawRectRedPiano2();
}

void ofApp::drawRectPiano() {
	int x = startX;

	//weisse tasten
	for (int i = 0; i < 10; i++) {
		ofSetColor(255, 255, 255, 255); //letze Zahl -> transparency
		ofFill();
		ofDrawRectangle(x, 20, whiteKeyWidth, whiteKeyLength);
		ofNoFill();
		ofSetHexColor(0x000000);
		ofDrawRectangle(x, 20, whiteKeyWidth, whiteKeyLength);
		ofSetHexColor(0xCCCCCC);
		x = x + whiteKeyWidth;
	}
	//schwarze tasten
	int a = startX + (whiteKeyWidth - (blackKeyWidth/2));
	for (int i = 0; i < 7; i++) {
		if (i == 2 || i == 5) {
			a = a + whiteKeyWidth;
		}
			
		ofSetColor(0, 0, 0, 255); //letze Zahl -> transparency
		ofFill();
		output.rect(a, 20, blackKeyWidth, blackKeyLength);
		a = a + whiteKeyWidth;
	}
}

void ofApp::drawRectRedPiano() {
	transparency = 0;
	transpBool = false;

	 if (bool transpBool = true) {
		transparency = 255;
	}
	else { 
		transparency = 0;
	}
	
	 if (reihenfolge > 0) {
		 xred = 328 + (reihenfolge * 32);
		 ofSetColor(255, 55, 55, transparency); //letze Zahl -> transparency
		 ofFill();
		 ofDrawRectangle(xred, 20, whiteKeyWidth, whiteKeyLength);
		 ofSetColor(255, 255, 255, transparency); //letze Zahl -> transparency
	 }
	 else  {
		 xred = 328 + (reihenfolge * 32);
		 ofSetColor(255, 55, 55, 0); //letze Zahl -> transparency
		 ofFill();
		 ofDrawRectangle(xred, 20, whiteKeyWidth, whiteKeyLength);
		 ofSetColor(255, 255, 255, transparency); //letze Zahl -> transparency
	 }
}

void ofApp::drawRectRedPiano2() {
	transparency = 0;
	transpBool = false;

	if (bool transpBool = true) {
		transparency = 255;
	}
	else {
		transparency = 0;
	}

	if (reihenfolge2 > 0) {
		xred2 = 352 + (reihenfolge2 * 32);
		ofSetColor(255, 55, 55, transparency); //letze Zahl -> transparency
		ofFill();
		ofDrawRectangle(xred2, 20, blackKeyWidth, 100);
		ofSetColor(255, 255, 255, transparency); //letze Zahl -> transparency
	}
	else {
		xred2 = 352 + (reihenfolge2 * 32);
		ofSetColor(255, 55, 55, 0); //letze Zahl -> transparency
		ofFill();
		ofDrawRectangle(xred2, 20, blackKeyWidth, 100);
		ofSetColor(255, 255, 255, transparency); //letze Zahl -> transparency
	}
}

//--------------------------------------------------------------

void ofApp::keyPressed(int key) {
	switch (key) {
	case ' ':
		bLearnBackground = true;
		break;
	case '+':
		threshold++;
		if (threshold > 255) threshold = 255;
		break;
	case '-':
		threshold--;
		if (threshold < 0) threshold = 0;
		break;
	}
}

void ofApp::searchDis2() {
	int x;
	int y;
	int i = 0;

	//Abtasten eines Feldes nach weißen Pixeln
	for (x = 24; x < 40; x++) {
		for (y = deadAreaBlack; y < 240; y++) {
			unsigned char pixel = grayDiff.getPixels()[x + y * grayDiff.width];
			if (pixel > 0) {
				i++;
			}
		}
	}

	// wenn die Pixelanzahl der weißen Fläche größer als int pixel ist 
	// wird Cgefunden auf true gesetzt
	Dis2gefunden = (i > pixel);

	//Prüfen Ton bereits gespielt wird
	if (Dis2gefunden && !playDis2) {
		midiOut.sendNoteOn(1, 75, 127);
		playDis2 = true;
		transpBool = true;
		farbe = farbe - farbDiff;
		reihenfolge2 = 9;
	}

	//ausschalten des Tones wenn der Finger das Feld verlässt
	if (!Dis2gefunden && playDis2) {
		midiOut.sendNoteOff(1, 75, 0);
		playDis2 = false;
		transpBool = false;
		farbe = farbe + farbDiff;
		reihenfolge2 = 0;
	}
}

void ofApp::searchCis2() {
	int x;
	int y;
	int i = 0;

	//Abtasten eines Feldes nach weißen Pixeln
	for (x = 56; x < 72; x++) {
		for (y = deadAreaBlack; y < 240; y++) {
			unsigned char pixel = grayDiff.getPixels()[x + y * grayDiff.width];
			if (pixel > 0) {
				i++;
			}
		}
	}

	// wenn die Pixelanzahl der weißen Fläche größer als int pixel ist 
	// wird Cgefunden auf true gesetzt
	Cis2gefunden = (i > pixel);

	//Prüfen Ton bereits gespielt wird
	if (Cis2gefunden && !playCis2) {
		midiOut.sendNoteOn(1, 73, 127);
		playCis2 = true;
		transpBool = true;
		farbe = farbe - farbDiff;
		reihenfolge2 = 8;
	}

	//ausschalten des Tones wenn der Finger das Feld verlässt
	if (!Cis2gefunden && playCis2) {
		midiOut.sendNoteOff(1, 73, 0);
		playCis2 = false;
		transpBool = false;
		farbe = farbe + farbDiff;
		reihenfolge2 = 0;
	}
}

void ofApp::searchAis() {
	int x;
	int y;
	int i = 0;

	//Abtasten eines Feldes nach weißen Pixeln
	for (x = 120; x < 136; x++) {
		for (y = deadAreaBlack; y < 240; y++) {
			unsigned char pixel = grayDiff.getPixels()[x + y * grayDiff.width];
			if (pixel > 0) {
				i++;
			}
		}
	}

	// wenn die Pixelanzahl der weißen Fläche größer als int pixel ist 
	// wird Cgefunden auf true gesetzt
	Aisgefunden = (i > pixel);

	//Prüfen Ton bereits gespielt wird
	if (Aisgefunden && !playAis) {
		midiOut.sendNoteOn(1, 70, 127);
		playAis = true;
		transpBool = true;
		farbe = farbe - farbDiff;
		reihenfolge2 = 6;
	}

	//ausschalten des Tones wenn der Finger das Feld verlässt
	if (!Aisgefunden && playAis) {
		midiOut.sendNoteOff(1, 70, 0);
		playAis = false;
		transpBool = false;
		farbe = farbe + farbDiff;
		reihenfolge2 = 0;
	}
}

void ofApp::searchGis() {
	int x;
	int y;
	int i = 0;

	//Abtasten eines Feldes nach weißen Pixeln
	for (x = 152; x < 168; x++) {
		for (y = deadAreaBlack; y < 240; y++) {
			unsigned char pixel = grayDiff.getPixels()[x + y * grayDiff.width];
			if (pixel > 0) {
				i++;
			}
		}
	}

	// wenn die Pixelanzahl der weißen Fläche größer als int pixel ist 
	// wird Cgefunden auf true gesetzt
	Gisgefunden = (i > pixel);
	//Prüfen Ton bereits gespielt wird
	if (Gisgefunden && !playGis) {
		midiOut.sendNoteOn(1, 68, 127);
		playGis = true;
		transpBool = true;
		farbe = farbe - farbDiff;
		reihenfolge2 = 5;
	}

	//ausschalten des Tones wenn der Finger das Feld verlässt
	if (!Gisgefunden && playGis) {
		midiOut.sendNoteOff(1, 68, 0);
		playGis = false;
		transpBool = false;
		farbe = farbe + farbDiff;
		reihenfolge2 = 0;
	}
}

void ofApp::searchFis() {
	int x;
	int y;
	int i = 0;

	//Abtasten eines Feldes nach weißen Pixeln
	for (x = 184; x < 200; x++) {
		for (y = deadAreaBlack; y < 240; y++) {
			unsigned char pixel = grayDiff.getPixels()[x + y * grayDiff.width];
			if (pixel > 0) {
				i++;
			}
		}
	}

	// wenn die Pixelanzahl der weißen Fläche größer als int pixel ist 
	// wird Cgefunden auf true gesetzt
	Fisgefunden = (i > pixel);

	//Prüfen Ton bereits gespielt wird
	if (Fisgefunden && !playFis) {
		midiOut.sendNoteOn(1, 66, 127);
		playFis = true;
		transpBool = true;
		farbe = farbe - farbDiff;
		reihenfolge2 = 4;
	}

	//ausschalten des Tones wenn der Finger das Feld verlässt
	if (!Fisgefunden && playFis) {
		midiOut.sendNoteOff(1, 66, 0);
		playFis = false;
		transpBool = false;
		farbe = farbe + farbDiff;
		reihenfolge2 = 0;
	}
}

void ofApp::searchDis() {
	int x;
	int y;
	int i = 0;

	//Abtasten eines Feldes nach weißen Pixeln
	for (x = 248; x < 264; x++) {
		for (y = deadAreaBlack; y < 240; y++) {
			unsigned char pixel = grayDiff.getPixels()[x + y * grayDiff.width];
			if (pixel > 0) {
				i++;
			}
		}
	}

	// wenn die Pixelanzahl der weißen Fläche größer als int pixel ist 
	// wird Cgefunden auf true gesetzt
	Disgefunden = (i > pixel);

	//Prüfen Ton bereits gespielt wird
	if (Disgefunden && !playDis) {
		midiOut.sendNoteOn(1, 63, 127);
		playDis = true;
		transpBool = true;
		farbe = farbe - farbDiff;
		reihenfolge2 = 2;
	}

	//ausschalten des Tones wenn der Finger das Feld verlässt
	if (!Disgefunden && playDis) {
		midiOut.sendNoteOff(1, 63, 0);
		playDis = false;
		transpBool = false;
		farbe = farbe + farbDiff;
		reihenfolge2 = 0;
	}
}

void ofApp::searchCis() {
	int x;
	int y;
	int i = 0;

	//Abtasten eines Feldes nach weißen Pixeln
	for (x = 280; x < 296; x++) {
		for (y = deadAreaBlack; y < 240; y++) {
			unsigned char pixel = grayDiff.getPixels()[x + y * grayDiff.width];
			if (pixel > 0) {
				i++;
			}
		}
	}

	// wenn die Pixelanzahl der weißen Fläche größer als int pixel ist 
	// wird Cgefunden auf true gesetzt
	Cisgefunden = (i > pixel);

	//Prüfen Ton bereits gespielt wird
	if (Cisgefunden && !playCis) {
		midiOut.sendNoteOn(1, 61, 127);
		playCis = true;
		transpBool = true;
		farbe = farbe - farbDiff;
		reihenfolge2 = 1;
	}

	//ausschalten des Tones wenn der Finger das Feld verlässt
	if (!Cisgefunden && playCis) {
		midiOut.sendNoteOff(1, 61, 0);
		playCis = false;
		transpBool = false;
		farbe = farbe + farbDiff;
		reihenfolge2 = 0;
	}
}

void ofApp::searchE2() {
	int x;
	int y;
	int i = 0;

	//Abtasten eines Feldes nach weißen Pixeln
	for (x = 0; x < 24; x++) {
		for (y = deadAreaWhite; y < 240; y++) {
			unsigned char pixel = grayDiff.getPixels()[x + y * grayDiff.width];
			if (pixel > 0) {
				i++;
			}
		}
	}

	// wenn die Pixelanzahl der weißen Fläche größer als int pixel ist 
	// wird Cgefunden auf true gesetzt
	E2gefunden = (i > pixel);

	//Prüfen Ton bereits gespielt wird
	if (E2gefunden && !playE2) {
		midiOut.sendNoteOn(1, 76, 127);
		playE2 = true;
		transpBool = true;
		farbe = farbe - farbDiff;
		reihenfolge = 10;	
	}

	//ausschalten des Tones wenn der Finger das Feld verlässt
	if (!E2gefunden && playE2) {
		midiOut.sendNoteOff(1, 76, 0);
		playE2 = false;
		transpBool = false;
		farbe = farbe + farbDiff;
		reihenfolge = 0;
	}
}

void ofApp::searchD2() {
	int x;
	int y;
	int i = 0;

	//Abtasten eines Feldes nach weißen Pixeln
	for (x = 40; x < 58; x++) {
		for (y = deadAreaWhite; y < 240; y++) {
			unsigned char pixel = grayDiff.getPixels()[x + y * grayDiff.width];
			if (pixel > 0) {
				i++;
			}
		}
	}

	// wenn die Pixelanzahl der weißen Fläche größer als int pixel ist 
	// wird Dgefunden auf true gesetzt
	D2gefunden = (i > pixel);

	//Prüfen Ton bereits gespielt wird
	if (D2gefunden && !playD2) {
		midiOut.sendNoteOn(1, 74, 127);
		playD2 = true;
		transpBool = true;
		farbe = farbe - farbDiff;
		reihenfolge = 9;
	}

	//ausschalten des Tones wenn der Finger das Feld verlässt
	if (!D2gefunden && playD2) {
		midiOut.sendNoteOff(1, 74, 0);
		playD2 = false;
		transpBool = false;
		farbe = farbe + farbDiff;
		reihenfolge = 0;
	}
}

void ofApp::searchC2() {
	int x;
	int y;
	int i = 0;

	//Abtasten eines Feldes nach weißen Pixeln
	for (x = 72; x < 96; x++) {
		for (y = deadAreaWhite; y < 240; y++) {
			unsigned char pixel = grayDiff.getPixels()[x + y * grayDiff.width];
			if (pixel > 0) {
				i++;
			}
		}
	}

	// wenn die Pixelanzahl der weißen Fläche größer als int pixel ist 
	// wird Egefunden auf true gesetzt
	C2gefunden = (i > pixel);
	//Prüfen Ton bereits gespielt wird
	if (C2gefunden && !playC2) {
		midiOut.sendNoteOn(1, 72, 127);
		playC2 = true;
		transpBool = true;
		farbe = farbe - farbDiff;
		reihenfolge = 8;
	}

	//ausschalten des Tones wenn der Finger das Feld verlässt
	if (!C2gefunden && playC2) {
		midiOut.sendNoteOff(1, 72, 0);
		playC2 = false;
		transpBool = false;
		farbe = farbe + farbDiff;
		reihenfolge = 0;
	}
}

void ofApp::searchH() {
	int x;
	int y;
	int i = 0;

	//Abtasten eines Feldes nach weißen Pixeln
	for (x = 96; x < 120; x++) {
		for (y = deadAreaWhite; y < 240; y++) {
			unsigned char pixel = grayDiff.getPixels()[x + y * grayDiff.width];
			if (pixel > 0) {
				i++;
			}
		}
	}

	// wenn die Pixelanzahl der weißen Fläche größer als int pixel ist 
	// wird Fgefunden auf true gesetzt
	Hgefunden = (i > pixel);

	//Prüfen Ton bereits gespielt wird
	if (Hgefunden && !playH) {
		midiOut.sendNoteOn(1, 71, 127);
		playH = true;
		transpBool = true;
		farbe = farbe - farbDiff;
		reihenfolge = 7;
	}

	//ausschalten des Tones wenn der Finger das Feld verlässt
	if (!Hgefunden && playH) {
		midiOut.sendNoteOff(1, 71, 0);
		playH = false;
		transpBool = false;
		farbe = farbe + farbDiff;
		reihenfolge = 0;
	}
}

void ofApp::searchA() {
	int x;
	int y;
	int i = 0;

	//Abtasten eines Feldes nach weißen Pixeln
	for (x = 136; x < 152; x++) {
		for (y = deadAreaWhite; y < 240; y++) {
			unsigned char pixel = grayDiff.getPixels()[x + y * grayDiff.width];
			if (pixel > 0) {
				i++;
			}
		}
	}

	// wenn die Pixelanzahl der weißen Fläche größer als int pixel ist 
	// wird Ggefunden auf true gesetzt
	Agefunden = (i > pixel);

	//Prüfen Ton bereits gespielt wird
	if (Agefunden && !playA) {
		midiOut.sendNoteOn(1, 69, 127);
		playA = true;
		transpBool = true;
		farbe = farbe - farbDiff;
		reihenfolge = 6;
	}

	//ausschalten des Tones wenn der Finger das Feld verlässt
	if (!Agefunden && playA) {
		midiOut.sendNoteOff(1, 69, 0);
		playA = false;
		transpBool = false;
		farbe = farbe + farbDiff;
		reihenfolge = 0;
	}
}

void ofApp::searchG() {
	int x;
	int y;
	int i = 0;

	//Abtasten eines Feldes nach weißen Pixeln
	for (x = 168; x < 184; x++) {
		for (y = deadAreaWhite; y < 240; y++) {
			unsigned char pixel = grayDiff.getPixels()[x + y * grayDiff.width];
			if (pixel > 0) {
				i++;
			}
		}
	}

	// wenn die Pixelanzahl der weißen Fläche größer als int pixel ist 
	// wird Agefunden auf true gesetzt
	Ggefunden = (i > pixel);

	//Prüfen Ton bereits gespielt wird
	if (Ggefunden && !playG) {
		midiOut.sendNoteOn(1, 67, 127);
		playG = true;
		transpBool = true;
		farbe = farbe - farbDiff;
		reihenfolge = 5;
	}

	//ausschalten des Tones wenn der Finger das Feld verlässt
	if (!Ggefunden && playG) {
		midiOut.sendNoteOff(1, 67, 0);
		playG = false;
		transpBool = false;
		farbe = farbe + farbDiff;
		reihenfolge = 0;
	}
}

void ofApp::searchF() {
	int x;
	int y;
	int i = 0;

	//Abtasten eines Feldes nach weißen Pixeln
	for (x = 200; x < 224; x++) {
		for (y = deadAreaWhite; y < 240; y++) {
			unsigned char pixel = grayDiff.getPixels()[x + y * grayDiff.width];
			if (pixel > 0) {
				i++;
			}
		}
	}

	// wenn die Pixelanzahl der weißen Fläche größer als int pixel ist 
	// wird Hgefunden auf true gesetzt
	Fgefunden = (i > pixel);

	//Prüfen Ton bereits gespielt wird
	if (Fgefunden && !playF) {
		midiOut.sendNoteOn(1, 65, 127);
		playF = true;
		transpBool = true;
		farbe = farbe - farbDiff;
		reihenfolge = 4;
	}

	//ausschalten des Tones wenn der Finger das Feld verlässt
	if (!Fgefunden && playF) {
		midiOut.sendNoteOff(1, 65, 0);
		playF = false;
		transpBool = false;
		farbe = farbe + farbDiff;
		reihenfolge = 0;
	}


}

void ofApp::searchE() {
	int x;
	int y;
	int i = 0;

	//Abtasten eines Feldes nach weißen Pixeln
	for (x = 224; x < 248; x++) {
		for (y = deadAreaWhite; y < 240; y++) {
			unsigned char pixel = grayDiff.getPixels()[x + y * grayDiff.width];
			if (pixel > 0) {
				i++;
			}
		}
	}

	// wenn die Pixelanzahl der weißen Fläche größer als int pixel ist 
	// wird Hgefunden auf true gesetzt
	Egefunden = (i > pixel);

	//Prüfen Ton bereits gespielt wird
	if (Egefunden && !playE) {
		midiOut.sendNoteOn(1, 64, 127);
		playE = true;
		transpBool = true;
		farbe = farbe - farbDiff;
		reihenfolge = 3;
	}

	//ausschalten des Tones wenn der Finger das Feld verlässt
	if (!Egefunden && playE) {
		midiOut.sendNoteOff(1, 64, 0);
		playE = false;
		transpBool = false;
		farbe = farbe + farbDiff;
		reihenfolge = 0;
	}


}

void ofApp::searchD() {
	int x;
	int y;
	int i = 0;

	for (x = 264; x < 280; x++) {
		for (y = deadAreaWhite; y < 240; y++) {
			unsigned char pixel = grayDiff.getPixels()[x + y * grayDiff.width];
			if (pixel > 0) {
				i++;
			}
		}
	}

	Dgefunden = (i > pixel);

	if (Dgefunden && !playD) {
		midiOut.sendNoteOn(1, 62, 127);
		playD = true;
		transpBool = true;
		farbe = farbe - farbDiff;
		reihenfolge = 2;
	}

	if (!Dgefunden && playD) {
		midiOut.sendNoteOff(1, 62, 0);
		playD = false;
		transpBool = false;
		farbe = farbe + farbDiff;
		reihenfolge = 0;
	}



}

void ofApp::searchC() {
	int x;
	int y;
	int i = 0;

	for (x = 296; x < 320; x++) {
		for (y = deadAreaWhite; y < 240; y++) {
			unsigned char pixel = grayDiff.getPixels()[x + y * grayDiff.width];
			if (pixel > 0) {
				i++;
			}
		}
	}
	Cgefunden = (i > pixel);

	if (Cgefunden && !playC) {
		midiOut.sendNoteOn(1, 60, 127);
		playC = true;
		transpBool = true;
		farbe = farbe - farbDiff;
		reihenfolge = 1;
	}

	if (!Cgefunden && playC) {
		midiOut.sendNoteOff(1, 60, 0);
		playC = false;
		transpBool = false;
		farbe = farbe + farbDiff;
		reihenfolge = 0;
	}
}













void ofApp::noteAusgeben() {
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {
}
