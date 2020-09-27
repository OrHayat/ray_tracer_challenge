


#define DOCTEST_CONFIG_IMPLEMENT
#define GLM_FORCE_SWIZZLE
#define GLM_FORCE_CTOR_INIT
#define  GLM_ENABLE_EXPERIMENTAL


#include <rtc/core/ray.hpp>
#include <rtc/core/collision_data.hpp>
#include <rtc/core/sphere.hpp>
#include <rtc/Camera.hpp>
#include <rtc/Scene.hpp>
#include <doctest/doctest.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>


int main(int argc, char** argv) {
    doctest::Context context;

    // !!! THIS IS JUST AN EXAMPLE SHOWING HOW DEFAULTS/OVERRIDES ARE SET !!!

    // defaults
//    context.addFilter("test-case-exclude", "*math*"); // exclude test cases with "math" in their name
    context.setOption("abort-after", 5);              // stop test execution after 5 failed assertions
    context.setOption("order-by", "name");            // sort the test cases by their name
    context.setOption("no_colors", false);
    context.applyCommandLine(argc, argv);

    // overrides
    context.setOption("no-breaks", true);             // don't break in the debugger when assertions fail

    int res = context.run(); // run

    if(context.shouldExit()) // important - query flags (and --exit) rely on the user doing this
        return res;          // propagate the result of the tests

    int client_stuff_return_code = 0;
    // your program - if the testing framework is integrated in your production code

    return res + client_stuff_return_code; // the result from doctest is propagated here as well
}
//

namespace glm {
// Writes a generic GLM vector type to a stream.
    template<int D, typename T, glm::qualifier P>
    std::ostream &operator<<(std::ostream &os, const glm::vec<D, T, P> &v) {
        return os << glm::to_string(v);
    }
    template<int C,int R, typename T, glm::qualifier P>
    std::ostream &operator<<(std::ostream &os, const glm::mat<C,R, T, P> &mat) {
        return os << glm::to_string(mat);
    }
    template<int count, typename T, glm::precision P>
    bool operator==(const glm::vec<count, T, P> &v1, const glm::vec<count, doctest::Approx, P> &v2) {
        for (int i = 0; i < count; i++) {
            if (v1[i] != v2[i]) {
                return false;
            }
        }
        return true;
    }
}

bool operator == (const material& mat1,const material& mat2)
{
    return (doctest::Approx(mat1.shininess)==mat2.shininess)
    &&mat1.ka==mat2.ka&&mat1.kd==mat2.kd&&mat1.kr==mat2.kr&&mat1.ks==mat2.ks&&mat1.kt==mat2.kt;
}
bool operator != (const material& mat1,const material& mat2)
{
    return !(mat1==mat2);
}

bool operator == (const light& light1,const light& light2)
{
    if(light1.type!=light2.type)
    {
        return false;
    }
    if(light1.intensity!=light2.intensity)
    {
        return false;
    }
    if(light1.dir!=light2.dir)
    {
        return false;
    }
    if(light1.pos!=light2.pos)
    {
        return false;
    }
    if(light1.cutoff_angel!=doctest::Approx(light2.cutoff_angel))
    {
        return false;
    }
    if(light1.kc!=doctest::Approx(light2.kc))
    {
        return false;
    }
    if(light1.kl!=doctest::Approx(light2.kl))
    {
        return false;
    }
    if(light1.kq!=doctest::Approx(light2.kq))
    {
        return false;
    }
    return true;
}
bool operator != (const light& light1,const light& light2)
{
    return !(light1==light2);
}
bool operator==(const sphere& s1, const sphere& s2) {
    if(s1.model!=s2.model)
    {
        return false;
    }
    if(s1.model_inv!=s2.model_inv)
    {
        return false;
    }
    if(s1.center!=s2.center)
    {

        return false;
    }
    if(s1.radius!=doctest::Approx(s2.radius))
    {
        return false;
    }
    if(s1.mat!=s2.mat)
    {
        return false;
    }
    return true;
}


std::ostream &operator<<(std::ostream &os, const material s) {
    return os<<"material(ka:"<<s.ka<<"kd:"<<s.kd<<",ks:"<<s.ks<<"\nkr:"<<s.kr<<",kt:"<<s.kt<<",shininess:"<<s.shininess;
}


