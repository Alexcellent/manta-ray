// COMP 371 - Team 5 Final Project
//		RayTracer Implementation File

#include <vector>;
#include <fstream>;
#include <iostream>;
#include <limits>;

#include "Shape.h";
#include "imageloader.h"

#define MAX_DEPTH 5

using namespace std;

// Some constants
const float INF = std::numeric_limits<int>::max();		// Infinity
const bool AREA = false;								// Toggle area-lighting on or off
const TVector LIGHT(4, 4, 4);							// Light emission
const TVector BACKGROUND(0, 0, 0);						// Background color
const TVector AMBIENT(2, 2, 2);							// Ambient light
int AREA_LIGHTING = 16;									// Num of rays for area lighting - MUST BE POWER OF 2 - 0 turns it off

// Texture map is global
Image* image;
TVector** textureMap;

// Light vector
vector<Shape*> lights;


// Calculate reflection and transparency mix of colors using fresnal equation
static float getFresnel(float ratio, float k, float mix){
	//	Source: Scratch-a-pixel tutorial at http://www.scratchapixel.com/
	return pow((1 - ratio), 3) * (1.0 - mix) + k * mix;
}

// Get shade of pixel, considering area lighting or no
static float getShade(TVector hit, TVector norm, const vector<Shape*> & shapes, int numShadowRays)
{
	TVector lightDir;
	TVector srOrigin(hit + norm * bias);
	float shade = 1; // 1 means not in shadow of any object
	if (numShadowRays > 0){
		// Area Light must be a rectangle
		// Divide it into a series of projection points based on desired
		// number of rays (which should be a power of 2)

		// Check if numShadowRays a power of 2, else do a default of 9
		if (floor(sqrt(numShadowRays)) != sqrt(numShadowRays)){ numShadowRays = 9; }
		for (int i = 0; i < shapes.size(); i++){
			if (shapes[i]->isLight){
				TVector projPoint;
				for (int y = 0; y < sqrt(numShadowRays); y++){
					TVector vy30 = shapes[i]->v3 - shapes[i]->v0;
					TVector vy21 = shapes[i]->v2 - shapes[i]->v1;
					projPoint = shapes[i]->v0 + (vy30)* (float(y) / (sqrt(numShadowRays) - 1));
					for (int x = 0; x < sqrt(numShadowRays); x++){

						// Get point on light to aim at - lots of linear algebra
						projPoint = projPoint + ((shapes[i]->v1 + (vy21)* (float(y) / (sqrt(numShadowRays) - 1))) - projPoint) * (float(x) / (sqrt(numShadowRays) - 1));;

						// Make shadow ray
						lightDir = projPoint - hit;
						lightDir.normalize2();

						Ray shadowRay(srOrigin, lightDir);

						// Verify intersection
						for (int j = 0; j < shapes.size(); j++){
							if (i != j){
								float oHit, oOut;
								if (shapes[j]->intersect(shadowRay, &oHit, &oOut)){
									shade -= (0.9 / float(numShadowRays));	// Return fragment of shading if no intersection
									break;									// Scaling by 0.9 rather than 1 makes nice shadows, we never ger fully black shadows
								}
							}
						}
					}
				}
			}
		}
	}
	else {
		for (int i = 0; i < shapes.size(); i++){

			if (shapes[i]->isLight){
				lightDir = shapes[i]->position - hit;
				lightDir.normalize2();

				Ray shadowRay(srOrigin, lightDir);

				for (int j = 0; j < shapes.size(); j++){
					if (i != j){
						float oHit, oOut;
						if (shapes[j]->intersect(shadowRay, &oHit, &oOut)){
							shade = 0.5;
							break;
						}
					}
				}
			}
		}
	}
	return shade;
}

