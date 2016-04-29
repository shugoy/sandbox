// The contents of this file are in the public domain. See LICENSE_FOR_EXAMPLE_PROGRAMS.txt
/*

 
 This example program shows how to find frontal human faces in an image and
 estimate their pose.  The pose takes the form of 68 landmarks.  These are
 points on the face such as the corners of the mouth, along the eyebrows, on
 the eyes, and so forth.
 
 
 This example is essentially just a version of the face_landmark_detection_ex.cpp
 example modified to use OpenCV's VideoCapture object to read from a camera instead
 of files.
 
 
 Finally, note that the face detector is fastest when compiled with at least
 SSE2 instructions enabled.  So if you are using a PC with an Intel or AMD
 chip then you should enable at least SSE2 instructions.  If you are using
 cmake to compile this program you can enable them by using one of the
 following commands when you create the build project:
 cmake path_to_dlib_root/examples -DUSE_SSE2_INSTRUCTIONS=ON
 cmake path_to_dlib_root/examples -DUSE_SSE4_INSTRUCTIONS=ON
 cmake path_to_dlib_root/examples -DUSE_AVX_INSTRUCTIONS=ON
 This will set the appropriate compiler options for GCC, clang, Visual
 Studio, or the Intel compiler.  If you are using another compiler then you
 need to consult your compiler's manual to determine how to enable these
 instructions.  Note that AVX is the fastest but requires a CPU from at least
 2011.  SSE4 is the next fastest and is supported by most current machines.
 */


#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

//#include <dlib/opencv.h>
//#include <dlib/image_processing/frontal_face_detector.h>
//#include <dlib/image_processing/render_face_detections.h>
//#include <dlib/image_processing.h>
//#include <dlib/gui_widgets.h>
#include "dlib/opencv.h"
#include "dlib/image_processing/frontal_face_detector.h"
#include "dlib/image_processing/render_face_detections.h"
#include "dlib/image_processing.h"
#include "dlib/gui_widgets.h"

using namespace dlib;
using namespace std;

namespace my {
    
    void cross(cv::Mat& dst, cv::Point pt, double length, cv::Scalar color, double angle = 45.0, double thickness = 1) {
        
        double theta = M_PI / 180.0 * angle;
        double lx = length/2.0 * cos(theta);
        double ly = length/2.0 * sin(theta);
        
        cv::Point pt1(pt.x + lx, pt.y + ly );
        cv::Point pt2(pt.x - lx, pt.y - ly );
        cv::line(dst, pt1, pt2, color);
        
        pt1 = cv::Point(pt.x - lx, pt.y + ly );
        pt2 = cv::Point(pt.x + lx, pt.y - ly );
        cv::line(dst, pt1, pt2, color, thickness);
        
    };
    
    cv::Point ptd2cv(dlib::full_object_detection shape, unsigned long i) {
        return cv::Point(shape.part(i).x(), shape.part(i).y());
    };
    
    int draw_shapes(cv::Mat &dst, full_object_detection shape) {
        
        const int size = shape.num_parts();
        
        cv::Scalar blue(255,0,0);
        cv::Scalar green(0,255,0);
        cv::Scalar red(0,0,255);
        
        if (size == 68) {
            cv::Point pt1,pt2;
            
            for (unsigned long i = 0; i < 16; ++i)
                cv::line(dst, my::ptd2cv(shape,i), my::ptd2cv(shape,i+1), green);
            
            for (unsigned long i = 27; i < 30; ++i)
                cv::line(dst, my::ptd2cv(shape,i), my::ptd2cv(shape,i+1), green);
            
            for (unsigned long i = 17; i < 21; ++i)
                cv::line(dst, my::ptd2cv(shape,i), my::ptd2cv(shape,i+1), green);
            
            for (unsigned long i = 22; i < 26; ++i)
                cv::line(dst, my::ptd2cv(shape,i), my::ptd2cv(shape,i+1), green);
            
            for (unsigned long i = 30; i < 35; ++i)
                cv::line(dst, my::ptd2cv(shape,i), my::ptd2cv(shape,i+1), green);
            cv::line(dst, my::ptd2cv(shape,35), my::ptd2cv(shape,30), green);
            
            for (unsigned long i = 36; i < 41; ++i)
                cv::line(dst, my::ptd2cv(shape,i), my::ptd2cv(shape,i+1), green);
            cv::line(dst, my::ptd2cv(shape,41), my::ptd2cv(shape,36), green);
            
            for (unsigned long i = 42; i < 47; ++i)
                cv::line(dst, my::ptd2cv(shape,i), my::ptd2cv(shape,i+1), green);
            cv::line(dst, my::ptd2cv(shape,47), my::ptd2cv(shape,42), green);
            
            for (unsigned long i = 48; i < 59; ++i)
                cv::line(dst, my::ptd2cv(shape,i), my::ptd2cv(shape,i+1), green);
            cv::line(dst, my::ptd2cv(shape,59), my::ptd2cv(shape,48), green);
            
            for (unsigned long i = 60; i < 67; ++i)
                cv::line(dst, my::ptd2cv(shape,i), my::ptd2cv(shape,i+1), green);
            cv::line(dst, my::ptd2cv(shape,67), my::ptd2cv(shape,60), green);
            
            // landmarks
            for (unsigned long i = 0; i <= 67; ++i)
                my::cross(dst, my::ptd2cv(shape,i), 10, red);
            
        }
        else {
            std::cout << "the dat cannot be applied" << std::endl;
        }
        
        return 0;
    };
    
}

