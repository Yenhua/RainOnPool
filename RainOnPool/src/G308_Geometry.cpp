//---------------------------------------------------------------------------
//
// Copyright (c) 2012 Taehyun Rhee
//
// This software is provided 'as-is' for assignment of COMP308 
// in ECS, Victoria University of Wellington, 
// without any express or implied warranty. 
// In no event will the authors be held liable for any
// damages arising from the use of this software.
//
// The contents of this file may not be copied or duplicated in any form
// without the prior permission of its owner.
//
//----------------------------------------------------------------------------

#include "G308_Geometry.h"
#include "G308_ImageLoader.h"
#include <stdio.h>
#include <math.h>

G308_Geometry::G308_Geometry(void) {
	m_pVertexArray = NULL;
	m_pNormalArray = NULL;
	m_pUVArray = NULL;
	m_pTriangles = NULL;

	mode = G308_SHADE_POLYGON;

	m_nNumPoint = m_nNumUV = m_nNumPolygon = 0;
	m_glGeomListPoly = m_glGeomListWire = 0;
}

G308_Geometry::~G308_Geometry(void) {
	if (m_pVertexArray != NULL)
		delete[] m_pVertexArray;
	if (m_pNormalArray != NULL)
		delete[] m_pNormalArray;
	if (m_pUVArray != NULL)
		delete[] m_pUVArray;
	if (m_pTriangles != NULL)
		delete[] m_pTriangles;
	free(texture);
}

//-------------------------------------------------------
// Read in the OBJ (Note: fails quietly, so take care)
//--------------------------------------------------------
void G308_Geometry::ReadOBJ(const char *filename) {
	FILE* fp;
	char mode, vmode;
	char str[200];
	int v1, v2, v3, n1, n2, n3, t1, t2, t3;
	int numVert, numNorm, numUV, numFace;
	float x, y, z;
	float u, v;

	numVert = numNorm = numUV = numFace = 0;

	fp = fopen(filename, "r");
	if (fp == NULL)
		printf("Error reading %s file\n", filename);
	else
		//printf("Reading %s file\n", filename);

	// Check number of vertex, normal, uvCoord, and Face
	while (fgets(str, 200, fp) != NULL) {
		sscanf(str, "%c%c", &mode, &vmode);
		switch (mode) {
		case 'v': /* vertex, uv, normal */
			if (vmode == 't') // uv coordinate
				numUV++;
			else if (vmode == 'n') // normal
				numNorm++;
			else if (vmode == ' ') // vertex
				numVert++;
			break;
		case 'f': /* faces */
			numFace++;
			break;
		}
	}

	m_nNumPoint = numVert;
	m_nNumUV = numUV;
	m_nNumPolygon = numFace;
	m_nNumNormal = numNorm;

	//printf("Number of Point %d, UV %d, Normal %d, Face %d\n", numVert, numUV,
	//		numNorm, numFace);
	//-------------------------------------------------------------
	//	Allocate memory for array
	//-------------------------------------------------------------

	if (m_pVertexArray != NULL)
		delete[] m_pVertexArray;
	m_pVertexArray = new G308_Point[m_nNumPoint];

	if (m_pNormalArray != NULL)
		delete[] m_pNormalArray;
	m_pNormalArray = new G308_Normal[m_nNumNormal];

	if (m_pUVArray != NULL)
		delete[] m_pUVArray;
	m_pUVArray = new G308_UVcoord[m_nNumUV];

	if (m_pTriangles != NULL)
		delete[] m_pTriangles;
	m_pTriangles = new G308_Triangle[m_nNumPolygon];

	//-----------------------------------------------------------
	//	Read obj file
	//-----------------------------------------------------------
	numVert = numNorm = numUV = numFace = 0;

	fseek(fp, 0L, SEEK_SET);
	while (fgets(str, 200, fp) != NULL) {
		sscanf(str, "%c%c", &mode, &vmode);
		switch (mode) {
		case 'v': /* vertex, uv, normal */
			if (vmode == 't') // uv coordinate
					{
				sscanf(str, "vt %f %f", &u, &v);
				m_pUVArray[numUV].u = u;
				m_pUVArray[numUV].v = v;
				numUV++;
			} else if (vmode == 'n') // normal
					{
				sscanf(str, "vn %f %f %f", &x, &y, &z);
				m_pNormalArray[numNorm].x = x;
				m_pNormalArray[numNorm].y = y;
				m_pNormalArray[numNorm].z = z;
				numNorm++;
			} else if (vmode == ' ') // vertex
					{
				sscanf(str, "v %f %f %f", &x, &y, &z);
				m_pVertexArray[numVert].x = x;
				m_pVertexArray[numVert].y = y;
				m_pVertexArray[numVert].z = z;
				numVert++;
			}
			break;
		case 'f': /* faces : stored value is index - 1 since our index starts from 0, but obj starts from 1 */
			if (numNorm > 0 && numUV > 0) {
				sscanf(str, "f %d/%d/%d %d/%d/%d %d/%d/%d", &v1, &t1, &n1, &v2, &t2, &n2, &v3, &t3, &n3);
			} else if(numNorm > 0 && numUV ==0){
				sscanf(str, "f %d//%d %d//%d %d//%d", &v1, &n1, &v2, &n2, &v3, &n3);
			} else if(numUV > 0 && numNorm==0){
				sscanf(str, "f %d/%d %d/%d %d/%d", &v1, &t1, &v2, &t2, &v3, &t3);
			} else if(numUV==0 && numNorm==0){
				sscanf(str, "f %d %d %d", &v1, &v2, &v3);
			}
			// Vertex indicies for triangle:
			if (numVert != 0) {
				m_pTriangles[numFace].v1 = v1 - 1;
				m_pTriangles[numFace].v2 = v2 - 1;
				m_pTriangles[numFace].v3 = v3 - 1;
			}

			// Normal indicies for triangle
			if (numNorm != 0) {
				m_pTriangles[numFace].n1 = n1 - 1;
				m_pTriangles[numFace].n2 = n2 - 1;
				m_pTriangles[numFace].n3 = n3 - 1;
			}

			// UV indicies for triangle
			if (numUV != 0) {
				m_pTriangles[numFace].t1 = t1 - 1;
				m_pTriangles[numFace].t2 = t2 - 1;
				m_pTriangles[numFace].t3 = t3 - 1;
			}

			numFace++;
			break;
		default:
			break;
		}
	}

	fclose(fp);
	//printf("Reading OBJ file is DONE.\n");

}

