/*******************************************************************************
 *                                                                              *
 *   PrimeSense NITE 1.3 - Point Viewer Sample                                  *
 *   Copyright (C) 2010 PrimeSense Ltd.                                         *
 *                                                                              *
 *******************************************************************************/

// Headers for OpenNI
#include <openni/XnOpenNI.h>
#include <openni/XnCppWrapper.h>
#include <openni/XnHash.h>
#include <openni/XnLog.h>
#include <opencv2/opencv.hpp>
#include <nite/XnVNite.h>
#include "PointDrawer.h"

#define fx_rgb 5.2921508098293293e+02
#define fy_rgb 5.2556393630057437e+02
#define cx_rgb 3.2894272028759258e+02
#define cy_rgb 2.6748068171871557e+02
#define k1_rgb 2.6451622333009589e-01
#define k2_rgb -8.3990749424620825e-01
#define p1_rgb -1.9922302173693159e-03
#define p2_rgb 1.4371995932897616e-03
#define k3_rgb 9.1192465078713847e-01

#define fx_d 5.9421434211923247e+02
#define fy_d 5.9104053696870778e+02
#define cx_d 3.3930780975300314e+02
#define cy_d 2.4273913761751615e+02
#define k1_d -2.6386489753128833e-01
#define k2_d 9.9966832163729757e-01
#define p1_d -7.6275862143610667e-04
#define p2_d 5.0350940090814270e-03
#define k3_d -1.3053628089976321e+00


double T[]={0.019985242312092553, -0.00074423738761617583,-0.010916736334336222};
double R[]={0.9998462882657779,0.0012635359098409581,-0.017487233004436643,
		-0.0014779096108364480,0.99992385683542895,-0.012251380107679535,
		0.017470421412464927,0.012275341476520762,0.99977202419716948};




#define CHECK_RC(rc, what)											\
		if (rc != XN_STATUS_OK)											\
		{																\
			printf("%s failed: %s\n", what, xnGetStatusString(rc));		\
			return rc;													\
		}

#define CHECK_ERRORS(rc, errors, what)		\
		if (rc == XN_STATUS_NO_NODE_PRESENT)	\
		{										\
			XnChar strError[1024];				\
			errors.ToString(strError, 1024);	\
			printf("%s\n", strError);			\
			return (rc);						\
		}


#include <GL/glut.h>

#include "signal_catch.h"

using namespace cv;

// OpenNI objects
xn::Context g_Context;
xn::ScriptNode g_ScriptNode;
xn::DepthGenerator g_DepthGenerator;
xn::ImageGenerator g_ImageGenerator;
xn::HandsGenerator g_HandsGenerator;
xn::GestureGenerator g_GestureGenerator;
xn::DepthMetaData depthMD;
xn::ImageMetaData imageMD;

// NITE objects
XnVSessionManager* g_pSessionManager;
XnVFlowRouter* g_pFlowRouter;

// the drawer
XnVPointDrawer* g_pDrawer;

#define GL_WIN_SIZE_X 720
#define GL_WIN_SIZE_Y 480

// Draw the depth map?
XnBool g_bDrawDepthMap = true;
XnBool g_bPrintFrameID = false;
// Use smoothing?
XnFloat g_fSmoothing = 0.0f;
XnBool g_bPause = false;
XnBool g_bQuit = false;

SessionState g_SessionState = NOT_IN_SESSION;


double vectors_dot_prod(const double *x, const double *y, int n)
{
	double res = 0.0;
	int i;
	for (i = 0; i < n; i++)
	{
		res += x[i] * y[i];
	}
	return res;
}

void matrix_vector_mult(const double **mat, const double *vec, double *result, int rows, int cols)
{ // in matrix form: result = mat * vec;
	int i;
	for (i = 0; i < rows; i++)
	{
		result[i] = vectors_dot_prod(mat[i], vec, cols);
	}
}



void CleanupExit()
{
	g_ScriptNode.Release();
	g_DepthGenerator.Release();
	g_HandsGenerator.Release();
	g_GestureGenerator.Release();
	g_Context.Release();

	exit (1);
}