int main(int argc, char* argv[])
{
    try
    {
        cv::VideoCapture cap;
        if (argc == 1) {
            cap = cv::VideoCapture(0);
        }
        else if (argc == 2) {
            cap = cv::VideoCapture(argv[1]);
        }
        
        // Load face detection and pose estimation models. //
        frontal_face_detector detector = get_frontal_face_detector();
        shape_predictor pose_model;
        //pose_model.
        proxy_deserialize hoge = deserialize("shape_predictor_68_face_landmarks.dat");
        hoge >> pose_model;
        //deserialize("shape_predictor_68_face_landmarks.dat") >> pose_model;
        
        cv::namedWindow("result");
        
        // Grab and process frames until the main window is closed by the user. //
        while(1)
        {
            
            // Grab a frame //
            cv::Mat temp, frame;
            cap >> frame;
            if (frame.empty()) break;
            cv::Mat dst = frame.clone();
            
            // in order to shorten the time of face rectangle detector, reduce the size of image //
            int ratio = 2;
            cv::resize(frame, temp, cv::Size(), 1.0/ratio, 1.0/ratio);
            
            cv_image<bgr_pixel> cimg(frame);
            cv_image<bgr_pixel> cimg_temp(temp);
            
            clock_t start = clock();
            
            // Detect faces //
            std::vector<rectangle> faces = detector(cimg_temp);
            
            clock_t end = clock();
            std::cout << "HOG: " << (double)(end-start)/CLOCKS_PER_SEC*1000 << "ms, ";

            for (unsigned long i = 0; i < faces.size(); ++i) {
                faces[i].set_top(faces[i].top() * ratio);
                faces[i].set_left(faces[i].left() * ratio);
                faces[i].set_right(faces[i].right() * ratio);
                faces[i].set_bottom(faces[i].bottom() * ratio);
            }

            
            // Find the pose of each face.
            std::vector<full_object_detection> shapes;
            for (unsigned long i = 0; i < faces.size(); ++i)
                shapes.push_back(pose_model(cimg, faces[i]));
            
            clock_t end2 = clock();
            std::cout << "cascade regressor: " << (double)(end2-end)/CLOCKS_PER_SEC*1000 << "ms ";
            
            // Display it all on the screen
            for (unsigned long i = 0; i < faces.size(); ++i) {
                cv::rectangle(dst,
                              cv::Point(faces[i].left(), faces[i].top()),
                              cv::Point(faces[i].right(), faces[i].bottom()),
                              cv::Scalar(255,0,0));
                my::draw_shapes(dst, shapes[i]);
            }
            //cv::resize(dst, temp, cv::Size(frame.cols/ratio,frame.rows/ratio));
            cv::imshow("result", dst);
            clock_t end3 = clock();
            std::cout << "imshow: " << (double)(end3-end2)/CLOCKS_PER_SEC*1000 << "ms ";
            std::cout << std::endl;
            if (cv::waitKey(1) == 27) {
                break;
            }
            
        }
    }
    catch(serialization_error& e)
    {
        cout << "You need dlib's default face landmarking model file to run this example." << endl;
        cout << "You can get it from the following URL: " << endl;
        cout << "   http://dlib.net/files/shape_predictor_68_face_landmarks.dat.bz2" << endl;
        cout << endl << e.what() << endl;
    }
    catch(exception& e)
    {
        cout << e.what() << endl;
    }
}