//--------------------------------------------------------------
// [Assignment4]
// Create a 2D GL texture from the file given
//--------------------------------------------------------------
void G308_Geometry::ReadTexture(const char* filename) {
	//Your code here
}

//--------------------------------------------------------------
// [Assignment4]
// Fill the following function to create display list
// of the obj file to show it as polygon, using texture and normal information (if any)
//--------------------------------------------------------------
void G308_Geometry::CreateGLPolyGeometry() {
	if (m_glGeomListPoly != 0)
		glDeleteLists(m_glGeomListPoly, 1);

	// Assign a display list; return 0 if err
	m_glGeomListPoly = glGenLists(1);
	glNewList(m_glGeomListPoly, GL_COMPILE);

	//Your code here
	//TODO

	glBegin(GL_TRIANGLES);

		G308_Point n1, n2, n3;
		G308_UVcoord t1, t2, t3;
		for(int i = 0 ; i < m_nNumPolygon ; i ++){

			G308_Point v1 = m_pVertexArray[m_pTriangles[i].v1];
			G308_Point v2 = m_pVertexArray[m_pTriangles[i].v2];
			G308_Point v3 = m_pVertexArray[m_pTriangles[i].v3];

			if (m_nNumNormal > 0) {
				n1 = m_pNormalArray[m_pTriangles[i].n1];
				n2 = m_pNormalArray[m_pTriangles[i].n2];
				n3 = m_pNormalArray[m_pTriangles[i].n3];
			}
			if (m_nNumUV > 0) {//texture coordinates
				t1 = m_pUVArray[m_pTriangles[i].t1];
				t2 = m_pUVArray[m_pTriangles[i].t2];
				t3 = m_pUVArray[m_pTriangles[i].t3];
			}
			float textMult = 10.0f;

			if (m_nNumUV > 0) {
				glTexCoord2f(t1.u * textMult, t1.v * textMult);//to deal with textures
			}
			if (m_nNumNormal > 0) {
				glNormal3f(n1.x, n1.y, n1.z);
			}
			glVertex3f(v1.x, v1.y, v1.z);

			if (m_nNumUV > 0) {
				glTexCoord2f(t2.u * textMult, t2.v * textMult);
			}
			if (m_nNumNormal > 0) {
				glNormal3f(n2.x, n2.y, n2.z);
			}
			glVertex3f(v2.x, v2.y, v2.z);

			if (m_nNumUV > 0) {
				glTexCoord2f(t3.u * textMult, t3.v * textMult);
			}
			if (m_nNumNormal > 0) {
				glNormal3f(n3.x, n3.y, n3.z);
			}
			glVertex3f(v3.x, v3.y, v3.z);

		}

		glEnd();

	glEndList();
}

