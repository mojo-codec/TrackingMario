/*
 *   Tracking Mario
 *   Copyright(c) 2019 mojo, all rights reserved
 */

#include <tensorflow/core/public/session.h>
#include <tensorflow/core/protobuf/saved_model.pb.h>
#include <tensorflow/core/public/session_options.h>
#include <tensorflow/cc/saved_model/loader.h>
#include <tensorflow/cc/saved_model/tag_constants.h>

#include <opencv2/opencv.hpp>

#include "Tracking.h"

using namespace std;
using namespace tensorflow;
using namespace cv;

CRectObj::CRectObj()
{
    m_ID=m_x=m_y=m_width=m_height=0;
    m_confidence=0.0f;
}
CRectObj::CRectObj(int ID,int x,int y,int w,int h,float confidence)
{
    m_ID=ID;
    m_x=x;
    m_y=y;
    m_width=w;
    m_height=h;
    m_confidence=confidence;
}
   
CRectObj::~CRectObj()
{
}

CTracking::CTracking()
{
    
}
CTracking::~CTracking()
{
}
int CTracking::LoadGraph(const string &tfGraphDir)
{
    Status s;
    SessionOptions session_options;
    RunOptions run_options;
    s=tensorflow::LoadSavedModel(session_options, run_options, tfGraphDir,
                                {kSavedModelTagServe}, &m_bundle);
    TF_CHECK_OK(s);
    return 0;
}

int CTracking::Run(const string &VideoFile)
{
    VideoCapture vc;
    vc.open(VideoFile);
    if(!vc.isOpened())
    {
        cout<<"Open "<<VideoFile<<" fault!!"<<endl;
        return -1;
    }
    
    Tensor InTensor(DT_FLOAT,{1,224,256,3});
    vector<Tensor> outputs;
    string  InTensorName="INPUT2018/Placeholder",
            OutTensorName="Concat_2";
    do
    {
        Mat frame;
        vc>>frame;
        if(frame.empty())
            break;
        
        float *pIn=InTensor.flat<float>().data();
        Mat tmp_mat(224,256,CV_32FC3,pIn);
        frame.convertTo(tmp_mat,CV_32FC3);
        
        Get_Session()->Run({{InTensorName,InTensor}},
                           {OutTensorName},{},&outputs);
        
        const float *pRcnnOut=outputs[0].flat<float>().data();
        vector<CRectObj> ImageObject;
        Get_result(pRcnnOut,ImageObject);
        
        for(auto &obj:ImageObject)
        {
            Rect r(obj.Get_x(),obj.Get_y(),obj.Get_width(),obj.Get_height());
            Scalar color(0,255,0);
            if(obj.Get_ID()==1 || obj.Get_ID()==2) 
                color=Scalar(0,0,255);
            else if(obj.Get_ID()==5 || obj.Get_ID()==8 || obj.Get_ID()==13)
                color=Scalar(255,0,0);
            rectangle(frame,r,color,2);
        }
        
        imshow("Window",frame);
        int key=cv::waitKey(5);
        if(key=='q')
            break;
    }while(true);
    
    vc.release();
    return 0;
}

int CTracking::Get_result(const float *pRcnnOut,vector<CRectObj> &ImageObject)
{
    constexpr int   BG_ID=0,
                    BoxNum=1240,
                    class_num=14;
    constexpr float prob_th=0.6f,
                    math_r_th=0.3f;
    const float   *pClassify=pRcnnOut,
                  *pPos=pRcnnOut+BoxNum*class_num;

    for(int i=0;i<BoxNum;i++)
    {
        const int id=class_out(pClassify,class_num);
        const float Prob=pClassify[id];
        if(id!=BG_ID && Prob>prob_th)
        {
            CRectObj obj(id,pPos[0],pPos[1],pPos[2]-pPos[0],pPos[3]-pPos[1],Prob);
            ImageObject.push_back(obj);
        }
        
        pClassify+=class_num;
        pPos+=4;
    }
    
    return ImageObject.size();
}

int CTracking::class_out(const float *pPro,int class_num)
{
    int c=0;
    float pro=-numeric_limits< float >::max();
    for(int i=0;i<class_num;i++)
    {
        if(pPro[i]>pro)
        {
            pro=pPro[i];
            c=i;
        }
    }
    return c;
}
