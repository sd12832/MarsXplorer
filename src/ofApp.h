#pragma once

#include "ofMain.h"
#include  "ofxAssimpModelLoader.h"

// A type for the
typedef enum { CamMoveForward, CamMoveBackward } CamMoveDirection;
typedef enum { CamRotateRight, CamRotateLeft, CamRotateUp, CamRotateDown}
                                CamRotateDirection;

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		void drawAxis(ofVec3f worldPos, int option);
		void initLightingAndMaterials();
		void savePicture();
		void toggleWireframeMode();
		void togglePointsDisplay();
		void setCameraTarget();
		void doPointSelection();
		void moveCamera(CamMoveDirection dir, float step);
		void rotateCamera(CamRotateDirection dir, float step);
		void teleportCamera();
        void teleportRover();
        void objSelect();
        bool mouseIntersectPlane(ofVec3f planePoint, ofVec3f planeNorm, ofVec3f &point);
        bool rayIntersectPlane(ofVec3f rayPoint, ofVec3f raydir, ofVec3f planePoint, ofVec3f planeNorm, ofVec3f &point);

		ofEasyCam cam;
		ofxAssimpModelLoader mars;
        ofxAssimpModelLoader rover;
		ofLight light;

        bool terrainSelect; 
		bool bAltKeyDown;
		bool bCtrlKeyDown;
		bool bWireframe;
		bool bDisplayPoints;
		bool bPointSelected;
		bool bMoveCameraForward;
		bool bMoveCameraBackward;
		bool bRotateCameraLeft;
		bool bRotateCameraRight;
		bool bTeleport;
        bool bRover;
        bool roverSelect;
        bool bRoverTeleport;
        bool bMoveX;
        bool bMoveY;
        bool bMoveZ;
        bool roverDrag;
		ofVec3f selectedPoint;
		ofVec3f teleportStartPoint;
        ofVec3f roverStartPoint;
        ofVec3f roverPoint;
        ofConePrimitive cone; 

		const float selectionRange = 4.0;
};
