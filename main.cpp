#include<stdio.h>
#include<iostream>
#include<math.h>
#include<string.h>
#include<GL/glut.h>
#include"MChead.h"
struct GLvector
{
	GLfloat fX;
	GLfloat fY;
	GLfloat fZ;
};
static const GLfloat a2fVertexOffset[8][3] =
{
	{ 0.0, 0.0, 0.0 },{ 1.0, 0.0, 0.0 },{ 1.0, 1.0, 0.0 },{ 0.0, 1.0, 0.0 },
	{ 0.0, 0.0, 1.0 },{ 1.0, 0.0, 1.0 },{ 1.0, 1.0, 1.0 },{ 0.0, 1.0, 1.0 }
};
static const GLint a2iEdgeConnection[12][2] =
{
	{ 0,1 },{ 1,2 },{ 2,3 },{ 3,0 },
	{ 4,5 },{ 5,6 },{ 6,7 },{ 7,4 },
	{ 0,4 },{ 1,5 },{ 2,6 },{ 3,7 }
};
static const GLfloat a2fEdgeDirection[12][3] =
{
	{ 1.0, 0.0, 0.0 },{ 0.0, 1.0, 0.0 },{ -1.0, 0.0, 0.0 },{ 0.0, -1.0, 0.0 },
	{ 1.0, 0.0, 0.0 },{ 0.0, 1.0, 0.0 },{ -1.0, 0.0, 0.0 },{ 0.0, -1.0, 0.0 },
	{ 0.0, 0.0, 1.0 },{ 0.0, 0.0, 1.0 },{ 0.0, 0.0, 1.0 },{ 0.0,  0.0, 1.0 }
};
GLvoid MC(GLfloat fX, GLfloat fY, GLfloat fZ, GLfloat fscale);
//GLfloat fGetOffset(GLfloat V1, GLfloat V2, GLfloat FT);
GLfloat fGetOffset(GLfloat fValue1, GLfloat fValue2, GLfloat fValueDesired);
GLvoid vNormalizeVector(GLvector &rfVectorResult, GLvector &rfVectorSource);
GLvoid vGetNormal(GLvector &rfNormal, GLfloat fX, GLfloat fY, GLfloat fZ);
GLvoid vGetColor(GLvector &rfColor, GLvector &rfPosition, GLvector &rfNormal);
//GLvoid vMarchCube1(GLfloat fX, GLfloat fY, GLfloat fZ, GLfloat fScale);
//GLvoid MCall();
GLvoid vMarchingCubes();
void vIdle();
void vDrawScene();
void vResize(GLsizei iWidth, GLsizei iHeight);
void vKeyboard(unsigned char cKey, int iX, int iY);
void vSpecial(int iKey, int iX, int iY);
void myMouseCall(int button, int state, int x, int y);
void spinDisplay(void);
void fYawDisplay(void);
#if !defined(GLUT_WHEEL_UP)
#  define GLUT_WHEEL_UP   3
#  define GLUT_WHEEL_DOWN 4
#endif
#define X 64
#define Y 64
#define Z 64
#define datatype short int
datatype ***ptr;
char path[256] = "fuel.raw";
GLfloat fTargetValue = 80.0;
GLfloat HoriRotate = 90;
GLfloat VertiRotate = 0;

GLfloat  Xtranform = -90.0;
GLfloat  Ytranform = 80.0;
GLfloat  Ztranform = -10.0;