// Callback for when the focus is in progress
void XN_CALLBACK_TYPE FocusProgress(const XnChar* strFocus, const XnPoint3D& ptPosition, XnFloat fProgress, void* UserCxt)
{
	//	printf("Focus progress: %s @(%f,%f,%f): %f\n", strFocus, ptPosition.X, ptPosition.Y, ptPosition.Z, fProgress);
}
// callback for session start
void XN_CALLBACK_TYPE SessionStarting(const XnPoint3D& ptPosition, void* UserCxt)
{
	printf("Session start: (%f,%f,%f)\n", ptPosition.X, ptPosition.Y, ptPosition.Z);
	g_SessionState = IN_SESSION;
}
// Callback for session end
void XN_CALLBACK_TYPE SessionEnding(void* UserCxt)
{
	printf("Session end\n");
	g_SessionState = NOT_IN_SESSION;
}
void XN_CALLBACK_TYPE NoHands(void* UserCxt)
{
	if (g_SessionState != NOT_IN_SESSION)
	{
		printf("Quick refocus\n");
		g_SessionState = QUICK_REFOCUS;
	}
}

void XN_CALLBACK_TYPE TouchingCallback(xn::HandTouchingFOVEdgeCapability& generator, XnUserID id, const XnPoint3D* pPosition, XnFloat fTime, XnDirection eDir, void* pCookie)
{
	g_pDrawer->SetTouchingFOVEdge(id);
}
/*
void XN_CALLBACK_TYPE MyGestureInProgress(xn::GestureGenerator& generator, const XnChar* strGesture, const XnPoint3D* pPosition, void* pCookie)
{
    printf("Gesture %s in progress\n", strGesture);
}
void XN_CALLBACK_TYPE MyGestureReady(xn::GestureGenerator& generator, const XnChar* strGesture, const XnPoint3D* pPosition, void* pCookie)
{
    printf("Gesture %s ready for next stage\n", strGesture);
}
 */

// this function is called each frame
void glutDisplay (void)
{

	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Setup the OpenGL viewpoint
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	XnMapOutputMode mode;
	g_DepthGenerator.GetMapOutputMode(mode);

	glOrtho(0, mode.nXRes, mode.nYRes, 0, -1.0, 1.0);

	glDisable(GL_TEXTURE_2D);

	if (!g_bPause)
	{
		// Read next available data
		g_Context.WaitOneUpdateAll(g_DepthGenerator);
		// Update NITE tree
		g_pSessionManager->Update(&g_Context);

		PrintSessionState(g_SessionState);

	}

	glutSwapBuffers();

}

void glutIdle (void)
{
	if (g_bQuit) {
		CleanupExit();
	}

	// Display the frame
	glutPostRedisplay();
}

