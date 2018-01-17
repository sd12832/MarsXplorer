
//--------------------------------------------------------------
//
//  CS235 - User Interface Design - Kevin M. Smith
//
//  Assignment 5 -  Mars HiRise Project - startup scene
// 
//  This is an openFrameworks 3D scene that includes an EasyCam
//  and example 3D geometry which I have reconstructed from Mars
//  HiRis photographs taken the Mars Reconnaisance Orbiter
//
//  You will use this source file (and include file) as a starting point
//  to implement assignment 5  (Parts I and II)
//
//  Please do not modify any of the keymappings.  I would like 
//  the input interface to be the same for each student's 
//  work.  Please also add your name/date below.

//
//  Student Name:   < Sharan Duggirala >
//  Date: <November 25 2017>


#include "ofApp.h"
#include <cmath>

//--------------------------------------------------------------
// setup scene, lighting, state and load geometry
//
void ofApp::setup(){
    // Booleans necessary for the functioning of the program
    roverSelect = false;
    terrainSelect = true;
	bWireframe = false;
	bDisplayPoints = false;
	bAltKeyDown = false;
	bCtrlKeyDown = false;
	bPointSelected = false;
    bRover = false;
    bMoveX = false;
    bMoveY = false;
    bMoveZ = false;
    
    // Setup Variables
	ofSetWindowShape(1024, 768);
	cam.setDistance(10);
	cam.setNearClip(.1);
    // Field of View: Approx equivalent to 28mm in 35mm format
	cam.setFov(65.5);
	ofSetVerticalSync(true);
	cam.disableMouseInput();
	ofEnableSmoothing();
	ofEnableDepthTest();

	// setup rudimentary lighting 
	//
	initLightingAndMaterials();

	// load model (but do not scale)
	//
	mars.loadModel("mars-low-v2.obj");
	mars.setScaleNormalization(false);
    
    // Cone Settings that I have abandoned due to not being able
    // to rotate it to the direction of the axes
    
    // cone.set(0.02, 0.05, 100, 100); //
    
}

//--------------------------------------------------------------
// incrementally update scene (animation)
//
void ofApp::update(){
	if (bMoveCameraForward && !bCtrlKeyDown) moveCamera(CamMoveForward, .05);
	else if (bMoveCameraBackward && !bCtrlKeyDown) moveCamera(CamMoveBackward, .05);

	else if (bRotateCameraLeft) rotateCamera(CamRotateLeft, -1.0);   // rotate left/right
	else if (bRotateCameraRight) rotateCamera(CamRotateRight, 1.0);
	else if (bMoveCameraForward && bCtrlKeyDown) rotateCamera(CamRotateUp, -1.0);  // rotate up/down
	else if (bMoveCameraBackward && bCtrlKeyDown) rotateCamera(CamRotateDown, 1.0);
    // Bool to Check Camera Teleportation
	else if (bTeleport) teleportCamera();
    // Bool to Check Rover Teleporatation
    else if (bRoverTeleport) teleportRover();
    
}