GLfloat ZoonAspect = 140.0;
GLfloat ZoomScale = 1.0;
short int themax = 0, themin = 255;
GLfloat   fTime = 0.0;
GLvector  sSourcePoint[3];
GLboolean bLight = true;
static const GLfloat afAmbientWhite[] = { 0.25, 0.25, 0.25, 1.00 };   // 周围 环绕 白
static const GLfloat afAmbientRed[] = { 0.25, 0.00, 0.00, 1.00 };   // 周围 环绕 红
static const GLfloat afAmbientGreen[] = { 0.00, 0.25, 0.00, 1.00 };   // 周围 环绕 绿
static const GLfloat afAmbientBlue[] = { 0.00, 0.00, 0.25, 1.00 };   // 周围 环绕 蓝
static const GLfloat afDiffuseWhite[] = { 0.75, 0.75, 0.75, 1.00 };   // 漫射 白
static const GLfloat afDiffuseRed[] = { 0.75, 0.00, 0.00, 1.00 };   // 漫射 红
static const GLfloat afDiffuseGreen[] = { 0.00, 0.75, 0.00, 1.00 };   // 漫射 绿
static const GLfloat afDiffuseBlue[] = { 0.00, 0.00, 0.75, 1.00 };   // 漫射 蓝
static const GLfloat afSpecularWhite[] = { 1.00, 1.00, 1.00, 1.00 };   // 反射 白
static const GLfloat afSpecularRed[] = { 1.00, 0.25, 0.25, 1.00 };   // 反射 红
static const GLfloat afSpecularGreen[] = { 0.25, 1.00, 0.25, 1.00 };   // 反射 绿
static const GLfloat afSpecularBlue[] = { 0.25, 0.25, 1.00, 1.00 };   // 反射 蓝


GLenum    ePolygonMode = GL_FILL;
//Edge flags 16进制表示的等值面与边相交的信息

void main(int argc, char **argv) {
	FILE *fp = NULL;
	

	int i, j, k, c;
	if ((fp = fopen(path,"rb"))==NULL) {
		printf("can not open the raw image");
		return;
	}
	else 
	{
		printf("read OK!");
	}
	ptr= (datatype ***)malloc(X * sizeof(datatype**));
	for (size_t i = 0; i < X; i++)
	{
		ptr[i] = (datatype **)malloc(Y * sizeof(datatype*));
	}
	for (size_t i = 0; i < X; i++)
	{
		for (size_t j = 0; j < Y; j++) {
			ptr[i][j] = (datatype *)malloc(Z * sizeof(datatype));
		}

	}
	for (int i = 0; i < Z; i++)
	{
		for (int j = 0; j < Y; j++)
		{
			for (int k= 0; k < X; k++) {
				ptr[k][j][i] = fgetc(fp);
				/*printf("%d", ptr[i][j][k]);*/
			}
		}
	}
	fclose(fp);
	
////////////////////////////////////////////
	GLfloat afPropertiesAmbient[] = { 0.25, 0.25, 0.25, 1.00 };
	GLfloat afPropertiesDiffuse[] = { 0.75, 0.75, 0.75, 1.00 };
	GLfloat afPropertiesSpecular[] = { 1.00, 0.25, 0.25, 1.00 };

	GLsizei iWidth = 640.0;
	GLsizei iHeight = 480.0;

	glutInit(&argc, argv);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(iWidth, iHeight);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutCreateWindow("Marching Cubes");
	glutDisplayFunc(vDrawScene);
	glutIdleFunc(vIdle);
	glutReshapeFunc(vResize);
	glutKeyboardFunc(vKeyboard);
	glutSpecialFunc(vSpecial);
	glutMouseFunc(myMouseCall);

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClearDepth(1.0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glPolygonMode(GL_FRONT_AND_BACK, ePolygonMode);

	glLightfv(GL_LIGHT0, GL_AMBIENT, afPropertiesAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, afPropertiesDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, afPropertiesSpecular);
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1.0);

	glEnable(GL_LIGHT0);


	// 材质颜色  ***里外 ***
	glMaterialfv(GL_BACK, GL_AMBIENT, afAmbientRed);
	glMaterialfv(GL_BACK, GL_DIFFUSE, afDiffuseRed);
	glMaterialfv(GL_FRONT, GL_AMBIENT, afAmbientWhite);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, afDiffuseWhite);
	glMaterialfv(GL_FRONT, GL_SPECULAR, afSpecularWhite);
	glMaterialf(GL_FRONT, GL_SHININESS, 10.0);

	vResize(iWidth, iHeight);

	glutMainLoop();
}
void vIdle()//   程序空闲时
{
	// 重绘函数
	//glutPostRedisplay();

}
void vKeyboard(unsigned char cKey, int iX, int iY)
{
	switch (cKey)
	{
	case 'w':
	{
		if (ePolygonMode == GL_LINE)
		{
			ePolygonMode = GL_FILL;
			glutPostRedisplay();
		}
		else
		{
			ePolygonMode = GL_LINE;
			glutPostRedisplay();
		}
		glPolygonMode(GL_FRONT_AND_BACK, ePolygonMode);
	} break;


	case 'l':
	{
		if (bLight)
		{
			glDisable(GL_LIGHTING);//use vertex colors
			glutPostRedisplay();
		}
		else
		{
			glEnable(GL_LIGHTING);//use lit material color
			glutPostRedisplay();
		}

		bLight = !bLight;
	};
	}
}
void myMouseCall(int button, int state, int x, int y)
{
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
			spinDisplay();
		break;


	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN)
			fYawDisplay();
		break;

	case GLUT_WHEEL_UP:

		ZoomScale = ZoomScale + 0.05;
		glutPostRedisplay();
		break;

	case GLUT_WHEEL_DOWN:
		ZoomScale = ZoomScale - 0.05;
		glutPostRedisplay();

		break;


	default:
		break;
	}
}

