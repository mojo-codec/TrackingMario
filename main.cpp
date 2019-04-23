/*
 *   Tracking Mario
 *   Copyright(c) 2019 mojo, all rights reserved
 */

#include <iostream>
#include "Tracking.h"

using namespace std;

void Help()
{
    cout<<"usage:: TrackingMario graph_dir video_file"<<endl;
}
int main(int argc, char **argv) 
{
    cout << "Hello, Tracking Mario!" << endl;
    if(argc!=3)
    {
        Help();
        return -1;
    }
    
    CTracking Tracker;
    int res=Tracker.LoadGraph(argv[1]);
    if(res!=0)
    {
        cout<<"Load Graph "<<argv[1]<<" fault!!"<<endl;
        return res;
    }
    
    res=Tracker.Run(argv[2]);
    return res;
}