//--------------------------------------------------------------
void ofApp::draw(){

//	ofBackgroundGradient(ofColor(20), ofColor(0));   // pick your own backgroujnd
	ofBackground(ofColor::black);

	cam.begin();
	ofPushMatrix();
	if (bWireframe) {                    // wireframe mode  (include axis)
		ofDisableLighting();
		ofSetColor(ofColor::slateGray);
		mars.drawWireframe();
        ofVec3f worldOrigin;
        worldOrigin.x=0;
        worldOrigin.y=0;
        worldOrigin.z=0;
		drawAxis(worldOrigin, 3);
	}
	else {
		ofEnableLighting();              // shaded mode
		mars.drawFaces();
	}
    
    if (bRover) {
        rover.setPosition(roverPoint.x, roverPoint.y, roverPoint.z);
        rover.setScale(0.001, 0.001, 0.001);
        rover.drawFaces();
    }

	if (bDisplayPoints) {                // display points as an option    
		glPointSize(3);
		ofSetColor(ofColor::green);
		mars.drawVertices();
	}

	// highlight selected point (draw sphere around selected point)
	//
	if (bPointSelected) {
		ofSetColor(ofColor::blue);
		ofDrawSphere(selectedPoint, .1);
	}
    
    // If the Terrain is Selected (Always true on Default)
    if (terrainSelect) {
        ofVec3f worldPos;
        worldPos.x = 0;
        worldPos.y = 0;
        worldPos.z = 0;
        drawAxis(worldPos, 0);
    }
    
    if (roverSelect && bRover) {
        ofVec3f worldPos = rover.getPosition();
        // The Vanilla Option
        if (!bMoveX && !bMoveY && !bMoveZ) {
            drawAxis(worldPos,0);
        // X translation
        } else if (bMoveX && !bMoveY && !bMoveZ) {
            drawAxis(worldPos,1);
        // Y translation
        } else if (!bMoveX && bMoveY && !bMoveZ) {
            drawAxis(worldPos,2);
        // Z translation
        } else if (!bMoveX && !bMoveY && bMoveZ) {
            drawAxis(worldPos,3);
        // XY translation
        } else if (bMoveX && bMoveY && !bMoveZ) {
            drawAxis(worldPos,4);
        // YZ translation
        } else if (!bMoveX && bMoveY && bMoveZ) {
            drawAxis(worldPos,5);
        // XZ translation
        } else if (bMoveX && !bMoveY && bMoveZ) {
            drawAxis(worldPos,6);
        }
    }
	
	ofPopMatrix();
	cam.end();
}

// ---------------------

void ofApp::keyPressed(int key) {

	switch (key) {
        case 'C':
        case 'c':
            if (cam.getMouseInputEnabled()) cam.disableMouseInput();
            else cam.enableMouseInput();
            break;
        case 'F':
        case 'f':
            ofToggleFullscreen();
            break;
        case 'H':
        case 'h':
            break;
        case 'r':
            cam.reset();
            break;
        case 's':
            savePicture();
            break;
        case 't':
            setCameraTarget();
            break;
        case 'u':
            break;
        case 'v':
            togglePointsDisplay();
            break;
        case 'V':
            break;
        case 'w':
            toggleWireframeMode();
            break;
        case 'm':
            objSelect();
            break;
        case OF_KEY_ALT:
            cam.enableMouseInput();
            bAltKeyDown = true;
            break;
        case OF_KEY_COMMAND:
            bCtrlKeyDown = true;
            break;
        case OF_KEY_CONTROL:
            bCtrlKeyDown = true;
            break;
        case OF_KEY_SHIFT:
            break;
        case OF_KEY_DEL:
            break;
        case OF_KEY_LEFT:
            bRotateCameraLeft = true;
            break;
        case OF_KEY_RIGHT:
            bRotateCameraRight = true;
            break;
        case OF_KEY_UP:
            bMoveCameraForward = true;
            break;
        case OF_KEY_DOWN:
            bMoveCameraBackward = true;
            break;
        case OF_KEY_F1:
            teleportStartPoint = cam.getPosition();
            bTeleport = true;
            break;
        case OF_KEY_F2:
            roverStartPoint = rover.getPosition();
            bRoverTeleport = true;
            break;
        case 'x':
        case 'X':
            bMoveX = true;
            break;
        case 'y':
        case 'Y':
            bMoveY = true;
            break;
        case 'z':
        case 'Z':
            bMoveZ = true;
            break;
	}
}

void ofApp::toggleWireframeMode() {
	bWireframe = !bWireframe;
}

void ofApp::togglePointsDisplay() {
	bDisplayPoints = !bDisplayPoints;
}

void ofApp::keyReleased(int key) {
	switch (key) {
        case OF_KEY_ALT:
            cam.disableMouseInput();
            bAltKeyDown = false;
            break;
        case OF_KEY_COMMAND:
            bCtrlKeyDown = false;
            break;
        case OF_KEY_CONTROL:
            bCtrlKeyDown = false;
            break;
        case OF_KEY_SHIFT:
            break;
        case OF_KEY_LEFT:
            bRotateCameraLeft = false;
            break;
        case OF_KEY_RIGHT:
            bRotateCameraRight = false;
            break;
        case OF_KEY_UP:
            bMoveCameraForward = false;
            break;
        case OF_KEY_DOWN:
            bMoveCameraBackward = false;
            break;
        case 'x':
        case 'X':
            bMoveX = false;
            break;
        case 'y':
        case 'Y':
            bMoveY = false;
            break;
        case 'z':
        case 'Z':
            bMoveZ = false;
            break;
	}
}