//特殊键响应 上下左右 PaUP PaDn
void vSpecial(int iKey, int iX, int iY)
{
	switch (iKey)
	{
	case GLUT_KEY_F1:
	{
		ZoonAspect -= 10;
		glutReshapeFunc(vResize);

	} break;


	////////////////////////////////////////////
	case GLUT_KEY_PAGE_UP:
	{
		if (fTargetValue <= themax - 1)
		{
			fTargetValue = fTargetValue + 1;
		}
		else
		{
			fTargetValue = themax;
		}
		glutPostRedisplay();


	} break;
	case GLUT_KEY_PAGE_DOWN:
	{
		if (fTargetValue >= themin + 1)
		{
			fTargetValue = fTargetValue - 1;
		}
		else
		{
			fTargetValue = themin;
		}
		glutPostRedisplay();

	} break;
	////////////////////////////////////////////////////////////////////////////


	//***                     键盘 上下左右 控制               ***//
	///////////////////////////////////////////////////////////////////////
	case GLUT_KEY_UP:    // 上
	{
		Ytranform += 10.0;
		glutPostRedisplay();
	} break;

	case GLUT_KEY_DOWN:   // 下
	{
		Ytranform -= 10.0;
		glutPostRedisplay();
	} break;

	case GLUT_KEY_LEFT:    // 左
	{
		Xtranform -= 10.0;
		glutPostRedisplay();
	} break;

	case GLUT_KEY_RIGHT:   //  右
	{
		Xtranform += 10.0;
		glutPostRedisplay();
	} break;
	/////////////////////////////////////////////////////////////////////////
	}
}
void spinDisplay(void)
{
	HoriRotate = HoriRotate + 10.0;
	if (HoriRotate >360.0)
		HoriRotate = HoriRotate - 360.0;
	glutPostRedisplay();
}
void fYawDisplay(void)
{
	VertiRotate = VertiRotate + 10.0;
	if (VertiRotate >360.0)
		VertiRotate = VertiRotate - 360.0;
	glutPostRedisplay();

}
void vDrawScene() // 绘制场景
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();


	glTranslatef(Xtranform, Ytranform, Ztranform);
	/*printf("%f,  %f,  %f \n" ,Xtranform, Ytranform, Ztranform );
	printf("%f,  %f \n" ,VertiRotate, HoriRotate);*/
	glRotatef(HoriRotate, 1.0, 0.0, 0.0);
	glRotatef(0.0, 0.0, 1.0, 0.0);
	glRotatef(VertiRotate, 0.0, 0.0, 1.0);

	glScaled(ZoomScale, ZoomScale, ZoomScale);

	glPushAttrib(GL_LIGHTING_BIT);
	glDisable(GL_LIGHTING);
	/* glColor3f(1.0, 1.0, 1.0);
	glutWireCube(100.0); */
	glPopAttrib();


	glBegin(GL_TRIANGLES);
	vMarchingCubes();
	glEnd();

	glPopMatrix();

	glutSwapBuffers();
}