std::ostream &operator<<(std::ostream &os, const light& l) {
   return os<<"light(type:"<<l.type<<", intensity:"<<l.intensity<<",pos\n"<<l.pos<<",direction:"<<l.dir<<",cutoff_angle:"<<l.cutoff_angel<<"\nkc:"<<l.kc<<",kl:"<<l.kl<<",kq:"<<l.kq<<")";
}


std::ostream &operator<<(std::ostream &os, const sphere& s) {
    return os<<"sphere(model:"<<s.model<<"\nmodel_inv:"<<s.model_inv<<"\ncenter:"<<s.center<<",radius:"<<s.radius<<"\nmat:"<<s.mat;
}
Scene default_scene()
{
    Scene res;
    light* l=light::make_point_light_ptr(glm::vec3(-10,10,-10));
    l->intensity=glm::vec4(1.0f);
    res.lights.push_back(l);
    sphere* s=new sphere();
    s->mat.ka=glm::vec4(0.8f,1.0f,0.6f,1.0f);
    s->mat.kd=glm::vec4(0.7f,0.7f,0.7f,1.0f);
    s->mat.ks=glm::vec4(0.2f,0.2f,0.2,1.0f);
    sphere* s2=new sphere();
    s2->set_model(glm::scale(glm::mat4(),glm::vec3(0.5f)));
    res.objects.push_back(s);
    res.objects.push_back(s2);
    res.I_ambient=glm::vec4(1.0f);
    return  res;
}

TEST_SUITE("scene") {
    SCENARIO ("Creating a scene") {
                GIVEN(" s ← scene()") {
            Scene s;
                    THEN("s contains no objects") {
                        REQUIRE_EQ(0, s.objects.size());
                        AND_THEN(" s has no light source") {
                            REQUIRE_EQ(0, s.lights.size());
                }
            }
        }
    }


    SCENARIO ("The default world")
    {
        GIVEN(" light ← point_light(point(-10, 10, -10), color(1, 1, 1))")
        {
            light *point_light = light::make_point_light_ptr(glm::vec3(-10, 10, -10));
            point_light->intensity=glm::vec4(1.0f);
            AND_WHEN("s1 ← sphere() with:| material.color=(0.8, 1.0, 0.6,1) ,material.diffuse=(0.7,0.7,0.7,1),.material.specular(0.2,0.2,0.2,1)")
            {
                sphere s1;
                s1.mat.ka=glm::vec4(0.8,1.0f,0.6,1.0f);
                s1.mat.kd=glm::vec4(0.7f,0.7f,0.7f,1.0f);
                s1.mat.ks=glm::vec4(0.2f,0.2f,0.2f,1.0f);
                AND_WHEN("s2 ← sphere() with:transform | scaling(0.5, 0.5, 0.5) |")
                {
                    sphere s2;
                    s2.set_model(glm::scale(glm::vec3(0.5f)));
                    WHEN("w ← default_world()")
                    {
                        Scene s=default_scene();
                        REQUIRE_EQ(s.objects.size(),2);
                                THEN ("s.light = light")
                        {
                                    REQUIRE_EQ(s.lights.size(),1);
                                    REQUIRE_EQ(*s.lights.at(0),*point_light);
                        }
                         THEN("s.objects contain s1 ")
                        {
                            bool found=false;
                            for(unsigned int i=0;(!found)&&i<s.objects.size();i++)
                            {
                                sphere* tmp=dynamic_cast<sphere*>(s.objects.at(i));
                                if(tmp!=nullptr)
                                {
                                    found= (*tmp==s1);
                                }
                            }
                        REQUIRE(found);
                        }
                        THEN("s.objects contain s2 ")
                        {
                            bool found_s2=false;
                            for(unsigned int i=0;!found_s2&&i<s.objects.size();i++)
                            {
                                sphere* tmp=dynamic_cast<sphere*>(s.objects.at(i));
                                if(tmp!=nullptr)
                                {
                                    found_s2= (*tmp==s2);
                                }
                            }
                                    REQUIRE(found_s2);
                        }
                    }
                }
            }
        }
    }






    SCENARIO("Intersect a world with a ray")
    {
        GIVEN("w ← default_world()")
        {
            Scene w=default_scene();
            AND_WHEN("r ← ray(point(0, 0, -5), vector(0, 0, 1))")
            {
                ray r=ray(glm::vec3(0,0,-5),glm::vec3(0,0,1));
                {
                    WHEN("xs ← intersect_world(w, r)")
                    {
                        collision_with_scene_result xs=w.scene_ray_collide(r);
                        THEN("xs.count==4")
                        {
                            REQUIRE_EQ(xs.data.size(),4);
                            AND_THEN("xs[0].t==4")
                            {
                                REQUIRE_EQ(xs[0].t,doctest::Approx(4.0f));
                            }
                                    AND_THEN("xs[1].t==4.5")
                            {
                                REQUIRE_EQ(xs[1].t,doctest::Approx(4.5f));
                            }
                                    AND_THEN("xs[2].t==5.5")
                            {
                                REQUIRE_EQ(xs[2].t,doctest::Approx(5.5f));

                            }
                                    AND_THEN("xs[3].t==6")
                            {
                                REQUIRE_EQ(xs[3].t,doctest::Approx(6.0f));
                            }
                        }
                    }
                }
            }
        }
    }

}