TVector traceRay(Ray ray, const vector<Shape*> &shapes, int rayDepth){
	const Shape *currShape = NULL;	// Nearest shape interesected by ray
	float minDist = INF;			// Distance to nearest shape
	TVector hit;					// Coordinates of point-in
	TVector norm;					// Normal at point-in
	bool inside = false;			// Whether we are inside a shape or not
	float shade;					// Shading factor
	TVector pix(0, 0, 0);			// Pixel color this ray will return

	// Find the intersected shape, if any
	for (int i = 0; i < shapes.size(); i++){
		float oHit(INF), oOut(INF);
		if (shapes[i]->intersect(ray, &oHit, &oOut)){
			if (oHit < 0.0) { oHit = oOut; } // oHit is behind camera, so use exit point
			if (oHit < minDist) { minDist = oHit, currShape = shapes[i]; }	// new closest shape	
		}
	}
	if (!currShape) return AMBIENT;	// No intersection

	// Get coordinates of hit point and normal at hit poit
	hit = ray.origin + ray.direction*minDist;
	norm = currShape->getHitNorm(hit);

	// Verify if we are inside the shape
	//		Norm and ray direction will be in same
	//		direction if we are inside, so dot 
	//		product will be positive
	// Reverse norm if inside, to pick up color from inside shape
	if ((norm | ray.direction) > 0.0) { norm = -norm, inside = true; }

	// Get shading factor
	shade = getShade(hit, norm, shapes, AREA_LIGHTING);
	// If shape is textured, fetch and return pixel color
	if (currShape->isTextured) {
		// Get vector v0 to hit point
		TVector vh = hit - currShape->v0;

		float rectWidth = TVector::magnitude(TVector(currShape->v1) - TVector(currShape->v0));
		float rectHeight = TVector::magnitude(TVector(currShape->v3) - TVector(currShape->v0));

		float x = vh[0];
		float y = vh[1];

		float xRatio = x / rectWidth;
		float yRatio = -y / rectHeight;

		int c = xRatio * image->width;		// Row
		int r = yRatio * image->height;		// Col

		TVector* row = textureMap[r];
		TVector pix = row[c];

		// Get shade factor
		pix = pix *	shade;

		return pix;
	}

	// If transparent or reflective, must trace further (if max recursive depth has not been met
	if (rayDepth < MAX_DEPTH && (currShape->transp > 0.0 || currShape->refl > 0.0)){
		// Get how directly shape is facing ray using norm
		float ratio = -(ray.direction | norm);

		// Get fresnel mix (see function for source)
		float fresnel = getFresnel(ratio, 1, 0.1);	// Values taken from scratchapixel, they work well

		// Get reflection component
		TVector reflecColor;
		if (currShape->refl){
			Ray reflectedRay = currShape->getReflectedRay(ray, hit, norm);
			reflecColor = traceRay(reflectedRay, shapes, rayDepth + 1);
		}

		// Get transparency component
		TVector refracColor;
		if (currShape->transp){
			Ray refractedRay = currShape->getRefractedRay(ray, hit, norm, inside);
			refracColor = traceRay(refractedRay, shapes, rayDepth + 1);
		}

		// Caluculate pixel color from these components using fresnel
		pix = reflecColor * fresnel * currShape->refl + refracColor * (1 - fresnel) * currShape->transp;
		pix = pix.mult(currShape->color);

		// Get shade factor
		// We ignore shadows for transparency, as the interiour of an object
		// always intersects itself, thus the interior is completely in shade,
		// nulling the transparency factor
		if (!currShape->transp){
			//cout << "Shading of " << shade << " applied." << endl;
			pix = pix * shade;
		}
	}
	else { // Diffuse
		// Get shade factor
		shade = 1;
		for (int i = 0; i < shapes.size(); i++){
			if (shapes[i]->isLight){
				TVector lightDir = shapes[i]->position - hit;
				lightDir.normalize2();

				Ray shadowRay(hit + norm * bias, lightDir);

				for (int j = 0; j < shapes.size(); j++){
					if (i != j){
						float oHit, oOut;
						if (shapes[j]->intersect(shadowRay, &oHit, &oOut)){
							shade = 0.5;
							break;
						}
					}

					pix = pix + (TVector(currShape->color) * shade * max(float(0.0), (norm | lightDir))).mult(LIGHT); // From scratchapixel	
				}
			}
		}
	}

	// If shape is a light, add light emission to pixel color
	if (currShape->isLight) pix = pix + LIGHT;

	return pix;
}

