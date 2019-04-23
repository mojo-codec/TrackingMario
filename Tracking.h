/*
 *   Tracking Mario
 *   Copyright(c) 2019 mojo, all rights reserved
 */

#ifndef _TRACKING_H
#define _TRACKING_H

#include <string>

#include <tensorflow/cc/saved_model/loader.h>

class CRectObj
{
public:    
    CRectObj();
    CRectObj(int ID,int x,int y,int w,int h,float confidence); 
    ~CRectObj();  
    
    int  Get_ID()const       {  return m_ID;}
    int  Get_x()const        {  return m_x;}
    int  Get_y()const        {  return m_y;}
    int  Get_width()const    {  return m_width;}
    int  Get_height()const   {  return m_height;}
    float Get_confidence()const{   return m_confidence;}
    
    void Set_ID(int v)       {   m_ID=v;}
    void Set_x(int v)        {   m_x=v;}
    void Set_y(int v)        {   m_y=v;}
    void Set_width(int v)    {   m_width=v;}
    void Set_height(int v)   {   m_height=v;}
    void Set_confidence(float v){   m_confidence=v;}
 
protected:
    float m_confidence;
    int m_ID,m_x,m_y,m_width,m_height;
};

class CTracking
{
public:
    CTracking();
    ~CTracking();
    
    int LoadGraph(const std::string &tfGraphDir);
    int Run(const std::string &VideoFile);
    
protected:
    tensorflow::Session* Get_Session()  {   return m_bundle.session.get();}
    int Get_result(const float *pRcnnOut,std::vector<CRectObj> &ImageObject);
    int class_out(const float *pPro,int class_num);
protected:    
    tensorflow::SavedModelBundle m_bundle;
    
};

#endif

