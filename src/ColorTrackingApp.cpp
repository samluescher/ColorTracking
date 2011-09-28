#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "CinderOpenCV.h"
#include "cinder/Capture.h"
#include "cinder/gl/Texture.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class ColorTrackingApp : public AppBasic {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
    void HSVImageToMask(IplImage * image, cv::Mat * mask);
    
	Capture mCap;
	gl::Texture mTexture;
};

void ColorTrackingApp::setup()
{
	try {
		mCap = Capture( 640, 480 );
		mCap.start();
	}
	catch( ... ) {
		console() << "Failed to initialize capture" << std::endl;
	}    
}

void hsvImageToMask(IplImage * image, cv::Mat * mask)
/* mask should be the same size as image, and of type CV_8UC1            */
/* e.g. cvMat * mask = cvCreateMat(image->width, image->height, CV8UC1); */
{
    double hMin = 6;
    double hMax = 36;
    double sMin = 10;  /* not sure what value you need */
    double sMax = 245; /* not sure what value you need */
    double vMin = 0;
    double vMax = 255;
    
    CvScalar hsvMin = cvScalar(hMin, sMin, vMin);
    CvScalar hsvMax = cvScalar(hMax, sMax, vMax);
    
    cvInRangeS(image, hsvMin, hsvMax, mask);
}

void ColorTrackingApp::mouseDown( MouseEvent event )
{
}

void ColorTrackingApp::update()
{
	if( mCap && mCap.checkNewFrame() ) {
		cv::Mat input( toOcv( mCap.getSurface() ) ), hsv, thresholded;
        
        cv::cvtColor(input, hsv, CV_RGB2HSV);
        hsvImageToMask(hsv, thresholded);

		mTexture = gl::Texture( fromOcv( thresholded ) );
	}	 
}

void ColorTrackingApp::draw()
{
	gl::clear();
	if( mTexture )
		gl::draw( mTexture );
}


CINDER_APP_BASIC( ColorTrackingApp, RendererGl )