//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    // Use Ray Intersection to figure out the Rover's Plane
    ofVec3f dragPoint;
    if (roverDrag) {
        if ((mouseIntersectPlane(rover.getPosition(),
                                rover.getMesh(0).getNormal(0),
                                dragPoint))) {
            ofVec3f newPoint;
            // Vanilla Translation
            // Vanilla Translation acts a bit weirdly!
            // I am not sure as to why this is the case
            // right now. Maybe translating from mouse
            // intersect coordinates to world coordinates
            // is a bit iffy!
            if (!bMoveX && !bMoveY && !bMoveZ) {
                roverPoint = dragPoint;
            // X translation
            } else if (bMoveX && !bMoveY && !bMoveZ) {
                newPoint.x = dragPoint.x;
                newPoint.y = rover.getPosition().y;
                newPoint.z = rover.getPosition().z;
                roverPoint = newPoint;
            // Y translation
            } else if (!bMoveX && bMoveY && !bMoveZ) {
                newPoint.x = rover.getPosition().x;
                newPoint.y = dragPoint.y;
                newPoint.z = rover.getPosition().z;
                roverPoint = newPoint;
            // Z translation
            } else if (!bMoveX && !bMoveY && bMoveZ) {
                newPoint.x = rover.getPosition().x;
                newPoint.y = rover.getPosition().y;
                newPoint.z = dragPoint.z;
                roverPoint = newPoint;
            // XY translation
            } else if (bMoveX && bMoveY && !bMoveZ) {
                newPoint.x = dragPoint.x;
                newPoint.y = dragPoint.y;
                newPoint.z = rover.getPosition().z;
                roverPoint = newPoint;
            // YZ translation
            } else if (!bMoveX && bMoveY && bMoveZ) {
                newPoint.x = rover.getPosition().x;
                newPoint.y = dragPoint.y;
                newPoint.z = dragPoint.z;
                roverPoint = newPoint;
            // XZ translation
            } else if (bMoveX && !bMoveY && bMoveZ) {
                newPoint.x = dragPoint.x;
                newPoint.y = rover.getPosition().y;
                newPoint.z = dragPoint.z;
                roverPoint = newPoint;
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
    // CASE 1: Point Selection
    // Only happens when the Terrain can be selected and the ALT
    // is not pressed
    if ((bAltKeyDown == false) && (terrainSelect == true)) {
        doPointSelection();
    }
    
    // CASE 2: Rover Selection
    // only happens when the Rover can be selected, rover exists
    // and the ALT is not pressed
    if (!bAltKeyDown && bRover && roverSelect) {
        roverDrag = true;
    }
    
}

//
//  Select Target Point on Terrain
//

void ofApp::doPointSelection() {

    // Code Taken from
    // https://github.com/openframeworks/openFrameworks/blob/master
    // /examples/3d/pointPickerExample/src/ofApp.cpp
    ofMesh mesh = mars.getMesh(0);
    int n = mesh.getNumVertices();
    float nearestDistance = 0;
    ofVec2f nearestVertex;
    int nearestIndex = 0;
    ofVec2f mouse(mouseX, mouseY);
    for(int i = 0; i < n; i++) {
        ofVec3f cur = cam.worldToScreen(mesh.getVertex(i));
        float distance = cur.distance(mouse);
        if(i == 0 || distance < nearestDistance) {
            bPointSelected = true;
            selectedPoint = mesh.getVertex(i);
            nearestDistance = distance;
            nearestVertex = cur;
            nearestIndex = i;
        }
    }
}

// Given an plane (point and normal), return intersection point with mouse ray.
// If there is intersection, return true, else return false;
// Author: KMS
//
bool ofApp::mouseIntersectPlane(ofVec3f planePoint, ofVec3f planeNorm, ofVec3f &point) {
    ofVec2f mouse(mouseX, mouseY);
    ofVec3f rayPoint = cam.screenToWorld(mouse);
    ofVec3f rayDir = rayPoint - cam.getPosition();
    rayDir.normalize();
    return (rayIntersectPlane(rayPoint, rayDir, planePoint, planeNorm, point));
}


//---------------------------------------------------------------
// test if a ray intersects a plane.  If there is an intersection,
// return true and put point of intersection in "point"
// Author: KMS
//

bool ofApp::rayIntersectPlane(ofVec3f rayPoint, ofVec3f raydir, ofVec3f planePoint,
                              ofVec3f planeNorm, ofVec3f &point) {
    
    // if d1 is 0, then the ray is on the plane or there is no intersection
    //
    const float eps = .000000001;
    float d1 = (planePoint - rayPoint).dot(planeNorm);
    if (abs(d1) < eps) return false;
    
    //  if d2 is 0, then the ray is parallel to the plane
    //
    float d2 = raydir.dot(planeNorm);
    if (abs(d2) < eps) return false;
    
    //  compute the intersection point and return it in "point"
    //
    point = (d1 / d2) * raydir + rayPoint;
    return true;
}


// Set the camera to use the selected point as it's new target
void ofApp::setCameraTarget() {
    cam.setTarget(selectedPoint);
}

// Animate camera by moving it to the selected points (using update())
void ofApp::moveCamera(CamMoveDirection dir, float step) {
    
    // Initialize for the Movement
    ofVec3f z = cam.getZAxis();
    z.normalize(); // Normalizing the orthogonal
    ofVec3f camP = cam.getPosition();
    ofVec3f newP;
    
    // Deal with both cases of motion here
    if (dir == CamMoveForward) {
        newP = camP - (step * z);
        cam.setPosition(newP);
    } else if (dir == CamMoveBackward) {
        newP = camP + (step * z);
        cam.setPosition(newP);
    }
    
}

// Rotate Camera Left/Right/Up/Down
void ofApp::rotateCamera(CamRotateDirection dir, float step) {
    if (dir == CamRotateRight) {
        // Camera rotating to the right
        cam.rotate(step*-1, cam.getYAxis());
    } else if (dir == CamRotateLeft) {
        // Camera rotating to the left
        cam.rotate(step*-1, cam.getYAxis());
    } else if (dir == CamRotateUp) {
        // Camera rotatting upwards
        cam.rotate(step*-1, cam.getXAxis());
    } else if (dir == CamRotateDown) {
        // Camera rotating downwards
        cam.rotate(step*-1, cam.getXAxis());
    }
}


//  "Teleport" Camera to Selected Target
void ofApp::teleportCamera() {
    
    // Find distance from start to goal
    float path = selectedPoint.distance(teleportStartPoint);
    // Find distance from current position and goal
    float comp = cam.getPosition().distance(teleportStartPoint);
    
    // Calculate percentage Complete. If it is 0, it is bumped
    // to 0.01
    float percComp = comp/path ;
    if (percComp == 0) percComp += 0.01;
    
    // Move towards the selected point with a sin function
    if (selectedPoint.distance(cam.getPosition()) >= 5) {
        
        // This function below is incorrect since we are
        // not supposed to change the cameras target and
        // then move it
        // moveCamera(CamMoveForward, (sin(PI*percComp)));
        
        // Old position and the orthogonal to the new position
        ofVec3f oP, nP;
        oP = cam.getPosition();
        nP = (selectedPoint - cam.getPosition()).normalize();
        // Now we can apply a sin to each of the functions till the
        // we are at a distance of 5 from the selected point
        cam.setPosition(oP.x + (sin(PI*percComp)*nP.x),
                        oP.y + (sin(PI*percComp)*nP.y),
                        oP.z + (sin(PI*percComp)*nP.z));
    } else {
        bTeleport = false;
    }
}

//  "Teleport" Rover to Selected Target
void ofApp::teleportRover() {
    
    // Make a new selectedPoint that is a few feet above
    // the selected point
    ofVec3f nselectedPoint = selectedPoint;
//    nselectedPoint.y += 0.05;
    
    // Find distance from start to goal
    float path = nselectedPoint.distance(roverStartPoint);
    // Find distance from current position and goal
    float comp = rover.getPosition().distance(roverStartPoint);
    
    // Calculate percentage Complete. If it is 0, it is bumped
    // to 0.01
    float percComp = comp/path;
    if (percComp == 0) percComp += 0.01;
    
    // Move towards the selected point with a sin function
    if (nselectedPoint.distance(rover.getPosition()) >= 0.01) {
        // Old position and the orthogonal to the new position
        ofVec3f oP, nP;
        oP = rover.getPosition();
        nP = (nselectedPoint - rover.getPosition()).normalize();
        roverPoint.x = oP.x + (0.4*(sin(PI*percComp)*nP.x));
        roverPoint.y = oP.y + (0.4*(sin(PI*percComp)*nP.y));
        roverPoint.z = oP.z + (0.4*(sin(PI*percComp)*nP.z));
    } else {
        bRoverTeleport = false;
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    roverDrag = false;
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    bRover = true;
    mouseIntersectPlane(cam.getPosition()+cam.getNearClip()-4,
        (cam.getTarget().getPosition()-cam.getPosition()).normalize(),
        roverPoint);
    rover.loadModel(dragInfo.files[0]);
}

//--------------------------------------------------------------
// setup basic ambient lighting in GL  (for now, enable just 1 light)
//
void ofApp::initLightingAndMaterials() {

	static float ambient[] =
	{ .5f, .5f, .5, 1.0f };
	static float diffuse[] =
	{ 1.0f, 1.0f, 1.0f, 1.0f };

	static float position[] =
	{5.0, 5.0, 5.0, 0.0 };

	static float lmodel_ambient[] =
	{ 1.0f, 1.0f, 1.0f, 1.0f };

	static float lmodel_twoside[] =
	{ GL_TRUE };


	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, position);


	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, lmodel_twoside);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
//	glEnable(GL_LIGHT1);
	glShadeModel(GL_SMOOTH);
} 

void ofApp::savePicture() {
	ofImage picture;
	picture.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
	picture.save("screenshot.png");
	cout << "picture saved" << endl;
}

// ---------------------

// Draw an XYZ axis in RGB at world (0,0,0) for reference.
//
void ofApp::drawAxis(ofVec3f worldPos, int option=0) {
    
    ofPushMatrix();
    
    // The X Line
    ofSetColor(ofColor(255, 0, 0));
    // In case we need the X Line Highlighted/ Vanilla Option
    if (option == 1 || option == 4 || option == 6) {
        ofSetLineWidth(4);
    } else if (option == 0) {
        ofSetLineWidth(1);
    }
    ofDrawLine(worldPos, ofPoint(worldPos.x+1, worldPos.y, worldPos.z));
    // Set it back the width to 1
    ofSetLineWidth(1);
    
    // The Y Line
    ofSetColor(ofColor(0, 255, 0));
    // In case we need the Y Line Highlighted/ Vanilla Option
    if (option == 2 || option == 4 || option == 5) {
        ofSetLineWidth(4);
    } else if (option == 0) {
        ofSetLineWidth(1);
    }
    ofDrawLine(worldPos, ofPoint(worldPos.x, worldPos.y+1, worldPos.z));
    // Set it back the width to 1
    ofSetLineWidth(1);
    
    ofSetColor(ofColor(0, 0, 255));
    // In case we need the Z Line Highlighted/ Vanilla Option
    if (option == 3 || option == 5 || option == 6) {
        ofSetLineWidth(4);
    } else if (option == 0) {
        ofSetLineWidth(1);
    }
    ofDrawLine(worldPos, ofPoint(worldPos.x, worldPos.y, worldPos.z+1));
    // Set it back the width to 1
    ofSetLineWidth(1);
    
    ofPopMatrix();
    
    // Cone can be drawn, but not rotated as of now
    // cone.setPosition(worldPos.x+1, worldPos.y, worldPos.z);
}

// ---------------------

// Code for varying the selection between the rover and the environment
//
void ofApp::objSelect() {
    // If the option to select the terrain is enabled, disable it and
    // vice-versa
    if (terrainSelect) {
        terrainSelect = false;
        // Hack check for seeing if something is dragged in or not
        // since bRover is set to true, whenever anything is dragged
        // into the canvas
        if (bRover) {
            roverSelect = true;
        }
    } else {
        terrainSelect = true;
        roverSelect = false;
    }
}