void glutKeyboard (unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		// Exit
		CleanupExit();
		break;
	case'p':
		// Toggle pause
		g_bPause = !g_bPause;
		break;
	case 'd':
		// Toggle drawing of the depth map
		g_bDrawDepthMap = !g_bDrawDepthMap;
		g_pDrawer->SetDepthMap(g_bDrawDepthMap);
		break;
	case 'f':
		g_bPrintFrameID = !g_bPrintFrameID;
		g_pDrawer->SetFrameID(g_bPrintFrameID);
		break;
	case 's':
		// Toggle smoothing
		if (g_fSmoothing == 0)
			g_fSmoothing = 0.1;
		else
			g_fSmoothing = 0;
		g_HandsGenerator.SetSmoothing(g_fSmoothing);
		break;
	case 'e':
		// end current session
		g_pSessionManager->EndSession();
		break;
	}
}
void glInit (int * pargc, char ** argv)
{
	glutInit(pargc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(GL_WIN_SIZE_X, GL_WIN_SIZE_Y);
	glutCreateWindow ("PrimeSense Nite Point Viewer");
	glutFullScreen();
	glutSetCursor(GLUT_CURSOR_NONE);

	glutKeyboardFunc(glutKeyboard);
	glutDisplayFunc(glutDisplay);
	glutIdleFunc(glutIdle);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	glEnableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}

void XN_CALLBACK_TYPE GestureIntermediateStageCompletedHandler(xn::GestureGenerator& generator, const XnChar* strGesture, const XnPoint3D* pPosition, void* pCookie)
{
	printf("Gesture %s: Intermediate stage complete (%f,%f,%f)\n", strGesture, pPosition->X, pPosition->Y, pPosition->Z);
}
void XN_CALLBACK_TYPE GestureReadyForNextIntermediateStageHandler(xn::GestureGenerator& generator, const XnChar* strGesture, const XnPoint3D* pPosition, void* pCookie)
{
	printf("Gesture %s: Ready for next intermediate stage (%f,%f,%f)\n", strGesture, pPosition->X, pPosition->Y, pPosition->Z);
}
void XN_CALLBACK_TYPE GestureProgressHandler(xn::GestureGenerator& generator, const XnChar* strGesture, const XnPoint3D* pPosition, XnFloat fProgress, void* pCookie)
{
	printf("Gesture %s progress: %f (%f,%f,%f)\n", strGesture, fProgress, pPosition->X, pPosition->Y, pPosition->Z);
}


// xml to initialize OpenNI
#define SAMPLE_XML_PATH "Sample-Tracking.xml"

int main(int argc, char ** argv)
{
	XnStatus rc = XN_STATUS_OK;
	xn::EnumerationErrors errors;

	// Initialize OpenNI
	rc = g_Context.InitFromXmlFile(SAMPLE_XML_PATH, g_ScriptNode, &errors);
	CHECK_ERRORS(rc, errors, "InitFromXmlFile");
	CHECK_RC(rc, "InitFromXmlFile");

	rc = g_Context.FindExistingNode(XN_NODE_TYPE_IMAGE, g_ImageGenerator);
	CHECK_RC(rc, "Find image generator");
	rc = g_Context.FindExistingNode(XN_NODE_TYPE_DEPTH, g_DepthGenerator);
	CHECK_RC(rc, "Find depth generator");
	rc = g_Context.FindExistingNode(XN_NODE_TYPE_HANDS, g_HandsGenerator);
	CHECK_RC(rc, "Find hands generator");
	rc = g_Context.FindExistingNode(XN_NODE_TYPE_GESTURE, g_GestureGenerator);
	CHECK_RC(rc, "Find gesture generator");

	XnCallbackHandle h;
	if (g_HandsGenerator.IsCapabilitySupported(XN_CAPABILITY_HAND_TOUCHING_FOV_EDGE))
	{
		g_HandsGenerator.GetHandTouchingFOVEdgeCap().RegisterToHandTouchingFOVEdge(TouchingCallback, NULL, h);
	}

	XnCallbackHandle hGestureIntermediateStageCompleted, hGestureProgress, hGestureReadyForNextIntermediateStage;
	g_GestureGenerator.RegisterToGestureIntermediateStageCompleted(GestureIntermediateStageCompletedHandler, NULL, hGestureIntermediateStageCompleted);
	g_GestureGenerator.RegisterToGestureReadyForNextIntermediateStage(GestureReadyForNextIntermediateStageHandler, NULL, hGestureReadyForNextIntermediateStage);
	g_GestureGenerator.RegisterGestureCallbacks(NULL, GestureProgressHandler, NULL, hGestureProgress);


	// Create NITE objects
	g_pSessionManager = new XnVSessionManager;
	rc = g_pSessionManager->Initialize(&g_Context, "Click,Wave", "RaiseHand");
	CHECK_RC(rc, "SessionManager::Initialize");

	g_pSessionManager->RegisterSession(NULL, SessionStarting, SessionEnding, FocusProgress);

	g_pDrawer = new XnVPointDrawer(20, g_DepthGenerator);
	g_pFlowRouter = new XnVFlowRouter;
	g_pFlowRouter->SetActive(g_pDrawer);

	g_pSessionManager->AddListener(g_pFlowRouter);

	g_pDrawer->RegisterNoPoints(NULL, NoHands);
	g_pDrawer->SetDepthMap(g_bDrawDepthMap);

	// Initialization done. Start generating
	rc = g_Context.StartGeneratingAll();
	CHECK_RC(rc, "StartGenerating");

	cv::Mat depthshow;
	int key_pre = 0 ;

	for( ; ; )
	{
		if (key_pre == 27)
			break ;
		XnMapOutputMode mode;
		g_DepthGenerator.GetMapOutputMode(mode);

		//g_Context.WaitOneUpdateAll(g_DepthGenerator);
		g_Context.WaitAnyUpdateAll();
		g_DepthGenerator.GetMetaData(depthMD);
		g_ImageGenerator.GetMetaData(imageMD);

		XnDepthPixel* pDepth = depthMD.WritableData();


		// Update NITE tree
		g_pSessionManager->Update(&g_Context);

		cv::Mat reprojected=cv::Mat::ones(480,640,CV_8UC1);
		////////////////////////////////////////
		//IF I FOUND THE HAND
		////////////////////////////////////////
		if(g_pDrawer->handFound)
		{
			XnRGB24Pixel* pRGB = imageMD.WritableRGB24Data();

			for (XnUInt y = 0; y < depthMD.YRes(); ++y)
			{
				for (XnUInt x = 0; x < depthMD.XRes(); ++x, ++pDepth,++pRGB)
				{

					if (*pDepth>g_pDrawer->pt3D.Z+50 || *pDepth<g_pDrawer->pt3D.Z-50)
					{
						*pDepth=0;
						pRGB->nBlue=0;
						pRGB->nRed=255;
						pRGB->nGreen=0;

					}

				}
			}




			//REPROJECTION

			XnDepthPixel* pDepthIterator = depthMD.WritableData();

			int depth;
			int ppX;
			int ppY;
			int ppZ;

			int ppXp;
			int ppYp;
			int ppZp;

			int P2D_rgbX;
			int P2D_rgbY;


			for(int i=0;i<480;i++)
			{


				for(int j=0;j<640;j++,++pDepthIterator)
				{
					depth=*pDepthIterator;
					ppX = (j - cx_d)*depth/fx_d;
					ppY = (i - cy_d)*depth/fy_d;
					ppZ = depth;

					ppXp = (ppX*R[0]+ppY*R[1]+ppZ*R[2]) + T[0];
					ppYp = (ppX*R[3]+ppY*R[4]+ppZ*R[5]) + T[1];
					ppZp = (ppX*R[6]+ppY*R[7]+ppZ*R[8]) + T[2];

					P2D_rgbX = (ppXp * fx_rgb / ppZp) + cx_rgb;
					P2D_rgbY = (ppYp * fy_rgb / ppZp) + cy_rgb;

					if(P2D_rgbY<480 && P2D_rgbX<640 && P2D_rgbY!=0 && P2D_rgbX!=0 && P2D_rgbY>0 && P2D_rgbX>0)
					{

						reprojected.ptr<uchar>(P2D_rgbY)[P2D_rgbX]=255;
					}
				}
			}
		}


		////////////////////////////////////////
		////////////////////////////////////////

		//for opencv Mat
		cv::Mat depth16(480,640,CV_16SC1,(unsigned short*)depthMD.WritableData());

		cv::Mat colorArr[3];
		cv::Mat colorImage;




		const XnRGB24Pixel* pImageRow;
		const XnRGB24Pixel* pPixel;
		pImageRow = imageMD.RGB24Data();


		colorArr[0] = cv::Mat(imageMD.YRes(),imageMD.XRes(),CV_8U);
		colorArr[1] = cv::Mat(imageMD.YRes(),imageMD.XRes(),CV_8U);
		colorArr[2] = cv::Mat(imageMD.YRes(),imageMD.XRes(),CV_8U);

		for (int y=0; y<imageMD.YRes(); y++)
		{
			pPixel = pImageRow;
			uchar* Bptr = colorArr[0].ptr<uchar>(y);
			uchar* Gptr = colorArr[1].ptr<uchar>(y);
			uchar* Rptr = colorArr[2].ptr<uchar>(y);
			for(int x=0;x<imageMD.XRes();++x , ++pPixel)
			{
				Bptr[x] = pPixel->nBlue;
				Gptr[x] = pPixel->nGreen;
				Rptr[x] = pPixel->nRed;
			}
			pImageRow += imageMD.XRes();
		}
		cv::merge(colorArr,3,colorImage);




		depth16.convertTo(depthshow, CV_8UC1, 0.025);

		if(g_pDrawer->handFound)
		{
			cv::Point handPos(g_pDrawer->ptProjective.X, g_pDrawer->ptProjective.Y);
			cv::circle(depthshow, handPos, 5, cv::Scalar(0xffff), 5, 8, 0);
			Mat src_gray;
			int thresh = 10;
			RNG rng(12345);

			blur( depthshow, src_gray, Size(3,3) );

			Mat threshold_output;
			vector<vector<Point> > contours;
			vector<Vec4i> hierarchy;

			/// Detect edges using Threshold
			threshold( src_gray, threshold_output, thresh, 255, THRESH_BINARY );

			/// Find contours
			findContours( threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

			/// Find the convex hull object for each contour
			vector<vector<int> > hullsI (contours.size());
			vector<vector<Point> >	hullsP (contours.size() );
			vector<vector<Vec4i> > 	defects (contours.size() );

			for( int i = 0; i < (int)contours.size(); i++ )
			{
				convexHull(contours[i], hullsI[i], false, false);
				convexHull(contours[i], hullsP[i], false, true);
				if (contours[i].size() >3 )
				{
					convexityDefects(contours[i], hullsI[i], defects[i]);
				}
			}

			/// Draw contours + hull results
			Mat drawing = Mat::zeros( threshold_output.size(), CV_8UC3 );
			vector <Vec4i> biggest (4);
			Vec4i initVec(0, 0, 0, 0);
			biggest[0] = initVec;
			biggest[1] = initVec;
			biggest[2] = initVec;
			biggest[3] = initVec;
			//std::cout << "{";
			for( int i = 0; i < (int)defects.size(); i++)
			{
				//std::cout << "[";
				for(int j = 0; j < (int)defects[i].size(); j++)
				{
					//std::cout << "(" << defects[i][j][0] << " " << defects[i][j][1] << " " << defects[i][j][2] << " " << defects[i][j][3] << " " << std::endl;
					Vec4i defect(defects[i][j]);
					Point start(contours[i][defect[0]]);
					Point end(contours[i][defect[1]]);
					Point far(contours[i][defect[2]]);
					int tmp = defect[3]; //distanza minima necessaria per rimuovere punti troppo vicini
					if(tmp > 1700)
					{
						line(drawing, start, handPos, Scalar( 255, 255, 0 ), 1);
						line(drawing, far, handPos, Scalar( 255, 255, 0 ), 1);
						line(drawing, far, start, Scalar( 255, 0, 255 ), 1);
						circle(drawing, far, 2, Scalar( 0, 0, 255 ), 5, 8, 0);		//difetto
						circle(drawing, start, 2, Scalar( 0, 255, 255 ), 5, 8, 0);	//fingertip
						//circle(drawing, end, 2, Scalar( 0, 0, 255 ), 5, 8, 0);	//inutile
					}
				}
				//std::cout << "]" << std::endl;
			}
			//std::cout << "}" << std::endl;

			for( int i = 0; i < (int)contours.size(); i++ )
			{
				drawContours( drawing, contours, i, Scalar( 0, 255, 0 ), 1, 8, vector<Vec4i>(), 0, Point() );
				//drawContours( drawing, hullsP, i, Scalar( 255, 0, 0 ), 1, 8, vector<Vec4i>(), 0, Point() );
			}

			/// Show in a window
			namedWindow( "Hull demo", CV_WINDOW_AUTOSIZE );
			imshow( "Hull demo", drawing );
			cv::imshow("Repro", reprojected);

		}

		cv::imshow("RGB", colorImage);
		cv::imshow("Depth", depthshow);


		key_pre = cv::waitKey(33);

	}
	cv::destroyWindow("PrimeSense Nite Point Viewer");
	CleanupExit();
	return 0;
}