TEST_SUITE("glm") {
    SCENARIO ("Reflecting a vector off a slanted surface") {
        GIVEN("v ← vector(0, -1, 0)")
        {
            glm::vec3 v(0,-1,0);
            AND_THEN("n ← vector(√2/2, √2/2, 0) ")
            {
                glm::vec3 n=glm::vec3(glm::sqrt(2.0f)/2.0f,glm::sqrt(2.0f)/2.0f,0.0f);
                WHEN("r ← reflect(v, n)")
                {
                    glm::vec3 r=glm::reflect(v,n);
                    THEN(" r == vector(1, 0, 0)")
                    {
                        REQUIRE_EQ(doctest::Approx(1.0f),r.x);
                        REQUIRE_EQ(doctest::Approx(0.0f),r.y);
                        REQUIRE_EQ(doctest::Approx(0.0f),r.z);
                    }
                }
            }
        }
    }
}

TEST_SUITE("sphere") {
    SCENARIO ("A default sphere") {
                GIVEN("s<- sphere()") {
            sphere s = sphere();
                    THEN("s.radius=1") {
                        REQUIRE_EQ(s.radius, doctest::Approx(1.0f));
            }
                    THEN("S.model is identity") {
                        REQUIRE_EQ(s.model, glm::mat4());
            }
        }
    }

    SCENARIO ("Intersecting scaled sphere with a ray") {
                GIVEN("r=ray(point(0,0,-5),vector(0,0,1))") {
            ray r = ray(glm::vec3(0, 0, -5), glm::vec3(0, 0, 1));
                    AND_WHEN("s=sphere") {
                sphere s = sphere();
                        WHEN("set transform s scale(2,2,2)") {
                    s.set_model(glm::scale(s.model, glm::vec3(2, 2, 2)));
                            AND_WHEN("xs=intersect(r,s)") {
                        collision_data xs = s.collide(r);
                                THEN("xs.t.size()==2") {
                                    REQUIRE_EQ(2, xs.t.size());
                                    AND_THEN("xs.t[0]==3") {
                                        REQUIRE_EQ(3, xs.t.at(0));
                            }
                                    AND_THEN("xs.t[1]==7") {
                                        REQUIRE_EQ(7, xs.t.at(1));
                            }
                        }
                    }
                }
            }
        }
    }


    SCENARIO ("Intersecting translated sphere with a ray") {
                GIVEN("r=ray(point(0,0,-5),vector(0,0,1))") {
            ray r = ray(glm::vec3(0, 0, -5), glm::vec3(0, 0, 1));
                    AND_WHEN("s=sphere") {
                sphere s = sphere();
                        WHEN("set transform s translate(5,0,0)") {
                    s.set_model(glm::translate(s.model, glm::vec3(5, 0, 0)));
                            AND_WHEN("xs=intersect(r,s)") {
                        collision_data xs = s.collide(r);
                                THEN("xs.t.size()==0") {
                                    REQUIRE_EQ(0, xs.t.size());
                        }
                    }
                }
            }
        }
    }

    SCENARIO ("normal on a sphere at a point on x axis") {
                GIVEN("s=sphere()") {
            sphere s;
                    WHEN("n=normal at(s,point(1,0,0))") {
                glm::vec3 n = s.get_normal_at_point(glm::vec3(1, 0, 0));
                        THEN("n=vector(1,0,0)") {
                            REQUIRE_EQ(n, glm::vec3(1, 0, 0));
                }
            }
        }
    }


    SCENARIO ("normal on a sphere at a point on y axis") {
                GIVEN("s=sphere()") {
            sphere s;
                    WHEN("n=normal at(s,point(0,1,0))") {
                glm::vec3 n = s.get_normal_at_point(glm::vec3(0, 1, 0));
                        THEN("n=vector(0,1,0)") {
                            REQUIRE_EQ(n, glm::vec3(0, 1, 0));
                }
            }
        }
    }


    SCENARIO ("normal on a sphere at a point on z axis") {
                GIVEN("s=sphere()") {
            sphere s;
                    WHEN("n=normal at(s,point(0,0,1))") {
                glm::vec3 n = s.get_normal_at_point(glm::vec3(0, 0, 1));
                        THEN("n=vector(0,0,1)") {
                            REQUIRE_EQ(n, glm::vec3(0, 0, 1));
                }
            }
        }
    }

    SCENARIO ("normal on a sphere at a nonaxial point") {
                GIVEN("s=sphere()") {
            sphere s;
                    WHEN("n=normal at(s,point(sqrt(3)/3,sqrt(3)/3,sqrt(3)/3)") {
                glm::vec3 n = s.get_normal_at_point(glm::vec3(glm::sqrt(3) / 3.0f));
                        THEN("n=vector(sqrt(3)/3,sqrt(3)/3,sqrt(3)/3)") {
                            REQUIRE_EQ(n,glm::vec3(glm::sqrt(3.0f)/3.0f));
                            REQUIRE_EQ(n.x, doctest::Approx(glm::sqrt(3.0f) / 3.0f));
                            REQUIRE_EQ(n.y, doctest::Approx(glm::sqrt(3.0f) / 3.0f));
                            REQUIRE_EQ(n.z, doctest::Approx(glm::sqrt(3.0f) / 3.0f));
                }
            }
        }
    }

    SCENARIO ("sphere normal is normalized vector") {
                GIVEN("s=sphere()") {
            sphere s;
                    WHEN("n=normal at(s,point(sqrt(3)/3,sqrt(3)/3,sqrt(3)/3)") {
                glm::vec3 n = s.get_normal_at_point(glm::vec3(glm::sqrt(3) / 3.0f));
                        THEN("n=normalized(n)") {
                    glm::vec3 norm_n = glm::normalize(n);
                            REQUIRE(n.x == doctest::Approx(norm_n.x));
                            REQUIRE(n.y == doctest::Approx(norm_n.y));
                            REQUIRE(n.z == doctest::Approx(norm_n.z));
                }
            }
        }
    }


    SCENARIO ("Computing normal on translated sphere") {
                GIVEN("s=sphere()") {
            sphere s;
                    AND_THEN("set transform (s,tranlate(0,1,0))") {
                s.set_model(glm::translate(s.model, glm::vec3(0, 1, 0)));
                        WHEN("n=normal_at(s,point(0,1.70711,-0.70711)") {
                    glm::vec3 n = s.get_normal_at_point(glm::vec3(0, 1.70711f, -0.70711f));
                            THEN("n=vector(0,0.70711,-0.70711)") {
                        //glm::vec3 new_n=glm::normalize(glm::vec3(0,0.70711f,-0.70711f));
                                REQUIRE_EQ(n.x, 0.0f);
                                REQUIRE_EQ(n.y, doctest::Approx(0.70711f));
                                REQUIRE_EQ(n.z, doctest::Approx(-0.70711f));
                    }
                }
            }
        }
    }

    SCENARIO ("Computing normal on transformed sphere")
    {
            GIVEN("s=sphere()") {
            sphere s;
                AND_THEN("m=scale(1,0.5,1)*rotation(π/5,vec(0,0,1))") {
//                    glm::mat4 m=glm::scale(glm::mat4(),glm::vec3(1,0.5,1));
                glm::mat4 m =  glm::scale(glm::mat4(),glm::vec3(1.0f,0.5f,1.0f))* glm::rotate(glm::pi<float>() / 5.0f, glm::vec3(0, 0, 1));
                    AND_THEN("set transform(s,m)")
                    {
                        s.set_model(m);
                        WHEN("When n ← normal_at(s, point(0, √2/2, -√2/2))")
                        {
                            std::cout<<"starting test"<<std::endl;
                            glm::vec3 n=s.get_normal_at_point(glm::vec3(0,glm::sqrt(2.0f)/2.0f,-glm::sqrt(2.0f)/2.0f));

                            THEN("n = vector(0, 0.97014, -0.24254)")
                            {
                                CHECK_EQ(doctest::Approx(0.0f),n.x);
                                CHECK_EQ(doctest::Approx(0.97014f),n.y);
                                CHECK_EQ(doctest::Approx(-0.24254),n.z);
                            }
                    }
                }
            }
        }
    }

    SCENARIO("A sphere's default transformation")
    {
        GIVEN("s ← sphere()")
        {
            sphere s;
            THEN("s.transform == identity_matrix")
            {
                REQUIRE_EQ(s.model,glm::mat4());
            }
        }
    }
    SCENARIO("Changing a sphere's transformation")
    {
        GIVEN("s ← sphere()")
        {
            sphere s;
            AND_WHEN("t ← translation(2, 3, 4)")
            {
                glm::mat4 t=glm::translate(glm::vec3(2,3,4));
                WHEN("set_transform(s, t)")
                {
                    s.set_model(t);
                    THEN("S.transform==t")
                    {
                        REQUIRE_EQ(s.model,t);
                    }
                }
            }
        }
    }

    SCENARIO("Intersecting a scaled sphere with a ray")
    {
        GIVEN("r ← ray(point(0, 0, -5), vector(0, 0, 1))")
        {
            ray r(glm::vec3(0,0,-5),glm::vec3(0,0,1));
            {
             AND_WHEN("s ← sphere()")
                {
                 sphere s;
                 WHEN("set_transform(s, scaling(2, 2, 2))")
                    {
                     s.set_model(glm::scale(glm::vec3(2.0f)));
                     AND_WHEN("xs ← intersect(s, r)")
                        {
                         collision_data xs=s.collide(r);
                         THEN("xs.t.size()==2")
                            {
                             REQUIRE_EQ(xs.t.size(),2);

                                AND_THEN("xs.t[0]==3")
                                {
                                    REQUIRE_EQ(xs.t[0],doctest::Approx(3.0f));
                                }
                                AND_THEN("xs.t[1]==7")
                                {
                                    REQUIRE_EQ(xs.t[1],doctest::Approx(7.0f));
                                }
                            }
                        }
                    }
                }
            }
        }
    }

/*


    And xs[0].t = 3
    And xs[1].t = 7*/



    SCENARIO("A sphere has a default material")
    {
        GIVEN("s ← sphere()")
        {
            sphere s=sphere();
            WHEN("m ← s.material")
            {
                material m=s.mat;
                THEN(" m = material()")
                {
                    REQUIRE_EQ(m,material());
                }
            }
        }
    }
    SCENARIO(" A sphere may be assigned a material")
    {
        GIVEN("s ← sphere()")
        {
            sphere s;
            AND_WHEN("m ← material()")
            {
                material m=material();
                AND_WHEN(" m.ka ← 1")
                {
                    m.ka=glm::vec4(1.0f);
                }
                AND_WHEN(" s.material ← m")
                {
                    s.mat=m;
                    THEN("s.material == m")
                    {
                        REQUIRE_EQ(s.mat,m);
                    }
                }
            }
        }
    }
}