void renderScene(const vector<Shape*> &shapes, TVector camera, float width, float height, string filename){

	int imageSize = width * height;
	TVector *image = new TVector[imageSize];
	int pixel = 0;	// Current pixel

	// Camera ray projection
	// Source: http://www.scratchapixel.com/lessons/3d-basic-lessons/lesson-6-rays-cameras-and-images/building-primary-rays-and-rendering-an-image/

	float aspectRatio = float(width) / float(height);
	float fov = 30.0;
	float bc = tan(fov * 0.5 * M_PI / 180.0); // Tan requires radians

	for (int y = 0; y < height; y++){
		for (int x = 0; x < width; x++, pixel++){
			// Normalize and remap pixels
			float px = ((2 * (x + 0.5) / float(width)) - 1) * bc * aspectRatio;
			float py = (1 - (2 * (y + 0.5) / float(height))) * bc;

			TVector direction(px, py, -1);
			direction.normalize2();
			Ray ray(camera, direction);

			image[pixel] = traceRay(ray, shapes, 0);
		}
		cout << "Progress: " << y * 100 / height << "%\n";
	}


	// Output to ppm file
	ofstream stream(filename, ios::out | ios::binary);
	stream << "P6\n" << width << " " << height << "\n255\n";
	for (int i = 0; i < width * height; i++) {
		stream << (unsigned char)(min(float(1.0), image[i][0]) * 255) <<
			(unsigned char)(min(float(1.0), image[i][1]) * 255) <<
			(unsigned char)(min(float(1.0), image[i][2]) * 255);
	}
	stream.close();
	delete[] image;
}

// Render demo scene, which encompasses all ray-tracer functions
void makeDemoScene(vector<Shape*> &shapes) {

	// Sphere(position, color, transp, refl, isLight, radius)
	// Plane(position, color, transp, refl, isLight, normal)
	// Rect(color, transp, refl, isLight, v0, v1, v2, v3) (Clockwise)
	// Tri(color, transp, refl, isLight, v0, v1, v2) (Clockwise)

	// For RECT and TRI, must initialize vertices in clockwise order
	// relative to direction face is facing, for normal calculation to be correct.
	//	A-----B
	//  |	  |
	//  C-----D
	// Initialize A->B->C->D and shape will be facing towards you reading this now.
	// Same for triangle, except theres only 3 vertices <-- obviously...

	// Sphere position
	TVector sP;
	float sTransp(0.9), sRefl(1), radius(2);
	sP = TVector(3, -2, -30);
	shapes.push_back(new Sphere(sP, TVector(0.7, 0.7, 0.8), sTransp, sRefl, false, radius));

	// Pyramid Info
	TVector pA, pB, pC, pD;
	pA = TVector(0, -5, -30);
	pB = TVector(3.45, -5, -20);
	pC = TVector(-3.45, -5, -20);
	pD = TVector(0, 0, -25);
	float pTransp(0.9), pRefl(1);
	shapes.push_back(new Tri(TVector(0.8, 0.8, 0.9), 0.9, 1, false, pA, pB, pC));
	shapes.push_back(new Tri(TVector(0.8, 0.8, 0.9), 0.9, 1, false, pA, pB, pD));
	shapes.push_back(new Tri(TVector(0.8, 0.8, 0.9), 0.9, 1, false, pA, pD, pC));
	shapes.push_back(new Tri(TVector(0.8, 0.8, 0.9), 0.9, 1, false, pB, pC, pD));

	// Cornel box coordinates
	TVector cA, cB, cC, cD, cE, cF, cG, cH;
	float width(14), height(14);
	cA = TVector(-width / 2, height / 2, -20);	// Top left front
	cB = TVector(width / 2, height / 2, -20);	// Top right front
	cC = TVector(width / 2, height / 2, -40);	// Top right back
	cD = TVector(-width / 2, height / 2, -40);	// Top left back
	cE = TVector(-width / 2, -height / 2, -40); // Bot left back
	cF = TVector(width / 2, -height / 2, -40);	// Bot right back
	cG = TVector(width / 2, -height / 2, -20);	// Bot right front
	cH = TVector(-width / 2, -height / 2, -20); // Bot left front 
	shapes.push_back(new Rect(TVector(0.8, 0.8, 0.9), 0.9, 1, false, cA, cD, cE, cH)); // Left
	shapes.push_back(new Rect(TVector(0.8, 0.8, 0.9), 0.9, 1, false, cB, cC, cF, cG)); // Right
	shapes.push_back(new Rect(TVector(0.8, 0.4, 0.4), 0.0, 0, false, cA, cB, cC, cD)); // Top
	shapes.push_back(new Rect(TVector(0.9, 0.9, 0.9), 0.0, 0.1, false, cE, cF, cG, cH)); // Bot


	// Back panel is textured
	Rect* back = new Rect(TVector(), 0, 0, false, cD, cC, cF, cE);	// Back
	back->isTextured = false;
	shapes.push_back(back);

	// Set up light
	TVector lA, lB, lC, lD;
	lA = TVector(-3, height / 2 - 0.1, -27);
	lB = TVector(3, height / 2 - 0.1, -27);
	lC = TVector(3, height / 2 - 0.1, -33);
	lD = TVector(-3, height / 2 - 0.1, -33);
	lights.push_back(new Rect(TVector(), 0.0, 1, true, lA, lB, lC, lD));
}

