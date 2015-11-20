#define _CRT_SECURE_NO_WARNINGS

#include <sstream>
#include <string>
#include <iostream>
#include <opencv\highgui.h>
#include <opencv\cv.h>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

//filtros del threshold
int H_MIN = 42;
int H_MAX = 93;
int S_MIN = 77;
int S_MAX = 154;
int V_MIN = 30;
int V_MAX = 256;


void ApicarMorfologia( Mat &thresh ){
	//crea el elemento estructural que se usara al momento de erosionar la imagen, un rectangulo 3x3
	Mat erodeElement = getStructuringElement(MORPH_RECT, Size(3, 3));
	//crea el elemento estructural que se usara al momento de dilatar la imagen, un rectangulo 8x8
	Mat dilateElement = getStructuringElement(MORPH_RECT, Size(8, 8));

	erode(thresh, thresh, erodeElement);
	erode(thresh, thresh, erodeElement);
	//morphologyEx(thresh, thresh, MORPH_CLOSE, dilateElement);
	//morphologyEx(thresh, thresh, MORPH_OPEN, erodeElement);

	dilate(thresh, thresh, dilateElement);
	dilate(thresh, thresh, dilateElement);
}

int main(int argc, char* argv[])
{
	
	Mat input;
	Mat HSV;
	Mat threshold;
	Mat CannyThresh;
	

	//video capture object to acquire webcam feed
	cv::VideoCapture capture;

	//open capture object at location zero (default location for webcam)
	capture.open(0);
	//set height and width of capture frame
	capture.set(CV_CAP_PROP_FRAME_WIDTH, 640);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
	//start an infinite loop where webcam feed is copied to cameraFeed matrix
	//all of our operations will be performed within this loop

	while (true){ 
		//lee del objeto capture (camara) y almacena la imagen en input
		capture.read(input);
		//Cambia el rango de colores de la matriz input a HSV (hue,saturation,value) y la almacena en la matriz HSV
		cvtColor(input, HSV, COLOR_BGR2HSV);
		//aplica el filtro Threshold con los valores hsv minimos y maximos y lo almacena en una matris "threshold"
		inRange(HSV, Scalar(H_MIN, S_MIN, V_MIN), Scalar(H_MAX, S_MAX, V_MAX), threshold);
		//aplica las morfologias a la imagen
		ApicarMorfologia(threshold);
		//aplicar canny edge
		Canny(threshold, CannyThresh, 100, 50);
		//aplicar houghline
		// detect lines
		std::vector<Vec2f> lines;
		HoughLines(CannyThresh, lines, 1, CV_PI/180, 100 );


		// draw lines 
	    for( size_t i = 0; i < lines.size(); i++ )
		{
			float rho = lines[i][0], theta = lines[i][1];
			Point pt1, pt2;
			double a = cos(theta), b = sin(theta);
			double x0 = a*rho, y0 = b*rho;
			pt1.x = cvRound(x0 + 1000*(-b));
			pt1.y = cvRound(y0 + 1000*(a));
			pt2.x = cvRound(x0 - 1000*(-b));
			pt2.y = cvRound(y0 - 1000*(a));
			line(input, pt1, pt2, Scalar(0, 0, 255), 3, CV_AA);
		} 
		
		//muestra en una ventana llamada "input" lo que esta contenido en la matriz input.
		//imshow("camera", CannyThresh); 
		imshow("camera", input);








        //radio de actualizacion (la imagen no se muestra sin esto)
		waitKey(30);
		
	}
return 0;
}