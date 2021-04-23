#ifndef Graph_h
#define Graph_h

#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include "../../Rendering/3D/GameObject.h"

template <typename TGridObject, typename ... Args>
class Graph
{
public:

    Graph() 
    {

    };

    Graph(int, float) {
    
    };

    Graph(int width_, int height_, float cellSize_, vec3 originPosition_, Args&& ... args_)
    {
        width = width_;
        height = height_;
        cellSize = cellSize_;
        originPosition = originPosition_;

        for (int x = 0; x < width; x++)
        {
            std::vector<TGridObject> v1;

            for (int y = 0; y < height_; y++)
            {
                TGridObject b2(this, x, y);

                v1.push_back(b2);

            }
            gridArray.push_back(v1);
        }
    };

    void OnCreate(int width_, int height_, float cellSize_, vec3 originPosition_, Args&& ... args_) 
    {
        width = width_;
        height = height_;
        cellSize = cellSize_;
        originPosition = originPosition_;

        for (int x = 0; x < width; x++)
        {
            std::vector<TGridObject> v1;

            for (int y = 0; y < height_; y++)
            {
                TGridObject b2(this, x, y);

                v1.push_back(b2);

            }
            gridArray.push_back(v1);
        }
    };
    ~Graph() {
        
    };

    void draw_graph();

    void findNode(int x, int y) 
    {

    }
    int GetWidth() {
        return width;
    }

    int GetHeight() {
        return height;
    }

    float GetCellSize() {
        return cellSize;
    }

    std::vector<std::vector<TGridObject>> getgridArray() 
    {
        return gridArray;
    }
    
    void setObj(int x, int y, GameObject * obj_) 
    {
        gridArray[x][y].setObjRef(obj_);
    }
    
    void setWalkable(int x, int y, bool walk_) 
    {   
        gridArray[x][y].SetIsWalkable(walk_);
    };

    TGridObject * GetGridObject(int x, int y)
    {
        if (x >= 0 && y >= 0 && x < width && y < height)
        {
            return &gridArray[x][y];
        }

        return nullptr;
    }

    TGridObject * GetGridObject(GameObject * obj_)
    {
        for (int x = 0; x < width; x++)
        {
            for (int y = 0; y < height; y++)
            {
                if (gridArray[x][y].getobjRef() == obj_)
                {
                    return &gridArray[x][y];
                }
            }
        }

        return nullptr;
    }


    int GetY(vec3 worldPosition)
    {
        int y;
        return y = floor((worldPosition - originPosition).z / cellSize);
    }    
    
    int GetX(vec3 worldPosition)
    {
        int x;
        return x = floor((worldPosition - originPosition).x / cellSize);
    }


private:
    int width;
    int height;
    float cellSize;
    vec3 originPosition;

    std::vector<std::vector<TGridObject>> gridArray;


};

#endif /* Graph_h */

