#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <cstdlib>
#include <ctime>

using namespace std;

class point{
friend class k_Mean;
private:
    int x_Coordinate;
    int y_Coordinate;
    int clusterID;
    double distance;
    int min_i;

public:
    point(){
    x_Coordinate=0;
    y_Coordinate=0;
    clusterID=0;
    distance=100000000000.000;
    min_i=99;
    };

    point(int x, int y){
        x_Coordinate=x;
        y_Coordinate=y;
        clusterID=0;
        distance=1000000.000;
        min_i=0;
    }
    void printPoint(ofstream &out){
            if(clusterID!=0)
                out<<x_Coordinate<<" "<<y_Coordinate<<" "<<clusterID<<endl;
            else{
                out<<x_Coordinate<<" "<<y_Coordinate<<" "<<endl;
            }
    }
};

class k_Mean{
private:
    int k;
    point *kCentroid;
    point *pointSet;
    int numOfPoints;
    int numofRows;
    int numofColumns;
    int **imageArray;
    fstream infile;
    ofstream outfile;
    ofstream outfile2;
public:
    k_Mean(char* file, char*arg, char*arg2){
        infile.open(file);
        outfile2.open(arg2);
        outfile.open(arg);
        assignALL();
        kCentroid= new point[k];
        pointSet= new point[numOfPoints];
        imageArray= new int*[numofRows];
        for(int i=0; i<numofRows;i++){
            imageArray[i]= new int[numofColumns];
        }
        for(int i=0; i<numofRows;i++){
            for(int j=0;j<numofColumns;j++){
                imageArray[i][j]=0;
            }
        }
        loadPointSet();
        assignLable();
        mapPointtoImage();
        displayImage();
        convergence();
    }
    void assignALL(){
        int x;
        infile>>x;
        k=x;
        infile>>x;
        numOfPoints=x;
        infile>>x;
        numofRows=x;
        infile>>x;
        numofColumns=x;
    }
    void loadPointSet(){
    for(int i=0; i<numOfPoints;i++){
        int x=0;
        infile>>x;
        pointSet[i].x_Coordinate=x;
        infile>>x;
        pointSet[i].y_Coordinate=x;
        pointSet[i].printPoint(outfile);
    }
}
    void assignLable(){
        int it=0;
        srand(time(NULL));
        for(int i=0; i<numOfPoints;i++){
            pointSet[i].clusterID=rand()%k+1;
            pointSet[i].printPoint(outfile);
            it++;
            outfile<<it<<" ";
        }
    }

    void mapPointtoImage(){
    for(int i=0; i<numOfPoints;i++){
        //cout<<"Assigning"<<pointSet[i].x_Coordinate<<" & "<<pointSet[i].y_Coordinate<<"to with label "<<pointSet[i].clusterID<<endl;
        imageArray[pointSet[i].x_Coordinate][pointSet[i].y_Coordinate]=pointSet[i].clusterID;
        }
    }

    void displayImage(){
    outfile2<<"Current Image : "<<endl;
    for(int i=0;i<numofRows;i++){
        for(int j=0;j<numofColumns;j++){
            if(imageArray[i][j]!=0){
            outfile2<<imageArray[i][j]<<" ";
            }
        else
            outfile2<<" ";
        }
        outfile2<<endl;
        }
    }
    void convergence(){
    bool convergence=true;
    //int iteration=0;
    while(convergence){
         bool flag=false;
//        iteration++;
        findCenter();
        for(int i=0;i<numOfPoints;i++){
            for(int j=0; j<k;j++){
                double dis=dist(pointSet[i],kCentroid[j]);
                if(dis<pointSet[i].distance){
                    pointSet[i].distance=dis;
                    pointSet[i].min_i=(j+1);
                        }
                    if(pointSet[i].clusterID!=pointSet[i].min_i){
                        pointSet[i].clusterID = pointSet[i].min_i;
                        flag=true;
                        }
                    }//end of for j
                //pointSet[i].distance=9999;
            }//end of for i
        mapPointtoImage();
        displayImage();
       // cout<<iteration;
        outfile<<"After Assigning membership: "<<endl;
    for(int i=0;i<numOfPoints;i++){
        pointSet[i].printPoint(outfile);
        }
    //debug();
        if(flag==true)
            continue;
        else
        convergence = false;
        }//end of while convergence
   /*  for(int j=0; j<k; j++){
        imageArray[kCentroid[j].x_Coordinate][kCentroid[j].y_Coordinate]=(j+1)*10;
    }
        displayImage();*/
    }
    double dist(point p, point c){
        double d=sqrt(pow(p.x_Coordinate-c.x_Coordinate,2)+pow(p.y_Coordinate-c.y_Coordinate,2));
        return d;
    }
    void findCenter(){
        double x1=0,x2=0,x3=0,x4=0;
        double y1=0,y2=0,y3=0,y4=0;
        int member1=0, member2=0,member3=0, member4=0;
        for(int i=0;i<numOfPoints;i++){
                if(pointSet[i].clusterID==1)
                {
                    x1+=pointSet[i].x_Coordinate;
                    y1+=pointSet[i].y_Coordinate;
                    member1++;
                }
               else if(pointSet[i].clusterID==2){
                    x2+=pointSet[i].x_Coordinate;
                    y2+=pointSet[i].y_Coordinate;
                    member2++;
                }
               else if(pointSet[i].clusterID==3){
                    x3+=pointSet[i].x_Coordinate;
                    y3+=pointSet[i].y_Coordinate;
                    member3++;
                }
               else{
                    x4+=pointSet[i].x_Coordinate;
                    y4+=pointSet[i].y_Coordinate;
                    member4++;
                }

        }
               if(member1==0){
                kCentroid[0].x_Coordinate=0;
                kCentroid[0].y_Coordinate=0;
                }
                if(member2==0){
                kCentroid[1].x_Coordinate=0;
                kCentroid[1].y_Coordinate=0;
                }
                if(member3==0){
                kCentroid[2].x_Coordinate=0;
                kCentroid[2].y_Coordinate=0;
                }
                if(member4==0){
                kCentroid[3].x_Coordinate=0;
                kCentroid[3].y_Coordinate=0;
                }
                kCentroid[0].x_Coordinate=(x1/member1);
                kCentroid[0].y_Coordinate=(y1/member1);
                kCentroid[1].x_Coordinate=(x2/member2);
                kCentroid[1].y_Coordinate=(y2/member2);
                kCentroid[2].x_Coordinate=(x3/member3);
                kCentroid[2].y_Coordinate=(y3/member3);
                kCentroid[3].x_Coordinate=(x4/member4);
                kCentroid[3].y_Coordinate=(y4/member4);
       /* for(int i=0;i<k;i++){
            cout<<kCentroid[i].x_Coordinate<< " "<<kCentroid[i].y_Coordinate<<endl;
        }
        cout<<endl;*/
    }
 /*   void debug(){
    cout<<imageArray[9][72]<<"HELOO!!!!";
    }*/

    ~k_Mean(){
        infile.close();
        outfile.close();
        outfile2.close();
        delete []kCentroid;
        delete []pointSet;
        for(int i=0; i<numofRows;i++){
            delete [] imageArray[i];
        }
        delete []imageArray;
    }
};


int main(int argc, char* argv[]){

k_Mean trial(argv[1],argv[2],argv[3]);
return 0;
}