TEST_SUITE("ray")
{
    SCENARIO("Creating and querying a ray")
    {
        GIVEN("origin=(1,2,3)")
        {
            glm::vec3 origin(1,2,3);
            GIVEN("direction=(1,0,-1)")
            {
                glm::vec3 direction(1,0,-1);
                glm::to_string(direction);

                WHEN("r=Ray(origin.direction)")
                {
                    ray r(origin,direction);
                    THEN("r.origin==origin")
                    {
                        REQUIRE_EQ(r.pos,origin);
                    }
                    AND_THEN("r.direction==direction ")
                    {
                        REQUIRE_EQ(r.dir,direction);
                    }
//                    std::cout<<glm::to_string(direction)<<std::endl;
                }
            }
        }
    }

    SCENARIO("Computing points from a distance")
    {
        GIVEN("r=Ray(2,3,4),(1,0,0))")
        {
            ray r(glm::vec3(2,3,4),glm::vec3(1,0,0));
            THEN("position at t=0 is (2,3,4)")
            {
                REQUIRE_EQ(r(0),glm::vec3(2.0f,3.0f,4.0f));
            }
                    THEN("position at t=1 is (3,3,4)")
            {
                        REQUIRE_EQ(r(1),glm::vec3(3.0f,3.0f,4.0f));
            }
                    THEN("position at t=-1 is (1,3,4)")
            {
                        REQUIRE_EQ(r(-1),glm::vec3(1.0f,3.0f,4.0f));
            }
                    THEN("position at t=2.5 is (4.5,3,4)")
            {
                        REQUIRE_EQ(r(2.5),glm::vec3(4.5,3.0f,4.0f));
            }
        }
    }

    SCENARIO("A ray intersect with sphere at 2 points")
    {
        GIVEN("r=Ray(0,0,-5),(0,0,1))")
        {
            GIVEN("s=sphere()")
            {
                ray r(glm::vec3(0, 0, -5), glm::vec3(0, 0, 1));
                sphere s;
                collision_data d=s.collide(r);
                WHEN("d=s.intersect(r)")
                {
                    THEN("number of intersections is 2")
                    {
                        REQUIRE_EQ(d.t.size(),2);
                    }
                    AND_THEN("first intersction is at (0,0,-1)")
                    {
                        REQUIRE_EQ(r(d.t[0]),glm::vec3(0,0,-1));
                    }
                    AND_THEN("second intersction is at (0,0,1)")
                    {
                        REQUIRE_EQ(r(d.t[1]),glm::vec3(0,0,1));
                    }
                }
            }
        }
    }


    SCENARIO("A ray miss a sphere")
    {
                GIVEN("r=Ray(0,10,-5),(0,0,1))")
        {
                    GIVEN("s=sphere()")
            {
                ray r(glm::vec3(0, 10, -5), glm::vec3(0, 0, 1));
                sphere s;
                collision_data d=s.collide(r);
                        WHEN("d=s.intersect(r)")
                {
                            THEN("number of intersections is 0")
                    {
                                REQUIRE_EQ(d.t.size(),0);
                    }
                }
            }
        }
    }
    SCENARIO("A ray originates inside a sphere")
    {
        GIVEN("r ← ray(point(0, 0, 0), vector(0, 0, 1))")
        {
            ray r(glm::vec3(0),glm::vec3(0,0,1));
            AND_WHEN("s ← sphere()")
            {
                sphere s;
                WHEN("xs ← intersect(s, r)")
                {
                    collision_data xs=s.collide(r);
                    THEN("xs.count ==2")
                    {
                        REQUIRE_EQ(xs.t.size(),2);
                        AND_THEN("xs[0] = -1.0")
                        {
                            REQUIRE_EQ(xs.t[0],-1.0f);
                        }
                                AND_THEN("xs[1] = 1.0")
                        {
                                    REQUIRE_EQ(xs.t[1],1.0f);
                        }
                    }
                }
            }
        }
    }

    SCENARIO("A sphere is behind ray")
    {
        GIVEN("r ← ray(point(0, 0, 5), vector(0, 0, 1))")
        {
            ray r(glm::vec3(0,0,5),glm::vec3(0,0,1));
            AND_WHEN("s ← sphere()")
            {
                sphere s;
                    WHEN("xs ← intersect(s, r)")
                {
                    std::cout<<"test starting"<<std::endl;
                    collision_data xs=s.collide(r);
                    THEN("xs.count ==2")
                    {
                        REQUIRE_EQ(xs.t.size(),2);
                        AND_THEN("xs[0] = -6.0")
                        {
                                    REQUIRE_EQ(xs.t[0],-6.0f);
                        }
                                AND_THEN("xs[1] = 4.0")
                        {
                                    REQUIRE_EQ(xs.t[1],-4.0f);
                        }
                    }
                }
            }
        }
    }

    SCENARIO("Intersect sets the object on the intersection")
    {
        GIVEN(" r ← ray(point(0, 0, -5), vector(0, 0, 1))")
        {
            ray r(glm::vec3(0,0,-5.0f),glm::vec3(0,0,1));
            AND_WHEN("s ← sphere()")
            {
                sphere s;
                WHEN("xs ← intersect(s, r)")
                {
                    collision_data xs=s.collide(r);
                    THEN("xs.count=2")
                    {
                        REQUIRE_EQ(xs.t.size(),2);
                        AND_THEN("xs.collided_shape is s")
                        {
                            REQUIRE_EQ(&xs.colided_shape,&s);
                        }
                    }
                }
            }
        }
    }
}

