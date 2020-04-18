#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Sphere {
    public:
        // the vertices are in world coordinates directly
        int vertex_count;
        int circular_segs;
        GLuint VAO;
        GLuint VBO;
        float *vertices;
        float *tex_coords;
        int no_of_vertices;
        float R;
        Sphere(float radius, int circular_segs = 200){
            this->circular_segs = circular_segs;
            vertex_count = 3*circular_segs*circular_segs;
            R = radius;
            genVertices();
            bind();
        }
        void bind(){
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);

            glBindVertexArray(VAO); 
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float)*15*circular_segs*circular_segs, NULL, GL_STATIC_DRAW);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float)*9*circular_segs*circular_segs, vertices);
            glBufferSubData(GL_ARRAY_BUFFER,  sizeof(float)*9*circular_segs*circular_segs, sizeof(float)*6*circular_segs*circular_segs, tex_coords);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)(sizeof(float)*9*circular_segs*circular_segs));
            glEnableVertexAttribArray(1);
        }
        void genVertices(){
            vertices = new float[9*circular_segs*circular_segs];
            tex_coords = new float[6*circular_segs*circular_segs];
            float pi = atan(1)*4;
            float increm = 2*pi/circular_segs;
            float tex_increm = 1.0f/circular_segs;
            int j=0, l = 0;
            for (int i=0; i<circular_segs; ++i){
                float azimuth = i*increm;
                float x = i*tex_increm;
                // rcosthet=z, rsinthetcosphi=x, rsinthetsinphi=y
                int k = 0; float angle = 0;
                while(k<9*circular_segs){
                    vertices[j] = R*sin(angle)*cos(azimuth);j++;k++;
                    vertices[j] = R*cos(angle);j++;k++;
                    vertices[j] = R*sin(angle)*sin(azimuth);j++;k++;
                    tex_coords[l] = 1-x; l++;
                    tex_coords[l] = (angle)/(pi); l++;
                    vertices[j] = R*sin(angle+increm)*cos(azimuth); j++;k++;
                    vertices[j] = R*cos(angle+increm);j++;k++;
                    vertices[j] = R*sin(angle+increm)*sin(azimuth); j++;k++;
                    tex_coords[l] = 1-x; l++;
                    tex_coords[l] = ((angle+increm))/(pi); l++;
                    vertices[j] = R*sin(angle+increm)*cos(azimuth+increm); j++;k++;
                    vertices[j] = R*cos(angle+increm);j++;k++;
                    vertices[j] = R*sin(angle+increm)*sin(azimuth+increm); j++;k++;
                    tex_coords[l] = 1-(x+tex_increm); l++;
                    tex_coords[l] = ((angle+increm))/(pi); l++;
                    vertices[j] = R*sin(angle)*cos(azimuth);j++;k++;
                    vertices[j] = R*cos(angle);j++;k++;
                    vertices[j] = R*sin(angle)*sin(azimuth);j++;k++;
                    tex_coords[l] = 1-x; l++;
                    tex_coords[l] = (angle)/(pi); l++;
                    vertices[j] = R*sin(angle)*cos(azimuth+increm); j++;k++;
                    vertices[j] = R*cos(angle);j++;k++;
                    vertices[j] = R*sin(angle)*sin(azimuth+increm); j++;k++;
                    tex_coords[l] = 1-(x+tex_increm); l++;
                    tex_coords[l] = ((angle))/(pi); l++;
                    vertices[j] = R*sin(angle+increm)*cos(azimuth+increm); j++;k++;
                    vertices[j] = R*cos(angle+increm);j++;k++;
                    vertices[j] = R*sin(angle+increm)*sin(azimuth+increm); j++;k++;
                    tex_coords[l] = 1-(x+tex_increm); l++;
                    tex_coords[l] = ((angle+increm))/(pi); l++;
                    angle += increm;
                }
            } 
        }
};