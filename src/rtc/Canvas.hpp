//
// Created by משתמש1 on 03/09/2020.
//

#ifndef RAY_TRACER_CANVAS_HPP
#define RAY_TRACER_CANVAS_HPP

template <class T>
struct  Canvas
{
    union {
        struct{
            int height;
            int width;
        };
        int shape[2];
    };
    int stride[2];
    T* data;
    Canvas(int width,int height):height(height),width(width),data(new T[width*height]){
//        std::cout<<"width="<<width<<"height="<<height<<std::endl;
        stride[0]=width;
        stride[1]=1;
    }
    void set_pixel(int x,int y,T item)
    {
        y=height-y;
//        printf("setting x=%d,y=%d\n",x,y);
        data[y*stride[0]+x*stride[1]]=item;
    }
    T get_pixel(int x,int y)
    {
        return data[i*stride[0]+j*stride[1]];//=item;
    }
    //todo linees on x axis only ,lines on y axis only m
    void drawline(glm::ivec2 p0,glm::ivec2 p1,glm::vec3 col)//TODO remove the branching  in the loop by implmenting better version of the algortihem
    {
        assert(p0.x<width);
        assert(p1.x<width);
        assert(p0.y<height);
        assert(p0.y<height);
        assert(p0.x>0);
        assert(p1.x>0);
        assert(p0.y>0);
        assert(p0.y>0);
        int dx=std::abs(p1.x-p0.x),sx=p0.x<p1.x ?1:-1;
        int dy=-std::abs(p1.y-p0.y),sy=p0.y<p1.y ?1:-1;
        int err=dx+dy,e2;
        while(true)
        {
            set_pixel(p0.x,p0.y,col);
            e2=2*err;
            if(e2>=dy)
            {
                if(p0.x==p1.x)break;
                err+=dy;
                p0.x+=sx;
            }
            if(e2<=dx)
            {
                if(p0.y==p1.y)break;
                err+=dx;
                p0.y+=sy;
            }
        }
    }
    void draw_pixel(glm::ivec2 point,glm::vec3 col)
    {
        if(point.x<0||point.x>this->width||point.y<0||point.y>this->height)
        {
            return;
        }
        this->set_pixel(point.x,point.y,col);
    }

    void draw_pixel(int x,int y,glm::vec3 col)
    {
        if((x<0)||(x>this->width)||(y<0)||(y>this->height))
        {
            return;
        }
        this->set_pixel(x,y,col);
    }
    void draw_circle(glm::ivec2 center,int r,glm::vec3 col,int tickness=3) {
        //radius is 4 tickness is 2 so equal so balance it draw radius 3 tickness 2
        r-=tickness/2;
        int xo = r+tickness;
        int xi = r;
        int y = 0;
        int erro = 1 - xo;
        int erri = 1 - xi;

        while(xo >= y) {
            xLine(center.x + xi, center.x + xo, center.y + y,  col);
            yLine(center.x + y,  center.y + xi, center.y + xo, col);
            xLine(center.x - xo, center.x - xi, center.y + y,  col);
            yLine(center.x - y,  center.y + xi, center.y + xo, col);
            xLine(center.x - xo, center.x - xi, center.y - y,  col);
            yLine(center.x - y,  center.y - xo, center.y - xi, col);
            xLine(center.x + xi, center.x + xo, center.y - y,  col);
            yLine(center.x + y,  center.y - xo, center.y - xi, col);
            y++;

            if (erro < 0) {
                erro += 2 * y + 1;
            } else {
                xo--;
                erro += 2 * (y - xo + 1);
            }

            if (y > r) {
                xi = y;
            } else {
                if (erri < 0) {
                    erri += 2 * y + 1;
                } else {
                    xi--;
                    erri += 2 * (y - xi + 1);
                }
            }
        }
        }

    void xLine(int x1, int x2, int y, glm::vec3 col)
    {
        if(x1<x2) {
            while (x1 <= x2) draw_pixel(x1++, y, col);
        } else
        {
            while (x2 <= x1) draw_pixel(x1--, y, col);
        }
    }
    void yLine(int x, int y1, int y2, glm::vec3 col) {
        if (y1 < y2) {
            while (y1 <= y2) draw_pixel(x, y1++, col);
        }
        else
        {
            while (y2 <= y1) draw_pixel(x, y1--, col);

        }
    }
//
//    void circle2(glm::ivec2 center, int inner, int outer, glm::vec3 color)
//    {
//        int xo = outer;
//        int xi = inner;
//        int y = 0;
//        int erro = 1 - xo;
//        int erri = 1 - xi;
//
//        while(xo >= y) {
//            xLine(xc + xi, xc + xo, yc + y,  colour);
//            yLine(xc + y,  yc + xi, yc + xo, colour);
//            xLine(xc - xo, xc - xi, yc + y,  colour);
//            yLine(xc - y,  yc + xi, yc + xo, colour);
//            xLine(xc - xo, xc - xi, yc - y,  colour);
//            yLine(xc - y,  yc - xo, yc - xi, colour);
//            xLine(xc + xi, xc + xo, yc - y,  colour);
//            yLine(xc + y,  yc - xo, yc - xi, colour);
//
//            y++;
//
//            if (erro < 0) {
//                erro += 2 * y + 1;
//            } else {
//                xo--;
//                erro += 2 * (y - xo + 1);
//            }
//
//            if (y > inner) {
//                xi = y;
//            } else {
//                if (erri < 0) {
//                    erri += 2 * y + 1;
//                } else {
//                    xi--;
//                    erri += 2 * (y - xi + 1);
//                }
//            }
//        }
//    }
//

//
//    void yLine(int x, int y1, int y2, glm::vec3 col) {
//        if (y1 < y2) {
//            while (y1 <= y2) draw_pixel(x, y1++, col);
//        }
//        else
//        {
//            while (y2 <= y1) draw_pixel(x, y1--, col);
//
//        }
//    }

private:

};

#endif //RAY_TRACER_CANVAS_HPP



/*todo
 *
 * void xLine(int x1, int x2, int y, int colour)
{
    while (x1 <= x2) setPixel(x1++, y, colour);
}

void yLine(int x, int y1, int y2, int colour)
{
    while (y1 <= y2) setPixel(x, y1++, colour);
}

void circle2(int xc, int yc, int inner, int outer, int colour)
{
    int xo = outer;
    int xi = inner;
    int y = 0;
    int erro = 1 - xo;
    int erri = 1 - xi;

    while(xo >= y) {
        xLine(xc + xi, xc + xo, yc + y,  colour);
        yLine(xc + y,  yc + xi, yc + xo, colour);
        xLine(xc - xo, xc - xi, yc + y,  colour);
        yLine(xc - y,  yc + xi, yc + xo, colour);
        xLine(xc - xo, xc - xi, yc - y,  colour);
        yLine(xc - y,  yc - xo, yc - xi, colour);
        xLine(xc + xi, xc + xo, yc - y,  colour);
        yLine(xc + y,  yc - xo, yc - xi, colour);

        y++;

        if (erro < 0) {
            erro += 2 * y + 1;
        } else {
            xo--;
            erro += 2 * (y - xo + 1);
        }

        if (y > inner) {
            xi = y;
        } else {
            if (erri < 0) {
                erri += 2 * y + 1;
            } else {
                xi--;
                erri += 2 * (y - xi + 1);
            }
        }
    }
}
 *
 * */