TEST_SUITE("camera") {
    SCENARIO ("The pixel size for horrizontal canvas") {
                GIVEN("c= camera(200,125,math.pi/2)") {
            Camera c(200, 125, glm::pi<float>() * 0.5f);
                    THEN("c.pixel_size is 0.001") {
                        REQUIRE_EQ(doctest::Approx(0.01), c.pixel_size);
            }
        }
    }


    SCENARIO ("The pixel size for vertical canvas") {
                GIVEN("c= camera(125,200,math.pi/2)") {
            Camera c(125, 200, glm::pi<float>() * 0.5f);
                    THEN("c.pixel_size is 0.001") {
                        REQUIRE_EQ(doctest::Approx(0.01), c.pixel_size);
            }
        }
    }

    SCENARIO ("Constructing a ray through the center of the canvas") {
                GIVEN ("c ← camera(201, 101, π/2)") {
            Camera c(201, 101, glm::pi<float>() / 2.0f);
                    WHEN("r ← ray_for_pixel(c, 100, 50)") {
                ray r = c.RayForPixel(100+0.5f, 50+0.5f);
                        THEN("r.origin = point(0, 0, 0)") {
                            REQUIRE_EQ(r.origin.x, doctest::Approx(0));
                            REQUIRE_EQ(r.origin.y, doctest::Approx(0));
                            REQUIRE_EQ(r.origin.z, doctest::Approx(0));
                            AND_THEN ("r.direction = vector(0, 0, -1)") {
                                REQUIRE_EQ(r.dir.x, doctest::Approx(0));
                                REQUIRE_EQ(r.dir.y, doctest::Approx(0));
                                REQUIRE_EQ(r.dir.z, doctest::Approx(-1));
                    }
                }
            }
        }
    }

    SCENARIO ("Constructing a ray through a corner of the canvas")
    {
    GIVEN(" c ← camera(201, 101, π/2)")
    {
        Camera c(201, 101, glm::pi<float>() / 2.0f);
        WHEN("r ← RayForPixel_mid_pixel(c, 0, 0)") {
            ray r=c.RayForPixel(0+0.5f,0+0.5f);
            THEN("r.origin == point(0, 0, 0)") {
                REQUIRE_EQ(r.origin.x, doctest::Approx(0));
                REQUIRE_EQ(r.origin.y, doctest::Approx(0));
                REQUIRE_EQ(r.origin.z, doctest::Approx(0));
                AND_THEN("r.direction == vector(0.66519, 0.33259, -0.66851))")
                    {
                        REQUIRE_EQ(r.dir.x, doctest::Approx(0.66519f));
                        REQUIRE_EQ(r.dir.y, doctest::Approx(0.33259f));
                        REQUIRE_EQ(r.dir.z, doctest::Approx(-0.66851f));
                    }
                }
            }
        }
    }


    SCENARIO("Constructing a ray when the camera is transformed") {
                GIVEN("c ← camera(201, 101, π/2)") {
                Camera c(201, 101, glm::pi<float>() / 2.0f);
            WHEN("c.transform ← rotation_y(π/4) * translation(0, -2, 5)") {
//                glm::mat4 rot=
            c.set_transform(glm::rotate(glm::mat4(1.0f),glm::pi<float>()/4.0f,glm::vec3(0,1,0))*glm::translate(glm::mat4(),glm::vec3(0,-2,5)));
                        AND_WHEN("r ← ray_for_pixel(c, 100, 50)") {
                            ray r=c.RayForPixel(100.0f+0.5f,50.0f+0.5f);
                            std::cout<<"ray is "<<r.origin<<",\nr.dir="<<r.dir<<std::endl;
                            THEN ("r.origin = point(0, 2, -5)") {
                                REQUIRE_EQ(r.origin.x,doctest::Approx(0.0f));
                                REQUIRE_EQ(r.origin.y,doctest::Approx(2.0f));
                                REQUIRE_EQ(r.origin.z,doctest::Approx(-5.0f));
                        AND_THEN("r.direction = vector(√2/2, 0, -√2/2)")
                        {
                            REQUIRE_EQ(r.dir.x,doctest::Approx(glm::sqrt(2.0f)/2.0f));
                            REQUIRE_EQ(r.dir.y,doctest::Approx(0.0f));
                            REQUIRE_EQ(r.dir.z,doctest::Approx(-glm::sqrt(2.0f)/2.0f));
                        }
                    }
                }
            }
        }
    }

}
