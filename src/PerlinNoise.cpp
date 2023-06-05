#include "PerlinNoise.hpp"
#define PI 3.14159

PerlinNoise::PerlinNoise(int kernelx, int kernely, int width, int height){

    PerlinNoise::kernelx = kernelx;
    PerlinNoise::kernely = kernely;
    PerlinNoise::height = height;
    PerlinNoise::width = width;

    //initalize randomset from which grid vectors chosen
    PerlinNoise::randomSet.push_back({1, 1});
    PerlinNoise::randomSet.push_back({-1, 1});
    PerlinNoise::randomSet.push_back({1, -1});
    PerlinNoise::randomSet.push_back({-1, -1});

    int nx = width/kernelx+1;
    int ny = height/kernely+1;

    std::vector<std::vector<double>> temp;

    for(int i=0; i<nx; i++){
        for(int j=0; j<ny; j++){
            temp.push_back(PerlinNoise::makeRandom());
            // std::cout<<"{"<<PerlinNoise::makeRandom()[0]<<", "<<PerlinNoise::makeRandom()[1]<<"}\n"; //STUPID DEBUG STATEMENT
        }
        PerlinNoise::grid.push_back(temp);
        temp.clear();
    }

    // std::cout<<PerlinNoise::grid.size(); //DEBUG
}

std::vector<double> PerlinNoise::makeRandom(){
    return PerlinNoise::randomSet[rand()%4];
}

double PerlinNoise::dotProduct(int a, int b, int i, int j){
    //calculate distance vector from pixel
    double dx = static_cast<double>(PerlinNoise::kernelx)*static_cast<double>(a);
    double dy = static_cast<double>(PerlinNoise::kernely)*static_cast<double>(b);
    dx = (static_cast<double>(i)-dx)/static_cast<double>(PerlinNoise::kernelx);
    dy = (static_cast<double>(j)-dy)/static_cast<double>(PerlinNoise::kernely);

    //find dot product and return
    return PerlinNoise::grid[a][b][0]*dx + PerlinNoise::grid[a][b][1]*dy;
}

double PerlinNoise::linearInterpolation(double y1, double y2, double mu){
    return(y1*(1-mu)+y2*mu);
}

double PerlinNoise::cosineInterpolation(double y1, double y2, double mu){
    double mu2;
    mu2 = (1-cos(mu*PI))/2;
    return(y1*(1-mu2)+y2*mu2);
}

double PerlinNoise::map(double value){
    double median = 255/2;
    return median+median*value;
}

double PerlinNoise::smoothstep(double value){
    return (1/(1+exp(-1*value)));
}

void PerlinNoise::generatePerlin(){

    std::vector<double> dotproduct;
    std::vector<int> pixeltemp;

    for(int i=0; i<PerlinNoise::width; i++){
        for(int j=0; j<PerlinNoise::height; j++){

            //get the four vectors needed + distance vector
            int a = i/PerlinNoise::kernelx;
            int b = j/PerlinNoise::kernely;

            // //calculate the dot products
            // std::cout<<PerlinNoise::dotProduct(a, b, i, j)<<" "; //DEBUG
            dotproduct.push_back(PerlinNoise::dotProduct(a, b, i, j));
            dotproduct.push_back(PerlinNoise::dotProduct(a+1, b, i, j));
            dotproduct.push_back(PerlinNoise::dotProduct(a, b+1, i, j));
            dotproduct.push_back(PerlinNoise::dotProduct(a+1, b+1, i, j));

            //interpolate
            double dx = static_cast<double>(PerlinNoise::kernelx)*static_cast<double>(a);
            double dy = static_cast<double>(PerlinNoise::kernely)*static_cast<double>(b);
            dx = (static_cast<double>(i)-dx)/static_cast<double>(PerlinNoise::kernelx);
            dy = (static_cast<double>(j)-dy)/static_cast<double>(PerlinNoise::kernely);
  
            double ab = PerlinNoise::cosineInterpolation(dotproduct[0], dotproduct[1], dx);
            double cd = PerlinNoise::cosineInterpolation(dotproduct[2], dotproduct[3], dx);
            double value = PerlinNoise::cosineInterpolation(ab, cd, dy);
            // std::cout<<value<<" "; //DEBUG

            //apply FADE function
            // value = PerlinNoise::fade(value);                                                                                                                                                                                                                                                  

            //scale value 0-255 and store in `pixels`
            value = map(value);
            // std::cout<<static_cast<int>(value)<<" "; //DEBUG 
            pixeltemp.push_back(static_cast<int>(value));

            dotproduct.clear();
        }

        pixels.push_back(pixeltemp);
        pixeltemp.clear();
    }
}