void G308_Geometry::CreateGLWireGeometry() {
	if (m_glGeomListWire != 0)
		glDeleteLists(m_glGeomListWire, 1);

	// Assign a display list; return 0 if err
	m_glGeomListWire = glGenLists(1);
	glNewList(m_glGeomListWire, GL_COMPILE);

	for (int i = 0; i < m_nNumPolygon; i++) {
		glBegin(GL_LINE_LOOP);
		glNormal3f(m_pNormalArray[m_pTriangles[i].n1].x,
				m_pNormalArray[m_pTriangles[i].n1].y,
				m_pNormalArray[m_pTriangles[i].n1].z);
		glVertex3f(m_pVertexArray[m_pTriangles[i].v1].x,
				m_pVertexArray[m_pTriangles[i].v1].y,
				m_pVertexArray[m_pTriangles[i].v1].z);
		glNormal3f(m_pNormalArray[m_pTriangles[i].n2].x,
				m_pNormalArray[m_pTriangles[i].n2].y,
				m_pNormalArray[m_pTriangles[i].n2].z);
		glVertex3f(m_pVertexArray[m_pTriangles[i].v2].x,
				m_pVertexArray[m_pTriangles[i].v2].y,
				m_pVertexArray[m_pTriangles[i].v2].z);
		glNormal3f(m_pNormalArray[m_pTriangles[i].n3].x,
				m_pNormalArray[m_pTriangles[i].n3].y,
				m_pNormalArray[m_pTriangles[i].n3].z);
		glVertex3f(m_pVertexArray[m_pTriangles[i].v3].x,
				m_pVertexArray[m_pTriangles[i].v3].y,
				m_pVertexArray[m_pTriangles[i].v3].z);
		glEnd();
	}

	glEndList();

}
void G308_Geometry::toggleMode() {
	if (mode == G308_SHADE_POLYGON) {
		mode = G308_SHADE_WIREFRAME;
	} else {
		mode = G308_SHADE_POLYGON;
	}
}

void G308_Geometry::RenderGeometry() {

	if (mode == G308_SHADE_POLYGON) {
		glCallList(m_glGeomListPoly);
	} else if (mode == G308_SHADE_WIREFRAME) {
		glCallList(m_glGeomListWire);
	} else {
		printf("Warning: Wrong Shading Mode. \n");
	}

}

/**
 * Original data has been loaded in...shoud be ok if we keep recreating the mesh
 */
void G308_Geometry::Translate(float tx, float ty, float tz){
	if (m_glGeomListPoly != 0)
		glDeleteLists(m_glGeomListPoly, 1);

	// Assign a display list; return 0 if err
	m_glGeomListPoly = glGenLists(1);
	glNewList(m_glGeomListPoly, GL_COMPILE);

	//Your code here
	//TODO

	glBegin(GL_TRIANGLES);

		G308_Point n1, n2, n3;
		G308_UVcoord t1, t2, t3;
		for(int i = 0 ; i < m_nNumPolygon ; i ++){

			G308_Point v1 = m_pVertexArray[m_pTriangles[i].v1];
			G308_Point v2 = m_pVertexArray[m_pTriangles[i].v2];
			G308_Point v3 = m_pVertexArray[m_pTriangles[i].v3];

			v1.x = v1.x + tx;
			v2.x = v2.x + tx;
			v3.x = v3.x + tx;
			v1.y = v1.y + ty;
			v2.y = v2.y + ty;
			v3.y = v3.y + ty;
			v1.z = v1.z + tz;
			v2.z = v2.z + tz;
			v3.z = v3.z + tz;
//			m_pVertexArray[m_pTriangles[i].v1].x = m_pVertexArray[m_pTriangles[i].v1].x + tx;
//			m_pVertexArray[m_pTriangles[i].v2].x = m_pVertexArray[m_pTriangles[i].v2].x + tx;
//			m_pVertexArray[m_pTriangles[i].v3].x = m_pVertexArray[m_pTriangles[i].v3].x + tx;
//
//			m_pVertexArray[m_pTriangles[i].v1].y = m_pVertexArray[m_pTriangles[i].v1].y + ty;
//			m_pVertexArray[m_pTriangles[i].v2].y = m_pVertexArray[m_pTriangles[i].v2].y + ty;
//			m_pVertexArray[m_pTriangles[i].v3].y = m_pVertexArray[m_pTriangles[i].v3].y + ty;
//
//			m_pVertexArray[m_pTriangles[i].v1].z = m_pVertexArray[m_pTriangles[i].v1].z + tz;
//			m_pVertexArray[m_pTriangles[i].v2].z = m_pVertexArray[m_pTriangles[i].v2].z + tz;
//			m_pVertexArray[m_pTriangles[i].v3].z = m_pVertexArray[m_pTriangles[i].v3].z + tz;

			if (m_nNumNormal > 0) {
				n1 = m_pNormalArray[m_pTriangles[i].n1];
				n2 = m_pNormalArray[m_pTriangles[i].n2];
				n3 = m_pNormalArray[m_pTriangles[i].n3];
			}
			if (m_nNumUV > 0) {//texture coordinates
				t1 = m_pUVArray[m_pTriangles[i].t1];
				t2 = m_pUVArray[m_pTriangles[i].t2];
				t3 = m_pUVArray[m_pTriangles[i].t3];
			}
			float textMult = 10.0f;

			if (m_nNumUV > 0) {
				glTexCoord2f(t1.u * textMult, t1.v * textMult);//to deal with textures
			}
			if (m_nNumNormal > 0) {
				glNormal3f(n1.x, n1.y, n1.z);
			}
			glVertex3f(v1.x, v1.y, v1.z);

			if (m_nNumUV > 0) {
				glTexCoord2f(t2.u * textMult, t2.v * textMult);
			}
			if (m_nNumNormal > 0) {
				glNormal3f(n2.x, n2.y, n2.z);
			}
			glVertex3f(v2.x, v2.y, v2.z);

			if (m_nNumUV > 0) {
				glTexCoord2f(t3.u * textMult, t3.v * textMult);
			}
			if (m_nNumNormal > 0) {
				glNormal3f(n3.x, n3.y, n3.z);
			}
			glVertex3f(v3.x, v3.y, v3.z);

		}

		glEnd();

	glEndList();
}