void vResize(GLsizei iWidth, GLsizei iHeight)
{
	GLfloat fAspect, fHalfWorldSize = (1.4142135623730950488016887242097 / 2);
	glViewport(0, 0, iWidth, iHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (iWidth <= iHeight)
	{
		fAspect = (GLfloat)iHeight / (GLfloat)iWidth;
		glOrtho(-fHalfWorldSize*ZoonAspect, fHalfWorldSize*ZoonAspect, -fHalfWorldSize*fAspect*ZoonAspect,
			fHalfWorldSize*fAspect*ZoonAspect, -10 * fHalfWorldSize*ZoonAspect, 10 * fHalfWorldSize*ZoonAspect);

	}
	else
	{
		fAspect = (GLfloat)iWidth / (GLfloat)iHeight;
		glOrtho(-fHalfWorldSize*fAspect*ZoonAspect, fHalfWorldSize*fAspect*ZoonAspect, -fHalfWorldSize*ZoonAspect,
			fHalfWorldSize*ZoonAspect, -10 * fHalfWorldSize*ZoonAspect, 10 * fHalfWorldSize*ZoonAspect);

	}
	glMatrixMode(GL_MODELVIEW);

}
////////MarchingCube
GLvoid MC(GLfloat fX, GLfloat fY, GLfloat fZ,GLfloat fscale)
{
	extern GLint aiCubeEdgeFlags[256];
	extern GLint a2iTriangleConnectionTable[256][16];
	GLint iCorner, iVertex, iVertexTest, iEdge, iTriangle, iFlagIndex, iEdgeFlags;
	GLfloat fOffset;
	GLvector sColor;
	GLfloat afCubeValue[8];
	GLvector asEdgeVertex[12];
	GLvector asEdgeNorm[12];
		for (iVertex = 0; iVertex < 8; iVertex++)
	{
		afCubeValue[iVertex] = (GLfloat)ptr[(int)(fX+a2fVertexOffset[iVertex][0])][(int)(fY + a2fVertexOffset[iVertex][1])][(int)(fZ + a2fVertexOffset[iVertex][2])];
	}
	iFlagIndex = 0;
	for ( iVertex = 0; iVertex < 8; iVertex++)
	{
		if (afCubeValue[iVertex] <= fTargetValue) {
			iFlagIndex |= 1 << iVertex;
			}
	}
		iEdgeFlags = aiCubeEdgeFlags[iFlagIndex];
		if (iEdgeFlags==0)
		{
			return;
		}
		for ( iEdge = 0; iEdge < 12; iEdge++)
		{
			if (iEdgeFlags & (1<<iEdge))
			{
				//计算交点的坐标
				fOffset = fGetOffset(afCubeValue[a2iEdgeConnection[iEdge][0]], afCubeValue[a2iEdgeConnection[iEdge][1]],fTargetValue);
				asEdgeVertex[iEdge].fX = fX + (a2fVertexOffset[a2iEdgeConnection[iEdge][0]][0] + fOffset*a2fEdgeDirection[iEdge][0]);
				asEdgeVertex[iEdge].fY = fY + (a2fVertexOffset[a2iEdgeConnection[iEdge][0]][1] + fOffset*a2fEdgeDirection[iEdge][1]);
				asEdgeVertex[iEdge].fZ = fZ + (a2fVertexOffset[a2iEdgeConnection[iEdge][0]][2] + fOffset*a2fEdgeDirection[iEdge][2]);
			    
				vGetNormal(asEdgeNorm[iEdge], asEdgeVertex[iEdge].fX, asEdgeVertex[iEdge].fY, asEdgeVertex[iEdge].fZ);
			}

		}
		///////Draw the triangles
		for ( iTriangle = 0; iTriangle < 5; iTriangle++)
		{
			if (a2iTriangleConnectionTable[iFlagIndex][3*iTriangle]<0) {
				break;
			}
			for ( iCorner = 0; iCorner < 3; iCorner++)
			{
				iVertex = a2iTriangleConnectionTable[iFlagIndex][3 * iTriangle + iCorner];
				vGetColor(sColor, asEdgeVertex[iVertex], asEdgeNorm[iVertex]);
				glColor3f(sColor.fX, sColor.fY, sColor.fZ);
				glNormal3f(asEdgeNorm[iVertex].fX, asEdgeNorm[iVertex].fY, asEdgeNorm[iVertex].fZ);
				glVertex3f(asEdgeVertex[iVertex].fX, asEdgeVertex[iVertex].fY, asEdgeVertex[iVertex].fZ);
			}

		}

}

//GLfloat fGetOffset(GLfloat V1, GLfloat V2, GLfloat FT) {
//	GLdouble fDelta = V2 - V1;
//	if (fDelta==0.0)
//	{
//		return 0.5;
//	}
//	else
//	{
//		return (FT - V1) / fDelta;
//	}
//	
//}
GLfloat fGetOffset(GLfloat fValue1, GLfloat fValue2, GLfloat fValueDesired)
{
	GLdouble fDelta = fValue2 - fValue1;

	if (fDelta == 0.0)
	{
		return 0.5;
	}
	return (fValueDesired - fValue1) / fDelta;
}
GLvoid vGetNormal(GLvector &rfNormal, GLfloat fX, GLfloat fY, GLfloat fZ) {
	if (fX>1 && fX<X-1 && fY>1 && fY<Y-1 && fZ>1 && fZ<Z-1)
	{
		rfNormal.fX = (GLfloat)ptr[(int)fX - 1][(int)fY][(int)fZ] - (GLfloat)ptr[(int)fX + 1][(int)fY][(int)fZ];
		rfNormal.fY = (GLfloat)ptr[(int)fX][(int)fY-1][(int)fZ] - (GLfloat)ptr[(int)fX][(int)fY+1][(int)fZ];
		rfNormal.fZ = (GLfloat)ptr[(int)fX][(int)fY][(int)fZ-1] - (GLfloat)ptr[(int)fX][(int)fY][(int)fZ+1];
		vNormalizeVector(rfNormal, rfNormal);
	}
}
GLvoid vNormalizeVector(GLvector &rfVectorResult, GLvector &rfVectorSource)
{
	GLfloat fOldLength;
	GLfloat fScale;

	fOldLength = sqrtf((rfVectorSource.fX * rfVectorSource.fX) +
		(rfVectorSource.fY * rfVectorSource.fY) +
		(rfVectorSource.fZ * rfVectorSource.fZ));

	if (fOldLength == 0.0)
	{
		rfVectorResult.fX = rfVectorSource.fX;
		rfVectorResult.fY = rfVectorSource.fY;
		rfVectorResult.fZ = rfVectorSource.fZ;
	}
	else
	{
		fScale = 1.0 / fOldLength;
		rfVectorResult.fX = rfVectorSource.fX*fScale;
		rfVectorResult.fY = rfVectorSource.fY*fScale;
		rfVectorResult.fZ = rfVectorSource.fZ*fScale;
	}
}
//GLvoid vGetColor(GLvector &rfColor, GLvector &rfPosition, GLvector &rfNormal) {
//	GLfloat fX = rfNormal.fX;
//	GLfloat fY = rfNormal.fY;
//	GLfloat fZ = rfNormal.fZ;
//	rfColor.fX = (fX > 0.0 ? fX : 0.0) + (fY < 0.0 ? -0.5*fY : 0.0) + (fZ < 0.0 ? -0.5*fZ : 0.0);
//	rfColor.fY = (fY > 0.0 ? fY : 0.0) + (fZ < 0.0 ? -0.5*fZ : 0.0) + (fX < 0.0 ? -0.5*fX : 0.0);
//	rfColor.fZ = (fZ > 0.0 ? fZ : 0.0) + (fX < 0.0 ? -0.5*fX : 0.0) + (fY < 0.0 ? -0.5*fY : 0.0); 
//		//rfColor.fX = 1;
//		//rfColor.fY = 1;
//		//rfColor.fZ = 1;
//}

//************************************************************************************************************************************************
/*                                                             Marching Cube 计算相关函数                                                       */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//fGetOffset finds the approximate point of intersection of the surface
// between two points with the values fValue1 and fValue2
//GLfloat fGetOffset(GLfloat fValue1, GLfloat fValue2, GLfloat fValueDesired)
//{
//	GLdouble fDelta = fValue2 - fValue1;
//
//	if (fDelta == 0.0)
//	{
//		return 0.5;
//	}
//	return (fValueDesired - fValue1) / fDelta;
//}
//
//// 根据 位置 以及 法向量 产生颜色    
GLvoid vGetColor(GLvector &rfColor, GLvector &rfPosition, GLvector &rfNormal)
{
	GLfloat fX = rfNormal.fX;
	GLfloat fY = rfNormal.fY;
	GLfloat fZ = rfNormal.fZ;
	rfColor.fX = (fX > 0.0 ? fX : 0.0) + (fY < 0.0 ? -0.5*fY : 0.0) + (fZ < 0.0 ? -0.5*fZ : 0.0);
	rfColor.fY = (fY > 0.0 ? fY : 0.0) + (fZ < 0.0 ? -0.5*fZ : 0.0) + (fX < 0.0 ? -0.5*fX : 0.0);
	rfColor.fZ = (fZ > 0.0 ? fZ : 0.0) + (fX < 0.0 ? -0.5*fX : 0.0) + (fY < 0.0 ? -0.5*fY : 0.0);
}
////向量标准化
//GLvoid vNormalizeVector(GLvector &rfVectorResult, GLvector &rfVectorSource)
//{
//	GLfloat fOldLength;
//	GLfloat fScale;
//
//	fOldLength = sqrtf((rfVectorSource.fX * rfVectorSource.fX) +
//		(rfVectorSource.fY * rfVectorSource.fY) +
//		(rfVectorSource.fZ * rfVectorSource.fZ));
//
//	if (fOldLength == 0.0)
//	{
//		rfVectorResult.fX = rfVectorSource.fX;
//		rfVectorResult.fY = rfVectorSource.fY;
//		rfVectorResult.fZ = rfVectorSource.fZ;
//	}
//	else
//	{
//		fScale = 1.0 / fOldLength;
//		rfVectorResult.fX = rfVectorSource.fX*fScale;
//		rfVectorResult.fY = rfVectorSource.fY*fScale;
//		rfVectorResult.fZ = rfVectorSource.fZ*fScale;
//	}
//}
//



//vGetNormal() finds the gradient of the scalar field at a point
//This gradient can be used as a very accurate vertx normal for lighting calculations
//GLvoid vGetNormal(GLvector &rfNormal, GLfloat fX, GLfloat fY, GLfloat fZ)                   // 计算 数据场一点的梯度
//{
//	if (fX>1 && fX<X - 1 && fY>1 && fY<Y - 1 && fZ>1 && fZ<Z - 1)
//	{
//		rfNormal.fX = (GLfloat)ptr[(int)fX - 1][(int)fY][(int)fZ] - (GLfloat)ptr[(int)fX + 1][(int)fY][(int)fZ];
//		rfNormal.fY = (GLfloat)ptr[(int)fX][(int)fY - 1][(int)fZ] - (GLfloat)ptr[(int)fX][(int)fY + 1][(int)fZ];
//		rfNormal.fZ = (GLfloat)ptr[(int)fX][(int)fY][(int)fZ - 1] - (GLfloat)ptr[(int)fX][(int)fY][(int)fZ + 1];
//		vNormalizeVector(rfNormal, rfNormal);
//	}
//}

//vMarchCube1 performs the Marching Cubes algorithm on a single cube
GLvoid vMarchCube1(GLfloat fX, GLfloat fY, GLfloat fZ, GLfloat fScale)                   // Marching Cubes 算法对于一个 单个立方体
{
	extern GLint aiCubeEdgeFlags[256];
	extern GLint a2iTriangleConnectionTable[256][16];

	GLint iCorner, iVertex, iVertexTest, iEdge, iTriangle, iFlagIndex, iEdgeFlags;
	GLfloat fOffset;
	GLvector sColor;
	GLfloat afCubeValue[8];
	GLvector asEdgeVertex[12];
	GLvector asEdgeNorm[12];

	//Make a local copy of the values at the cube's corners  保存 每一个Cube的八个顶点。		
	for (iVertex = 0; iVertex < 8; iVertex++)
	{
		afCubeValue[iVertex] = (GLfloat)ptr[(int)(fX + a2fVertexOffset[iVertex][0])]
			[(int)(fY + a2fVertexOffset[iVertex][1])]
		[(int)(fZ + a2fVertexOffset[iVertex][2])];
	}

	//Find which vertices are inside of the surface and which are outside   找出哪些顶点是在等值面内，哪些是在等值面外
	iFlagIndex = 0;
	for (iVertexTest = 0; iVertexTest < 8; iVertexTest++)
	{
		if (afCubeValue[iVertexTest] <= fTargetValue)
			iFlagIndex |= 1 << iVertexTest;//左移iVertexTest位，当全部都是<=时，255,|用来恢复第一位的二进制数，1|1<<2=00000001|00000100=00000101=5
	}

	//Find which edges are intersected by the surface  找到与等值面有交点的 边
	iEdgeFlags = aiCubeEdgeFlags[iFlagIndex];

	//If the cube is entirely inside or outside of the surface, then there will be no intersections
	if (iEdgeFlags == 0)
	{
		return;
	}
	//Find the point of intersection of the surface with each edge 找到每一个与等值面相交的边，并且找到等值面在这一点的法向量
	//Then find the normal to the surface at those points
	for (iEdge = 0; iEdge < 12; iEdge++)
	{
		//if there is an intersection on this edge 如果在这一个边上有交点的话
		if (iEdgeFlags & (1 << iEdge))
		{
			fOffset = fGetOffset(afCubeValue[a2iEdgeConnection[iEdge][0]],
				afCubeValue[a2iEdgeConnection[iEdge][1]], fTargetValue);


			//计算交点的坐标
			asEdgeVertex[iEdge].fX = fX + (a2fVertexOffset[a2iEdgeConnection[iEdge][0]][0] + fOffset * a2fEdgeDirection[iEdge][0]);//* fScale;
			asEdgeVertex[iEdge].fY = fY + (a2fVertexOffset[a2iEdgeConnection[iEdge][0]][1] + fOffset * a2fEdgeDirection[iEdge][1]);//* fScale;
			asEdgeVertex[iEdge].fZ = fZ + (a2fVertexOffset[a2iEdgeConnection[iEdge][0]][2] + fOffset * a2fEdgeDirection[iEdge][2]);//* fScale;
																																   //计算交点的法向量
			vGetNormal(asEdgeNorm[iEdge], asEdgeVertex[iEdge].fX, asEdgeVertex[iEdge].fY, asEdgeVertex[iEdge].fZ);
		}
	}


	//Draw the triangles that were found.  There can be up to five per cube   画出等值面三角形 每个立方体至多 5个三角形
	for (iTriangle = 0; iTriangle < 5; iTriangle++)
	{
		if (a2iTriangleConnectionTable[iFlagIndex][3 * iTriangle] < 0)
			break;
		for (iCorner = 0; iCorner < 3; iCorner++)
		{
			iVertex = a2iTriangleConnectionTable[iFlagIndex][3 * iTriangle + iCorner];

			vGetColor(sColor, asEdgeVertex[iVertex], asEdgeNorm[iVertex]);
			glColor3f(sColor.fX, sColor.fY, sColor.fZ);
			glNormal3f(asEdgeNorm[iVertex].fX, asEdgeNorm[iVertex].fY, asEdgeNorm[iVertex].fZ);
			glVertex3f(asEdgeVertex[iVertex].fX, asEdgeVertex[iVertex].fY, asEdgeVertex[iVertex].fZ);
		}
	}
}

//vMarchingCubes iterates over the entire ptrset, calling vMarchCube on each cube
GLvoid vMarchingCubes()
{
	GLint iX, iY, iZ;
	for (iX = 0; iX < X - 1; iX++)
		for (iY = 0; iY < Y - 1; iY++)
			for (iZ = 0; iZ < Z - 1; iZ++)
			{
				MC(iX, iY, iZ, 1);
			}
	printf("finish one Draw ! !\n");
}

// For any edge, if one vertex is inside of the surface and the other is outside of the surface
//  then the edge intersects the surface
// For each of the 8 vertices of the cube can be two possible states : either inside or outside of the surface
// For any cube the are 2^8=256 possible sets of vertex states
// This table lists the edges intersected by the surface for all 256 possible vertex states
// There are 12 edges.  For each entry in the table, if edge #n is intersected, then bit #n is set to 1