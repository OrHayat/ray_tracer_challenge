


#define DOCTEST_CONFIG_IMPLEMENT
#define GLM_FORCE_SWIZZLE
#define GLM_FORCE_CTOR_INIT

#include <rtc/core/ray.hpp>
#include <rtc/core/collision_data.hpp>
#include <rtc/core/sphere.hpp>
#include <rtc/Camera.hpp>
#include <doctest/doctest.h>

#define  GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/ext.hpp>

// Writes a generic GLM vector type to a stream.
namespace glm {
    template<int D, typename T, glm::qualifier P>
    std::ostream &operator<<(std::ostream &os, const glm::vec<D, T, P> &v) {
        return os << glm::to_string(v);
    }
}
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
                            REQUIRE_EQ(n.x, doctest::Approx(glm::sqrt(3.0f)/3.0f));
                            REQUIRE_EQ(n.y, doctest::Approx(glm::sqrt(3.0f)/3.0f));
                            REQUIRE_EQ(n.z, doctest::Approx(glm::sqrt(3.0f)/3.0f));
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


    SCENARIO ("Computing normal on translated sphere")
    {
    GIVEN("s=sphere()") {
        sphere s;
            AND_THEN("set transform (s,tranlate(0,1,0))")
            {
            s.set_model(glm::translate(s.model, glm::vec3(0, 1, 0)));
                    WHEN("n=normal_at(s,point(0,1.70711,-0.70711)")
                    {
                    glm::vec3 n = s.get_normal_at_point(glm::vec3(0,1.70711f, -0.70711f));
                   THEN("n=vector(0,0.70711,-0.70711)")
                   {
                       //glm::vec3 new_n=glm::normalize(glm::vec3(0,0.70711f,-0.70711f));
                        REQUIRE_EQ(n.x, 0.0f);
                       CHECK_EQ(n.y, doctest::Approx(0.70711f));
                        REQUIRE_EQ(n.z, doctest::Approx(-0.70711f));
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
                    AND_THEN("r.direction==direction after normalization")
                    {
                        REQUIRE_EQ(r.dir,glm::normalize(direction));
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
                    THEN("position at t=2.5 is (3,3,4)")
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
}

TEST_SUITE("camera")
{
    SCENARIO("The pixel size for horrizontal canvas")
    {
        GIVEN("c= camera(200,125,math.pi/2)")
        {
            Camera c(200,125,glm::pi<float>()*0.5f);
            THEN("c.pixel_size is 0.001")
            {
                REQUIRE_EQ(doctest::Approx(0.01),c.pixel_size);
            }
        }
    }


    SCENARIO("The pixel size for vertical canvas")
    {
                GIVEN("c= camera(125,200,math.pi/2)")
        {
            Camera c(125,200,glm::pi<float>()*0.5f);
                    THEN("c.pixel_size is 0.001")
            {
                        REQUIRE_EQ(doctest::Approx(0.01),c.pixel_size);
            }
        }
    }

}