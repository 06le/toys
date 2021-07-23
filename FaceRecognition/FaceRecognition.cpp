#include <opencv2/opencv.hpp>
#include "opencv2/core.hpp"
#include "opencv2/face.hpp" 
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <io.h>
#include <direct.h> //_mkdir _rmkdir

using namespace cv;
using namespace cv::face;
using namespace std;

string haar_face_datapath = "C:/study/opencv_compile/install/etc/haarcascades/haarcascade_frontalface_alt.xml";

int checkFolder(string Folder);
int faceDataCollect(string DataPath);
int CSVAppend(string DataPath, string CSVFliename, int label);
void read_csv(const string& filename, vector<Mat>& images, vector<int>& labels, char separator = ';');
Mat norm_0_255(InputArray _src);
void trainData(string CSVPath, string XMLPath);
double facerecognition(string XMLPath);
double getLabel(string XMLPath);


int main()
{
    /*Test1 人脸数据收集*/
    // argv 数据集文件夹 
    //faceDataCollect("C:/study/facedata/66");

    /*Test2 准备人脸数据CSV文件*/
    // argv 数据集文件夹 保存数据集信息的csv文件路径  标签
    //CSVAppend("C:/study/facedata/66", "C:/study/facedata/sta.csv",66);

    /*Test3 训练数据 保存训练的xml文件*/
    // argv CSV文件路径 存放训练好的xml文件路径
    //trainData("C:/study/facedata/sta.csv", "C:/study/facedata/test.xml");

    /*Test4 根据训练的xml文件识别人脸*/
    //成功返回人物对应label，失败返回-1
    double result = facerecognition("C:/study/facedata/test.xml");
    //cout << "result: " << result;

    /*Test5 检测人脸返回标签*/
    //成功返回人物对应label，失败返回-1
    //double label = getLabel("C:/study/facedata/test.xml");
    
}

//检查给定路径文件夹是否存在
int checkFolder(string folder)  
{
    if (_access(folder.c_str(), 00) != -1)
        return 1;
    else
        if (_mkdir(folder.c_str()) == 0)
            return 1;
        else
            return -1;
}

int faceDataCollect(string dataPath)
{
    Mat frame;
    vector<Rect> faces;
    int count = 0;

    CascadeClassifier faceDetector;  //人脸检测器
    faceDetector.load(haar_face_datapath);

    checkFolder(dataPath);  

    VideoCapture capture(0);
    if (!capture.isOpened())
    {
        cout << "could not open camera...\n";
        return -1;
    }

    while (capture.read(frame))
    {
        flip(frame, frame, 1);

        faceDetector.detectMultiScale(frame, faces, 1.1, 1, 0, Size(100, 120), Size(380, 400));
        for (int i = 0; i < faces.size(); i++)
        {
            if (count % 5 == 0 )
            {
                Mat dst;
                //resize(frame(faces[i]), dst, Size(100, 100)); //更改人脸照片尺寸
                resize(frame(faces[i]), dst, Size(112, 92));
                string path = dataPath + "/";
                path += to_string(count);
                path += ".jpg";
                imwrite(path, dst); //将照片写入文件夹
            }
            rectangle(frame, faces[i], Scalar(0, 0, 255), 2, 8, 0);  //标记脸
        }
        imshow("camera-demo", frame);
        char c = waitKey(10);
        if (c == 27)
        {
            break;
        }
        count++;
    }

    capture.release();
    waitKey(0);
    return 1;
}

//将人脸数据集加到CSV文件，默认一次添加一个人
int CSVAppend(string DataPath, string CSVFliename, int label)
{
    vector<string> filesList;
    ofstream file_out(CSVFliename, ofstream::out | ofstream::app);

    struct _finddata_t fileinfo;
    intptr_t hFile;
    string aim = DataPath + "/*";

    if ((hFile = _findfirst(aim.c_str(), &fileinfo)) == -1L)
        cout << "No file in current directory!\n";
    else
    {
        do {
            string name = DataPath;
            name = name + "/" + fileinfo.name + ";" + to_string(label);
            filesList.push_back(name);
        } while (_findnext(hFile, &fileinfo) == 0);
        _findclose(hFile);
    }

    //读取文件列表开始有两行 C:/study/facedata/sta.csv/. C:/study/facedata/sta.csv/.. 不知道怎么去除，所以i从filesList[2]开始
    for (size_t i = 2; i < filesList.size(); i++)  
    {
        file_out << filesList[i].c_str() << endl;
    }
    file_out.close();

    return 1;
}

void read_csv(const string& filename, vector<Mat>& images, vector<int>& labels, char separator) {
    std::ifstream file(filename.c_str(), ifstream::in);
    if (!file) {
        string error_message = "No valid input file was given, please check the given filename.";
        CV_Error(Error::StsBadArg, error_message);
    }
    string line, path, classlabel;
    while (getline(file, line)) {
        stringstream liness(line);
        getline(liness, path, separator);
        getline(liness, classlabel);
        if (!path.empty() && !classlabel.empty()) {
            images.push_back(imread(path, 0));
            labels.push_back(atoi(classlabel.c_str()));
        }
    }
}