int main(){

	// Vector holds shapes to render
	vector<Shape*> shapes;

	// Texture mapping
	/*image = loadBMP("texture5.bmp");
	textureMap = getPixelArray(image);*/

// Example 1 Area shading
	////// Sphere
	//shapes.push_back(new Sphere(TVector(0, -2, -35), TVector(0.6, 0.6, 0.7), 0.9, 1, false, 3));

	////// Rectangles
	//shapes.push_back(new Rect(TVector(0.4, 0.4, 0.4), 0.0, 1, false, TVector(-10, -10, -40), TVector(10, -10, -40), TVector(10, -10, -30), TVector(-10, -10, -30)));
	////shapes.push_back(new Rect(TVector(0.4, 0.4, 0.4), 0.0, 1, false, TVector(-10, 10, -40), TVector(10, 10, -40), TVector(10, -10, -40), TVector(-10, 10, -40)));

	////// Light
	//AREA_LIGHTING = 0;
	//shapes.push_back(new Rect(TVector(), 0.0, 1, true, TVector(3, 3.9, -32), TVector(-3, 3.9, -32), TVector(-3, 3.9, -38), TVector(3, 3.9, -38)));

// Example 2 Texture mapping


	//Rect* temp = new Rect(TVector(0, 1, 0), 0.0, 1, false, TVector(-10, 10, -25), TVector(10, 10, -25), TVector(10, -10, -25), TVector(-10, -10, -25));
	//temp->isTextured = true;
	//shapes.push_back(temp);

	//shapes.push_back(new Sphere(TVector(3, 0, -15), TVector(0.7, 0.7, 0.8), 0.9, 1, false, 3));


	//// Light
	////shapes.push_back(new Sphere(TVector(0, 0, 25), TVector(), 0, 1, true, 5));
	//shapes.push_back(new Rect(TVector(), 0.0, 1, true, TVector(2, 2, 20), TVector(-2, 2, 20), TVector(-2, -2, 20), TVector(2, -2, 20)));

// Example 3 Texture mapping + area-lighting
	//// Sphere
	shapes.push_back(new Sphere(TVector(1, -3, -35), TVector(1, 1, 1), 0.9, 1, false, 3));

	//// Rectangles
	shapes.push_back(new Rect(TVector(0.7, 0.7, 0.8), 0.0, 1, false, TVector(-10, -8, -40), TVector(10, -8, -40), TVector(10, -8, -30), TVector(-10, -8, -30)));
	shapes.push_back(new Rect(TVector(0.8,0.8,1), 0.7, 1, false, TVector(-10, 12, -30), TVector(-10, 12, -40), TVector(-10, -8, -40), TVector(-10, -8, -30)));
	shapes.push_back(new Rect(TVector(1, 0.8, 0.8), 0.7, 1, false, TVector(10, 12, -40), TVector(10, 12, -30), TVector(10, -8, -30), TVector(10, -8, -40)));
	Rect* temp = new Rect(TVector(0.4, 0.4, 0.4), 0.0, 1, false, TVector(-10, 12, -40), TVector(10, 12, -40), TVector(10, -8, -40), TVector(-10, -8, -40));
	temp->isTextured = false;
	shapes.push_back(temp);

	//// Light
	AREA_LIGHTING = 32;
	shapes.push_back(new Rect(TVector(), 0.0, 1, true, TVector(5, 10.1, -32.5), TVector(-5, 10.1, -32.5), TVector(-5, 10.1, -37.5), TVector(5, 10.1, -37.5)));

	// Set up image size, camera position, filename
	TVector camera;
	int iWidth = 640;
	int iHeight = 480;

	renderScene(shapes, camera, float(iWidth), float(iHeight), "./renderSuperGewd.ppm");

	cout << "Render Complete." << endl;
	char x;
	cin >> x;
}