void G308_Geometry::Scale(float tx, float ty, float tz){
	if (m_glGeomListPoly != 0)
		glDeleteLists(m_glGeomListPoly, 1);

	// Assign a display list; return 0 if err
	m_glGeomListPoly = glGenLists(1);
	glNewList(m_glGeomListPoly, GL_COMPILE);

	//Your code here
	//TODO

	glBegin(GL_TRIANGLES);

		G308_Point n1, n2, n3;
		G308_UVcoord t1, t2, t3;
		for(int i = 0 ; i < m_nNumPolygon ; i ++){

			G308_Point v1 = m_pVertexArray[m_pTriangles[i].v1];
			G308_Point v2 = m_pVertexArray[m_pTriangles[i].v2];
			G308_Point v3 = m_pVertexArray[m_pTriangles[i].v3];

			m_pVertexArray[m_pTriangles[i].v1].x = v1.x * tx;
			m_pVertexArray[m_pTriangles[i].v2].x = v2.x * tx;
			m_pVertexArray[m_pTriangles[i].v3].x = v3.x * tx;

			m_pVertexArray[m_pTriangles[i].v1].y = v1.y * ty;
			m_pVertexArray[m_pTriangles[i].v2].y = v2.y * ty;
			m_pVertexArray[m_pTriangles[i].v3].y = v3.y * ty;

			m_pVertexArray[m_pTriangles[i].v1].z = v1.z * tz;
			m_pVertexArray[m_pTriangles[i].v2].z = v2.z * tz;
			m_pVertexArray[m_pTriangles[i].v3].z = v3.z * tz;

			if (m_nNumNormal > 0) {
				n1 = m_pNormalArray[m_pTriangles[i].n1];
				n2 = m_pNormalArray[m_pTriangles[i].n2];
				n3 = m_pNormalArray[m_pTriangles[i].n3];
			}
			if (m_nNumUV > 0) {//texture coordinates
				t1 = m_pUVArray[m_pTriangles[i].t1];
				t2 = m_pUVArray[m_pTriangles[i].t2];
				t3 = m_pUVArray[m_pTriangles[i].t3];
			}
			float textMult = 10.0f;

			if (m_nNumUV > 0) {
				glTexCoord2f(t1.u * textMult, t1.v * textMult);//to deal with textures
			}
			if (m_nNumNormal > 0) {
				glNormal3f(n1.x, n1.y, n1.z);
			}
			glVertex3f(v1.x+tx, v1.y+ty, v1.z+tz);

			if (m_nNumUV > 0) {
				glTexCoord2f(t2.u * textMult, t2.v * textMult);
			}
			if (m_nNumNormal > 0) {
				glNormal3f(n2.x, n2.y, n2.z);
			}
			glVertex3f(v2.x+tx, v2.y+ty, v2.z+tz);

			if (m_nNumUV > 0) {
				glTexCoord2f(t3.u * textMult, t3.v * textMult);
			}
			if (m_nNumNormal > 0) {
				glNormal3f(n3.x, n3.y, n3.z);
			}
			glVertex3f(v3.x+tx, v3.y+ty, v3.z+tz);

		}

		glEnd();

	glEndList();
}