Mat norm_0_255(InputArray _src) {
    Mat src = _src.getMat();
    // Create and return normalized image:
    Mat dst;
    switch (src.channels()) {
    case 1:
        cv::normalize(_src, dst, 0, 255, NORM_MINMAX, CV_8UC1);
        break;
    case 3:
        cv::normalize(_src, dst, 0, 255, NORM_MINMAX, CV_8UC3);
        break;
    default:
        src.copyTo(dst);
        break;
    }
    return dst;
}

void trainData(string CSVPath, string XMLPath)
{
    vector<Mat> images;
    vector<int> labels;
    read_csv(CSVPath, images, labels);

    if (images.size() < 1 || labels.size() < 1)
    {
        string error_message = "This demo needs at least 2 images to work. Please add more images to your data set!";
        CV_Error(Error::StsError, error_message);
    }

    Mat testSample = images[images.size() - 1];
    int testLabel = labels[labels.size() - 1];
    images.pop_back();
    labels.pop_back();

    //训练数据
    Ptr<FisherFaceRecognizer> model = FisherFaceRecognizer::create();
    model->train(images, labels);
    model->write(XMLPath); //保存训练数据
}

double facerecognition(string XMLPath)
{
    //加载人脸识别器
    //Ptr<BasicFaceRecognizer> model_test = EigenFaceRecognizer::create();
    Ptr<FisherFaceRecognizer> model_test = FisherFaceRecognizer::create();
    model_test->read(XMLPath);

    //加载人脸检测分类器
    CascadeClassifier faceDetector;
    faceDetector.load(haar_face_datapath);

    //检测传入的摄像头
    VideoCapture capture(0);
    if (!capture.isOpened())
    {
        std::cerr << "could not open camera...！" << endl;
        return -1;
    }
    Mat frame;
    namedWindow("faceRecognition", WINDOW_AUTOSIZE);
    vector<Rect> faces;
    Mat dst;
    Mat test_sample;
    string name;
     
    int count = 0;
    double temlabel = -1;

    while (capture.read(frame) && count < 45)
    {
        flip(frame, frame, 1);

        faceDetector.detectMultiScale(frame, faces, 1.1, 1, 0, Size(100, 120), Size(380, 400));

        for (int i = 0; i < faces.size(); i++)
        {
            Mat roi = frame(faces[i]);
            cvtColor(roi, dst, COLOR_BGR2GRAY);
            resize(dst, test_sample, Size(112, 92));

            int predicted_label = -1;
            double predicted_confidence = 0.0;
            //cout << test_sample.rows << " " << test_sample.cols;
            model_test->predict(test_sample, predicted_label, predicted_confidence);

            cout << "predicted_label: " <<  predicted_label << " confidence: " << predicted_confidence << endl;

            rectangle(frame, faces[i], Scalar(255, 0, 0), 2, 8, 0);
            if (predicted_confidence > 3000)
            {
                name = "Unknown";
                temlabel = -1;
            }
            else
            {
                switch (predicted_label)
                {
                case 46:
                    name = "zengjun";
                    break;
                default:
                    name = "Unknown";
                    break;
                }
                temlabel = predicted_label;
            }
            putText(frame, name, faces[i].tl(), FONT_HERSHEY_PLAIN, 1.0, Scalar(0, 0, 255), 2, 8);
        }

        imshow("faceRecognition", frame);
        count++;
        char c = waitKey(10);
        //if (c == 81)
        //{
        //    break;
        //}
    }
    return temlabel;
}

double getLabel(string XMLPath)
{
    //加载人脸识别器
    Ptr<BasicFaceRecognizer> model_test = EigenFaceRecognizer::create();
    model_test->read(XMLPath);

    //加载人脸检测分类器
    CascadeClassifier faceDetector;
    faceDetector.load(haar_face_datapath);

    //检测传入的摄像头
    VideoCapture capture(0);
    if (!capture.isOpened())
    {
        std::cerr << "could not open camera...！" << endl;
        return -1.0;
    }

    Mat frame;
    vector<Rect> faces;
    Mat dst;
    Mat test_sample;
    string name;

    while (capture.read(frame))
    {
        flip(frame, frame, 1);

        faceDetector.detectMultiScale(frame, faces, 1.1, 1, 0, Size(100, 120), Size(380, 400));

        for (int i = 0; i < faces.size(); i++)
        {
            Mat roi = frame(faces[i]);
            cvtColor(roi, dst, COLOR_BGR2GRAY);
            resize(dst, test_sample, Size(112, 92));

            int predicted_label = -1;
            double predicted_confidence = 0.0;  //置信度，一般小于80 下面由于人脸数据不够完善设的值比较大
            model_test->predict(test_sample, predicted_label, predicted_confidence);

            if (predicted_confidence > 3000)
            {
                return -1.0;
            }
            else
            {
                return predicted_label;
            }   
        }
    }
    return -1.0;
}

